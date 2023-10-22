#include "Raytracer.h"

Raytracer::Raytracer() { }

Raytracer::~Raytracer() { }

glm::vec3 Raytracer::Raytrace(const Ray& ray, std::vector<Sphere*>& objects, const Light& light, glm::vec3& hitPoint, 
								glm::vec3& hitPointNormal, const float Ia, const glm::vec3& ambColor, const int depth, 
								const int maxDepth, const int id) const
{
	glm::vec3 color;

	double closerDist = NULL;
	Sphere* closeObj = NULL;
	glm::vec3 closeHitPoint;

	if (depth < maxDepth) 
	{
		for (int k = 0; k < objects.size(); k++) 
		{
			if (objects.at(k)->Hit(ray, hitPoint, hitPointNormal) && objects.at(k)->id != id) 
			{
				double currentDist = glm::distance(hitPoint, ray.origin);

				if (closerDist == NULL || currentDist < closerDist) 
				{
					closerDist = currentDist;
					closeObj = objects.at(k);
					closeHitPoint = hitPoint;
				}
			}
		}

		if (closeObj != NULL) //has found closest object
		{
			hitPoint = closeHitPoint;

			glm::vec3 lightDir = glm::normalize(light.position - hitPoint);

			hitPointNormal = glm::normalize(hitPointNormal);

			double r, g, b;
			glm::vec3 rayIn;
			rayIn = glm::normalize(ray.origin - hitPoint);

			Ray reflectedRay;
			reflectedRay = Reflection(hitPointNormal, rayIn, hitPoint);

			glm::vec3 V; //vector pointing to observer
			V = glm::normalize(glm::vec3(0, 0, 0) - hitPoint);

			Ray shadowRay;
			shadowRay.origin = hitPoint;
			shadowRay.direction = lightDir;
			glm::vec3 hp = hitPoint;
			glm::vec3 hpN = hitPointNormal;

			float cosAngleNormalight = glm::max(0.0f, glm::dot(hitPointNormal, lightDir));
			cosAngleNormalight = glm::clamp(cosAngleNormalight, 0.0f, 1.0f);
			float cosAngleSpecular = glm::dot(reflectedRay.direction, V);
			cosAngleSpecular = glm::clamp(cosAngleSpecular, 0.0f, 1.0f);

			glm::vec3 ambient(closeObj->ka.r * Ia * ambColor.r,
							  closeObj->ka.g * Ia * ambColor.g,
				              closeObj->ka.b * Ia * ambColor.b);

			glm::vec3 diffuse(closeObj->kd.r * light.intensity * light.color.r * cosAngleNormalight,
							  closeObj->kd.g * light.intensity * light.color.g * cosAngleNormalight,
							  closeObj->kd.b * light.intensity * light.color.b * cosAngleNormalight);

			glm::vec3 specular(closeObj->ks.r * glm::pow(cosAngleSpecular, closeObj->n),
							   closeObj->ks.g * glm::pow(cosAngleSpecular, closeObj->n),
							   closeObj->ks.b * glm::pow(cosAngleSpecular, closeObj->n));

			glm::vec3 original_hp = hitPoint;

			bool collided = false;
			double d = 0;
			double f = 0;
			float distanceLight = 0;
			
			for (int m = 0; m < objects.size(); m++) //checks if there is an object between the light
			{
				if (objects.at(m)->Hit(shadowRay, hp, hpN) && objects.at(m)->id != closeObj->id) //if yes, add shadow
				{
					d = glm::distance(original_hp, hp);
					distanceLight = glm::distance(light.position, hp);
					if (d < distanceLight) 
					{
						collided = true;
						f = 1 / (d * d);
						break;
					}
				}
			}

			color = ambient + diffuse + specular;
			color *= closeObj->color;

			if (collided) 
			{
				color *= (1 - f) * cosAngleNormalight;
			}

			glm::vec3 reflectedColor(0,0,0);

			if (closeObj->kr > 0 && closeObj->kt == 0.0) 
			{
				reflectedColor = Raytrace(reflectedRay, objects, light, hitPoint, hitPointNormal, Ia, ambColor, depth + 1, maxDepth, closeObj->id);
			}

			glm::vec3 refractedColor(0, 0, 0);
			Ray refractedRay;
			
			if (closeObj->kt > 0) 
			{
				refractedRay = Refraction(hitPointNormal, rayIn, hitPoint, 1.325);
				refractedColor = Raytrace(refractedRay, objects, light, hitPoint, hitPointNormal, Ia, ambColor, depth + 1, maxDepth, closeObj->id);
			}
			
			color = MixColors(color, reflectedColor, closeObj->kr, refractedColor, closeObj->kt);
		}
		else 
		{
			color.r = 0.5;
			color.g = 0.5;
			color.b = 0.5;
		}
	}
	else 
	{
		color.r = 0.5;
		color.g = 0.5;
		color.b = 0.5;
	}
	return color;
}

