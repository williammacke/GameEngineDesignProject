#ifndef MODELS_H_
#define MODELS_H_
#include <vector>
#include <string>
#include <glad/glad.h>
#include "linAlg.h"
#include <cassert>


struct Mesh {
	Mesh(void* data);
	~Mesh();
	void setLoaded(GLuint VAO, GLuint shader, void *offset, size_t baseV, size_t size);
	void setUnloaded();
	bool loaded;
	GLuint VAO;
	GLuint shader;
	void *offset;
	size_t baseV;
	size_t size;
	void *data;
	colVector<float, 4> mat;
};



struct Model {
	Model(Mesh *meshes, size_t numMeshes);
	Model(const Model &m);
	~Model();
	size_t numMeshes;
	Mesh *meshes;
};

class Entity {
public:
	Entity(Model *m);
	Entity(const Entity &e);
	~Entity();
	Model *m;
	Matrix<float, 4, 4> getModel();
	void scaleBy(float x, float y, float z);
	void rotateBy(quaternion<float> nrotation);
	void translateBy(float x, float y, float z);
	size_t numChildren;
	Entity *children;
	Entity *parent;
	GLuint tex;
private:
	Matrix<float, 4, 4> translation;
	quaternion<float> rotation;
	Matrix<float, 4, 4> scaling;
};


#endif
