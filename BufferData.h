#ifndef BUFFERDATA_H_
#define BUFFERDATA_H_
#include "StackArray.h"
#include "linAlg.h"

struct BufferData {
	StackArray<float> vertices;
	StackArray<unsigned int> indices;
	StackArray<float> normals;
	StackArray<float> textureCoord;
	colVector<float, 4> mat;
};

#endif
