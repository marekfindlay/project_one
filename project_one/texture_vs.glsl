#version 400
in vec3 vertex_position;
in vec2 vt;
in vec3 vertex_normal;

uniform mat4 view_matrix, proj_matrix, model_matrix;

out vec2 texture_coordinates;
out vec3 position_eye, normal_eye;

void main() {
	position_eye = vec3 (view_matrix * model_matrix * vec4 (vertex_position, 1.0));
	normal_eye = vec3 (view_matrix * model_matrix * vec4 (vertex_normal, 0.0));
	gl_Position = proj_matrix * vec4 (position_eye, 1.0);
	texture_coordinates = vt;
};