# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/),
and this project adheres to [Semantic Versioning](https://semver.org/).

## [Unreleased]

## [0.3.0] - 2024-05-19

### Added

- Add capability to handle multiple images through command-line options.
- Add command-line flags for:
  - OCR language (`-lang`)
  - page segmentation mode (`-psm`)
- Add enhanced error handling with more context
- Add the use of `log` for logging and using log levels.

### Changed

- Change default configuration settings if not specified in the TOML file.
- General code refactoring to improve maintainability and readability.

## [0.2.0] - 2024-04-24

### Changed

  - Port from C++ (to Rust), to Go.

## [0.1.0] - 2023-10-31

### Added

  - Added directory structures (`bin/`, `src/`, `include/`)

## [0.0.1] - 2023-08-13

### Added

  - Initial release of `cpp-ocr-tool`.
