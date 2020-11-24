#include <engine/graphics/renderer/mesh.hpp>
#include <engine/graphics/renderer/meshrenderer.hpp>
#include <engine/utils/meshloader.hpp>
#include <engine/graphics/core/device.hpp>
#include <engine/input/inputmanager.hpp>
#include <engine/utils/meshloader.hpp>
#include <engine/graphics/core/opengl.hpp>
#include <engine/graphics/core/geometrybuffer.hpp>
#include <engine/graphics/core/vertexformat.hpp>
#include <engine/graphics/core/texture.hpp>
#include <gl/GL.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <glm\ext\matrix_transform.hpp>

// CRT's memory leak detection
#ifndef NDEBUG 
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <iostream>

#endif

using namespace std::chrono_literals;

int main(int argc, char* argv[])
{
#ifndef NDEBUG 
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//	_CrtSetBreakAlloc(2760);
#endif
#endif

	glm::vec3 plantetPositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	graphics::Device::initialize(1366, 768, false);
	GLFWwindow* window = graphics::Device::getWindow();
	input::InputManager::initialize(window);

	using namespace graphics;
	Camera camera(45.f, 0.1f, 10000.f);

	glm::mat4 view = camera.getView();
	glm::mat4 model = glm::mat4(1.0f);

	Sampler sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR);
	const Texture2D& texturePlanet = *Texture2DManager::get("textures/planet1.png", sampler);
	const Texture2D& textureCrate = *Texture2DManager::get("textures/cratetex.png", sampler);
	Mesh meshPlanet(*utils::MeshLoader::get("models/sphere.obj"));
	Mesh meshCrate(*utils::MeshLoader::get("models/crate.obj"));
	MeshRenderer renderer[10];
	for (int i = 0; i < 10; i++)
		renderer[i].draw(meshPlanet, texturePlanet, glm::mat4(1.f));

	auto lastCursorPos = input::InputManager::getCursorPos();

	float lastTime = glfwGetTime();
	float currentTime = glfwGetTime();
	float delta = glfwGetTime()- lastTime;

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::vec3 direction;
	float yaw = -90.0f;
	float pitch = 0.0f;

	float sensitivity = 0.05f;

	input::InputManager::setCursorMode(input::InputManager::CursorMode::DISABLED);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

		using namespace input;

		// camera dreht um Punkt
		if (InputManager::isKeyPressed(input::Key::ESCAPE))		glfwSetWindowShouldClose(window, true);
		if (InputManager::isKeyPressed(input::Key::W))			plantetPositions[0] += glm::vec3(0.0, 0.0, 2.0 * delta);
		if (InputManager::isKeyPressed(input::Key::S))			plantetPositions[0] += glm::vec3(0.0, 0.0, -2.0 * delta);
		if (InputManager::isKeyPressed(input::Key::A))			plantetPositions[0] += glm::vec3(2.0 * delta, 0.0, 0.0);
		if (InputManager::isKeyPressed(input::Key::D))			plantetPositions[0] += glm::vec3(-2.0 * delta, 0.0, 0.0);
		if (InputManager::isKeyPressed(input::Key::Q))			plantetPositions[0] += glm::vec3(0.0, 2.0 * delta, 0.0);
		if (InputManager::isKeyPressed(input::Key::E))			plantetPositions[0] += glm::vec3(0.0, -2.0 * delta, 0.0);

		if (InputManager::getCursorPos() != lastCursorPos) {

			float deltaX = InputManager::getCursorPos().x - lastCursorPos.x;
			float deltaY = lastCursorPos.y - InputManager::getCursorPos().y;

			lastCursorPos.y = InputManager::getCursorPos().y;
			lastCursorPos.x = InputManager::getCursorPos().x;

			yaw += deltaX * sensitivity;
			pitch += deltaY * sensitivity;

			if (pitch > 89.0f)
				pitch = 89.0f;
			if (pitch < -89.0f)
				pitch = -89.0f;

			glm::vec3 direction;
			direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			direction.y = sin(glm::radians(pitch));
			direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

			cameraFront = glm::normalize(direction);
			view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp * delta);
		}
				
		camera.setView(view);
		for (unsigned int i = 0; i < 10; i++){
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, plantetPositions[i]);
				camera.setView(camera.getView() * model);
				renderer[i].present(camera);
		}

		std::cout << delta << "\n";
				
		glEnable(GL_CULL_FACE);
		glfwPollEvents();
		glfwSwapBuffers(window);
		std::this_thread::sleep_for(8ms);
	}

	utils::MeshLoader::clear();
	graphics::Device::close();
	return EXIT_SUCCESS;
}
