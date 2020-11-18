#include <engine/graphics/renderer/mesh.hpp>
#include <engine/graphics/renderer/meshrenderer.hpp>
#include <engine/utils/meshloader.hpp>
#include <engine/graphics/core/device.hpp>
#include <engine/input/inputmanager.hpp>
#include <engine/utils/meshloader.hpp>
#include <engine\graphics\core\opengl.hpp>
#include <engine\graphics\core\geometrybuffer.hpp>

#include <engine\graphics\core\vertexformat.hpp>
#include <engine\graphics\core\texture.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <gl/GL.h>
#include <GLFW/glfw3.h>

#include <thread>

// CRT's memory leak detection
#ifndef NDEBUG 
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

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

	graphics::Device::initialize(1000, 1000, false);
	GLFWwindow* window = graphics::Device::getWindow();
	input::InputManager::initialize(window);

	using namespace graphics;
	//Camera camera(45.f, 0.01f, 10000.f);
	Camera camera(0.f, 0.f, 0.f);
	Sampler sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR);
	const Texture2D& planet = *Texture2DManager::get("textures\\planet1.png", sampler);
	Mesh mesh(*utils::MeshLoader::get("models\\sphere.obj"));
	MeshRenderer renderer;
	glm::mat4 id(1.f);
	glm::mat4 rotate = glm::rotate(id, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

	//glClearColor(0.f, 1.f, 0.f, 1.f);

	glm::mat4 rotateX = glm::rotate(id, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 rotateY = glm::rotate(id, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

	auto radX = glm::radians(1.0f);
	auto radY = glm::radians(1.0f);

	auto cur = input::InputManager::getCursorPos();
	glm::mat4 translate = glm::translate(glm::vec3(0.0, 0.0, 90.));

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.draw(mesh, planet, rotate);
		camera.setView(rotate);
		renderer.present(camera);

		using namespace input;
		if (InputManager::getCursorPos().x < cur.x) {
			cur.x = InputManager::getCursorPos().x;
			radX -= glm::radians(1.f);
			rotateX = glm::rotate(id, radX, glm::vec3(0.0, 1.0, 0.0));
		}
		if (InputManager::getCursorPos().x > cur.x) {
			cur.x = InputManager::getCursorPos().x;
			radX += glm::radians(1.f);
			rotateX = glm::rotate(id, radX, glm::vec3(0.0, 1.0, 0.0));
		}
		if (InputManager::getCursorPos().y < cur.y) {
			cur.y = InputManager::getCursorPos().y;
			radY -= glm::radians(1.f);
			rotateY = glm::rotate(id, radY, glm::vec3(1.0, 0.0, 0.0));
		}
		if (InputManager::getCursorPos().y > cur.y) {
			cur.y = InputManager::getCursorPos().y;
			radY += glm::radians(1.f);
			rotateY = glm::rotate(id, radY, glm::vec3(1.0, 0.0, 0.0));
		}



		if (InputManager::isKeyPressed(input::Key::W)) {
			rotateY = rotateY * glm::translate(glm::vec3(0.0, 0.01, 0.0));
		}
		if (InputManager::isKeyPressed(input::Key::S)) {
			rotateY = rotateY * glm::translate(glm::vec3(0.0, -0.01, 0.0));
		}
		if (InputManager::isKeyPressed(input::Key::A)) {
			rotateX = rotateX * glm::translate(glm::vec3(-0.01, 0.0, 0.0));
		}
		if (InputManager::isKeyPressed(input::Key::D)) {
			rotateX = rotateX * glm::translate(glm::vec3(0.01, 0.0, 0.0));
		}

		if (InputManager::isKeyPressed(input::Key::UP)) {
			rotateX = rotateX * glm::scale(glm::vec3(0.9, 0.9, 0.9));
		}
		if (InputManager::isKeyPressed(input::Key::DOWN)) {
			rotateX = rotateX * glm::scale(glm::vec3(1.1, 1.1, 1.1));
		}
		if (InputManager::isKeyPressed(input::Key::LEFT)) {
			rotateX = rotateX * glm::translate(glm::vec3(0.0, 0.0, 0.01));
		}
		if (InputManager::isKeyPressed(input::Key::RIGHT)) {
			rotateX = rotateX * glm::translate(glm::vec3(0.0, 0.0, -0.01));
		}


		rotate = rotateX * rotateY;

		glEnable(GL_CULL_FACE);

		glfwPollEvents();
		glfwSwapBuffers(window);
		std::this_thread::sleep_for(16ms);
	}

	utils::MeshLoader::clear();
	graphics::Device::close();
	return EXIT_SUCCESS;
}