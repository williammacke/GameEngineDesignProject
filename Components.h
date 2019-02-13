#ifndef COMPONENTS_H_
#define COMPONENTS_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "memMan.h"

class MemoryManager {
public:
	MemoryManager();
	~MemoryManager();
	static MemoryManager& get();
private:
	StackAlloc s;
};

class WindowManager {
public:
	WindowManager();
	~WindowManager();
	static WindowManager& get();
	GLFWwindow* getWindow();
private:
	GLFWwindow* window;
};

class RenderEngine {
public:
	RenderEngine();
	~RenderEngine();
	static RenderEngine& get();
	void render();
private:
	WindowManager& w;
};

class GameEngine {
public:
	GameEngine();
	~GameEngine();
	static GameEngine& get();
	void update();
private:
	RenderEngine& r;
	MemoryManager& m;
};

#endif
