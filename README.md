# textquill: a simple ocr tool

> [!WARNING] This software is under heavy developent. Things are likely to
> break.

An OCR tool that extracts text from images using
[`gosseract`](https://github.com/otiai10/gosseract).

## Table of Contents

  - [Usage](#usage)
  - [Installation](#installation)
  - [License](#license)

## Usage

  - `images`: The comma-separated paths of the images to scan.
  - `format`: The output format of the recognized text. The possible values are:
      - `text` (default)
      - `hocr`
  - `config`: The path of the config file. Default is at "config.toml".
  - `output-dir`: The directory to save the output file(s). Default is in
    current working directory.
  - `lang`: The language that Tesseract will use to scan the image. Default is
    `eng`.
  - `psm`: Page segmentation mode (0-13). Default is 3.

To build this program, you need Go and its toolchain. You can install Go
[here](https://go.dev/dl/). With that out of the way, you can build it from
source by using:

``` console
go build -ldflags="-s -w" -o textquill ./src/textquill.go
```

## Roadmap

  - [X] Support for providing multiple files
  - [ ] Exporting files in mutliple other formats, as right now, only text and
    HOCR are supported

## License

This project is licensed under the [GNU General Public License
v3.0](LICENSE.md). You can get a full copy of the license
[here](https://www.gnu.org/licenses/gpl-3.0.html).
