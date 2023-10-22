#include "Sphere.h"

Sphere::Sphere() { }

Sphere::Sphere(const int Id, const glm::vec3& c, const double r, const glm::vec3& clr, 
				const glm::vec3& a, const glm::vec3& d, const glm::vec3& s, const int shininess, const double ref, const double refrac)
{
	center = c;
	radius = r;
	color = clr;
	ka = a;
	kd = d;
	ks = s;
	n = shininess;
	kr = ref;
	kt = refrac;
	id = Id;
}

Sphere::~Sphere() { }

bool Sphere::Hit(const Ray& ray, glm::vec3& hitP, glm::vec3& hitNormal) const
{
	double A = 0.0;
	double B = 0.0;
	double C = 0.0;

	glm::vec3 temp = ray.origin - center;

	A = glm::dot(ray.direction, ray.direction);
	B = 2.0 * glm::dot(temp, ray.direction);
	C = glm::dot(temp, temp) - (radius * radius);

	double delta = B * B - 4.0 * A * C;

	if (delta < 0.0) return false; //don't intersects sphere

	double t0;
	double t1;
	double t;
	double e = glm::sqrt(delta);
	double denom = 2.0 * A;

	t0 = (-B - e) / denom;
	t1 = (-B + e) / denom;
	t = (-B - e) / denom;

	if (t > kEpsilon)
	{
		hitNormal = (temp + (float)t * ray.direction) / (float)radius;
		hitP = ray.origin + (float)t * ray.direction;

		return true;
	}

	t = (-B + e) / denom;

	if (t > kEpsilon)
	{
		hitNormal = (temp + (float)t * ray.direction) / (float)radius;
		hitP = ray.origin + (float)t * ray.direction;

		return true;
	}
	
	return false;
}