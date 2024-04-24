package main

import (
	"flag"
	"fmt"
	"os"
	"strings"

	"github.com/otiai10/gosseract/v2"
	"github.com/pelletier/go-toml"
)

// Command-line arguments.
type Args struct {
	ImagePath    string
	OutputFormat string
	ConfigPath   string
	OutputPath   string
}

func main() {
	var args Args

	flag.StringVar(&args.ImagePath, "image", "", "The image to scan")
	flag.StringVar(&args.OutputFormat, "format", "", "The output format of the recognized text. Possible formats: 'hocr', 'text'")
	flag.StringVar(&args.ConfigPath, "config", "config.toml", "The path of the TOML config file")
	flag.StringVar(&args.OutputPath, "output", "image.txt", "The path of the output file")

	flag.Parse()

	if flag.NFlag() == 0 {
		fmt.Println("Usage: textquill [image path] [text format] [config path] [output file]")
		fmt.Println("Options:")
		flag.PrintDefaults()
		return
	}

	configData, err := os.ReadFile(args.ConfigPath)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error opening config file for reading: %v\n", err)
		return
	}

	config, err := toml.Load(string(configData))
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error parsing config file: %v\n", err)
		return
	}

	language := config.Get("textquill.language").(string)

	client := gosseract.NewClient()
	defer client.Close()

	client.SetLanguage(language)
	client.SetPageSegMode(gosseract.PageSegMode(1))

	imageData, err := os.ReadFile(args.ImagePath)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error reading image file: %v\n", err)
		return
	}

	client.SetImageFromBytes(imageData)

	var recognizedText string

	switch strings.ToLower(args.OutputFormat) {
	case "text":
		recognizedText, _ = client.Text()
	case "hocr":
		recognizedText, _ = client.HOCRText()
	default:
		fmt.Fprintf(os.Stderr, "Invalid format. Possible values: 'text', 'hocr'\n")
		return
	}

	err = os.WriteFile(args.OutputPath, append([]byte(recognizedText), '\n'), 0644)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error writing to output file: %v\n", err)
		return
	}

	err = client.Close()	
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error closing Tesseract API: %v\n", err)
		return
	}
	fmt.Printf("Text in image successfully written to '%s'.\n", args.OutputPath)
}
