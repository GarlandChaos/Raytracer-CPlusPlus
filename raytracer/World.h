#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Light.h"
#include "Raytracer.h"
#include "Filter.h"
#include "FilterConstants.h"

class World 
{
public:

	Raytracer raytracer;
	std::vector<Sphere*> objects;
	glm::vec3 ambientColor = glm::vec3(1.0f, 1.0f, 1.0f);
	Light light;
	float Ia = 1.0f; //ambient light intensity
	float fov = 0.0f;
	glm::vec3 eye = glm::vec3(0.0f);
	int maxDepth = 10;
	int sX = 0;
	int sY = 0;
	Filter filter;
	std::vector<FilterConstants> postProcessing;

	World();
	~World();
	void AddObject(Sphere* obj);
	void RenderOrtogonal(const float sX, const float sY, sf::Image* img);
	void RenderPerspective(const float sX, const float sY, sf::Image* img);
	void RenderFlat(const float sX, const float sY, sf::Image* img);
	void RenderCellShading(const float sX, const float sY, sf::Image* img, const sf::Image* borderImg);
	void RenderCrayon(const float sX, const float sY, sf::Image* img, const sf::Image* crayonImg);
	void CheckPostProcessing(sf::Image* img);
	void AddFilterToPostProcessingByString(const std::string& filterString);
	void ApplyFilter(const FilterConstants& filterConstant, sf::Image* img, sf::Texture* texture);
};