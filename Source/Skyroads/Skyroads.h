#pragma once
#include <Component/SimpleScene.h>
#include "Camera.h"
#include "Platform.h"

class Skyroads : public SimpleScene
{
public:
	Skyroads();
	~Skyroads();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;
	void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 colour);
	void RenderMesh2D(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	Homework::Camera* camera;
	glm::mat4 modelMatrix;
	glm::mat4 projectionMatrix;
	glm::mat4 lastProjectionMatrix;

	float sphereX, sphereY, sphereZ;
	float sphereScale;
	float speed;
	float fuel;
	float accelerationCooldown;
	float colourChangedTime;

	unsigned int crtPlatform;

	bool goUp, goDown;
	bool canJump;
	bool thirdPersonCamera;
	bool isColourChanged;

	std::vector<Platform> platforms;

	glm::vec3 sphereColour;
};
