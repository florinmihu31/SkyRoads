#pragma once
#include "Skyroads/UtilsSkyroads.h"

struct Platform
{
	Platform(const std::string& name, glm::vec3 centerPoint, glm::vec3 colour);
	
	std::string name;

	float translateX, translateY, translateZ;
	float scaleX, scaleY, scaleZ;

	bool isRendered;

	glm::vec3 colour;
};
