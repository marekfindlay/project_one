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

	

	//// finally, create the buffers and bind the data to them
	//std::vector<unsigned short> indices;
	//std::vector<glm::vec3> indexed_vertices;
	//std::vector<glm::vec2> indexed_uvs;
	//std::vector<glm::vec3> indexed_normals;
	//indexVBO(v, t, n, indices, indexed_vertices, indexed_uvs, indexed_normals);


	//glGenBuffers(1, &vertexbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);


	//glGenBuffers(1, &uvbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	//glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);


	//glGenBuffers(1, &normalbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	//glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	//// Generate a buffer for the indices as well 
	//glGenBuffers(1, &elementbuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);


	//// store the number of indices for later use
	//size = indices.size();

	//// clean up after us
	//indexed_normals.clear();
	//indexed_uvs.clear();
	//indexed_vertices.clear();
	//indices.clear();
	//n.clear();
	//v.clear();
	//t.clear();
}