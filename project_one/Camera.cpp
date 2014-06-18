#include "Camera.hpp"

/*default constructor*/
Camera::Camera()
{
	/* Camera Config*/
	cam_moved = false;
	cam_speed = 5.0f; // 1 unit per second
	cam_yaw_speed = 10.0f; // 10 degrees per second
	cam_yaw = 0.0f; // y-rotation in degrees
	near_clip = 0.1f; // clipping plane
	far_clip = 100.0f; // clipping plane
	fov_y = glm::radians(50.625f); //y field of view
	cam_pos = glm::vec3(0.0f, 0.0f, 50.0f); // the location of the camera eye
	cam_target = glm::vec3(0.0f, 0.0f, 0.0f); // target of the camera
	cam_up_direction = glm::vec3(0.0f, 1.0f, 0.0f); // the up direction from the camera's perspective
}


Camera::~Camera()
{
}
