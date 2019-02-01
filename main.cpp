#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "linAlg.h"
#include "utils.h"
#include "memMan.h"


float vData[] = {0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f};


void error_callback(int err, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_A && action == GLFW_PRESS) {
	    glClearColor(0,0,0,1);
    }
    if (key == GLFW_KEY_S && action == GLFW_PRESS) {
	    glClearColor(1,0,0,1);
    }
}

int main() {
	char* testS = tprintf("this is %d\n", 5);
	printf("%s", testS);
	auto blargh = poolAlloc<Matrix<float, 3, 3>>::alloc();
	auto blargh2 = poolAlloc<Matrix<float, 3, 3>>::alloc(*blargh);
	printf("%p\n", blargh);
	colVector<int, 3> vec1;
	colVector<int, 3> vec2;
	(*blargh)*vec1;
	poolAlloc<Matrix<float, 3, 3>>::remove(blargh);
	printf("%p\n", blargh);
	vec1 << 1,1,1;
	vec2 << 1,1,1;
	auto vec3 = trans(vec1)*vec2;
	printf("%d\n", vec3);
	if (!glfwInit()) {
		return 1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(1000, 1000, "test", nullptr, nullptr);

	auto model = translate(0,0,-1);
	model *= scale(0.5, 0.5, 0.5);
	auto proj = projection(1.0, 1.0, 1.0, 40.0);
	auto MVP = proj*model;

	if (!window) {
		return 1;
	}				



	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	
	printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);
	auto program = LoadShader("shaders/vshader.vert", "shaders/fshader.frag");
	glfwSetKeyCallback(window, key_callback);

	GLuint VAID;
	glGenVertexArrays(1, &VAID);
	glBindVertexArray(VAID);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vData), vData, GL_STATIC_DRAW);
	glVertexAttribPointer(
   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
   3,                  // size
   GL_FLOAT,           // type
   GL_FALSE,           // normalized?
   0,                  // stride
   (void*)0            // array buffer offset
);



	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwSwapInterval(1);
	glClearColor(1,1,1,1);

	while (!glfwWindowShouldClose(window)) {
		// ...
		glClear( GL_COLOR_BUFFER_BIT );
		glUseProgram(program);
		GLuint MatrixID = glGetUniformLocation(program, "MVP");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP(0));
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

