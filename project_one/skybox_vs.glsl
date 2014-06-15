#version 400

in vec3 vt;
uniform mat4 proj_matrix, view_matrix;
out vec3 texcoords;

void main () {
  texcoords = vt;
  gl_Position = proj_matrix * view_matrix * vec4 (vt, 1.0);
}

