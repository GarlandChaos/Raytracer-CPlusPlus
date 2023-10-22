#include "World.h"

#define COLOR_CONVERSION_MAX_VALUE 255.999

World::World() 
{
	light.position = glm::vec3(10.0, 40.0, -20.0);
}

World::~World() { }

void World::AddObject(Sphere* obj) 
{
	objects.push_back(obj);
}

void World::RenderOrtogonal(const float sX, const float sY, sf::Image* img) 
{
	Ray ray;
	ray.direction = glm::vec3(0.0, 0.0, -1.0);

	Light light;
	light.position = glm::vec3(0.0, -300.0, -100.0);

	for (int j = 0; j < sY; j++) 
	{
		for (int i = 0; i < sX; i++) 
		{
			sf::Color color = img->getPixel(i, j);
			float closerDist = NULL;

			glm::vec3 hitPoint;
			glm::vec3 hitPointNormal;
			float x = i - (0.5 * sX);
			float y = j - (0.5 * sY);

			ray.origin = glm::vec3(x, y, -1.0);
			
			int depth = 0;
			int id = -1;

			glm::vec3 vecColor;
			vecColor = raytracer.Raytrace(ray, objects, light, hitPoint, hitPointNormal, Ia, ambientColor, depth, maxDepth, id);
			
			int r = (int)glm::floor(glm::min(1.0f, glm::max(vecColor.x, 0.0f)) * COLOR_CONVERSION_MAX_VALUE);
			int g = (int)glm::floor(glm::min(1.0f, glm::max(vecColor.y, 0.0f)) * COLOR_CONVERSION_MAX_VALUE);
			int b = (int)glm::floor(glm::min(1.0f, glm::max(vecColor.z, 0.0f)) * COLOR_CONVERSION_MAX_VALUE);

			color.r = r;
			color.g = g;
			color.b = b;
			
			img->setPixel(i, j, color);
		}
	}
}

void World::RenderPerspective(const float sX, const float sY, sf::Image* img) 
{
	Ray ray;

	float invWidth = 1 / float(sX);
	float invHeight = 1 / float(sY);
	float aspectratio = sX / float(sY);
	float angle = tan(0.5 * fov * glm::pi<double>() / 180.0);

	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			sf::Color color = img->getPixel(i, j);
			float closerDist = NULL;

			glm::vec3 hitPoint;
			glm::vec3 hitPointNormal;
			
			ray.origin = glm::vec3(eye.x, eye.y, eye.z);

			float x = (2 * ((i + 0.5) * invWidth) - 1) * angle * aspectratio;
			float y = (1 - 2 * ((j + 0.5) * invHeight)) * angle;

			ray.direction = glm::vec3(x, y, -1);
			ray.direction = glm::normalize(ray.direction);

			int depth = 0;
			int id = -1;

			glm::vec3 vecColor;
			vecColor = raytracer.Raytrace(ray, objects, light, hitPoint, hitPointNormal, Ia, ambientColor, depth, maxDepth, id);
			
			int r = (int)glm::floor(glm::min(1.0f, glm::max(vecColor.x, 0.0f)) * COLOR_CONVERSION_MAX_VALUE);
			int g = (int)glm::floor(glm::min(1.0f, glm::max(vecColor.y, 0.0f)) * COLOR_CONVERSION_MAX_VALUE);
			int b = (int)glm::floor(glm::min(1.0f, glm::max(vecColor.z, 0.0f)) * COLOR_CONVERSION_MAX_VALUE);

			color.r = r;
			color.g = g;
			color.b = b;

			img->setPixel(i, j, color);
		}
	}
}

void World::RenderFlat(const float sX, const float sY, sf::Image* img) 
{
	Ray ray;

	float invWidth = 1 / float(sX);
	float invHeight = 1 / float(sY);
	float aspectratio = sX / float(sY);
	float angle = tan(0.5 * fov * glm::pi<double>() / 180.0);

	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			sf::Color color = img->getPixel(i, j);
			float closerDist = NULL;

			glm::vec3 hitPoint;
			glm::vec3 hitPointNormal;

			ray.origin = glm::vec3(0, 0, 0);

			float x = (2 * ((i + 0.5) * invWidth) - 1) * angle * aspectratio;
			float y = (1 - 2 * ((j + 0.5) * invHeight)) * angle;

			ray.direction = glm::vec3(x, y, -1);
			ray.direction = glm::normalize(ray.direction);

			int depth = 0;
			int id = -1;

			glm::vec3 vecColor;
			vecColor = raytracer.RaytraceFlat(ray, objects, light, hitPoint, hitPointNormal, Ia, ambientColor, depth, id);

			int r = (int)glm::floor(vecColor.x * COLOR_CONVERSION_MAX_VALUE);
			int g = (int)glm::floor(vecColor.y * COLOR_CONVERSION_MAX_VALUE);
			int b = (int)glm::floor(vecColor.z * COLOR_CONVERSION_MAX_VALUE);

			color.r = r;
			color.g = g;
			color.b = b;

			img->setPixel(i, j, color);
		}
	}
}

