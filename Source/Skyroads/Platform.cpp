#include "Platform.h"

Platform::Platform(const std::string& name, glm::vec3 centerPoint, glm::vec3 colour)
{
	this->name = name;
	this->colour = colour;

	translateX = centerPoint.x;
	translateY = centerPoint.y;
	translateZ = centerPoint.z;

	scaleX = 1.0f;
	scaleY = 0.0f;
	scaleZ = 3.0f;
}
