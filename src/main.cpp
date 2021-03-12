#pragma once
#include <engine/game/game.hpp>
#include <engine/game/spring.hpp>
#include <engine/game/upanddown.hpp>
#include <engine/game/freefall.hpp>
#include <engine/game/shooter.hpp>
#include <engine/game/lights.hpp>
#include <engine/game/buttondemo.hpp>
#include <engine/game/sliderdemo.hpp>

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

	Game game;
	game.addState(std::make_unique<Shooter>());
	game.addState(std::make_unique<UpAndDown>());
	game.addState(std::make_unique<FreeFall>());
	game.addState(std::make_unique<Spring>());
	game.addState(std::make_unique<Lights>());
	game.addState(std::make_unique<SliderDemo>());
	game.run(std::make_unique<ButtonDemo>());
	
	//utils::MeshLoader::clear();
	//graphics::ShaderManager::clear();
	//graphics::Texture2DManager::clear();
	graphics::FontManager::clear();

	return EXIT_SUCCESS;
}
