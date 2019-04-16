#ifndef MODELGEN_H_
#define MODELGEN_H_

#include <vector>
#include <cstdint>
#include <glad/glad.h>
#include "BufferData.h"


BufferData genCube();
BufferData genPlane(int r, int c);
BufferData genPyramid();
BufferData genSphere(int rings, int sectors);

#endif
