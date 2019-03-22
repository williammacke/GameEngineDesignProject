#ifndef COMPONENTS_H_
#define COMPONENTS_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "memMan.h"
#include <unordered_map>
#include <string>
#include "Camera.h"
#include "models.h"
#include "ModelGen.h"


class WindowManager {
public:
	WindowManager();
	~WindowManager();
	static WindowManager& get();
	GLFWwindow* getWindow();
	int windowWidth, windowHeight;
private:
	GLFWwindow* window;
};

class RenderEngine {
public:
	RenderEngine();
	~RenderEngine();
	static RenderEngine& get();
	void render();
	std::unordered_map<std::string, GLuint> programs;
	GLuint VAID;
	int currentModel;
	Camera c;
};

class MemoryManager {
public:
	MemoryManager();
	~MemoryManager();
	static MemoryManager& get();
	StackAlloc& getStack();
	tempAlloc& getTempSeg();
	template <class T>
	PoolAlloc<T>& getPool();
private:
	StackAlloc s;
	tempAlloc t;
};

template <class T>
PoolAlloc<T>& MemoryManager::getPool() {
	static PoolAlloc<T> p;
	return &p;
}

class AssetManager {
public:
	AssetManager();
	~AssetManager();
	static AssetManager& get();
	Model getModel(int index);
	GLuint brickTex;
	//void LoadModel(std::string filename);
private:
	size_t numModels;
	size_t modelCap;
	Model *modelList;
};

class GameEngine {
public:
	GameEngine();
	~GameEngine();
	static GameEngine& get();
	void update();
	bool isRunning();
};

#endif