void World::RenderCellShading(const float sX, const float sY, sf::Image* img, const sf::Image* borderImg) 
{
	Ray ray;

	float invWidth = 1 / float(sX);
	float invHeight = 1 / float(sY);
	float aspectratio = sX / float(sY);
	float angle = tan(0.5 * fov * glm::pi<double>() / 180.0);

	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			sf::Color color = img->getPixel(i, j);

			sf::Color borderColor = borderImg->getPixel(i, j);
			
			float closerDist = NULL;

			glm::vec3 hitPoint;
			glm::vec3 hitPointNormal;

			ray.origin = glm::vec3(0, 0, 0);

			float x = (2 * ((i + 0.5) * invWidth) - 1) * angle * aspectratio;
			float y = (1 - 2 * ((j + 0.5) * invHeight)) * angle;

			ray.direction = glm::vec3(x, y, -1);
			ray.direction = glm::normalize(ray.direction);

			int depth = 0;
			int id = -1;

			glm::vec3 vecColor;
			vecColor = raytracer.RaytraceCellShading(ray, objects, light, hitPoint, hitPointNormal, Ia, ambientColor, depth, id, borderColor);

			int r = (int)glm::floor(vecColor.x * COLOR_CONVERSION_MAX_VALUE);
			int g = (int)glm::floor(vecColor.y * COLOR_CONVERSION_MAX_VALUE);
			int b = (int)glm::floor(vecColor.z * COLOR_CONVERSION_MAX_VALUE);

			color.r = r;
			color.g = g;
			color.b = b;

			img->setPixel(i, j, color);
		}
	}
}

void World::RenderCrayon(const float sX, const float sY, sf::Image* img, const sf::Image* crayonImg) 
{
	Ray ray;

	float invWidth = 1 / float(sX);
	float invHeight = 1 / float(sY);
	float aspectratio = sX / float(sY);
	float angle = tan(0.5 * fov * glm::pi<double>() / 180.0);

	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			sf::Color color = img->getPixel(i, j);

			sf::Color crayonColor = crayonImg->getPixel(i, j);

			float closerDist = NULL;

			glm::vec3 hitPoint;
			glm::vec3 hitPointNormal;

			ray.origin = glm::vec3(0, 0, 0);

			float x = (2 * ((i + 0.5) * invWidth) - 1) * angle * aspectratio;
			float y = (1 - 2 * ((j + 0.5) * invHeight)) * angle;

			ray.direction = glm::vec3(x, y, -1);
			ray.direction = glm::normalize(ray.direction);

			int depth = 0;
			int id = -1;

			glm::vec3 vecColor;
			vecColor = raytracer.RaytraceCrayon(ray, objects, light, hitPoint, hitPointNormal, Ia, ambientColor, depth, id, crayonColor);

			int r = (int)glm::floor(vecColor.x * COLOR_CONVERSION_MAX_VALUE);
			int g = (int)glm::floor(vecColor.y * COLOR_CONVERSION_MAX_VALUE);
			int b = (int)glm::floor(vecColor.z * COLOR_CONVERSION_MAX_VALUE);

			color.r = r;
			color.g = g;
			color.b = b;

			img->setPixel(i, j, color);
		}
	}
}

