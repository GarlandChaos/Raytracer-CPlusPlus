#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Light.h"
#include <math.h>
#include <noise.h>
#include <glm/gtc/noise.hpp>
#include "FilterConstants.h"

class Filter 
{
public:
	int d = 9;
	double factor = 1.0 / 9.0;
	double alpha = 0.0;
	double mask[9][9] =
	{
	  1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	  0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	  0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	  0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
	  0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
	  0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
	  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
	  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
	  0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0
	};

	Filter();
	~Filter();
	void Blur(sf::Image* img) const;
	void Hdr(sf::Image* img) const;
	void Hdr2(sf::Image* img) const;
	sf::Image* Sobel(const sf::Image* img) const;
	sf::Image* Sobel2(const sf::Image* img) const;
	sf::Image* MergeSobel(const sf::Image* img, const sf::Image* img2) const;
	void PerlinNoise(sf::Image* img) const;
	sf::Image* MixPerlinNoise(const sf::Image* img, const sf::Image* perlin, const sf::Color colorNoise) const;
	sf::Image* SobelNoise(const sf::Image* sobel) const;
	sf::Image* Crayon(const sf::Image* render, const sf::Image* noise, const sf::Image* edge) const;
};