#pragma once
#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Light 
{
public:
	glm::vec3 position = glm::vec3(0.0f);
	float intensity = 1.0f;
	glm::vec3 color = glm::vec3(1.0f);

	Light();
	~Light();
};