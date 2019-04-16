#include "Components.h"
#include "utils.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include "stb_image.h"
#include "linAlg.h"
#include <cmath>
#include <nlohmann/json.hpp>
#include "ModelGen.h"
#include "BufferData.h"

static void error_callback(int err, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

bool keys[512];

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (action == GLFW_PRESS) {
		keys[key] = 1;
    }
	else {
		keys[key] = 0;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	static double lastXPos, lastYPos = 0.0;
	double dx = xpos-lastXPos;
	double dy = ypos-lastYPos;
	Camera *c = &RenderEngine::get().c;
	c->horizTurn(dx*-.001);
	c->vertTurn(dy*-.001);
	lastXPos = xpos;
	lastYPos = ypos;
}

WindowManager::WindowManager() {
	if (!glfwInit()) {
		exit(1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(error_callback);
	auto monitor = glfwGetPrimaryMonitor();
	auto mode = glfwGetVideoMode(monitor);
	window = glfwCreateWindow(mode->width, mode->height, "test", glfwGetPrimaryMonitor(), nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
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
	currentModel = 0;
	WindowManager::get();
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	std::string w1, w2, w3;
	std::ifstream shaderFile("shaders/programs.dat");
	while (!shaderFile.eof()) {
		shaderFile >> w1 >> w2 >> w3;
		programs[w1] = LoadShader(("shaders/"+w2).c_str(), ("shaders/"+w3).c_str());
	}
	glClearColor(1,0,0,1);
	glEnable(GL_DEPTH_TEST);
}

RenderEngine::~RenderEngine() {
}

RenderEngine& RenderEngine::get() {
	static RenderEngine r;
	return r;
}

void RenderEngine::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	auto cMat = c.getCameraMat();
	auto pMat = projection(3.14f/4, (float) WindowManager::get().windowWidth / WindowManager::get().windowHeight, 1.0f,100.0f);
	glUseProgram(programs["texture"]);
	glBindVertexArray(VAID);
	auto entityM = &EntityManager::get();
	GLuint MVP = glGetUniformLocation(programs["texture"], "MVP");
	GLuint MAT = glGetUniformLocation(programs["texture"], "mat");
	for (int  i = 0; i < entityM->getSize(); i++) {
		auto e = entityM->getEntity(i);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, AssetManager::get().GetTexture(e->tex));
		auto model = e->m;
		auto mMat = e->getModel();
		auto MVPMat  = pMat*cMat*mMat;
		glUniformMatrix4fv(MVP, 1, GL_TRUE, &MVPMat(0));
		for (int j = 0; j < model->numMeshes; j++) {
			auto m = &model->meshes[j];
			glUniform4fv(MAT, 1, &m->mat(0));
			void *offset = m->offset;
			int baseV = m->baseV;
			size_t size = m->size;
			glDrawElementsBaseVertex(GL_TRIANGLES, size, GL_UNSIGNED_INT, offset, baseV);
		}

	}
	


	glfwSwapBuffers(WindowManager::get().getWindow());
}

MemoryManager::MemoryManager() : s(512000) {
}

MemoryManager::~MemoryManager() {
}

StackAlloc& MemoryManager::getStack() {
	return s;
}

tempAlloc& MemoryManager::getTempSeg() {
	return t;
}

MemoryManager& MemoryManager::get() {
	static MemoryManager m;
	return m;
}

AssetManager::AssetManager() {
	MemoryManager::get();
	RenderEngine::get();
	LoadScene("Assets/scene.json");
	/*
	auto stack = &MemoryManager::get().getStack();
	stack->setFrame();
	modelList = (Model*) MemoryManager::get().getStack().allocSpace(4*sizeof(Model));
	BufferData *data = (BufferData*) stack->allocSpace(sizeof(BufferData));
	*data = genPlane(2,2);
	BufferData data2 = genCube();
	BufferData data3 = genPyramid();
	BufferData data4 = genSphere(64,64);
	GLuint VAID;
	glGenVertexArrays(1, &VAID);
	
	RenderEngine::get().VAID = VAID;

	glBindVertexArray(VAID);

	tempAlloc *tempSeg = &MemoryManager::get().getTempSeg();
	size_t numV = data->vertices.size()
                      +data2.vertices.size()
                      +data3.vertices.size()
                      +data4.vertices.size();


	auto vertexSeg = tempSeg->allocNum<float>(numV);
	std::copy(data->vertices.begin(), data->vertices.end(), vertexSeg);
	std::copy(data2.vertices.begin(), data2.vertices.end(), vertexSeg+data->vertices.size());
	std::copy(data3.vertices.begin(), data3.vertices.end(), vertexSeg+data->vertices.size()+data2.vertices.size());
	std::copy(data4.vertices.begin(), data4.vertices.end(), vertexSeg+data->vertices.size()+data2.vertices.size()+data3.vertices.size());
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, numV*sizeof(float), vertexSeg, 
			GL_STATIC_DRAW);
	glVertexAttribPointer(
   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   3,                  // size
   GL_FLOAT,           // type
   GL_FALSE,           // normalized?
   0,                  // stride
   (void*)0            // array buffer offset
);

	size_t numI = data->indices.size()
		+data2.indices.size()
		+data3.indices.size()
		+data4.indices.size();
	auto indexSeg = tempSeg->allocNum<unsigned int>(numI);
	std::copy(data->indices.begin(), data->indices.end(), indexSeg);
	std::copy(data2.indices.begin(), data2.indices.end(), indexSeg+data->indices.size());
	std::copy(data3.indices.begin(), data3.indices.end(), indexSeg+data->indices.size()+data2.indices.size());
	std::copy(data4.indices.begin(), data4.indices.end(), indexSeg+data->indices.size()+data2.indices.size()+data3.indices.size());
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			numI*sizeof(unsigned int),
			indexSeg, GL_STATIC_DRAW);

	size_t numT = data->textureCoord.size()
		+data2.textureCoord.size()
		+data3.textureCoord.size()
		+data4.textureCoord.size();

	auto texSeg = tempSeg->allocNum<float>(numT);
	std::copy(data->textureCoord.begin(), data->textureCoord.end(), texSeg);
	std::copy(data2.textureCoord.begin(), data2.textureCoord.end(), texSeg+data->textureCoord.size());
	std::copy(data3.textureCoord.begin(), data3.textureCoord.end(), texSeg+data->textureCoord.size()+data2.textureCoord.size());
	std::copy(data4.textureCoord.begin(), data4.textureCoord.end(), texSeg+data->textureCoord.size()+data2.textureCoord.size()+data3.textureCoord.size());



	GLuint textureBuffer;
	glGenBuffers(1, &textureBuffer);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, numT*sizeof(float),
			texSeg, GL_STATIC_DRAW);
	glVertexAttribPointer(
		   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_TRUE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);
	new (modelList) Model(VAID, RenderEngine::get().programs["texture"],
		(void*) 0, 0, data->indices.size(), "plane", nullptr);
	new (modelList+1) Model(VAID, RenderEngine::get().programs["texture"],
		(void*) (data->indices.size()*sizeof(unsigned int)), data->vertices.size()/3, data2.indices.size(), "box", nullptr);
	new (modelList+2) Model(VAID, RenderEngine::get().programs["texture"],
		(void*) ((data->indices.size()+data2.indices.size())*sizeof(unsigned int)), (data->vertices.size()+data2.vertices.size())/3, data3.indices.size(), "pyramid", nullptr);
	new (modelList+3) Model(VAID, RenderEngine::get().programs["texture"],
		(void*) ((data->indices.size()+data2.indices.size()+data3.indices.size())*sizeof(unsigned int)), (data->vertices.size()+data2.vertices.size()+data3.vertices.size())/3, 
		data4.indices.size(), "sphere", nullptr);

	int width, height, nrChannels;
	unsigned char *texData = stbi_load("Assets/bricks.jpeg", &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(texData);
	brickTex = texture;
	auto entityM = &EntityManager::get();
	entityM->allocSpace(64);
	Entity e(&modelList[0]);
	e.scaleBy(20,1,20);
	entityM->addEntity(e);
	for (int i = 0; i < 5; i++) {
		e = Entity(modelList+1);
		e.translateBy(-5,0,-20+4*i);
		entityM->addEntity(e);

		e = Entity(modelList+3);
		e.translateBy(-5,2,-20+4*i);
		entityM->addEntity(e);


		e = Entity(modelList+1);
		e.translateBy(5,0,-20+4*i);
		entityM->addEntity(e);

		e = Entity(modelList+3);
		e.translateBy(5,2,-20+4*i);
		entityM->addEntity(e);
	}
	e = Entity(modelList+2);
	e.translateBy(0,0,-10);
	entityM->addEntity(e);
	*/
}

AssetManager::~AssetManager() {
}

AssetManager& AssetManager::get() {
	static AssetManager a;
	return a;
}

Model* AssetManager::getModel(int index) {
	return &modelList[index];
}

void AssetManager::LoadScene(std::string filename) {
	using json = nlohmann::json;
	std::ifstream input(filename);
	json j;
	input >> j;
	input.close();
	auto stack = &MemoryManager::get().getStack();
	stack->setFrame();
	numTextures = j[0].size();
	textures = (GLuint*) stack->allocSpace(numTextures*sizeof(GLuint));
	for (int i = 0; i < numTextures; i++) {
		textures[i] = LoadTexture(j[0][i]);
	}
	BufferData **modelData = (BufferData**) stack->allocSpace((j[1].size())*sizeof(BufferData*));
	numModels = j[1].size();
	modelList = (Model*) stack->allocSpace(numModels*sizeof(Model));
	size_t totalVertices = 0, totalTex = 0, totalNormals = 0, totalIndices = 0;
	for (int i = 0; i < j[1].size(); i++) {
		size_t numMeshes;
		if (j[1][i] == "sphere") {
			numMeshes = 1;
			modelData[i] = (BufferData*) stack->allocSpace(sizeof(BufferData));
			modelData[i][0] = genSphere(32,32);
		}
		else if (j[1][i] == "cube") {
			numMeshes = 1;
			modelData[i] = (BufferData*) stack->allocSpace(sizeof(BufferData));
			modelData[i][0] = genCube();
		}
		else if (j[1][i] == "plane") {
			numMeshes = 1;
			modelData[i] = (BufferData*) stack->allocSpace(sizeof(BufferData));
			modelData[i][0] = genPlane(2, 2);
		}
		else if (j[1][i] == "pyramid") {
			numMeshes = 1;
			modelData[i] = (BufferData*) stack->allocSpace(sizeof(BufferData));
			modelData[i][0] = genPyramid();
		}
		else {
			modelData[i] = (BufferData*) LoadModel(j[1][i], &numMeshes);
		}
		for (int j = 0; j < numMeshes; j++) {
			totalVertices += modelData[i][j].vertices.size();
			totalTex += modelData[i][j].textureCoord.size();
			totalNormals += modelData[i][j].normals.size();
			totalIndices += modelData[i][j].indices.size();
		}
		auto meshes = (Mesh*) stack->allocSpace(numMeshes*sizeof(Mesh));
		new (modelList+i) Model(meshes, numMeshes);
		for (int j = 0; j < numMeshes; j++) {
			new (meshes+i) Mesh((void*) &modelData[i][j]);
		}
	}
	GLuint VAID;
	glGenVertexArrays(1, &VAID);
	
	RenderEngine::get().VAID = VAID;

	glBindVertexArray(VAID);

	tempAlloc *tempSeg = &MemoryManager::get().getTempSeg();


	auto vertexSeg = tempSeg->allocNum<float>(totalVertices);
	int curr = 0;
	for (int i = 0; i < numModels; i++) {
		for (int j = 0; j < modelList[i].numMeshes; j++) {
			std::copy(modelData[i][j].vertices.begin(), modelData[i][j].vertices.end(), vertexSeg+curr);
			curr += modelData[i][j].vertices.size();
		}
	}
	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, totalVertices*sizeof(float), vertexSeg, 
			GL_STATIC_DRAW);
	glVertexAttribPointer(
   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   3,                  // size
   GL_FLOAT,           // type
   GL_FALSE,           // normalized?
   0,                  // stride
   (void*)0            // array buffer offset
);
	auto indexSeg = tempSeg->allocNum<GLuint>(totalIndices);
	curr = 0;
	for (int i = 0; i < numModels; i++) {
		for (int j = 0; j < modelList[i].numMeshes; j++) {
			std::copy(modelData[i][j].indices.begin(), modelData[i][j].indices.end(), indexSeg+curr); 
			curr += modelData[i][j].indices.size();
		}
	}
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, totalIndices*sizeof(GLuint), indexSeg, 
			GL_STATIC_DRAW);


	auto normalSeg = tempSeg->allocNum<float>(totalVertices);
	curr = 0;
	for (int i = 0; i < numModels; i++) {
		for (int j = 0; j < modelList[i].numMeshes; j++) {
			std::copy(modelData[i][j].normals.begin(), modelData[i][j].normals.end(), normalSeg+curr);
			curr += modelData[i][j].normals.size();
		}
	}
	GLuint normalBuffer;
	glGenBuffers(1, &normalBuffer);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, totalNormals*sizeof(float), normalSeg, 
			GL_STATIC_DRAW);
	glVertexAttribPointer(
   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   3,                  // size
   GL_FLOAT,           // type
   GL_FALSE,           // normalized?
   0,                  // stride
   (void*)0            // array buffer offset
   );


	auto texSeg = tempSeg->allocNum<float>(totalTex);
	curr = 0;
	for (int i = 0; i < numModels; i++) {
		for (int j = 0; j < modelList[i].numMeshes; j++) {
			std::copy(modelData[i][j].textureCoord.begin(), modelData[i][j].textureCoord.end(), texSeg+curr);
			curr += modelData[i][j].textureCoord.size();
		}
	}
	GLuint texBuffer;
	glGenBuffers(1, &texBuffer);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
	glBufferData(GL_ARRAY_BUFFER, totalTex*sizeof(float), texSeg, 
			GL_STATIC_DRAW);
	glVertexAttribPointer(
   2,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   2,                  // size
   GL_FLOAT,           // type
   GL_FALSE,           // normalized?
   0,                  // stride
   (void*)0            // array buffer offset
);

	curr = 0;
	int curr2 = 0;
	for (int i = 0; i < numModels; i++) {
		for (int j = 0; j < modelList[i].numMeshes; j++) {
			auto meshes = modelList[i].meshes;
			auto shader = RenderEngine::get().programs["texture"];
			meshes[j].setLoaded(VAID, shader, (void*) (curr*sizeof(GLuint)), curr2/3, modelData[i][j].indices.size());
			meshes[j].mat = modelData[i][j].mat;
			curr += modelData[i][j].indices.size();
			curr2 += modelData[i][j].vertices.size();
		}
	}
	stack->setFrame();
	processNode(j[2], nullptr);
}

