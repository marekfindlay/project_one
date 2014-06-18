#include "stdafx.h"

using namespace std;

/* Variable declarations */
OpenGLContext openglContext; // Our OpenGL Context class  
/* Frame Timer */
double elapsed_seconds;
double current_seconds;
double previous_seconds;
int fps_frame_count;
double fps_previous_seconds;
double fps_current_seconds;
double fps_elapsed_seconds;
double physics_current_seconds;
double physics_new_seconds;
double physics_frame_time;
double physics_accumulator;
double physics_time;
const double physics_update_period = 0.25;
bool FPS_COUNT;
int g_gl_width = 1600;
int g_gl_height = 900;
const float  gravitational_constant = 6.67384e-11; // N(m/kg)^2

glm::vec3 gravitationalForce(float mass_one, float mass_two, float radius, glm::vec3 unit_direction)
{
	
	return unit_direction * mass_one * mass_two * gravitational_constant / (radius * radius);
}

State interpolate(const State &previous, const State &current, float alpha)
{
	State state;
	state.position = current.position*alpha + previous.position*(1 - alpha);
	state.v = current.v*alpha + previous.v*(1 - alpha);
	return state;
}

glm::vec4 acceleration(const State &state, double t)
{
	return glm::vec4(0.0,
		-1.6249,
		0.0,
		0.0);
}

Derivative evaluate(const State &initial, double t)
{
	Derivative output;
	output.dposition = initial.v;
	output.dv = acceleration(initial, t);
	return output;
}

Derivative evaluate(const State &initial,
	float t,
	float dt,
	const Derivative &d)
{
	State state;
	state.position = initial.position + d.dposition*dt;
	state.v = initial.v + d.dv*dt;
	/* Could include terminal velocity to make a limit */

	Derivative output;
	output.dposition = state.v;
	output.dv = acceleration(state, t + dt);
	return output;
}


void integrate(State &state, float t, float dt)
{
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);

	const glm::vec4 dxdt = 1.0f / 6.0f * (a.dposition + 2.0f*(b.dposition + c.dposition) + d.dposition);
	const glm::vec4 dvdt = 1.0f / 6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);

	state.position = state.position + dxdt*dt;
	state.v = state.v + dvdt*dt;
}



int main(void)
{
	/* Create a valid logfile */
	restart_gl_log();
	gl_log("Starting GLFW\n%s\n", glfwGetVersionString());

	/* create a camera for viewing */
	Camera *cam = new Camera();

	/* Rendering */

	/*setup scene; load vaos, compile shader programs, etc */
	openglContext.setupScene();

	/* generate view and projection matrices from camera config */
	glm::mat4 view_matrix = glm::lookAt(cam->cam_pos, cam->cam_target, cam->cam_up_direction);
	float aspect_ratio = (float) g_gl_width / (float) g_gl_height; //window aspect ratio
	glm::mat4 proj_matrix = glm::perspective(cam->fov_y, aspect_ratio, cam->near_clip, cam->far_clip);

	/* set the view and projection matrices in the shader program */
	openglContext.setViewMatrix(openglContext.shader_program, view_matrix);
	openglContext.setProjMatrix(openglContext.shader_program, proj_matrix);
	openglContext.setViewMatrix(openglContext.texture_program, view_matrix);
	openglContext.setProjMatrix(openglContext.texture_program, proj_matrix);
	openglContext.setModelMatrix(openglContext.texture_program, glm::mat4(1.0));
	openglContext.setViewMatrix(openglContext.skybox_program, glm::lookAt(glm::vec3(0, 0, 0), cam->cam_target - cam->cam_pos, cam->cam_up_direction));
	openglContext.setProjMatrix(openglContext.skybox_program, proj_matrix);
	openglContext.setModelMatrix(openglContext.shader_program, glm::mat4(1.0));

	/*fps variables*/
	if (FPS_COUNT)
	{
		fps_frame_count = 0;
		fps_previous_seconds = glfwGetTime();
		fps_current_seconds = 0;
	}

	/* physics initialisation ! */
	physics_time = 0.0;
	physics_accumulator = 0.0;
	physics_frame_time = 0.0;
	physics_current_seconds = 0.0;
	physics_new_seconds = 0.0;
	/* Rendering loop */
	while (!glfwWindowShouldClose(openglContext.window))
	{

		/* Update our frame timer */
		previous_seconds = current_seconds;
		current_seconds = glfwGetTime();
		elapsed_seconds = current_seconds - previous_seconds;

		/* Consistent physics updater with accumulator */
		physics_current_seconds = glfwGetTime();
		physics_frame_time = physics_new_seconds - physics_current_seconds;
		physics_current_seconds = physics_new_seconds;
		physics_accumulator += physics_frame_time;
		while (physics_accumulator >= physics_update_period)
		{
			//update world by integrating
//			integrate(state, physics_time, physics_update_period);
			//update world-time by consuming physics_update_period from the accumulator
			physics_accumulator -= physics_update_period; 
			physics_time += physics_update_period; 
		}
		/* FPS Counter */
		if (FPS_COUNT)
		{
			fps_current_seconds = glfwGetTime();
			fps_elapsed_seconds = fps_current_seconds - fps_previous_seconds;
			fps_frame_count++;
			if (fps_elapsed_seconds > 0.1)
			{
				fps_previous_seconds = fps_current_seconds;
				cout << "FPS: " << fps_frame_count / fps_elapsed_seconds << "\n";
				fps_frame_count = 0;
			}
		}
		// control keys
		bool cam_moved = false;
		if (glfwGetKey(openglContext.window, GLFW_KEY_A))
		{
			cam->cam_pos[0] -= cam->cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_D))
		{
			cam->cam_pos[0] += cam->cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_PAGE_UP))
		{
			cam->cam_pos[1] += cam->cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_PAGE_DOWN))
		{
			cam->cam_pos[1] -= cam->cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_W))
		{
			cam->cam_pos[2] -= cam->cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_S))
		{
			cam->cam_pos[2] += cam->cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_LEFT))
		{
			cam->cam_yaw += cam->cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_RIGHT))
		{
			cam->cam_yaw -= cam->cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (cam_moved)
		{
			/* if the camera has moved, update the view matrix */
			glm::mat4 view_matrix = glm::lookAt(cam->cam_pos, cam->cam_target, cam->cam_up_direction);
			openglContext.setViewMatrix(openglContext.shader_program, view_matrix);
			openglContext.setViewMatrix(openglContext.texture_program, view_matrix);
			//cam_target - cam_pos is the camera direction vector
			openglContext.setViewMatrix(openglContext.skybox_program, glm::lookAt(glm::vec3(0, 0, 0), cam->cam_target - cam->cam_pos, cam->cam_up_direction));
			cam_moved = false;
		}
		openglContext.renderScene();
	}

	/* Close GL context and GLFW resources */
	openglContext.~OpenGLContext();
	return 0;
}