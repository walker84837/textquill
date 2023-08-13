# cpp-ocr-tool

An OCR (Optical Character Recognition) tool that extracts text from images using Tesseract and OpenCV. This program can extract text content from images, supports various image formats (JPEG, PNG, etc.), utilizes Tesseract for accurate text recognition, and applies additional configuration options in a JSON file for improved customisation.

## How to Use

- If you wish, you can download the pre-compiled binaries, and its SHA256 checksum if you'd like to check the binary's checksum. Or else, if you feel like compiling the program, you can clone the repository:

    ```bash
    git clone https://github.com/walker84837/cpp-ocr-tool.git
    ```

- If you wish to, you can download the `config.json` from the cloned GitHub repository, or if you wish to make your own, there is only one thing to write to a `config.json` file:

    ```json
    {
        "language": "xxx"
    }
    ```

    `xxx` is a country code that's supported by Tesseract, and make sure you have its package from your package manager or anything.
    **Please move the `config.json` in the same folder as the executable, or it won't work!**

- Then, do the `make` command to build the binary.

- Drag your image, and put it in the same folder as the executable file.
  - Make sure the image is valid and readable with any image viewer/editor, or the program won't work.

- Pass the image and output file to the program:

    ```bash
    ./executable.app image.ext output.ext
    ```

    `ext` can be an extension of your liking, especially for the output file.

- You're done! It will take some seconds, depending on how big the image is, or how much it's converting to UTF-8 text.

## Contributing

Contributions to this project are always welcome. If you'd like to contribute, please follow these steps:

1. Clone the repository:

    ```bash
    git clone https://github.com/walker84837/cpp-ocr-tool.git
    ```

2. Create a new branch for your changes:

    ```bash
    git switch -c new-feature 
    ```

3. Make your changes and test them thoroughly.

4. Commit your changes:

    ```bash
    git commit -m "Add a description of your changes"
    ```

5. Push your changes to your branch:

    ```bash
    git push origin new-feature
    ```

6. Open a pull request on the repository. Describe your changes and the problem they solve.

## License

This project is licensed under the GNU General Public License v3.0. You can find the full license text in the [LICENSE](LICENSE.md) file. You can get a full copy of the license [here](https://www.gnu.org/licenses/gpl-3.0.html).
