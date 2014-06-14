#ifndef CAMERA_HPP
#define CAMERA_HPP

/* Define function prototypes */
#include "stdafx.h"
class Camera
{
public:
	Camera();
	~Camera();
	glm::vec3 cam_pos;
	glm::vec3 cam_target;
	glm::vec3 cam_up_direction;
	float cam_speed;
	float cam_yaw_speed;
	float cam_yaw;
	float near_clip;
	float far_clip;
	float fov_y;
	bool cam_moved;
private:
protected:
}; 
#endif //CAMERA_HPP