glm::vec3 Raytracer::RaytraceFlat(const Ray& ray, std::vector<Sphere*>& objects, const Light& light, glm::vec3& hitPoint, 
									glm::vec3& hitPointNormal, const float Ia, const glm::vec3& ambColor, const int depth, const int id) const
{
	glm::vec3 color;

	double closerDist = NULL;
	Sphere* closeObj = NULL;
	glm::vec3 closeHitPoint;

	if (depth < 1)
	{
		for (int k = 0; k < objects.size(); k++)
		{
			if (objects.at(k)->Hit(ray, hitPoint, hitPointNormal) && objects.at(k)->id != id)
			{
				double currentDist = glm::distance(hitPoint, ray.origin);

				if (closerDist == NULL || currentDist < closerDist)
				{
					closerDist = currentDist;
					closeObj = objects.at(k);
					closeHitPoint = hitPoint;
				}
			}
		}

		if (closeObj != NULL) //has found closest object
		{

			color = closeObj->color;
			color = ClampColor(color);

			glm::vec3 reflectedColor(0, 0, 0);
			glm::vec3 refractedColor(0, 0, 0);

			color = MixColors(color, reflectedColor, closeObj->kr, refractedColor, closeObj->kt);
		}
		else
		{
			color.r = 0.5;
			color.g = 0.5;
			color.b = 0.5;
		}
	}
	else
	{
		color.r = 0.5;
		color.g = 0.5;
		color.b = 0.5;
	}
	return color;
}

glm::vec3 Raytracer::RaytraceCellShading(const Ray& ray, std::vector<Sphere*>& objects, const Light& light, glm::vec3& hitPoint, 
											glm::vec3& hitPointNormal, const float Ia, const glm::vec3& ambColor, const int depth, 
											const int id, const sf::Color& borderColor) const
{
	glm::vec3 color;

	double closerDist = NULL;
	Sphere* closeObj = NULL;
	glm::vec3 closeHitPoint;

	if (depth < 1)
	{
		for (int k = 0; k < objects.size(); k++)
		{
			if (objects.at(k)->Hit(ray, hitPoint, hitPointNormal) && objects.at(k)->id != id)
			{
				double currentDist = glm::distance(hitPoint, ray.origin);

				if (closerDist == NULL || currentDist < closerDist)
				{
					closerDist = currentDist;
					closeObj = objects.at(k);
					closeHitPoint = hitPoint;
				}
			}
		}

		if (closeObj != NULL) //has found closest object
		{
			hitPoint = closeHitPoint;

			glm::vec3 lightDir = glm::normalize(light.position - hitPoint);

			hitPointNormal = glm::normalize(hitPointNormal);

			double r, g, b;
			glm::vec3 rayIn;
			rayIn = glm::normalize(ray.origin - hitPoint);

			Ray reflectedRay;
			reflectedRay = Reflection(hitPointNormal, rayIn, hitPoint);

			glm::vec3 V; //vetor em direção ao observador
			V = glm::normalize(glm::vec3(0, 0, 0) - hitPoint);

			Ray shadowRay;
			shadowRay.origin = hitPoint;
			shadowRay.direction = lightDir;
			glm::vec3 hp = hitPoint;
			glm::vec3 hpN = hitPointNormal;

			double cosAngleNormalight = glm::max(0.0f, glm::dot(hitPointNormal, lightDir));
			//cout << cosAngleNormalight << endl;
			double cosAngleSpecular = glm::dot(reflectedRay.direction, V);

			glm::vec3 ambient(closeObj->ka.r * Ia * ambColor.r,
				closeObj->ka.g * Ia * ambColor.g,
				closeObj->ka.b * Ia * ambColor.b);

			glm::vec3 diffuse(closeObj->kd.r * light.intensity * light.color.r * cosAngleNormalight,
				closeObj->kd.g * light.intensity * light.color.g * cosAngleNormalight,
				closeObj->kd.b * light.intensity * light.color.b * cosAngleNormalight);

			glm::vec3 specular(closeObj->ks.r * glm::pow(cosAngleSpecular, closeObj->n),
				closeObj->ks.g * glm::pow(cosAngleSpecular, closeObj->n),
				closeObj->ks.b * glm::pow(cosAngleSpecular, closeObj->n));

			if (borderColor == sf::Color::White)
			{
				color = glm::vec3(0, 0, 0);
			}
			else
			{
				if (cosAngleNormalight > 0.4)
				{
					color = specular;
					color += closeObj->color;
					color = ClampColor(color);
				}
				else
				{
					color = closeObj->color;
					color *= 0.9f;
					color = ClampColor(color);
				}
			}
		}
		else
		{
			color.r = 0.5;
			color.g = 0.5;
			color.b = 0.5;
		}
	}
	else
	{
		color.r = 0.5;
		color.g = 0.5;
		color.b = 0.5;
	}
	return color;
}

