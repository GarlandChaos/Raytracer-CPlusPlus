#include <iostream>
#include <noise.h>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Light.h"
#include "World.h"
#include "Filter.h"
#include "ReadTxtUtility.h"
#include "ImageSaveUtility.h"

int main()
{
	ImageSaveUtility imageSaveUtility;
	World world;

	ReadTxtUtility readTxtUtility;
	readTxtUtility.ReadAndBuild("Read File/ObjectsFile.txt", world);

	sf::RenderWindow window;
	window.create(sf::VideoMode(world.sX, world.sY), "Bernardo's Raytracer");

	sf::Image* img = new sf::Image();
	img->create(world.sX, world.sY);

	std::cout << "Rendering..." << std::endl;
	world.RenderPerspective(world.sX, world.sY, img);
	world.CheckPostProcessing(img);

	sf::Texture* texture = new sf::Texture();
	texture->loadFromImage(*img);

	sf::Sprite sprt;
	sprt.setTexture(*texture, true);
	
	std::cout << "----------COMMANDS----------" << std::endl;
	std::cout << "Press H for HDR1." << std::endl;
	std::cout << "Press J for HDR2." << std::endl;
	std::cout << "Press M for Motion Blur." << std::endl;
	std::cout << "Press G for Crayon." << std::endl;
	std::cout << "Press C for Cell Shading." << std::endl;
	std::cout << "Press L for Sobel." << std::endl;
	std::cout << "Press R for Raytracing (also resets filters)." << std::endl;
	std::cout << "Press S for save." << std::endl << std::endl;

	while (window.isOpen()) 
	{
		sf::Event event;
		while (window.pollEvent(event)) 
		{
			if (event.type == sf::Event::Closed) 
			{
				window.close();
			}
			if (event.type == sf::Event::KeyPressed) 
			{
				if (event.key.code == sf::Keyboard::L)
				{
					std::cout << "Applying Sobel..." << std::endl;
					world.ApplyFilter(FilterConstants::SOBEL, img, texture);
				}
				else if (event.key.code == sf::Keyboard::M)
				{
					std::cout << "Applying Motion Blur..." << std::endl;
					world.ApplyFilter(FilterConstants::MOTION_BLUR, img, texture);
				}
				else if (event.key.code == sf::Keyboard::H)
				{
					std::cout << "Applying HDR1..." << std::endl;
					world.ApplyFilter(FilterConstants::HDR, img, texture);
				}
				else if (event.key.code == sf::Keyboard::J)
				{
					std::cout << "Applying HDR2..." << std::endl;
					world.ApplyFilter(FilterConstants::HDR2, img, texture);
				}
				else if (event.key.code == sf::Keyboard::G)
				{
					std::cout << "Applying Crayon..." << std::endl;
					world.ApplyFilter(FilterConstants::CRAYON, img, texture);
				}
				else if (event.key.code == sf::Keyboard::C)
				{
					std::cout << "Applying Cell Shading..." << std::endl;
					world.ApplyFilter(FilterConstants::CELL_SHADING, img, texture);
				}
				else if (event.key.code == sf::Keyboard::R)
				{
					std::cout << "Executing raycast/raytracing with depth " << world.maxDepth << "..." << std::endl;
					world.RenderPerspective(world.sX, world.sY, img);
					texture->loadFromImage(*img);
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					imageSaveUtility.SaveImage(img);
				}
				else if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
			}
		}
		window.clear();
		window.draw(sprt);
		window.display();
	}
	system("pause");
}