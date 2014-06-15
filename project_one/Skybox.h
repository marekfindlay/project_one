#ifndef SKYBOX_HPP
#define SKYBOX_HPP
class Skybox;
#include "stdafx.h"
class Skybox
{
public:
	Skybox();
	~Skybox();
	GLuint skybox_vao;
	GLuint skybox_vbo;
	void createCubeMap(const char* front,
		const char* back,
		const char* top,
		const char* bottom,
		const char* left,
		const char* right,
		GLuint* tex_cube);
private:
	bool loadCubeMapSide(
		GLuint texture, GLenum side_target, const char* file_name
		);
};
#endif
