#version 400
in vec3 vertex_position;
in vec2 vt;

out vec2 texture_coordinates;

uniform mat4 view_matrix, proj_matrix;

void main() {
	gl_Position = proj_matrix * view_matrix * vec4 (vertex_position, 1.0);
	texture_coordinates = vt;
};