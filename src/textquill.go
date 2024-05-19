package main

import (
	"flag"
	"log"
	"os"
	"path/filepath"
	"strings"

	"github.com/otiai10/gosseract/v2"
	"github.com/pelletier/go-toml"
)

const (
	defaultConfigPath   = "config.toml"
	defaultOutputFormat = "text"
	defaultLang         = "eng"
	defaultPSM          = 3
)

// Command-line arguments.
type Args struct {
	ImagePaths  []string
	Format      string
	ConfigPath  string
	OutputDir   string
	Language    string
	PageSegMode int
}

func main() {
	var args Args

	flag.StringVar(&args.ConfigPath, "config", defaultConfigPath, "The path of the TOML config file")
	flag.StringVar(&args.Format, "format", defaultOutputFormat, "The output format of the recognized text. Possible formats: 'hocr', 'text'")
	flag.StringVar(&args.OutputDir, "output-dir", ".", "The directory to save the output files")
	flag.StringVar(&args.Language, "lang", "", "OCR language")
	flag.IntVar(&args.PageSegMode, "psm", defaultPSM, "Page segmentation mode (0-13)")

	flag.Func("images", "Comma-separated list of image paths to process", func(flagValue string) error {
		args.ImagePaths = strings.Split(flagValue, ",")
		return nil
	})

	flag.Parse()

	if flag.NFlag() == 0 {
		flag.Usage()
		return
	}

	log.SetFlags(log.Ldate | log.Ltime | log.Lshortfile)
	config := loadConfig(args.ConfigPath)

	if args.Language == "" {
		args.Language = config.GetDefault("textquill.language", defaultLang).(string)
	}
	if args.PageSegMode == defaultPSM {
		args.PageSegMode = int(config.GetDefault("textquill.pagesegmode", defaultPSM).(int64))
	}

	client := gosseract.NewClient()
	defer client.Close()
	client.SetLanguage(args.Language)
	client.SetPageSegMode(gosseract.PageSegMode(args.PageSegMode))

	for _, imagePath := range args.ImagePaths {
		processImage(client, imagePath, args.Format, args.OutputDir)
	}
}

// loadConfig loads and validates the TOML configuration file.
func loadConfig(configPath string) *toml.Tree {
	configContents, err := os.ReadFile(configPath)
	if err != nil {
		log.Fatalf("Error opening config file for reading: %v", err)
	}

	config, err := toml.Load(string(configContents))
	if err != nil {
		log.Fatalf("Error parsing config file as TOML: %v", err)
	}

	return config
}

// processImage performs OCR on the specified image and saves the result to a file.
func processImage(client *gosseract.Client, imagePath, format, outputDir string) {
	imageData, err := os.ReadFile(imagePath)
	if err != nil {
		log.Printf("Error reading image file %s: %v", imagePath, err)
		return
	}

	client.SetImageFromBytes(imageData)
	var recognizedText string

	switch strings.ToLower(format) {
	case "text":
		recognizedText, err = client.Text()
	case "hocr":
		recognizedText, err = client.HOCRText()
	default:
		log.Printf("Invalid format %s for image %s. Possible values: 'text', 'hocr'", format, imagePath)
		return
	}

	if err != nil {
		log.Printf("Error recognizing text from image %s: %v", imagePath, err)
		return
	}

	outputFile := generateOutputFileName(imagePath, format, outputDir)
	err = os.WriteFile(outputFile, []byte(recognizedText), 0644)
	if err != nil {
		log.Printf("Error writing text to file '%s': %v", outputFile, err)
		return
	}

	log.Printf("Recognized text from image %s has been successfully written to file '%s'.", imagePath, outputFile)
}

// generateOutputFileName generates the output file name based on the original image path and desired format.
func generateOutputFileName(imagePath, format, outputDir string) string {
	baseName := filepath.Base(imagePath)
	ext := filepath.Ext(baseName)
	newExt := ".txt"
	if strings.ToLower(format) == "hocr" {
		newExt = ".hocr"
	}
	newName := strings.TrimSuffix(baseName, ext) + newExt
	return filepath.Join(outputDir, newName)
}
