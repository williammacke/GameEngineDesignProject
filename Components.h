#ifndef COMPONENTS_H_
#define COMPONENTS_H_
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "memMan.h"
#include <unordered_map>
#include <string>
#include "Camera.h"
#include "models.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <nlohmann/json.hpp>


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
	Model* getModel(int index);
	GLuint brickTex;
	void LoadScene(std::string filename);
	void* LoadModel(std::string filename, size_t *size);
	GLuint LoadTexture(std::string filename);
	GLuint GetTexture(size_t i);
private:
	size_t numModels;
	Model *modelList;
	GLuint *textures;
	size_t numTextures;
	void processNode(nlohmann::json j, Entity *parent);
};

class EntityManager {
public:
	EntityManager();
	~EntityManager();
	void allocSpace(size_t size);
	Entity* addEntity(const Entity &e);
	size_t getSize();
	Entity *getEntity(int index);
	static EntityManager& get();
private:
	size_t size;
	Entity *entities;
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
