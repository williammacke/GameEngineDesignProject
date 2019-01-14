#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include "linAlg.h"

Matrix<int, 1, 5> mat;
colVector<int, 5> vec;


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
	mat << 1,1,1,1,1;
	vec << 2,2,3,2,2;
	auto vec2 = (vec)*mat;
	colVector<double, 4> vec3;
	vec3 << 3,2.,1.,1.;
	auto mat2 = rotationZ<double>(.9f);
	vec3 = mat2*vec3;
	for (int i = 0; i < 4; i++) {
		printf("%f ", vec3(i));
	}
	printf("\n");
	vec2 *= 5;
	for (int i = 0; i < 25; i++) {
		printf("%d ", vec2(i));
	}
	printf("\n");
	if (!glfwInit()) {
		return 1;
	}
	glfwSetErrorCallback(error_callback);

	GLFWwindow* window = glfwCreateWindow(1000, 1000, "test", nullptr, nullptr);

	if (!window) {
		return 1;
	}				



	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
	glfwSetKeyCallback(window, key_callback);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glfwSwapInterval(1);
	glClearColor(1,1,1,1);

	while (!glfwWindowShouldClose(window)) {
		// ...
		glClear( GL_COLOR_BUFFER_BIT );
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}

