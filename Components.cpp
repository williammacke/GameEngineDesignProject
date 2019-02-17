#include "Components.h"
#include <cstdlib>
#include <cstdio>



static void error_callback(int err, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
	    glClearColor(0,0,0,1);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
	    glClearColor(1,0,0,1);
    }
}

WindowManager::WindowManager() {
	if (!glfwInit()) {
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(error_callback);
	window = glfwCreateWindow(1000, 1000, "test", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
}

WindowManager::~WindowManager() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

WindowManager& WindowManager::get() {
	static WindowManager w;
	return w;
}

GLFWwindow* WindowManager::getWindow() {
	return window;
}


RenderEngine::RenderEngine()  {
	WindowManager::get();
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glClearColor(1,1,1,1);
}

RenderEngine::~RenderEngine() {
}

RenderEngine& RenderEngine::get() {
	static RenderEngine r;
	return r;
}

void RenderEngine::render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(WindowManager::get().getWindow());
}

GameEngine::GameEngine() : s(512000) {
	RenderEngine::get();
}

GameEngine::~GameEngine() {
}

GameEngine& GameEngine::get() {
	static GameEngine g;
	return g;
}

void GameEngine::update() {
	RenderEngine::get().render();
	glfwPollEvents();
}

bool GameEngine::isRunning() {
	return !glfwWindowShouldClose(WindowManager::get().getWindow());
}
