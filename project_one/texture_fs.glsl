#version 400
in vec3 position_eye, normal_eye;
in vec2 texture_coordinates;
uniform sampler2D basic_texture;
uniform mat4 view_matrix;
out vec4 frag_colour;

// fixed point light properties
vec3 light_position_world  = vec3 (0.0, 100.0, 100.0);
vec3 Ls = vec3 (1.0, 1.0, 1.0); // white specular colour
vec3 Ld = vec3 (0.8, 0.8, 0.8); // dull white diffuse light colour
vec3 La = vec3 (0.05, 0.05, 0.05); // grey ambient colour
  
// surface reflectance
vec3 Ks = vec3 (0.0, 0.0, 0.0); // no reflect specular light
vec3 Kd = vec3 (0.7, 0.7, 0.7); // orange diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 10.0; // specular 'power'

void main () {
	  // ambient intensity
	  vec3 Ia = La * Ka;

	  // diffuse intensity
	// raise light position to eye space
	vec3 light_position_eye = vec3 (view_matrix * vec4 (light_position_world, 1.0));
	vec3 distance_to_light_eye = light_position_eye - position_eye;
	vec3 direction_to_light_eye = normalize (distance_to_light_eye);
	float dot_prod = dot (direction_to_light_eye, normal_eye);
	dot_prod = max (dot_prod, 0.0);
	vec3 Id = Ld * Kd * dot_prod; // final diffuse intensity
  
	  // specular intensity
	  vec3 reflection_eye = reflect (-direction_to_light_eye, normal_eye);
  vec3 surface_to_viewer_eye = normalize (-position_eye);
  float dot_prod_specular = dot (reflection_eye, surface_to_viewer_eye);
  dot_prod_specular = max (dot_prod_specular, 0.0);
  float specular_factor = pow (dot_prod_specular, specular_exponent);
  vec3 Is = Ls * Ks * specular_factor; // final specular intensity

	vec4 texel = texture (basic_texture, texture_coordinates);
	frag_colour = vec4 (Is + Id + Ia, 1.0) * texel;
}