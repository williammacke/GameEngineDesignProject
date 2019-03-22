#ifndef MODELS_H_
#define MODELS_H_
#include <vector>
#include <string>
#include <glad/glad.h>
#include "linAlg.h"

extern GLuint currentVAO;
extern GLuint currentProgram;

class Model {
public:
	Model();
	Model(GLuint VAO, GLuint shaderProgram, void *offset, size_t size, std::string name, void *data);
	Model(const Model& m);
	~Model();
	GLuint VAO;
	GLuint shaderProgram;
	void *offset;
	size_t size;
	std::string name;
	void *data;
};

class Entity {
public:
	Entity(Model *m);
	~Entity();
	Model *m;
	Matrix<float, 4, 4> getModel();
private:
	Matrix<float, 4, 4> translation;
	quaternion<float> rotation;
	Matrix<float, 4, 4> scaling;
};


#endif
