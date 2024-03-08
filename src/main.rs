use anyhow::{anyhow, Context, Result};
use clap::Parser;
use std::{
    fs::File,
    io::{BufReader, Read},
    path::PathBuf,
};
use tesseract::{OcrEngineMode, Tesseract};
use toml::Value;

#[derive(Parser)]
struct Args {
    #[arg(help = "The image to scan")]
    image_path: PathBuf,

    #[arg(help = "The output format of the recognized text")]
    output_format: String,

    #[arg(
        help = "The number of the page to get the text from",
        requires = "output_format == 'hocr' || output_format == 'tsv'"
    )]
    page_number: i32,

    #[arg(
        short,
        long,
        help = "The path of the config file",
        default_value = "config.toml"
    )]
    config: PathBuf,
}

fn main() -> Result<()> {
    let args = Args::parse();

    let mut reader = BufReader::new(File::open(args.config)?);
    let mut config_str = String::new();
    reader.read_to_string(&mut config_str)?;

    let config: Value = toml::from_str(&config_str)?;

    let language = &config["textquill"]["language"].as_str().ok_or(anyhow!("Failed to get language from config file."))?[0..3];

    let ocr = Tesseract::new_with_oem(None, Some(language), OcrEngineMode::TesseractLstmCombined)
        .with_context(|| "Failed to initialize Tesseract instance.")?;

    let mut bytes: Vec<u8> = vec![];
    let mut reader = BufReader::new(File::open(&args.image_path)?);
    reader.read_to_end(&mut bytes)?;

    ocr.set_image_from_mem(bytes.as_slice())
        .with_context(|| "Failed to set image for use with Leptonica.")?;

    let recognized_text = match args.output_format.to_lowercase().trim() {
        "text" => &ocr
            .recognize()
            .with_context(|| "Failed to recognize text in the image.")?
            .get_text()
            .with_context(|| "Failed to get UTF-8 text from the image.")?,
        "hocr" => &ocr
            .recognize()
            .with_context(|| "Failed to recognize text in the image.")?
            .get_hocr_text(args.page_number)
            .with_context(|| "Failed to get hOCR-formatted text from the image.")?,
        "tsv" => &ocr
            .recognize()
            .with_context(|| "Failed to recognize text in the image.")?
            .get_tsv_text(args.page_number)
            .with_context(|| "Failed to get TSV-formatted text from the image.")?,
        _ => {
            return Err(anyhow!(
                "Invalid format. Possible values: 'text', 'hocr', 'tsv'"
            ));
        }
    };

    // Do something with recognized_text if needed

    Ok(())
}
