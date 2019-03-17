#ifndef COMPONENTS_H_
#define COMPONENTS_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "memMan.h"
#include <unordered_map>
#include <string>
#include "Camera.h"


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
	std::unordered_map<std::string, GLuint> programs;
	Camera c;
};

class GameEngine {
public:
	GameEngine();
	~GameEngine();
	static GameEngine& get();
	void update();
	bool isRunning();
	StackAlloc s;
};

#endif
