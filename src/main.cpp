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

	graphics::Device::initialize(1366, 768, false);
	GLFWwindow* window = graphics::Device::getWindow();
	input::InputManager::initialize(window);

	//static const GLfloat g_vertex_buffer_data[] = {
	//	-1.0f, -1.0f, 0.0f,
	//	1.0f, -1.0f, 0.0f,
	//	0.0f,  1.0f, 0.0f,
	//};
	//struct graphics::VertexAttribute va[] = { {graphics::PrimitiveFormat::FLOAT, 3, false, false} };
	//graphics::GeometryBuffer gb(graphics::GLPrimitiveType::TRIANGLES, va, 1, 0, 1024);
	//gb.setData(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));

	using namespace graphics;
	//Camera camera(45.f, 0.01f, 10000.f);
	Camera camera(0.f, 0.f, 0.f);
	Sampler sampler(Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Filter::LINEAR, Sampler::Border::MIRROR);
	const Texture2D& texture = *Texture2DManager::get("textures\\planet1.png", sampler);
	Mesh mesh(*utils::MeshLoader::get("models\\sphere.obj"));
	MeshRenderer renderer;
	glm::mat4 perspective = glm::perspective(45.f, 1.f, 0.1f, 100.f);
	glm::mat4 rot(1.f);
	glm::mat4 rotate = glm::rotate(rot, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));

	//glClearColor(0.f, 1.f, 0.f, 1.f);

	glm::mat4 rotateX = glm::rotate(rot, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 rotateY = glm::rotate(rot, glm::radians(0.0f), glm::vec3(0.0, 0.0, 1.0));
	auto radX = glm::radians(1.0f);
	auto radY = glm::radians(1.0f);
	auto cur = input::InputManager::getCursorPos();
	glm::mat4 translate = glm::translate(glm::vec3(0.0, 0.0, 0.1));

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.draw(mesh, texture, rotate);
		renderer.present(camera);
		renderer.clear();
		//gb.draw();

		using namespace input;
		if (InputManager::getCursorPos().x < cur.x) {
			cur.x = InputManager::getCursorPos().x;
			radX -= glm::radians(1.0f);
			rotateX = glm::rotate(rot, radX, glm::vec3(0.0, 1.0, 0.0));
		}
		if (InputManager::getCursorPos().x > cur.x) {
			cur.x = InputManager::getCursorPos().x;
			radX += glm::radians(1.0f);
			rotateX = glm::rotate(rot, radX, glm::vec3(0.0, 1.0, 0.0));
		}
		if (InputManager::getCursorPos().y < cur.y) {
			cur.y = InputManager::getCursorPos().y;
			radY -= glm::radians(1.0f);
			rotateY = glm::rotate(rot, radY, glm::vec3(1.0, 0.0, 0.0));
		}
		if (InputManager::getCursorPos().y > cur.y) {
			cur.y = InputManager::getCursorPos().y;
			radY += glm::radians(1.0f);
			rotateY = glm::rotate(rot, radY, glm::vec3(1.0, 0.0, 0.0));
		}



		if (input::InputManager::isKeyPressed(input::Key::W)) {
			radY += glm::radians(1.0f);
			rotateY = rotateY * glm::translate(glm::vec3(0.0, 0.01, 0.0));
		}
		if (input::InputManager::isKeyPressed(input::Key::S)) {
			radY -= glm::radians(1.0f);
			rotateY = rotateY * glm::translate(glm::vec3(0.0, -0.01, 0.0));
		}
		if (input::InputManager::isKeyPressed(input::Key::A)) {
			radX -= glm::radians(1.0f);
			rotateX = rotateX * glm::translate(glm::vec3(0.0, 0.0, -0.01));
		}
		if (input::InputManager::isKeyPressed(input::Key::D)) {
			radX += glm::radians(1.0f);
			rotateX = rotateX * glm::translate(glm::vec3(0.0, 0.0, 0.01));
		}

		rotate = rotateX * rotateY;

		glfwPollEvents();
		glfwSwapBuffers(window);
		std::this_thread::sleep_for(16ms);
	}


	//graphics::glCall(glDeleteVertexArrays, 1, &vertexArrayID);


	utils::MeshLoader::clear();
	graphics::Device::close();
	return EXIT_SUCCESS;
}