void* AssetManager::LoadModel(std::string filename, size_t *size) {
	static Assimp::Importer importer;
	auto scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices);
	auto stack = &MemoryManager::get().getStack();
	BufferData *bdata = (BufferData*) stack->allocSpace(scene->mNumMeshes*sizeof(BufferData));
	if (size != nullptr) {
		*size = scene->mNumMeshes;
	}
	colVector<float, 4> mat;
	for (int i = 0; i < scene->mNumMeshes; i++) {
		auto mesh = scene->mMeshes[i];
		if (mesh->mMaterialIndex >= 0) {
			auto aiMat = scene->mMaterials[mesh->mMaterialIndex];
			aiColor3D color(0.0f,0.0f,0.0f);
			aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			mat << color.b,color.g,color.r,1.0f;
		}
		else {
			mat << 1.0f,1.0f,1.0f,1.0f;
		}
		auto vertices = StackArray<float>(mesh->mNumVertices*3);
		auto normals = StackArray<float>(mesh->mNumVertices*3);
		auto texCoords = StackArray<float>(mesh->mNumVertices*2);
		auto indices = StackArray<unsigned int>(mesh->mNumFaces*3);
		for (int j = 0; j < mesh->mNumVertices; j++) {
			vertices[3*j] = mesh->mVertices[j].x;
			vertices[3*j+1] = mesh->mVertices[j].y;
			vertices[3*j+2] = mesh->mVertices[j].z;

			normals[3*j] = mesh->mNormals[j].x;
			normals[3*j+1] = mesh->mNormals[j].y;
			normals[3*j+2] = mesh->mNormals[j].z;

			texCoords[2*j] = mesh->mTextureCoords[0][j].x;
			texCoords[2*j+1] = mesh->mTextureCoords[0][j].y;
		}
		for (int j = 0; j < mesh->mNumFaces; j++) {
			indices[3*j] = mesh->mFaces[j].mIndices[0];
			indices[3*j+1] = mesh->mFaces[j].mIndices[1];
			indices[3*j+2] = mesh->mFaces[j].mIndices[2];
		}
		bdata[i] = BufferData{vertices, indices, normals, texCoords,mat};
	}
	return bdata;
}


