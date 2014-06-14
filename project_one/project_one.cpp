#include "stdafx.h"

using namespace std;

/* Variable declarations */
OpenGLContext openglContext; // Our OpenGL Context class  
///* Camera Config*/
//bool cam_moved = false; 
//float cam_speed = 1.0f; // 1 unit per second
//float cam_yaw_speed = 10.0f; // 10 degrees per second
//float cam_yaw = 0.0f; // y-rotation in degrees
//float near_clip = 0.1f; // clipping plane
//float far_clip = 100.0f; // clipping plane
//float fov_y = glm::radians(50.625f); //y field of view
//glm::vec3 cam_pos = glm::vec3(10.0f, 10.0f, 10.0f); // the location of the camera eye
//glm::vec3 cam_target = glm::vec3(0.0f, 0.0f, 0.0f); // target of the camera
//glm::vec3 cam_up_direction = glm::vec3(0.0f, 1.0f, 0.0f); // the up direction from the camera's perspective
/* Frame Timer */
double elapsed_seconds;
double current_seconds;
double previous_seconds;
int fps_frame_count;
double fps_previous_seconds;
double fps_current_seconds;
double fps_elapsed_seconds;
bool FPS_COUNT;
int g_gl_width = 1024;
int g_gl_height = 768;

struct State
{
	glm::vec4 position;
	glm::vec4 v;
};

struct Derivative
{
	glm::vec4 dposition;
	glm::vec4 dv;
};

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
	assert(restart_gl_log());
	gl_log("Starting GLFW\n%s\n", glfwGetVersionString());

	/* create a camera for viewing */
	Camera cam = Camera();

	/* Rendering */

	/*setup scene; load vaos, compile shader programs, etc */
	openglContext.setupScene();

	/* generate view and projection matrices from camera config */
	glm::mat4 view_matrix = glm::lookAt(cam.cam_pos, cam.cam_target, cam.cam_up_direction);
	float aspect_ratio = (float) g_gl_width / (float) g_gl_height; //window aspect ratio
	glm::mat4 proj_matrix = glm::perspective(cam.fov_y, aspect_ratio, cam.near_clip, cam.far_clip);

	/* set the view and projection matrices in the shader program */
	openglContext.setViewMatrix(openglContext.shader_program, view_matrix);
	openglContext.setProjMatrix(openglContext.shader_program, proj_matrix);
	openglContext.setViewMatrix(openglContext.texture_program, view_matrix);
	openglContext.setProjMatrix(openglContext.texture_program, proj_matrix);

	/*fps variables*/
	if (FPS_COUNT)
	{
		fps_frame_count = 0;
		fps_previous_seconds = glfwGetTime();
		fps_current_seconds = 0;
	}

	/* Moon ! */


	/* Rendering loop */
	while (!glfwWindowShouldClose(openglContext.window))
	{

		/* Update our frame timer */
		previous_seconds = current_seconds;
		current_seconds = glfwGetTime();
		elapsed_seconds = current_seconds - previous_seconds;

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
			cam.cam_pos[0] -= cam.cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_D))
		{
			cam.cam_pos[0] += cam.cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_PAGE_UP))
		{
			cam.cam_pos[1] += cam.cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_PAGE_DOWN))
		{
			cam.cam_pos[1] -= cam.cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_W))
		{
			cam.cam_pos[2] -= cam.cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_S))
		{
			cam.cam_pos[2] += cam.cam_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_LEFT))
		{
			cam.cam_yaw += cam.cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (glfwGetKey(openglContext.window, GLFW_KEY_RIGHT))
		{
			cam.cam_yaw -= cam.cam_yaw_speed * elapsed_seconds;
			cam_moved = true;
		}
		if (cam_moved)
		{
			/* if the camera has moved, update the view matrix */
			glm::mat4 view_matrix = glm::lookAt(cam.cam_pos, cam.cam_target, cam.cam_up_direction);
			openglContext.setViewMatrix(openglContext.shader_program, view_matrix);
			openglContext.setViewMatrix(openglContext.texture_program, view_matrix);
			cam_moved = false;
		}
		openglContext.renderScene();
	}

	/* Close GL context and GLFW resources */
	openglContext.~OpenGLContext();
	return 0;
}