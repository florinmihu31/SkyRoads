#pragma once
#include <string>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>
#include <GL/glew.h>

namespace SkyroadConstants
{
	const float BALL_JUMP_MODIFIER = 2.0f;
	const float BALL_MOVE_MODIFIER = 2.0f;
	const float PLATFORM_LENGTH = 3.0f;
	const float PLATFORM_WIDTH = 1.0f;
	const float SCREEN_LIMIT = 2.0f;
	const float SPEED_MODIFIER = 2.0f;
	const float OX_OFFSET = -1.5f;
	const float OY_OFFSET = 0.25f;
	const float OZ_OFFSET = 0.5f;
	const float SPHERE_RADIUS = 0.25f;
	const float MIN_SPEED = 2.0f;
	const float MAX_SPEED = 3.0f;

	const unsigned int ROWS = 10;
	const unsigned int PLATFORMS_PER_ROW = 3;

	const std::string PLATFORM_NAME = "platform";
	const std::string FUEL_BAR_NAME = "fuel_bar";
	const std::string BACKGROUND_BAR_NAME = "background_bar";
	const std::string SPHERE_NAME = "sphere";
	
	const glm::vec3 RED = glm::vec3(1.0f, 0.0f, 0.0f);
	const glm::vec3 GREEN = glm::vec3(0.0f, 1.0f, 0.0f);
	const glm::vec3 BLUE = glm::vec3(0.0f, 0.0f, 1.0f);
	const glm::vec3 YELLOW = glm::vec3(1.0f, 1.0f, 0.0f);
	const glm::vec3 ORANGE = glm::vec3(1.0f, 0.5f, 0.0f);
	const glm::vec3 PURPLE = glm::vec3(0.5f, 0.0f, 1.0f);
	const glm::vec3 BLACK = glm::vec3(0.0f, 0.0f, 0.0f);
	const glm::vec3 WHITE = glm::vec3(1.0f, 1.0f, 1.0f);
}


Mesh* createPlatform(const std::string& name, glm::vec3 topLeftCorner, glm::vec3 colour);
void powerUp(glm::vec3 colour, float& fuel, float& accelerationCooldown);
Mesh* createFuelBar(const std::string& name, glm::vec3 topLeftCorner, glm::vec3 colour);