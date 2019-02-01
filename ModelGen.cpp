#include "ModelGen.h"

BufferData genBube() {
	std::vector<float> vertices = { -1.0f,1.0f,-1.0f,//top
					1.0f,1.0f,-1.0f,
					1.0f,1.0f,1.0f,
					-1.0f,1.0f,1.0f,
					-1.0f,-1.0f,-1.0f, //bottom
					1.0f, -1.0f, -1.0f,
					1.0f,-1.0f,1.0f,
					-1.0f,-1.0f,1.0f,
					-1.0f,1.0f,-1.0f, //left
					-1.0f,1.0f,1.0f,
					-1.0f,-1.0f,1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f,1.0f,1.0f, //right
					-1.0f,-1.0f,1.0f,
					-1.0f,-1.0f,-1.0f,
					-1.0f,1.0f,-1.0f,
					-1.0f,1.0f,-1.0f, //front
					-1.0f,-1.0f,-1.0f,
					1.0f,-1.0f,-1.0f,
					1.0f,1.0f,-1.0f,
					-1.0f,1.0f,1.0f, //back
					1.0f,1.0f,1.0f,
					1.0f,-1.0f,1.0f,
					-1.0f,-1.0f,1.0f

	};
	std::vector<unsigned int> indices = {0,1,2, 2,3,0,//top
						4,5,6, 6,7,4,//bottom
						8,9,10, 10,11,8,//left
						12,13,14, 14,15,12, //right
						16,17,18, 18,19,16, //front
						20,21,22, 22,23,20 //back

	};
	std::vector<float> normals = {0.0f, 1.0f, 0.0f, //top
					0.0f,1.0f,0.0f,
					0.0f,1.0f,0.0f,
					0.0f,1.0f,0.0f,
					0.0f,-1.0f,0.0f, //bottom


	
	};
	std::vector<float> textureCoord = {};
	return BufferData{vertices,indices,normals,textureCoord};
}
