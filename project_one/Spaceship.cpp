#include "Spaceship.h"


Spaceship::Spaceship()
{
	state.position = glm::vec4(20.0f, 20.0f, 0.0f, 1.0f);
	state.v = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	update = false;
}


Spaceship::~Spaceship()
{
}

void Spaceship::draw(void)
{

}
void Spaceship::createSpaceship(State st, Derivative der)
{
	state = st;
	derivative = der;
	update = true;
	float colours[] = {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
	};
	float points[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		-1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,

		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,

		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f, 0.0f,
	};
	glGenBuffers(1, &pyramid_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo);
	glBufferData(GL_ARRAY_BUFFER, 54 * sizeof (float), points, GL_STATIC_DRAW);
	glGenBuffers(1, &pyramid_colours_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_colours_vbo);
	glBufferData(GL_ARRAY_BUFFER, 54 * sizeof (float), colours, GL_STATIC_DRAW);
	glGenVertexArrays(1, &pyramid_vao);
	glBindVertexArray(pyramid_vao);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_vbo);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, pyramid_colours_vbo);
	glVertexAttribPointer(glGetAttribLocation(shader_program, "vertex_colour"), 3, GL_FLOAT, GL_FALSE, 0, NULL);
	/* Enable the vertex attributes as they are DISABLED by default. This means the colours would be off and you get a black object! */
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
}