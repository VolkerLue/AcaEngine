#pragma once
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
#include <glm/ext/matrix_transform.hpp>
#include <engine/game/game.hpp>
#include <engine/game/spring.hpp>
#include <engine/game/upanddown.hpp>
#include <engine/game/freefall.hpp>
#include <engine/game/shooter.hpp>
#include <engine/game/buttondemo.hpp>
#include <engine/game/sliderdemo.hpp>
#include <memory>
#include <chrono>
#include <engine/utils/resourcemanager.hpp>

// CRT's memory leak detection
#ifndef NDEBUG 
#if defined(_MSC_VER)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#include <iostream>

#endif

int main(int argc, char* argv[])
{
#ifndef NDEBUG 
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		//_CrtSetBreakAlloc(769028);
#endif
#endif
	
	srand(static_cast<unsigned int>(time(NULL)));
	
	//Game game;
	//game.addState(std::make_unique<UpAndDown>());
	//game.addState(std::make_unique<Spring>());
	//game.run(std::make_unique<FreeFall>());
	//game.addState(std::make_unique<ButtonDemo>());
	//game.run(std::make_unique<Shooter>());	

	//game.run(std::make_unique<SliderDemo>());

	Game game;
	game.addState(std::make_unique<FreeFall>());
	game.addState(std::make_unique<Spring>());
	game.run(std::make_unique<ButtonDemo>());
	
	//utils::MeshLoader::clear();
	//graphics::ShaderManager::clear();
	//graphics::Texture2DManager::clear();
	graphics::FontManager::clear();
	
	//TODO: auswahl Gamestates, bearbeitung slider
	
	
	//System system;
	//system.deleteFontRenderer();

	return EXIT_SUCCESS;
}
