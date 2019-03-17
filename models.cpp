#include "models.h"

GLuint currentVAO = -1;
GLuint currentProgram  = -1;

Model::Model(GLuint VAO, GLuint shaderProgram, void *offset, 
		size_t size, std::string name, void *data):
	VAO(VAO), shaderProgram(shaderProgram), offset(offset), 
	size(size), name(name), data(data)
{
	
}

Model::Model(const Model& m) : VAO(m.VAO), shaderProgram(m.shaderProgram),
offset(m.offset), size(m.size), name(m.name), data(m.data)
{
}

Model::~Model() { }


Entity::Entity(Model m) : m(m) 
{
	translation = Identity<float, 4>();
	colVector<float, 3> axis;
	axis << 0,1,0;
	rotation = genQuaternion(axis, 0);
	scaling = Identity<float, 4>();

}

Entity::~Entity() {
}

Matrix<float, 4, 4> Entity::getModel() {
	return scaling*quatToMat(rotation)*translation;
}
