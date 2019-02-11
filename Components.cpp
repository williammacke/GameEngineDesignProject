#include "Components.h"

WindowManager::WindowManager() {
}

WindowManager::~WindowManager() {
}

WindowManager WindowManager::get() {
	static WindowManager w;
	return w;
}


RenderEngine::RenderEngine() {
	w = WindowManager::get();
}

RenderEngine::~RenderEngine() {
}

RenderEngine RenderEngine::get() {
	static RenderEngine r;
	return r;
}

GameEngine::GameEngine() {
	r = RenderEngine::get();
}

GameEngine::~GameEngine() {
}

GameEngine GameEngine::get() {
	static GameEngine g;
	return g;
}
