#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <expected>

#include <cxxopts.hpp>
#include <cpptoml.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

namespace fs = std::filesystem;

struct Args {
    std::vector<fs::path> image_paths;
    std::string format;
    fs::path config_path;
    fs::path output_dir;
    std::string language;
    int page_seg_mode;
};

std::expected<std::shared_ptr<cpptoml::table>, std::string> load_config(const fs::path& config_path) {
    try {
        return cpptoml::parse_file(config_path.string());
    } catch (const std::exception& e) {
        return std::unexpected("Error opening or parsing config file: " + std::string(e.what()));
    }
}

std::expected<std::string, std::string> process_image(tesseract::TessBaseAPI& api, const fs::path& image_path, const std::string& format) {
    Pix* image = ::pixRead(image_path.c_str());
    if (!image) {
        return std::unexpected("Error reading image file " + image_path.string());
    }

    api.SetImage(image);
    char* out_text = nullptr;

    if (format == "text") {
        out_text = api.GetUTF8Text();
    } else if (format == "hocr") {
        out_text = api.GetHOCRText(0);
    } else {
        ::pixDestroy(&image);
        return std::unexpected("Invalid format " + format + " for image " + image_path.string());
    }

    if (!out_text) {
        ::pixDestroy(&image);
        return std::unexpected("Error recognizing text from image " + image_path.string());
    }

    std::string result(out_text);
    delete[] out_text;
    pixDestroy(&image);
    return result;
}

fs::path generate_output_file_name(const fs::path& image_path, const std::string& format, const fs::path& output_dir) {
    fs::path base_name = image_path.filename();
    std::string new_ext = (format == "hocr") ? ".hocr" : ".txt";
    fs::path new_name = base_name.stem().string() + new_ext;
    return output_dir / new_name;
}

int main(int argc, char* argv[]) {
    cxxopts::Options options("textquill", "Extract text from images using Tesseract OCR");
    options.add_options()
        ("c,config", "The path of the TOML config file", cxxopts::value<std::string>()->default_value("config.toml"))
        ("f,format", "The output format of the recognized text. Possible formats: 'hocr', 'text'", cxxopts::value<std::string>()->default_value("text"))
        ("o,output-dir", "The directory to save the output files", cxxopts::value<std::string>()->default_value("."))
        ("l,lang", "OCR language", cxxopts::value<std::string>()->default_value("eng"))
        ("p,psm", "Page segmentation mode (0-13)", cxxopts::value<int>()->default_value("3"))
        ("i,images", "Comma-separated list of image paths to process", cxxopts::value<std::vector<std::string>>())
        ("h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    Args args;
    args.config_path = result["config"].as<std::string>();
    args.format = result["format"].as<std::string>();
    args.output_dir = result["output-dir"].as<std::string>();
    args.language = result["lang"].as<std::string>();
    args.page_seg_mode = result["psm"].as<int>();

    if (result.count("images")) {
        for (const auto& path : result["images"].as<std::vector<std::string>>()) {
            args.image_paths.push_back(path);
        }
    }

    if (args.image_paths.empty()) {
        std::cerr << "Error: No image paths provided." << std::endl;
        std::cerr << options.help() << std::endl;
        return 1;
    }

    auto config = load_config(args.config_path);
    if (config) {
        if (result["lang"].has_default()) {
            args.language = (*config)->get_qualified_as<std::string>("textquill.language").value_or(args.language);
        }
        if (result["psm"].has_default()) {
            args.page_seg_mode = (*config)->get_qualified_as<int>("textquill.pagesegmode").value_or(args.page_seg_mode);
        }
    }

    tesseract::TessBaseAPI api;
    if (api.Init(nullptr, args.language.c_str())) {
        std::cerr << "Could not initialize tesseract." << std::endl;
        return 1;
    }
    api.SetPageSegMode(static_cast<tesseract::PageSegMode>(args.page_seg_mode));

    for (const auto& image_path : args.image_paths) {
        auto recognized_text = process_image(api, image_path, args.format);
        if (recognized_text) {
            fs::path output_file = generate_output_file_name(image_path, args.format, args.output_dir);
            std::ofstream out_stream(output_file);
            if (out_stream) {
                out_stream << *recognized_text;
                std::cout << "Recognized text from image " << image_path << " has been successfully written to file " << output_file << std::endl;
            } else {
                std::cerr << "Error writing text to file " << output_file << std::endl;
            }
        } else {
            std::cerr << "Error processing image " << image_path << ": " << recognized_text.error() << std::endl;
        }
    }

    return 0;
}
