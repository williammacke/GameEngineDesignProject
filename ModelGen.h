#ifndef MODELGEN_H_
#define MODELGEN_H_

#include <vector>
#include <cstdint>
#include <glad/glad.h>

struct BufferData {
	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<float> normals;
	std::vector<float> textureCoord;
};

BufferData genCube();

#endif
