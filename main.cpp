#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include "linAlg.h"
#include "utils.h"
#include "memMan.h"
#include <iostream>
#include <chrono>
#include <cstdlib>
#include "Components.h"
#include <cassert>





int main() {
	auto g = &GameEngine::get();
	auto time1 = std::chrono::high_resolution_clock::now();
	colVector<float, 4> *vecs = g->s.allocNum<colVector<float,4>>(5000);
	auto time2 = std::chrono::high_resolution_clock::now();
	auto diff = time2-time1;
	std::cout << "Stack Allocation Time: " << diff.count() << std::endl;
	colVector<float, 3> axis;
	axis << 0,1,0;
	colVector<float, 3> axis2;
	axis2 << 1,0,0;
	colVector<float, 4> testVec;
	float angle;
	time1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 5000; i++) {
		vecs[i] << 1,0,0,1;
		angle = (i/5000.) * 3.14/2;
		auto quat = genQuaternion(axis, angle);
		auto nv = rotate(vecs[i], quat);
		testVec << cos(angle),0,-1*sin(angle),1;
		assert(nv == testVec);
		nv = rotate(nv, invert(quat));
		testVec << 1,0,0,1;
		assert(nv == testVec);
		auto quat2 = genQuaternion(axis2, angle);
		nv = rotate(vecs[i], quat);
		nv = rotate(nv, quat2);
		nv = rotate(nv, invert(quat2));
		nv = rotate(nv, invert(quat));
		assert(nv == testVec);
	}
	time2 = std::chrono::high_resolution_clock::now();
	diff = time2-time1;
	std::cout << "Quaternion Time: " << diff.count() << std::endl;
	time1 = std::chrono::high_resolution_clock::now();
	Matrix<float, 4, 4> mat;
	for (int i = 1; i < 5000; i++) {
		vecs[i] << 1,0,0,1;
		angle = (i/5000.)*3.14/2;
		mat = rotationY(angle);
		auto nv = mat*vecs[i];
		testVec << cos(angle),0,-1*sin(angle),1;
		assert(nv == testVec);
	}

	time2 = std::chrono::high_resolution_clock::now();
	diff = time2-time1;
	std::cout << "Matrix Rotation Y Time: " << diff.count() << std::endl;
	time1 = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < 5000; i++) {
		vecs[i] << 1,0,0,1;
		angle = (i/5000.)*3.14/2;
		mat = rotationZ(angle);
		auto nv = mat*vecs[i];
		testVec << cos(angle),sin(angle),0,1;
		assert(nv == testVec);
	}
	time2 = std::chrono::high_resolution_clock::now();
	diff = time2-time1;
	std::cout << "Matrix Rotation Z Time: " << diff.count() << std::endl;
	time1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 5000; i++) {
		vecs[i] << 0,1,0,1;
		angle = (i/5000.)*3.14/2;
		mat = rotationX(angle);
		auto nv = mat*vecs[i];
		testVec << 0,cos(angle),sin(angle),1;
		assert(nv == testVec);
	}
	time2 = std::chrono::high_resolution_clock::now();
	diff = time2-time1;
	std::cout << "Matrix Rotation X Time: " << diff.count() << std::endl;
	time1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 5000; i++) {
		vecs[i] << 1,1,1,1;
		mat = scale(i,5000-i,1);
		auto nv = mat*vecs[i];
		testVec << i,5000-i,1,1;
		assert(nv == testVec);
	}

	time2 = std::chrono::high_resolution_clock::now();
	diff = time2-time1;
	std::cout << "Matrix Scale Time: " << diff.count() << std::endl;
	time1 = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < 5000; i++) {
		vecs[i] << 0,0,0,1;
		mat = translate(i,i*2,5000-i);
		auto nv = mat*vecs[i];
		testVec << i,i*2,5000-i,1;
		assert(nv == testVec);
	}
	time2 = std::chrono::high_resolution_clock::now();
	diff = time2-time1;
	std::cout << "Matrix Translate Time: " << diff.count() << std::endl;
	time1 = std::chrono::high_resolution_clock::now();

	while (g->isRunning()) {
		g->update();
	}



	/*
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
	*/
}