void World::CheckPostProcessing(sf::Image* img)
{
	if (postProcessing.size() != NULL) 
	{
		for (int i = 0; i < postProcessing.size(); i++) 
		{
			if (postProcessing[i] == FilterConstants::SOBEL)
			{
				sf::Image* flatImg = new sf::Image();
				flatImg->create(sX, sY);
				RenderFlat(sX, sY, flatImg);

				sf::Image* imgSobel = new sf::Image();
				imgSobel->create(sX, sY);
				imgSobel = filter.Sobel(flatImg);

				sf::Image* imgSobel2 = new sf::Image();
				imgSobel2->create(sX, sY);
				imgSobel2 = filter.Sobel2(flatImg);

				sf::Image* imgSobelComplete;
				imgSobelComplete = new sf::Image();
				imgSobelComplete->create(sX, sY);
				imgSobelComplete = filter.MergeSobel(imgSobel, imgSobel2);
				for (int i = 0; i < sX; i++) 
				{
					for (int j = 0; j < sY; j++) 
					{
						img->setPixel(i, j, imgSobelComplete->getPixel(i, j));
					}
				}
			}
			else if (postProcessing[i] == FilterConstants::MOTION_BLUR)
			{
				filter.Blur(img);
			}
			else if (postProcessing[i] == FilterConstants::HDR)
			{
				filter.Hdr(img);
			}
			else if (postProcessing[i] == FilterConstants::HDR2)
			{
				filter.Hdr2(img);
			}
			else if (postProcessing[i] == FilterConstants::CELL_SHADING) 
			{
				sf::Image* flatImg = new sf::Image();
				flatImg->create(sX, sY);
				RenderFlat(sX, sY, flatImg);

				sf::Image* imgSobel = new sf::Image();
				imgSobel->create(sX, sY);
				imgSobel = filter.Sobel(flatImg);

				sf::Image* imgSobel2 = new sf::Image();
				imgSobel2->create(sX, sY);
				imgSobel2 = filter.Sobel2(flatImg);

				sf::Image* imgSobelComplete = new sf::Image();
				imgSobelComplete->create(sX, sY);
				imgSobelComplete = filter.MergeSobel(imgSobel, imgSobel2);

				RenderCellShading(sX, sY, img, imgSobelComplete);
			}
			else if (postProcessing[i] == FilterConstants::CRAYON)
			{
				sf::Image* perlinImg = new sf::Image();
				perlinImg->create(sX, sY);
				filter.PerlinNoise(perlinImg);
				perlinImg->saveToFile("perlin.png");

				RenderFlat(sX, sY, img);

				sf::Image* flatImg = new sf::Image();
				flatImg->create(sX, sY);
				RenderFlat(sX, sY, flatImg);

				sf::Image* imgSobel = new sf::Image();
				imgSobel->create(sX, sY);
				imgSobel = filter.Sobel(img);

				sf::Image* imgSobel2 = new sf::Image();
				imgSobel2->create(sX, sY);
				imgSobel2 = filter.Sobel2(img);

				sf::Image* imgSobelComplete = new sf::Image();
				imgSobelComplete->create(sX, sY);
				imgSobelComplete = filter.MergeSobel(imgSobel, imgSobel2);

				sf::Image* imgNoise = new sf::Image();
				imgNoise->create(sX, sY);
				imgNoise = filter.MixPerlinNoise(imgSobelComplete, perlinImg, sf::Color::Red);

				sf::Image* sobelNoise = new sf::Image();
				sobelNoise->create(sX, sY);
				sobelNoise = filter.SobelNoise(imgNoise);

				sf::Image* imgCrayon = new sf::Image();
				imgCrayon->create(sX, sY);
				imgCrayon = filter.Crayon(flatImg, perlinImg, sobelNoise);

				RenderCrayon(sX, sY, img, imgCrayon);
			}
		}
	}
}

void World::AddFilterToPostProcessingByString(const std::string& filterString)
{
	if (filterString == "hdr")
	{
		postProcessing.push_back(FilterConstants::HDR);
	}
	else if (filterString == "hdr2")
	{
		postProcessing.push_back(FilterConstants::HDR2);
	}
	else if (filterString == "motionBlur")
	{
		postProcessing.push_back(FilterConstants::MOTION_BLUR);
	}
	else if (filterString == "cellShading")
	{
		postProcessing.push_back(FilterConstants::CELL_SHADING);
	}
	else if (filterString == "crayon")
	{
		postProcessing.push_back(FilterConstants::CRAYON);
	}
	else if (filterString == "sobel")
	{
		postProcessing.push_back(FilterConstants::SOBEL);
	}
}

void World::ApplyFilter(const FilterConstants& filterConstant, sf::Image* img, sf::Texture* texture)
{
	postProcessing.clear();
	postProcessing.push_back(filterConstant);
	CheckPostProcessing(img);
	texture->loadFromImage(*img);
}