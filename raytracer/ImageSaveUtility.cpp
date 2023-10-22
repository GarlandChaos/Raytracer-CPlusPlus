#include "ImageSaveUtility.h"

ImageSaveUtility::ImageSaveUtility() { }

ImageSaveUtility::~ImageSaveUtility() { }

void ImageSaveUtility::SaveImage(const sf::Image* img)
{
	std::string fileCompleteName = fileName + std::to_string(savedImageCount) + fileExtension;
	bool saved = img->saveToFile(filePath + fileCompleteName);

	if (!saved) return;

	savedImageCount++;
	std::cout << fileCompleteName << successMessage << std::endl;
}