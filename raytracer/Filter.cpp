#include "Filter.h"

Filter::Filter() { }

Filter::~Filter() { }

void Filter::Blur(sf::Image* img) const
{
	double radius = 2;
	for (int j = 0; j < img->getSize().y; j++) 
	{
		for (int i = 0; i < img->getSize().x; i++) 
		{
			sf::Color color = img->getPixel(i, j);

			double r = 0;
			double g = 0;
			double b = 0;

			for (int m = 0; m < d; m++) 
			{
				for (int n = 0; n < d; n++) 
				{
					int x = i - (4 - 1 * n);
					int y = j - (4 - 1 * m);

					if (x >= 0 && 
						y >= 0 && 
						x < img->getSize().x && 
						y < img->getSize().y) 
					{
					
						sf::Color color2 = img->getPixel(x, y);
						r += color2.r * mask[m][n];
						g += color2.g * mask[m][n];
						b += color2.b * mask[m][n];
					}
				}
			}
			
			color.r = glm::min(glm::max(int(factor * r), 0), 255);
			color.g = glm::min(glm::max(int(factor * g), 0), 255);
			color.b = glm::min(glm::max(int(factor * b), 0), 255);
		
			img->setPixel(i, j, color);
		}
	}
}

void Filter::Hdr(sf::Image* img) const
{
	double intAvg = 0;
	double radius = 2;
	for (int j = 0; j < img->getSize().y; j++) 
	{
		for (int i = 0; i < img->getSize().x; i++) 
		{
			sf::Color color = img->getPixel(i, j);

			double r = 0;
			double g = 0;
			double b = 0;

			intAvg = (color.r * 0.27) + (color.g * 0.67) + (color.b * 0.06);
			intAvg = 255 - intAvg;
			double temp = alpha / intAvg;
			r = temp * color.r;
			g = temp * color.g;
			b = temp * color.b;
			r *= 255;
			g *= 255;
			b *= 255;
			
			color.r = glm::min(glm::max(int(r), 0), 255);
			color.g = glm::min(glm::max(int(g), 0), 255);
			color.b = glm::min(glm::max(int(b), 0), 255);

			img->setPixel(i, j, color);
		}
	}
}

void Filter::Hdr2(sf::Image* img) const
{
	double intAvg = 0;
	double radius = 2;
	for (int j = 0; j < img->getSize().y; j++) 
	{
		for (int i = 0; i < img->getSize().x; i++) 
		{
			sf::Color color = img->getPixel(i, j);

			double r = 0;
			double g = 0;
			double b = 0;

			int pCounter = 0;
			for (int m = 0; m < d; m++) 
			{
				for (int n = 0; n < d; n++) 
				{
					int x = i - (4 - 1 * n);
					int y = j - (4 - 1 * m);

					if (x >= 0 &&
						y >= 0 &&
						x < img->getSize().x &&
						y < img->getSize().y) 
					{
						pCounter++;
						sf::Color color2 = img->getPixel(x, y);
						intAvg += (color2.r * 0.27) + (color2.g * 0.67) + (color2.b * 0.06);
					}
				}
			}
			intAvg /= pCounter;
			intAvg = 255 - intAvg;
			double temp = alpha / intAvg;
			r = temp * color.r;
			g = temp * color.g;
			b = temp * color.b;
			r *= 255;
			g *= 255;
			b *= 255;

			color.r = glm::min(glm::max(int(r), 0), 255);
			color.g = glm::min(glm::max(int(g), 0), 255);
			color.b = glm::min(glm::max(int(b), 0), 255);

			img->setPixel(i, j, color);
		}
	}
}

