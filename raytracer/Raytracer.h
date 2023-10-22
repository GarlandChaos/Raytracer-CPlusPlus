#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Ray.h"
#include "Sphere.h"
#include "Light.h"

class Raytracer 
{
public:

	Raytracer();
	~Raytracer();
	glm::vec3 Raytrace(const Ray& ray, std::vector<Sphere*>& objects, const Light& light, glm::vec3& hitPoint, glm::vec3& hitPointNormal, const float Ia, const glm::vec3& ambColor, const int depth, const int maxDepth, const int id) const;
	glm::vec3 RaytraceFlat(const Ray& ray, std::vector<Sphere*>& objects, const Light& light, glm::vec3& hitPoint, glm::vec3& hitPointNormal, const float Ia, const glm::vec3& ambColor, const int depth, const int id) const;
	glm::vec3 RaytraceCellShading(const Ray& ray, std::vector<Sphere*>& objects, const Light& light, glm::vec3& hitPoint, glm::vec3& hitPointNormal, const float Ia, const glm::vec3& ambColor, const int depth, const int id, const sf::Color& borderColor) const;
	glm::vec3 RaytraceCrayon(const Ray& ray, std::vector<Sphere*>& objects, const Light& light, glm::vec3& hitPoint, glm::vec3& hitPointNormal, const float Ia, const glm::vec3& ambColor, const int depth, const int id, const sf::Color& borderColor) const;
	Ray Reflection(const glm::vec3& normal, const glm::vec3& rayIn, const glm::vec3& origin) const;
	Ray Refraction(const glm::vec3& normal, const glm::vec3& rayIn, const glm::vec3& origin, const double ior) const;
	glm::vec3 MixColors(const glm::vec3& localColor, const glm::vec3& reflectedColor, const double kr, const glm::vec3& refractedColor, const double krefrac) const;
	glm::vec3 ClampColor(const glm::vec3& color) const;
};