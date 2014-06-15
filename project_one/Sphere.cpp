#include "Sphere.h"


Sphere::Sphere()
{
	/* set default values for radius and theta */
	radius = 5.0f;
	theta = 4.5f; 
}


Sphere::~Sphere()
{
}

void Sphere::LoadTexture(const char* textureName)
{
	
}

void Sphere::DisplaySphere(double R, GLuint texture)
{
	/* need to fill this in*/
}

void Sphere::CreateSphere()
{

	const int num_lats = int((180.0f / theta) + 1.5); //add 0.5 for casting to int with rounding
	const int num_longis = int((360.0f / theta) + 0.5);

	int vertex_index = 0;

	for (float lat = -90.0f; lat <= 90.0f; lat += theta)
	{
		for (float longi = 0; longi < 360.0f; longi += theta)
		{
			colours.push_back(glm::vec3(
				0, 
				glm::abs(glm::sin(glm::radians(lat))), 
				1 - glm::abs(glm::sin(glm::radians(lat)))));
			vertices.push_back(glm::vec3(
				radius * glm::cos(glm::radians(lat))*glm::cos(glm::radians(longi)), 
				radius * glm::sin(glm::radians(lat)), 
				radius * glm::cos(glm::radians(lat))*glm::sin(glm::radians(longi))));
			texcoords.push_back(glm::vec2(
				1.0f-longi / 360.0f,
				(lat+90.0f)/180.0f));
			normals.push_back(glm::normalize(vertices[vertex_index++]));
		}
	}
	/* define the elements for drawing triangles*/
	int tempValue = 0;
	for (int lat_index = 0; lat_index < num_lats - 1; lat_index++)
	{
		for (int long_index = 0; long_index < num_longis; long_index++)
		{
			tempValue = (lat_index * num_longis);
			/*triangle 1 of quad*/
			elements.push_back(tempValue + ((long_index + 1) % num_longis) + num_longis);
			elements.push_back(tempValue + long_index + num_longis);
			elements.push_back(tempValue + long_index);

			/*triangle 2 of quad*/
			elements.push_back(tempValue + long_index);
			elements.push_back(tempValue + ((long_index + 1) % num_longis));
			elements.push_back(tempValue + ((long_index + 1) % num_longis) + num_longis);
		}
	}
}