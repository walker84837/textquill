# textquill: a simple ocr tool

An OCR tool that extracts text from images using [`tesseract`](https://crates.io/crates/tesseract), written in Rust.

## Table of contents

  - [Usage](#usage)
  - [Installation](#installation)
  - [License](#license)

## Usage

  - `image_path`: The path of the image to get the text from.

  - `output_format`: The output format of the recognized text. The possible values are:
    - `text`
    - `hocr`
    - `tsv`

  - `page_number`: The number of the page to get the text from. Required only if output_format is `hocr` or `tsv`.

  - `config`: The path of the config file. Default is at "config.toml".

## Installation

To build this program, you need Rust and its package manager, Cargo. To install Rust, you can follow the guide [here](https://doc.rust-lang.org/book/ch01-01-installation.html). With that out of the way, you can build it from source by using:

``` console
cargo build --release
```

> [!WARNING]
> This software is expected to not compile as of now.

## License

This project is licensed under the GNU General Public License v3.0. You can find the full license text in the [LICENSE](LICENSE.md) file. You can get a full copy of the license [here](https://www.gnu.org/licenses/gpl-3.0.html).
