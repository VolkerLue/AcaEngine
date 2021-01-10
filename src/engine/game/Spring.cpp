#include "Spring.hpp"
#include <GL/glew.h>


struct Mesh {
	graphics::Mesh mesh;
};

struct Transfrom {
	glm::mat4 transfrom;
};

struct Velocity {
	float velo;
};




Spring::Spring() : GameState(), times(0),
	difference(glm::mat4(1.f)),
	camera(graphics::Camera(45.f, 0.1f, 10000.f)),
	texture(*graphics::Texture2DManager::get("textures/planet1.png",
	graphics::Sampler(graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR, graphics::Sampler::Filter::LINEAR))),
	mesh(graphics::Mesh(*utils::MeshLoader::get("models/sphere.obj"))),
	renderer(graphics::MeshRenderer())
{
	utils::MeshLoader::clear();

	planet1 = registry.create();
	registry.addComponent<Mesh>(planet1, *utils::MeshLoader::get("models/sphere.obj"));
	
}

void Spring::newState() {
	registry.addComponent<Transfrom>(planet1, glm::mat4(1.f));
	registry.addComponent<Velocity>(planet1, 0);
	
}

void Spring::update(float _time, float _deltaTime) {
	times = times + 1;
	if (_time > 5) {
		finished = true;
	}

}

void Spring::draw(float _time, float _deltaTime) {
	
	registry.execute<Velocity>([&](Velocity& velo) {velo.velo = 300 * sin((_time) * 3.14 / 180) * _deltaTime; });
	if (times < 50) {
		
		registry.execute<Transfrom>([&](Transfrom& trans) {trans.transfrom = trans.transfrom * glm::translate(glm::vec3(0.f, 0.f, -1.1f)); });
		
	}
	else {
		registry.execute<Transfrom, Velocity>([&](Transfrom& trans, Velocity& velo) {trans.transfrom = trans.transfrom * glm::translate(glm::vec3(velo, 0.f, 0.f)); });
		
	}


	renderer.clear();
	renderer.draw(registry.getComponent<Mesh>(planet1)->mesh, texture, registry.getComponent<Transfrom>(planet1)->transfrom);
	renderer.present(camera);
}

void Spring::onResume() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	renderer.clear();
	renderer.draw(mesh, texture, difference);
	renderer.present(camera);
}

void Spring::onPause() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//auto test_start = std::chrono::steady_clock::now();
//float test_time = std::chrono::duration_cast<TimeT>(std::chrono::steady_clock::now() - test_start).count();
