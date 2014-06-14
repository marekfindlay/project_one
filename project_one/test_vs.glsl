#version 400
in vec3 vertex_position;
in vec3 vertex_colour;

uniform mat4 view_matrix, proj_matrix;

out vec3 colour;

void main() {
colour = vertex_colour;
  gl_Position = proj_matrix * view_matrix * vec4 (vertex_position, 1.0);
}