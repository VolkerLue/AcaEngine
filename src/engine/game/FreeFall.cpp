#include "FreeFall.hpp"
#include <GL/glew.h>

struct Mesh {
	graphics::Mesh mesh;
};

struct Transfrom {
	glm::mat4 transfrom;
};

FreeFall::FreeFall() :	GameState(),
	a(-0.01f), 
	v(0.f),
	camera(graphics::Camera(45.f, 0.1f, 10000.f)),
	texture(*graphics::Texture2DManager::get("textures/planet1.png", 
		graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	renderer(graphics::MeshRenderer()) 
{
	utils::MeshLoader::clear();

	planet1 = registry.create();
	registry.addComponent<Mesh>(planet1, *utils::MeshLoader::get("models/sphere.obj"));
	registry.addComponent<Transfrom>(planet1, glm::translate(glm::vec3(0.f, 5.f, -10.0f)));
}

void FreeFall::newState() {
	registry.addComponent<Transfrom>(planet1, glm::translate(glm::vec3(0.f, 5.f, -10.0f)));
}

void FreeFall::update(float _time, float _deltaTime) {
	if (_time > 15) {
		finished = true;
	}
	v = v + a * _deltaTime;
	registry.execute<Transfrom>([&](Transfrom& trans) {trans.transfrom = trans.transfrom * glm::translate(glm::vec3(0.f, v, 0.f)); });

	if (registry.getComponent<Transfrom>(planet1)->transfrom[3][1] <= -5.f) {
		registry.execute<Transfrom>([&](Transfrom& trans) {trans.transfrom = glm::translate(glm::vec3(0.f, 5.f, -10.0f)); });
	}
}

void FreeFall::draw(float _time, float _deltaTime) {
	renderer.clear();
	renderer.draw(registry.getComponent<Mesh>(planet1)->mesh, texture, registry.getComponent<Transfrom>(planet1)->transfrom);
	renderer.present(camera);
}

void FreeFall::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.clear();
	renderer.draw(registry.getComponent<Mesh>(planet1)->mesh, texture, registry.getComponent<Transfrom>(planet1)->transfrom);
	renderer.present(camera);
}

void FreeFall::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
