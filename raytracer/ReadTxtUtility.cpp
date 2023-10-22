#include "ReadTxtUtility.h"

ReadTxtUtility::ReadTxtUtility() { }

ReadTxtUtility::~ReadTxtUtility() { }

void ReadTxtUtility::ReadAndBuild(const std::string& file, World& world) const
{
	std::ifstream inFile;
	inFile.open(file);

	if (!inFile) 
	{
		std::cerr << "Unable to open file datafile.txt";
		exit(1);   // call system to stop
	}

	//Camera
	float fov;

	//Lights
	int nLights;
	double posLX;
	double posLY;
	double posLZ;
	double LZcolorR;
	double LZcolorG;
	double LZcolorB;
	float intensity;

	//Objects
	int nObjects;
	int id;
	double centerX;
	double centerY;
	double centerZ;
	double radius;
	double colorR;
	double colorG;
	double colorB;
	double kaR;
	double kaG;
	double kaB;
	double kdR;
	double kdG;
	double kdB;
	double ksR;
	double ksG;
	double ksB;
	int n;
	double kr;
	double kt;

	std::string s;
	std::stringstream ss;
	while (!inFile.eof()) 
	{
		getline(inFile, s);
		if (s == "begin stage 1") 
		{
			getline(inFile, s);
			if (s == "	begin cam") 
			{
				getline(inFile, s);
				ss << s;
				ss >> s;
				if (s == "fov:") 
				{
					ss >> world.fov;
				}
				else 
				{
					std::cerr << "fov not found";
					exit(1);
				}
				getline(inFile, s);
				ss.clear();
				ss << s;
				ss >> s;
				if (s == "eyeX:") 
				{
					ss >> world.eye.x;
				}
				else 
				{
					std::cerr << "eyeX not found";
					exit(1);
				}
				getline(inFile, s);
				ss.clear();
				ss << s;
				ss >> s;
				if (s == "eyeY:") 
				{
					ss >> world.eye.y;
				}
				else 
				{
					std::cerr << "eyeY not found";
					exit(1);
				}
				getline(inFile, s);
				ss.clear();
				ss << s;
				ss >> s;
				if (s == "eyeZ:") 
				{
					ss >> world.eye.z;
				}
				else 
				{
					std::cerr << "eyeZ not found";
					exit(1);
				}
				getline(inFile, s);
				if (s == "	end cam") 
				{
					getline(inFile, s);
				}
				else 
				{
					std::cerr << "end cam not found";
					exit(1);
				}
			}
			else 
			{
				std::cerr << "cam not found";
				exit(1);
			}
			getline(inFile, s);
			ss.clear();
			ss << s;
			ss >> s;
			if (s == "nlight") 
			{
				int nLight = 0;
				ss >> nLight;
				for (int i = 0; i < nLight; i++) 
				{
					getline(inFile, s);
					ss.clear();
					ss << s;
					ss >> s;
					if (s == "begin") 
					{
						ss >> s;
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "position:") 
						{
							ss >> world.light.position.x >> world.light.position.y >> world.light.position.z;
						}
						else 
						{
							std::cerr << "light position not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "color:") 
						{
							ss >> world.light.color.r >> world.light.color.g >> world.light.color.b;
						}
						else 
						{
							std::cerr << "light color not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "intensity:") 
						{
							ss >> world.light.intensity;
						}
						else 
						{
							std::cerr << "light intensity not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "end") 
						{
							ss >> s;
							getline(inFile, s);
						}
						else 
						{
							std::cerr << "end light not found";
							exit(1);
						}
					}
					else 
					{
						std::cerr << "begin light not found";
						exit(1);
					}
				}
			}
			else 
			{
				std::cerr << "lights not found";
				exit(1);
			}
			getline(inFile, s);
			ss.clear();
			ss << s;
			ss >> s;
			if (s == "nobjects") 
			{
				int nObjects = 0;
				ss >> nObjects;
				for (int i = 0; i < nObjects; i++) 
				{
					getline(inFile, s);
					ss.clear();
					ss << s;
					ss >> s;
					if (s == "begin") 
					{
						ss >> s;
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "id:") 
						{
							ss >> id;
						}
						else 
						{
							std::cerr << "sphere id not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "radius:") 
						{
							ss >> radius;
						}
						else 
						{
							std::cerr << "sphere radius not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "color:") 
						{
							ss >> colorR >> colorG >> colorB;
						}
						else 
						{
							std::cerr << "sphere color not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "position:") 
						{
							ss >> centerX >> centerY >> centerZ;
						}
						else 
						{
							std::cerr << "sphere position not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "ambient:") 
						{
							ss >> kaR >> kaG >> kaB;
						}
						else 
						{
							std::cerr << "sphere ambient not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "diffuse:") 
						{
							ss >> kdR >> kdG >> kdB;
						}
						else 
						{
							std::cerr << "sphere diffuse not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "specular:") 
						{
							ss >> ksR >> ksG >> ksB;
						}
						else 
						{
							std::cerr << "sphere specular not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "shininess:") 
						{
							ss >> n;
						}
						else 
						{
							std::cerr << "sphere shininess not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "reflection:") 
						{
							ss >> kr;
						}
						else 
						{
							std::cerr << "sphere reflection not found";
							exit(1);
						}
						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "refraction:") 
						{
							ss >> kt;
						}
						else 
						{
							std::cerr << "sphere refraction not found";
							exit(1);
						}
						glm::vec3 center(centerX, centerY, centerZ);
						glm::vec3 color(colorR, colorG, colorB);
						glm::vec3 ka(kaR, kaG, kaB);
						glm::vec3 kd(kdR, kdG, kdB);
						glm::vec3 ks(ksR, ksG, ksB);

						Sphere* sp;
						sp = new Sphere(id, center, radius, color, ka, kd, ks, n, kr, kt);

						world.objects.push_back(sp);

						getline(inFile, s);
						ss.clear();
						ss << s;
						ss >> s;
						if (s == "end") 
						{
							ss >> s;
						}
						else 
						{
							std::cerr << "end sphere not found";
							exit(1);
						}
					}
					else 
					{
						std::cerr << "begin sphere not found";
						exit(1);
					}
				}
			}
			else 
			{
				std::cerr << "objects not found";
				exit(1);
			}
		}

		if (s == "begin stage 3") 
		{
			getline(inFile, s);
			ss.clear();
			ss << s;
			ss >> s;
			if (s == "program:") 
			{
				ss >> s;
				if (s == "rt") 
				{
					world.maxDepth = 10;
				}
				else if (s == "rc") 
				{
					world.maxDepth = 1;
				}
			}
			else 
			{
				std::cerr << "program not found";
				exit(1);
			}
		}

		if (s == "begin stage 4") 
		{
			getline(inFile, s);
			ss.clear();
			ss << s;
			ss >> s;
			if (s == "nfilter") 
			{
				int nfilter = 0;
				ss >> nfilter;
				for (int i = 0; i < nfilter; i++) 
				{
					getline(inFile, s);
					ss.clear();
					ss << s;
					ss >> s;
					if (s == "hdr2" || s == "hdr") 
					{
						ss >> world.filter.alpha;
					}
					world.AddFilterToPostProcessingByString(s);
				}
			}
			else 
			{
				std::cerr << "nfilter not found";
				exit(1);
			}
		}

		if (s == "begin stage 5") 
		{
			getline(inFile, s);
			ss.clear();
			ss << s;
			ss >> s;
			if (s == "output:") 
			{
				ss >> world.sX >> world.sY;
			}
			else 
			{
				std::cerr << "output not found";
				exit(1);
			}
		}
	}
	inFile.close();
}