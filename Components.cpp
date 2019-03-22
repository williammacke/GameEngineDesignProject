#include "Components.h"
#include "utils.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include "stb_image.h"
#include "linAlg.h"
#include <cmath>



static void error_callback(int err, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_A) {
	    colVector<float, 3> pos;
	    pos << 0,0,-0.1;
	    RenderEngine::get().c.move(pos);
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
	window = glfwCreateWindow(1000, 1000, "test", glfwGetPrimaryMonitor(), nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
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
	static float xPos = 0;
	xPos -=0.1;
	
	glUseProgram(programs["texture"]);
	glBindVertexArray(VAID);
	glBindTexture(GL_TEXTURE_2D, AssetManager::get().brickTex);
	Model m = AssetManager::get().getModel(currentModel);
	GLuint MVP = glGetUniformLocation(programs["texture"], "MVP");
	void *offset = m.offset;
	size_t size = m.size;
	auto cMat = c.getCameraMat();
	//auto cMat = Identity<float, 4>();
	//auto mMat = translate(0,0,5)*rotationY(xPos);
	auto mMat = translate(10*sin(xPos),0,-50);
	//auto mMat = rotationX(3.14f/2)*translate(0,0,10);
	auto pMat = projection(3.14f/4, (float) WindowManager::get().windowWidth / WindowManager::get().windowHeight, 1.0f,100.0f);
	auto MVPMat  = pMat*cMat*mMat;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << MVPMat(i,j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, &MVPMat(0));


	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, offset);

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
	modelList = MemoryManager::get().getStack().allocNum<Model>(4);
	//BufferData data = genPlane(2,2);
	BufferData data = genCube();
	GLuint VAID;
	glGenVertexArrays(1, &VAID);
	
	RenderEngine::get().VAID = VAID;

	glBindVertexArray(VAID);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, data.vertices.size()*sizeof(float), &data.vertices[0], 
			GL_STATIC_DRAW);
	glVertexAttribPointer(
   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   3,                  // size
   GL_FLOAT,           // type
   GL_FALSE,           // normalized?
   0,                  // stride
   (void*)0            // array buffer offset
);
	GLuint indexBuffer;
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			data.indices.size()*sizeof(unsigned int),
			&data.indices[0], GL_STATIC_DRAW);

	GLuint textureBuffer;
	glGenBuffers(1, &textureBuffer);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, data.textureCoord.size()*sizeof(float),
			&data.textureCoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_TRUE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);
	new (modelList) Model(VAID, RenderEngine::get().programs["texture"],
		(void*) 0, data.indices.size(), "plane", nullptr);

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
}

AssetManager::~AssetManager() {
}

AssetManager& AssetManager::get() {
	static AssetManager a;
	return a;
}

Model AssetManager::getModel(int index) {
	return modelList[index];
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
	RenderEngine::get().render();
	glfwPollEvents();
}

bool GameEngine::isRunning() {
	return !glfwWindowShouldClose(WindowManager::get().getWindow());
}