GLuint AssetManager::LoadTexture(std::string filename) {
	int width, height, nrChannels;
	unsigned char *texData = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	stbi_image_free(texData);
	return texture;
}

void AssetManager::processNode(nlohmann::json j, Entity *parent) {
	if (j.size() == 0) return;
	Entity *entities;
	auto stack = &MemoryManager::get().getStack();
	if (parent == nullptr) {
		EntityManager::get().allocSpace(j.size());
		entities = EntityManager::get().getEntity(0);
	}
	else {
		parent->numChildren = j.size();
		parent->children = (Entity*) stack->allocSpace(j.size()*sizeof(Entity));
		entities = parent->children;
	}
	for (int i = 0; i < j.size(); i++) {
		auto model = getModel(j[i]["model"]);
		new (entities+i) Entity(model);
		entities[i].parent = parent;
		entities[i].scaleBy(j[i]["scale"][0], j[i]["scale"][1], j[i]["scale"][2]);
		quaternion<float> rot;
		rot << j[i]["rotation"][0],j[i]["rotation"][1],j[i]["rotation"][2],j[i]["rotation"][3];
		entities[i].rotateBy(rot);
		entities[i].translateBy(j[i]["translation"][0], j[i]["translation"][1], j[i]["translation"][2]);
		entities[i].tex = j[i]["tex"];
		processNode(j[i]["children"], &entities[i]);
	}
}

