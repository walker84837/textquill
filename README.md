# textquill: a simple ocr tool

> [!WARNING]
> This software is under heavy developent. Things are likely to break.

An OCR tool that extracts text from images using
[`gosseract`](https://github.com/otiai10/gosseract).

## Table of Contents

  - [Usage](#usage)
  - [Installation](#installation)
  - [License](#license)

## Usage

  - `image`: The path of the image to scan.
  - `format`: The output format of the recognized text. The possible values are:
    - `text`
    - `hocr`
  - `config`: The path of the config file. Default is at "config.toml".
  - `output`: The path of the file containing the extracted text.

## Installation

To build this program, you need Go and its toolchain. You can install Go
[here](https://go.dev/dl/). With that out of the way, you can build it from
source by using:

``` console
go build -ldflags="-s -w" -o textquill ./src/textquill.go
```

## Roadmap

  - [ ] Support for providing multiple files

## License

This project is licensed under the [GNU General Public License
v3.0](LICENSE.md). You can get a full copy of the license
[here](https://www.gnu.org/licenses/gpl-3.0.html).
