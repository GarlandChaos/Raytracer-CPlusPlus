#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

class ImageSaveUtility
{
private:
	const std::string filePath = "Images/";
	const std::string fileName = "Raytracing_saved";
	const std::string fileExtension = ".png";
	const std::string successMessage = " saved successfully on Images folder.";
	int savedImageCount = 0;

public:
	ImageSaveUtility();
	~ImageSaveUtility();
	void SaveImage(const sf::Image* img);
};