GLuint AssetManager::GetTexture(size_t i) {
	return textures[i];
}



EntityManager::EntityManager() : size(0), entities(nullptr) {
}

EntityManager::~EntityManager() {
}

void EntityManager::allocSpace(size_t cap) {
	auto stack = &MemoryManager::get().getStack();
	entities = (Entity*) stack->allocSpace(sizeof(Entity)*cap);
	size = cap;
}

Entity* EntityManager::addEntity(const Entity &e) {
	new (entities+size) Entity(e);
	return entities+(size++);
}

size_t EntityManager::getSize() {
	return size;
}

Entity* EntityManager::getEntity(int index) {
	return entities+index;
}

EntityManager& EntityManager::get() {
	static EntityManager e;
	return e;
}

GameEngine::GameEngine()  {
	RenderEngine::get();
	AssetManager::get();
}

GameEngine::~GameEngine() {
}

GameEngine& GameEngine::get() {
	static GameEngine g;
	return g;
}

void GameEngine::update() {
	if (keys[GLFW_KEY_A]) {
		colVector<float, 3> pos;
		pos << -.05,0,0;
		RenderEngine::get().c.move(pos);
	}
	else if (keys[GLFW_KEY_D]) {
		colVector<float, 3> pos;
		pos << .05,0,0;
		RenderEngine::get().c.move(pos);
	}
	if (keys[GLFW_KEY_W]) {
	    colVector<float, 3> pos;
	    pos << 0,0,-0.05;
	    RenderEngine::get().c.move(pos);
	}
	else if (keys[GLFW_KEY_S]) {
	    colVector<float, 3> pos;
	    pos << 0,0,0.05;
	    RenderEngine::get().c.move(pos);
	}
	RenderEngine::get().render();
	glfwPollEvents();
}

bool GameEngine::isRunning() {
	return !glfwWindowShouldClose(WindowManager::get().getWindow());
}
