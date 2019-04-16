#include "models.h"



Mesh::Mesh(void *data) : data(data),loaded(false) { }

Mesh::~Mesh() { }

void Mesh::setLoaded(GLuint VAO, GLuint shader, void *offset, size_t baseV, size_t size) {
	this->VAO = VAO;
	this->shader = shader;
	this->offset = offset;
	this->baseV = baseV;
	this->size = size;
	this->loaded = true;
}

void Mesh::setUnloaded() {
	this->loaded = false;
}



Model::Model(Mesh *meshes, size_t numMeshes) : numMeshes(numMeshes), meshes(meshes) 
{
}

Model::Model(const Model& m) : numMeshes(m.numMeshes), meshes(m.meshes)
{
}

Model::~Model() { }


Entity::Entity(Model *m) : m(m), numChildren(0), children(nullptr), parent(nullptr)
{
	translation = Identity<float, 4>();
	colVector<float, 3> axis;
	axis << 0,1,0;
	rotation = genQuaternion(axis, 0);
	scaling = Identity<float, 4>();

}

Entity::Entity(const Entity &e) : m(e.m), numChildren(e.numChildren), 
		children(e.children), parent(e.parent) {
	translation = e.translation;
	rotation = e.rotation;
	scaling = e.scaling;
}

Entity::~Entity() {
}

Matrix<float, 4, 4> Entity::getModel() {
	return translation*quatToMat(rotation)*scaling;
}

void Entity::scaleBy(float x, float y, float z) {
	scaling = scale(x,y,z)*scaling;
}

void Entity::rotateBy(quaternion<float> nrotation) {
	rotation = quatMult(nrotation, rotation);
}

void Entity::translateBy(float x, float y, float z) {
	translation = translate(x,y,z)*translation;
}
