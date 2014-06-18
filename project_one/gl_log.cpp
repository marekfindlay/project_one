#include <time.h>
#include <stdarg.h>
#include <iostream>
#include "gl_log.h"
#include "stdafx.h"
#define GL_LOG_FILE "gl.log"
#define DATE_SIZE 26

using namespace std;

bool restart_gl_log()
{
	FILE* file;
	errno_t err;
	err = fopen_s(&file, GL_LOG_FILE, "w");
	if (!file)
	{
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE log file %s for writing\n",
			GL_LOG_FILE
			);
		return false;
	}
	time_t now = time(NULL);
	wchar_t date[DATE_SIZE];
	err = _wctime_s(date, DATE_SIZE, &now);
	fprintf(file, "GL_LOG_FILE log. local time %s\n", date);
	fclose(file);
	return true;
}

bool gl_log(const char* message, ...)
{
	va_list argptr;
	FILE* file;
	errno_t err;
	err = fopen_s(&file, GL_LOG_FILE, "a");
	if (!file)
	{
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
			);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

bool gl_log_err(const char* message, ...)
{
	va_list argptr;
	FILE* file;
	errno_t err;
	err = fopen_s(&file, GL_LOG_FILE, "a");
	if (!file)
	{
		fprintf(
			stderr,
			"ERROR: could not open GL_LOG_FILE %s file for appending\n",
			GL_LOG_FILE
			);
		return false;
	}
	va_start(argptr, message);
	vfprintf(file, message, argptr);
	va_end(argptr);
	va_start(argptr, message);
	vfprintf(stderr, message, argptr);
	va_end(argptr);
	fclose(file);
	return true;
}

void log_gl_params()
{
	GLenum params[] = {
		GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
		GL_MAX_CUBE_MAP_TEXTURE_SIZE,
		GL_MAX_DRAW_BUFFERS,
		GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
		GL_MAX_TEXTURE_IMAGE_UNITS,
		GL_MAX_TEXTURE_SIZE,
		GL_MAX_VARYING_FLOATS,
		GL_MAX_VERTEX_ATTRIBS,
		GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
		GL_MAX_VERTEX_UNIFORM_COMPONENTS,
		GL_MAX_VIEWPORT_DIMS,
		GL_STEREO,
		GL_MAX_UNIFORM_BUFFER_BINDINGS,
	};
	const char* names[] = {
		"GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
		"GL_MAX_CUBE_MAP_TEXTURE_SIZE",
		"GL_MAX_DRAW_BUFFERS",
		"GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
		"GL_MAX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_TEXTURE_SIZE",
		"GL_MAX_VARYING_FLOATS",
		"GL_MAX_VERTEX_ATTRIBS",
		"GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
		"GL_MAX_VERTEX_UNIFORM_COMPONENTS",
		"GL_MAX_VIEWPORT_DIMS",
		"GL_STEREO",
		"GL_MAX_UNIFORM_BUFFER_BINDINGS",
	};
	gl_log("GL Context Params:\n");
	char msg[256];
	// integers - only works if the order is 0-10 integer return types
	for (int i = 0; i < 10; i++)
	{
		int v = 0;
		glGetIntegerv(params[i], &v);
		gl_log("%s %i\n", names[i], v);
	}
	// others
	int v[2];
	v[0] = v[1] = 0;
	glGetIntegerv(params[10], v);
	gl_log("%s %i %i\n", names[10], v[0], v[1]);
	unsigned char s = 0;
	glGetBooleanv(params[11], &s);
	gl_log("%s %u\n", names[11], (unsigned int) s);
	gl_log("-----------------------------\n");
}

const char* GL_type_to_string(GLenum type)
{
	switch (type)
	{
	case GL_BOOL: return "bool";
	case GL_INT: return "int";
	case GL_FLOAT: return "float";
	case GL_FLOAT_VEC2: return "vec2";
	case GL_FLOAT_VEC3: return "vec3";
	case GL_FLOAT_VEC4: return "vec4";
	case GL_FLOAT_MAT2: return "mat2";
	case GL_FLOAT_MAT3: return "mat3";
	case GL_FLOAT_MAT4: return "mat4";
	case GL_SAMPLER_2D: return "sampler2D";
	case GL_SAMPLER_3D: return "sampler3D";
	case GL_SAMPLER_CUBE: return "samplerCube";
	case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
	default: break;
	}
	return "other";
}