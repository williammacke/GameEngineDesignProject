#include "Components.h"
#include <cstdlib>
#include <cstdio>

MemoryManager::MemoryManager() : s(256000000) {

}

MemoryManager::~MemoryManager() {
}

MemoryManager& MemoryManager::get() {
	static MemoryManager m;
	return m;
}


static void error_callback(int err, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

WindowManager::WindowManager() {
	printf("blargh\n");
	if (!glfwInit()) {
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(error_callback);
	window = glfwCreateWindow(1000, 1000, "test", nullptr, nullptr);
	glfwMakeContextCurrent(window);
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


RenderEngine::RenderEngine() : w(WindowManager::get()) {
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
	glfwSwapBuffers(w.getWindow());
}

GameEngine::GameEngine() : r(RenderEngine::get()), 
	m(MemoryManager::get()) {
}

GameEngine::~GameEngine() {
}

GameEngine& GameEngine::get() {
	static GameEngine g;
	return g;
}

void GameEngine::update() {
	r.render();
}