sf::Image* Filter::Sobel(const sf::Image* img) const
{
	float horizontalSobel[3][3] = 
	{ 
		{ 1,2,1 },
		{ 0,0,0 },
		{ -1,-2,-1 } 
	};

	float verticalSobel[3][3] = 
	{ 
		{ 1,0,-1 },
		{ 2,0,-2 },
		{ 1,0, -1 } 
	};

	int dSobel = 3;
	int w = img->getSize().x;
	int h = img->getSize().y;

	sf::Image* img2;
	img2 = new sf::Image();

	img2->create(w, h);

	for (int x = 0; x < w; x++) 
	{
		for (int y = 0; y < h; y++) 
		{
			sf::Color color = img->getPixel(x, y);

			int r = 0;
			int g = 0;
			int b = 0;

			int magX_r = 0.0;
			int magX_g = 0.0;
			int magX_b = 0.0;

			int magY_r = 0.0;
			int magY_g = 0.0;
			int magY_b = 0.0;

			for (int filterX = 0; filterX < dSobel; filterX++) 
			{
				for (int filterY = 0; filterY < dSobel; filterY++) 
				{
					int imageX = x + (filterX - 1);
					int imageY = y + (filterY - 1);
					if (imageX >= 0 &&
						imageY >= 0 &&
						imageX < w &&
						imageY < h) 
					{

						sf::Color color2 = img->getPixel(imageX, imageY);
						int cR = (int)color2.r;
						int cG = (int)color2.g;
						int cB = (int)color2.b;

						magX_r += cR * horizontalSobel[filterY][filterX];
						magX_g += cG * horizontalSobel[filterY][filterX];
						magX_b += cB * horizontalSobel[filterY][filterX];

						magY_r += cR * verticalSobel[filterY][filterX];
						magY_g += cG * verticalSobel[filterY][filterX];
						magY_b += cB * verticalSobel[filterY][filterX];
					}
				}
			}

			magX_r = glm::clamp(magX_r, 0, 255);
			magX_g = glm::clamp(magX_g, 0, 255);
			magX_b = glm::clamp(magX_b, 0, 255);

			magY_r = glm::clamp(magY_r, 0, 255);
			magY_g = glm::clamp(magY_g, 0, 255);
			magY_b = glm::clamp(magY_b, 0, 255);


			double magR = sqrt(pow(magX_r, 2) + pow(magY_r, 2));
			double magG = sqrt(pow(magX_g, 2) + pow(magY_g, 2));
			double magB = sqrt(pow(magX_b, 2) + pow(magY_b, 2));

			color.r = glm::clamp((int)sqrt(pow(magX_r, 2) + pow(magY_r, 2)), 0, 255);
			color.g = glm::clamp((int)sqrt(pow(magX_g, 2) + pow(magY_g, 2)), 0, 255);
			color.b = glm::clamp((int)sqrt(pow(magX_b, 2) + pow(magY_b, 2)), 0, 255);

			img2->setPixel(x, y, color);
		}
	}
	return img2;
}

sf::Image* Filter::Sobel2(const sf::Image* img) const
{
	float horizontalSobel[3][3] = 
	{ 
		{ -1,-2,-1 },
		{ 0,0,0 },
		{ 1,2,1 } 
	};

	float verticalSobel[3][3] = 
	{ 
		{ -1,0,1 },
		{ -2,0,2 },
		{ -1,0, 1 } 
	};

	int dSobel = 3;
	int w = img->getSize().x;
	int h = img->getSize().y;

	sf::Image* img2 = new sf::Image();
	img2->create(w, h);

	for (int x = 0; x < w; x++) 
	{
		for (int y = 0; y < h; y++) 
		{
			sf::Color color = img->getPixel(x, y);

			int r = 0;
			int g = 0;
			int b = 0;

			int magX_r = 0.0;
			int magX_g = 0.0;
			int magX_b = 0.0;

			int magY_r = 0.0;
			int magY_g = 0.0;
			int magY_b = 0.0;

			for (int filterX = 0; filterX < dSobel; filterX++) 
			{
				for (int filterY = 0; filterY < dSobel; filterY++) 
				{
					int imageX = x + (filterX - 1);
					int imageY = y + (filterY - 1);
					if (imageX >= 0 &&
						imageY >= 0 &&
						imageX < w &&
						imageY < h) 
					{

						sf::Color color2 = img->getPixel(imageX, imageY);
						int cR = (int)color2.r;
						int cG = (int)color2.g;
						int cB = (int)color2.b;

						magX_r += cR * horizontalSobel[filterY][filterX];
						magX_g += cG * horizontalSobel[filterY][filterX];
						magX_b += cB * horizontalSobel[filterY][filterX];

						magY_r += cR * verticalSobel[filterY][filterX];
						magY_g += cG * verticalSobel[filterY][filterX];
						magY_b += cB * verticalSobel[filterY][filterX];
					}
				}
			}

			magX_r = glm::clamp(magX_r, 0, 255);
			magX_g = glm::clamp(magX_g, 0, 255);
			magX_b = glm::clamp(magX_b, 0, 255);

			magY_r = glm::clamp(magY_r, 0, 255);
			magY_g = glm::clamp(magY_g, 0, 255);
			magY_b = glm::clamp(magY_b, 0, 255);

			double magR = sqrt(pow(magX_r, 2) + pow(magY_r, 2));
			double magG = sqrt(pow(magX_g, 2) + pow(magY_g, 2));
			double magB = sqrt(pow(magX_b, 2) + pow(magY_b, 2));

			color.r = glm::clamp((int)sqrt(pow(magX_r, 2) + pow(magY_r, 2)), 0, 255);
			color.g = glm::clamp((int)sqrt(pow(magX_g, 2) + pow(magY_g, 2)), 0, 255);
			color.b = glm::clamp((int)sqrt(pow(magX_b, 2) + pow(magY_b, 2)), 0, 255);

			img2->setPixel(x, y, color);
		}
	}
	return img2;
}

