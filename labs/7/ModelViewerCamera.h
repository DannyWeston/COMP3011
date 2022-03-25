#pragma once

#include <stdio.h>
#include <glm/glm/glm.hpp>
#include "camera.h"

float cam_dist = 2.f;

void MoveAndOrientCamera(SCamera& in, glm::vec3 target, float distance, float xoffset, float yoffset)
{
	in.Yaw -= xoffset * in.MovementSpeed;
	in.Pitch -= yoffset * in.MovementSpeed;

	if (in.Pitch >= 90.0f) in.Pitch = 90.0f;
	if (in.Pitch <= -90.0f) in.Pitch = -90.0f;

	float ryaw = glm::radians(in.Yaw);
	float rpitch = glm::radians(in.Pitch);

	in.Position = glm::vec3(
		glm::cos(ryaw) * glm::cos(rpitch), 
		glm::sin(rpitch), 
		glm::sin(ryaw) * glm::cos(rpitch)
	) * cam_dist;

	in.Front = glm::normalize(target - in.Position);
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
	in.Up = glm::normalize(glm::cross(in.Right, in.Front));
}