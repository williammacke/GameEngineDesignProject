#include "models.h"

GLuint currentVAO = -1;
GLuint currentProgram  = -1;

Model::Model(GLuint VAO, GLuint shaderProgram, void *offset, size_t size, std::string name):
	VAO(VAO), shaderProgram(shaderProgram), offset(offset), size(size), name(name)
{
	
}

Model::Model(const Model& m) : VAO(m.VAO), shaderProgram(m.shaderProgram),
offset(m.offset), size(m.size), name(m.name)
{
}

Model::~Model() { }


Entity::Entity(Model m) : m(m) 
{
	translation = Identity<float, 4>();
	rotation = Identity<float, 4>();
	scaling = Identity<float, 4>();

}

Entity::~Entity() {
}

Matrix<float, 4, 4> Entity::getModel() {
	return scaling*rotation*translation;
}
