# Configuration

This document provides guidelines for creating a configuration file for
TextQuill. The configuration file is in TOML format and is used to specify
various settings required for the Tesseract, the OCR engine.

## Configuration structure

The configuration has all of its variables in the `textquill` section. Here are
all possible variables and their possible value(s):

|Variable|Possible value(s)|Description|
|---|---|---|
|`language`|any language in ISO 639-3|The language Tesseract will use to detect text.|
|`psm`|0-13|The page segmentation mode option.|

Here's an example configuration file:

```toml
[textquill]
language = "eng"
psm = 3
```

-----

The documentation is licensed under the [GNU Free Documentation License 
v1.3](LICENSE.md). You can get a full copy at <https://www.gnu.org/licenses/fdl-1.3.html>.
