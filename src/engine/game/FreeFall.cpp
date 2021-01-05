#include "FreeFall.hpp"
#include <GL/glew.h>

struct Mesh {
	graphics::Mesh mesh;
};

struct Transfrom {
	glm::mat4 transfrom;
};

struct Velocity {
	glm::vec3 velo;
};

struct Accelaration {
	glm::vec3 accel;
};

FreeFall::FreeFall() : GameState(),

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
	registry.addComponent<Velocity>(planet1, glm::vec3(0.f, 0.f, 0.f));
	registry.addComponent<Accelaration>(planet1, glm::vec3(0.f, -0.01f, 0.f));
}

void FreeFall::update(float _time, float _deltaTime) {
	if (_time > 15) {
		finished = true;
	}
	registry.execute<Velocity, Accelaration>([&](Velocity& velo, Accelaration& accel) {velo.velo = velo.velo + accel.accel * _deltaTime; });
	registry.execute<Transfrom, Velocity>([&](Transfrom& trans, Velocity& velo) {trans.transfrom = trans.transfrom * glm::translate(velo.velo); });

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

