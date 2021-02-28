#include "Skyroads/UtilsSkyroads.h"

#include <iostream>

#include "Core/Managers/ResourcePath.h"

using namespace SkyroadConstants;

Mesh* createPlatform(const std::string& name, glm::vec3 topLeftCorner, glm::vec3 colour)
{
	std::vector<VertexFormat> verticesPlatform = {
		VertexFormat(topLeftCorner, colour),
		VertexFormat(topLeftCorner + 
			glm::vec3(0.0f, 0.0f, SkyroadConstants::PLATFORM_LENGTH), colour),
		VertexFormat(topLeftCorner + 
			glm::vec3(SkyroadConstants::PLATFORM_WIDTH, 0.0f, 0.0f), colour),
		VertexFormat(topLeftCorner + 
			glm::vec3(SkyroadConstants::PLATFORM_WIDTH, 0.0f, SkyroadConstants::PLATFORM_LENGTH), colour)
	};

	std::vector<unsigned short> indicesPlatform = {
		0, 1, 3,
		0, 3, 2
	};
	
	Mesh* platform = new Mesh(name);
	platform->SetDrawMode(GL_TRIANGLES);
	platform->InitFromData(verticesPlatform, indicesPlatform);
	return platform;
}

void powerUp(glm::vec3 colour, float& fuel, float& accelerationCooldown)
{
	if (colour == RED)
	{
		fuel = 0.0f;
	}
	else if (colour == YELLOW)
	{
		fuel -= 5.0f;
	}
	else if (colour == GREEN)
	{
		fuel += 5.0f;

		if (fuel > 100.0f)
		{
			fuel = 100.0f;
		}
	}
	else if (colour == ORANGE)
	{
		accelerationCooldown = 10.0f;
	}
}


Mesh* createFuelBar(const std::string& name, glm::vec3 topLeftCorner, glm::vec3 colour)
{
	std::vector<VertexFormat> verticesFuelBar = {
		VertexFormat(topLeftCorner, colour),
		VertexFormat(topLeftCorner + glm::vec3(200, 0, 0), colour),
		VertexFormat(topLeftCorner + glm::vec3(0, -20, 0), colour),
		VertexFormat(topLeftCorner + glm::vec3(200, -20, 0), colour)
	};

	std::vector<unsigned short> indicesFuelBar = {
		0, 1, 2,
		1, 3, 2
	};

	Mesh* fuelBar = new Mesh(name);
	fuelBar->SetDrawMode(GL_TRIANGLES);
	fuelBar->InitFromData(verticesFuelBar, indicesFuelBar);
	return fuelBar;
}
