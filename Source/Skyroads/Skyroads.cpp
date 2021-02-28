#include "Skyroads.h"

#include <ctime>

#include "Platform.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>

using namespace std;
using namespace SkyroadConstants;

Skyroads::Skyroads()
{
}

Skyroads::~Skyroads()
{
}

void Skyroads::Init()
{
	camera = new Homework::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	lastProjectionMatrix = projectionMatrix;

	// Initializare coordonate sfera
	sphereX = 0.0f;
	sphereY = 5.0f;
	sphereZ = 0.0f;

	// Initializare scalare sfera
	sphereScale = 0.5f;

	// Initializare numar curent platforma
	crtPlatform = 0;

	// Initializare viteza platforme
	speed = 0.0f;

	// Initializare variabile miscare si saritura minge
	goUp = false;
	goDown = true;
	canJump = true;
	accelerationCooldown = true;

	// Initializare benzina
	fuel = 100.0f;

	// Initializare cooldown acceleratie
	accelerationCooldown = 0.0f;

	// Initializare camera
	thirdPersonCamera = true;

	// Initializare culoare sfera
	sphereColour = RED;
	isColourChanged = false;

	// Generare seed pentru numere aleatoare
	srand((unsigned int) time(nullptr));
	
	for (crtPlatform = 0; crtPlatform < PLATFORMS_PER_ROW * ROWS; ++crtPlatform)
	{
		std::string name = PLATFORM_NAME + std::to_string(crtPlatform);
		glm::vec3 topLeftCorner = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 colour;
		int randomColour = rand() % 10;

		if (crtPlatform < PLATFORMS_PER_ROW * ROWS / 2)
		{
			colour = BLUE;
		}
		else
		{
			switch (randomColour)
			{
				case 0:
				case 1:
				case 2:
					colour = BLUE;
					break;
				case 3:
				case 4:
				case 5:
					colour = GREEN;
					break;
				case 6:
				case 7:
					colour = YELLOW;
					break;
				case 8:
					colour = ORANGE;
					break;
				case 9:
					colour = RED;
					break;
				default:
					colour = BLUE;
					break;
			}
		}

		platforms.emplace_back(name, topLeftCorner, colour);
		platforms[crtPlatform].translateX = crtPlatform % 3 + OX_OFFSET;
		platforms[crtPlatform].translateY = OY_OFFSET;
		platforms[crtPlatform].translateZ = -PLATFORM_LENGTH / 2 - crtPlatform / 3 * PLATFORM_LENGTH;
		platforms[crtPlatform].isRendered = true;

		Mesh* platform = createPlatform(name, topLeftCorner, colour);
		meshes[name] = platform;
	}

	Mesh* sphere = new Mesh(SPHERE_NAME);
	sphere->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[sphere->GetMeshID()] = sphere;
	
	Mesh* fuelBar = createFuelBar(FUEL_BAR_NAME, glm::vec3(0.0f, 10.0f, 0.0f),GREEN);
	meshes[FUEL_BAR_NAME] = fuelBar;

	Mesh* backgroundBar = createFuelBar(BACKGROUND_BAR_NAME, glm::vec3(0.0f, 0.0f, 0.0f), WHITE);
	meshes[BACKGROUND_BAR_NAME] = backgroundBar;

	Shader* shader = new Shader("ShaderSkyroads");
	shader->AddShader("Source/Skyroads/Shaders/DistortionVertexShader.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/Skyroads/Shaders/DistortionFragmentShader.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
}

void Skyroads::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// Sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Skyroads::Update(float deltaTimeSeconds)
{
	if (!thirdPersonCamera)
	{
		camera->Set(glm::vec3(sphereX, sphereY + SPHERE_RADIUS, sphereZ), 
			glm::vec3(0.0f, 1.0f, -30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else
	{
		camera->Set(glm::vec3(0.0f, 2.0f, 3.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	// Randare platforme
	for (Platform& platform : platforms)
	{
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix,
			glm::vec3(platform.translateX, platform.translateY, platform.translateZ));

		if (platform.isRendered)
		{
			RenderMesh(meshes[platform.name], shaders["VertexColor"], modelMatrix);
		}

		platform.translateZ += deltaTimeSeconds * speed * SPEED_MODIFIER;
	}

	// Verfificare daca platformele au intrecut limita
	if (platforms[0].translateZ >= SCREEN_LIMIT)
	{
		for (auto i = 0; i < PLATFORMS_PER_ROW; ++i)
		{
			platforms.erase(platforms.begin());
		}
	}

	// Regenerare platforme
	if (platforms.size() < PLATFORMS_PER_ROW * ROWS)
	{
		for (auto i = 0; i < PLATFORMS_PER_ROW; ++i)
		{
			std::string name = PLATFORM_NAME + std::to_string(crtPlatform);
			glm::vec3 topLeftCorner = glm::vec3(0.0f, 0.0f, 0.0f);
			glm::vec3 colour;
			int randomColour = rand() % 10;

			switch (randomColour)
			{
				case 0:
				case 1:
				case 2:
					colour = BLUE;
					break;
				case 3:
				case 4:
				case 5:
					colour = GREEN;
					break;
				case 6:
				case 7:
					colour = YELLOW;
					break;
				case 8:
					colour = ORANGE;
					break;
				case 9:
					colour = RED;
					break;
				default:
					colour = BLUE;
					break;
			}

			platforms.emplace_back(name, topLeftCorner, colour);
			platforms[platforms.size() - 1].translateX = crtPlatform % 3 + OX_OFFSET;
			platforms[platforms.size() - 1].translateY = OY_OFFSET;
			platforms[platforms.size() - 1].translateZ = platforms[platforms.size() - 4].translateZ - PLATFORM_LENGTH;
			platforms[platforms.size() - 1].isRendered = rand() % 3 < 2;
			crtPlatform++;

			Mesh* platform = createPlatform(name, topLeftCorner, colour);
			meshes[name] = platform;
		}
	}

	// Verificare daca sfera sare
	if (goUp)
	{
		if (sphereY < 1.5f)
		{
			sphereY += deltaTimeSeconds * BALL_JUMP_MODIFIER;
		}
		else
		{
			goDown = true;
			goUp = false;
		}
	}

	if (goDown)
	{
		sphereY -= deltaTimeSeconds * BALL_JUMP_MODIFIER;

		for (auto i = 0; i < 2 * PLATFORMS_PER_ROW; ++i)
		{
			glm::vec3 point(sphereX, sphereY - SPHERE_RADIUS, sphereZ);

			// Coliziune sfera-platforma
			if (sphereY > 0.0f &&
				platforms[i].isRendered &&
				point.x >= platforms[i].translateX && point.x <= platforms[i].translateX + PLATFORM_WIDTH &&
				point.y - platforms[i].translateY < 0.0001f &&
				point.z >= platforms[i].translateZ && point.z <= platforms[i].translateZ + PLATFORM_LENGTH)
			{
				Platform& currentPlatform = platforms[i];

				if (currentPlatform.colour != BLUE && currentPlatform.colour != PURPLE)
				{
					powerUp(currentPlatform.colour, fuel, accelerationCooldown);
					sphereColour = currentPlatform.colour;
					colourChangedTime = 10.0f;
					isColourChanged = true;
				}

				sphereY = 0.5f;
				
				currentPlatform.colour = PURPLE;
				canJump = true;
				
				Mesh* platform = createPlatform(currentPlatform.name, 
					glm::vec3(0.0f, 0.0f, 0.0f),
					currentPlatform.colour);
				meshes[currentPlatform.name] = platform;
			}
		}
	}

	if (colourChangedTime > 0.0f)
	{
		colourChangedTime -= 0.25f;
	}
	else
	{
		isColourChanged = false;
		sphereColour = RED;
	}
	
	// Afisat sfera daca camera este third person
	if (thirdPersonCamera)
	{
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereX, sphereY, sphereZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(sphereScale));
		RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
		RenderMesh(meshes["sphere"], shaders["ShaderSkyroads"], modelMatrix, sphereColour);
	}

	// Afisare benzina
	fuel -= speed * 0.025f;

	// Scadere cooldown acceleratie
	if (accelerationCooldown > 0.0f)
	{
		accelerationCooldown -= 0.05f;
	}

	// Verificare daca jocul este gata
	if (sphereY < -1.0f || fuel < 0.0f)
	{
		std::cout << "Game Over\n";
		exit(EXIT_SUCCESS);
	}
}

void Skyroads::FrameEnd()
{
	glm::ivec2 resolution = window->GetResolution();
	projectionMatrix = glm::ortho(0.0f, (float)resolution.x, 0.0f, (float)resolution.y, 0.0f, 100.0f);
	
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix *= glm::translate(modelMatrix, glm::vec3(0.0f, 355.0f, 0.0f));
	modelMatrix *= glm::scale(modelMatrix, glm::vec3(fuel / 100.0f, 1.0f, 0.0f));
	RenderMesh2D(meshes[FUEL_BAR_NAME], shaders["VertexColor"], modelMatrix);

	modelMatrix = glm::mat4(1.0f);
	modelMatrix *= glm::translate(modelMatrix, glm::vec3(0.0f, 720.0f, 0.0f));
	RenderMesh2D(meshes[BACKGROUND_BAR_NAME], shaders["VertexColor"], modelMatrix);
	
	projectionMatrix = lastProjectionMatrix;
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Skyroads::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
	{
		return;
	}

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Skyroads::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 colour)
{
	if (!mesh || !shader || !shader->program)
	{
		return;
	}

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	int timeLocation = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(timeLocation, (float) Engine::GetElapsedTime());

	int objectColourLocation = glGetUniformLocation(shader->GetProgramID(), "ObjColour");
	glUniform3fv(objectColourLocation, 1, glm::value_ptr(colour));
	
	mesh->Render();
}

void Skyroads::RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
	if (!mesh || !shader || !shader->program)
	{
		return;
	}

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Skyroads::OnInputUpdate(float deltaTime, int mods)
{
	float cameraSpeed = 2.0f;

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{

		if (window->KeyHold(GLFW_KEY_W)) {
			camera->TranslateForward(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			camera->TranslateRight(deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			camera->TranslateUpward(-deltaTime * cameraSpeed);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			camera->TranslateUpward(deltaTime * cameraSpeed);
		}
	}
	else
	{
		if (window->KeyHold(GLFW_KEY_A) && sphereX > OX_OFFSET + 0.05f)
		{
			sphereX -= deltaTime * BALL_MOVE_MODIFIER;

			if (!thirdPersonCamera)
			{
				camera->TranslateRight(-deltaTime * cameraSpeed);
			}
		}

		if (window->KeyHold(GLFW_KEY_D) && sphereX < -OX_OFFSET - 0.05f)
		{
			sphereX += deltaTime * BALL_MOVE_MODIFIER;

			if (!thirdPersonCamera)
			{
				camera->TranslateRight(deltaTime * cameraSpeed);
			}
		}

		if (window->KeyHold(GLFW_KEY_W) && speed < MAX_SPEED && accelerationCooldown <= 0.0f)
		{
			speed += deltaTime;
		}

		if (window->KeyHold(GLFW_KEY_S) && speed > MIN_SPEED && accelerationCooldown <= 0.0f)
		{
			speed -= deltaTime;
		}
	}
}

void Skyroads::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE && !goUp && canJump)
	{
		goUp = true;
		goDown = false;
		canJump = false;
	}

	if (key == GLFW_KEY_C)
	{
		thirdPersonCamera = !thirdPersonCamera;
	}
}

void Skyroads::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Skyroads::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			//renderCameraTarget = false;
			camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
			camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			//renderCameraTarget = true;
			camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
			camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
		}
	}
}

void Skyroads::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Skyroads::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Skyroads::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Skyroads::OnWindowResize(int width, int height)
{
}
