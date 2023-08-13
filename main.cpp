#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <nlohmann/json.hpp>
#include <tesseract/baseapi.h>
#include <chrono>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/core/utility.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/opencv_modules.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;

bool areFilesPresent(fs::path json_config, fs::path input_image)
{
	if (!fs::exists(json_config) && !fs::exists(input_image))
	{
		return false;
	}
	else
	{
		return true;
	}
}

int main(int argc, char *argv[])
{
	if (argv[1] == nullptr)
	{
		throw std::invalid_argument("Please provide at least two arguments.");
	}

	if (
		std::strcmp(argv[1], "--help") == 0 ||
		std::strcmp(argv[1], "-h") == 0 ||
		std::strcmp(argv[1], "-help") == 0)
	{
		std::cout << "Usage: " << argv[0] << " <input_file> <output_filename>" << '\n';
		return 0;
	}

	if (!areFilesPresent(fs::path(argv[1]), "config.json"))
	{
		throw std::invalid_argument("Files are missing!");
	}

	auto start = std::chrono::high_resolution_clock::now();

	std::string inputFile = argv[1];
	std::string outputFilename = argv[2];

	try
	{
		std::ofstream outputFile(outputFilename);
		std::ifstream json_configs("config.json");
		json data = json::parse(json_configs);

		std::string rawLanguage = data["language"];
		std::string language = rawLanguage.substr(0, 3);

		if (language.size() < 3)
		{
			language = "eng";
		}

		tesseract::TessBaseAPI ocr;
		ocr.Init(nullptr, language.c_str(), tesseract::OEM_DEFAULT);

		// Set additional configuration options
		ocr.SetVariable("classify_bln_numeric_mode", "1");
		ocr.SetVariable("textord_force_make_prop_words", "1");
		ocr.SetVariable("tessedit_pageseg_mode", "6"); // PSM_AUTO_ONLY

		cv::Mat image = cv::imread(inputFile, cv::IMREAD_COLOR);

		// Convert image to sRGB
		cv::Mat srgbImage;
		cv::cvtColor(image, srgbImage, cv::COLOR_BGR2RGB);

		ocr.SetImage(srgbImage.data, srgbImage.cols, srgbImage.rows, 3, srgbImage.step);

		char *text = ocr.GetUTF8Text();
		outputFile << text << '\n';
		delete[] text;

		ocr.End();
		outputFile.close();
	}

	catch (std::exception &e)
	{
		std::cerr << "Error: " << e.what() << '\n';
		return 1;
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	double minutes = static_cast<double>(duration.count()) / 1000;

	std::cout << "Text extraction completed in: " << duration.count() << " ms (" << minutes << " seconds)" << '\n';

	return 0;
}