glm::vec3 Raytracer::RaytraceCrayon(const Ray& ray, std::vector<Sphere*>& objects, const Light& light, glm::vec3& hitPoint,
									glm::vec3& hitPointNormal, const float Ia, const glm::vec3& ambColor, const int depth,
									const int id, const sf::Color& crayonColor) const
{
	glm::vec3 color;

	double closerDist = NULL;
	Sphere* closeObj = NULL;
	glm::vec3 closeHitPoint;

	if (depth < 1)
	{
		for (int k = 0; k < objects.size(); k++)
		{
			if (objects.at(k)->Hit(ray, hitPoint, hitPointNormal) && objects.at(k)->id != id)
			{
				double currentDist = glm::distance(hitPoint, ray.origin);

				if (closerDist == NULL || currentDist < closerDist) {
					closerDist = currentDist;
					closeObj = objects.at(k);
					closeHitPoint = hitPoint;
				}
			}
		}

		if (closeObj != NULL) //has found closest object
		{
			hitPoint = closeHitPoint;

			glm::vec3 lightDir = glm::normalize(light.position - hitPoint);

			hitPointNormal = glm::normalize(hitPointNormal);

			double r, g, b;
			glm::vec3 rayIn;
			rayIn = glm::normalize(ray.origin - hitPoint);

			if (crayonColor != sf::Color(1, 1, 1))
			{
				if ((int)crayonColor.r == 0 && (int)crayonColor.g == 0 && (int)crayonColor.b == 0)
				{
					color = glm::vec3(0, 0, 0);
				}
				else
				{
					float modifier = 1.1;

					float cR = closeObj->color.r;
					float cG = closeObj->color.g;
					float cB = closeObj->color.b;

					int maxChannel = glm::max(glm::max(cR, cG), cB);

					if (maxChannel == cR)
					{
						cR = closeObj->color.r * modifier;
					}
					else if (maxChannel == cG)
					{
						cG = closeObj->color.g * modifier;
					}
					else if (maxChannel == cB)
					{
						cB = closeObj->color.b * modifier;

					}
					color = glm::vec3(1, 1, 1);
				}
			}
			else
			{
				color = closeObj->color;
			}
		}
		else
		{
			if (crayonColor != sf::Color(1, 1, 1) && crayonColor != sf::Color(27, 27, 27))
			{
				color = glm::vec3(0, 0, 0);
			}
			else
			{
				color.r = 1;
				color.g = 1;
				color.b = 1;
			}

		}
	} //end depth
	else
	{
		color.r = 1;
		color.g = 1;
		color.b = 1;
	}
	return color;
}

Ray Raytracer::Reflection(const glm::vec3& normal, const glm::vec3& rayIn, const glm::vec3& origin) const
{
	Ray rayOut;
	rayOut.origin = origin;

	rayOut.direction = 2.0f * normal * (glm::dot(normal, rayIn)) - rayIn;
	rayOut.direction = glm::normalize(rayOut.direction);

	return rayOut;
}

Ray Raytracer::Refraction(const glm::vec3& normal, const glm::vec3& rayIn, const glm::vec3& origin, const double ior) const
{
	Ray rayOut;
	float bias = 1e-4;
	rayOut.origin = origin + normal * bias;

	double n1 = 1; //coeficiente do ar
	double n2 = ior;
	double cos1 = glm::dot(normal, rayIn);
	glm::vec3 n = normal;

	if (cos1 < 0)
	{
		cos1 = -cos1;
	}
	else
	{
		std::swap(n1, n2);
		n = -normal;
	}

	double eta = n1 / n2;
	double k = 1 - eta * eta * (1 - cos1 * cos1);

	if (k < 0)
	{
		rayOut.direction = glm::vec3(0, 0, 0);
	}
	else
	{
		rayOut.direction = (float)eta * rayIn + (float)(eta * cos1 - sqrtf(k)) * n;
	}

	rayOut.direction = glm::normalize(rayOut.direction);
	return rayOut;
}

glm::vec3 Raytracer::MixColors(const glm::vec3& localColor, const glm::vec3& reflectedColor, const double kr, const glm::vec3& refractedColor, const double kt) const
{
	glm::vec3 mixedColor;

	double r = localColor.r* (1.0f - (kr + kt)) + reflectedColor.r * kr + refractedColor.r * kt;
	double g = localColor.g * (1.0f - (kr + kt)) + reflectedColor.g * kr + refractedColor.g * kt;
	double b = localColor.b * (1.0f - (kr + kt)) + reflectedColor.b * kr + refractedColor.b * kt;

	mixedColor = glm::vec3(r, g, b);

	return mixedColor;
}

glm::vec3 Raytracer::ClampColor(const glm::vec3& color) const
{
	glm::vec3 clamped = color;

	clamped.r = glm::min(1.0f, glm::max(color.r, 0.0f));
	clamped.g = glm::min(1.0f, glm::max(color.g, 0.0f));
	clamped.b = glm::min(1.0f, glm::max(color.b, 0.0f));

	return clamped;
}