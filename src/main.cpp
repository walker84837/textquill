#include <chrono>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>
#include "../include/opencv4/opencv2/core.hpp"
#include "../include/opencv4/opencv2/core/mat.hpp"
#include "../include/opencv4/opencv2/core/utility.hpp"
#include "../include/opencv4/opencv2/opencv.hpp"
#include "../include/opencv4/opencv2/opencv_modules.hpp"
#include "../include/tesseract/baseapi.h"

#include "wrappers.hpp"

using std::strcmp;
using std::string;
using json = nlohmann::json;
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Please provide at least two arguments." << '\n';
		return 1;
	}

	else if (wrappers::stringcheck(argv[1], "--help")) {
		std::cout << "Usage: " << argv[0] << " <input_file> <output_filename>" << '\n';
		return 0;
	}

	string inputFile = argv[1];
	string outputFilename = argv[2];

	auto start = std::chrono::high_resolution_clock::now();

	try {
		std::ofstream outputFile(outputFilename);
		std::ifstream json_configs("config.json");
		json data = json::parse(json_configs);

		string rawLanguage = data["language"];
		string language = rawLanguage.substr(0, 3);

		if (language.size() != 3) {
			language = "eng";
		}

		tesseract::TessBaseAPI ocr;
		ocr.Init(nullptr, language.c_str(), tesseract::OEM_DEFAULT);

		// Set additional configuration options
		ocr.SetVariable("classify_bln_numeric_mode", "1");
		ocr.SetVariable("textord_force_make_prop_words", "1");
		ocr.SetVariable("tessedit_pageseg_mode", "6");

		cv::Mat image = cv::imread(inputFile, cv::IMREAD_COLOR);

		cv::Mat srgbImage;
		cv::cvtColor(image, srgbImage, cv::COLOR_BGR2RGB);

		ocr.SetImage(srgbImage.data, srgbImage.cols, srgbImage.rows, 3, srgbImage.step);

		char *text = ocr.GetUTF8Text();
		outputFile << text << '\n';
		delete[] text;

		ocr.End();
		outputFile.close();
	}

	catch (std::exception &e) {
		std::cerr << "Error while extracting text or parsing the config.json file: " << e.what() << '\n';
		return 1;
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	double seconds = static_cast<double>(duration.count()) / 1000;

	std::cout << "Text extraction completed in: " << duration.count() << " ms (" << seconds << " seconds)" << '\n';

	return 0;
}
