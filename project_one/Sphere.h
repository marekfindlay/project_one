#pragma once
#include "stdafx.h"

class Sphere
{
public:
	Sphere();
	~Sphere();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> colours;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<GLuint> elements;
	float radius;
	float theta;
	void CreateSphere();
	
	GLuint texture[1];
	double texture_ratio;
	// initiate buffers
	GLuint elementbuffer;
	GLuint normalbuffer;
	GLuint uvbuffer;
	GLuint vertexbuffer;
	
	// this variable will later be used to hold the number of indices
	int size = 0;
	GLuint getUvBuffer() { return uvbuffer; }
	GLuint getElemBuffer() { return elementbuffer; }
	GLuint getVertexBuffer() { return vertexbuffer; }
	GLuint getNormalBuffer() { return normalbuffer; }
	int getSize() { return size; }
	void LoadTexture(const char* textureName);
	void DisplaySphere(double R, GLuint texture);

private:
protected:
};

