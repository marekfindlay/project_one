#pragma once
class Spaceship;
#include "stdafx.h"

struct State
{
	glm::vec4 position;
	glm::vec4 v;
	float mass;
};

struct Derivative
{
	glm::vec4 dposition;
	glm::vec4 dv;
};

class Spaceship
{
public:
	Spaceship();
	~Spaceship();
	void draw(void);
	void createSpaceship(State state, Derivative derivative);
	bool update; // boolean for whether or not the State and Derivative should be updated 
	State state; 
	Derivative derivative; 
	GLuint pyramid_vao = 0;
	GLuint pyramid_vbo = 0;
	GLuint pyramid_colours_vbo = 0;
	GLuint shader_program; // the shader to be used by the renderer

};

