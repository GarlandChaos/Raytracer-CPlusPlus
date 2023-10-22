#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Ray.h"

class Sphere 
{
private:
	const double kEpsilon = 0.0001;
public: 
	double radius = 0.0;
	glm::vec3 center = glm::vec3(0.0);
	glm::vec3 color = glm::vec3(0.0);
	glm::vec3 ka = glm::vec3(0.0435f, 0.0435f, 0.0435f);;
	glm::vec3 kd = glm::vec3(0.8f, 0.5f, 0.9f);;
	glm::vec3 ks = glm::vec3(0, 0, 0);;
	int n = 0; //shininess 
	double kr = 0.0; //reflection coefficient
	double kt = 0.0; //refraction/transmission coefficient
	int id = 0;

	Sphere();
	Sphere(const int id, const glm::vec3& center, const double radius, const glm::vec3& color, const glm::vec3& ka, const glm::vec3& kd, const glm::vec3& ks, const int n, const double kr, const double kt);
	~Sphere();

	bool Hit(const Ray& ray, glm::vec3& hitP, glm::vec3& normal) const;

};