sf::Image* Filter::MergeSobel(const sf::Image* img, const sf::Image* img2) const
{
	int sX = img->getSize().x;
	int sY = img->getSize().y;

	sf::Image* sobelImg = new sf::Image();
	sobelImg->create(sX, sY);

	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			sf::Color color = img->getPixel(i, j);
			sf::Color color2 = img2->getPixel(i, j);

			if (color != sf::Color::Black || color2 != sf::Color::Black) 
			{
				sobelImg->setPixel(i, j, sf::Color::White);
			}
			else 
			{
				sobelImg->setPixel(i, j, sf::Color::Black);
			}
		}
	}
	return sobelImg;
}

void Filter::PerlinNoise(sf::Image* img) const
{
	noise::module::Perlin myModule;
	int sX = img->getSize().x;
	int sY = img->getSize().y;
	sf::Color color;
	char *data = new char[sX * sY * 4];
	double xFactor = 1.0f / (sX - 1);
	double yFactor = 1.0f / (sY - 1);
	float a = 10;
	float b = 1;
	int limit = 240;

	//Reference: https://cmaher.github.io/posts/working-with-simplex-noise/
	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			double x = xFactor * i;
			double y = yFactor * j;
			float sum = 0.0f;
			double freq = a;
			float scale = b;

			for (int oct = 0; oct < 4; oct++) 
			{
				double val = 0.0;
				val = myModule.GetValue(x * freq, y * freq, freq) / scale;
				sum += val;
				float result = (sum + 1.0f) / 2.0f;

				data[((j * sX + i) * 4) + oct] = (char)(result * 255.0f);

				freq *= 2.0f;   
				scale *= b;
			}
			
			if ((int)(data[((j * sX + i) * 4) + 3]) + 128 > limit)
			{
				img->setPixel(i, j, sf::Color::White);
			}
		}
	}
}

sf::Image* Filter::MixPerlinNoise(const sf::Image* img, const sf::Image* perlin, const sf::Color colorNoise) const
{
	int sX = img->getSize().x;
	int sY = img->getSize().y;

	sf::Image* noiseImg = new sf::Image();
	noiseImg->create(sX, sY);

	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			sf::Color color = img->getPixel(i, j);
			sf::Color color2 = perlin->getPixel(i, j);

			int medium = color2.r + color2.g + color2.b;
			medium /= 3;

			if (medium > 230) 
			{
				color.r /= 255;
				color.g /= 255;
				color.b /= 255;

				color2.r /= 255;
				color2.g /= 255;
				color2.b /= 255;

				color.r *= color2.r;
				color.g *= color2.g;
				color.b *= color2.b;

				color.r *= 255;
				color.g *= 255;
				color.b *= 255;

				color = colorNoise;
			}
			
			noiseImg->setPixel(i, j, color);
		}
	}
	return noiseImg;
}

sf::Image* Filter::SobelNoise(const sf::Image* sobel) const 
{
	int sX = sobel->getSize().x;
	int sY = sobel->getSize().y;

	sf::Image* sobelN = new sf::Image();
	sobelN->create(sX, sY);

	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			sf::Color sobelColor = sobel->getPixel(i, j);

			if (sobelColor != sf::Color::White) 
			{
				sobelN->setPixel(i, j, sf::Color::Black);
			}
			else 
			{
				sobelN->setPixel(i, j, sf::Color::White);
			}
		}
	}
	return sobelN;
}

sf::Image* Filter::Crayon(const sf::Image* render, const sf::Image* noise, const sf::Image* edge) const
{
	int sX = render->getSize().x;
	int sY = render->getSize().y;

	sf::Image* crayonImg = new sf::Image();
	crayonImg->create(sX, sY);

	float fFill = 0.2f;

	for (int i = 0; i < sX; i++) 
	{
		for (int j = 0; j < sY; j++) 
		{
			sf::Color noiseColor = noise->getPixel(i, j);
			float offset = (int)noiseColor.r * 0.02f;
			sf::Color color = render->getPixel((i + (int)offset) % sX, (j + (int)offset) % sY);
			sf::Color edgeColor = edge->getPixel((i + (int)offset) % sX, (j + (int)offset) % sY);

			if (edgeColor.r > 0) 
			{
				crayonImg->setPixel(i, j, sf::Color::Black);
			}
			else 
			{
				int r = 1 - (1 - color.r) * (noiseColor.r * fFill);
				int g = 1 - (1 - color.g) * (noiseColor.g * fFill);
				int b = 1 - (1 - color.b) * (noiseColor.b * fFill);

				sf::Color crayonColor;
				crayonColor.r = r;
				crayonColor.g = g;
				crayonColor.b = b;

				crayonImg->setPixel(i, j, crayonColor);
			}
		}
	}
	return crayonImg;
}