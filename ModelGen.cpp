#include "ModelGen.h"
#include <iostream>

BufferData genCube() {
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
	std::vector<unsigned int> indices = {2,1,0, 0,3,2,//top
						6,5,4, 4,7,6,//bottom
						10,9,8, 8,11,10,//left
						14,13,12, 12,15,14, //right
						18,17,16, 16,19,18, //front
						22,21,20, 20,23,22 //back

	};
	std::vector<float> normals = {0.0f, 1.0f, 0.0f, //top
					0.0f,1.0f,0.0f,
					0.0f,1.0f,0.0f,
					0.0f,1.0f,0.0f,
					0.0f,-1.0f,0.0f, //bottom
					0.0f,-1.0f,0.0f,
					0.0f,-1.0f,0.0f,
					0.0f,-1.0f,0.0f,
					-1.0f,0.0f,0.0f, //left
					-1.0f,0.0f,0.0f,
					-1.0f,0.0f,0.0f,
					-1.0f,0.0f,0.0f,
					1.0f,0.0f,0.0f, //right
					1.0f,0.0f,0.0f,
					1.0f,0.0f,0.0f,
					1.0f,0.0f,0.0f,
					0.0f,0.0f,-1.0f, //font
					0.0f,0.0f,-1.0f,
					0.0f,0.0f,-1.0f,
					0.0f,0.0f,-1.0f,
					0.0f,0.0f,1.0f, //back
					0.0f,0.0f,1.0f,
					0.0f,0.0f,1.0f,
					0.0f,0.0f,1.0f


	
	};
	std::vector<float> textureCoord = {0.0f,0.0f, //top
						1.0f,0.0f,
						1.0f,1.0f,
						0.0f,1.0f,
						0.0f,0.0f, //bottom
						1.0f,0.0f,
						1.0f,1.0f,
						0.0f,1.0f,
						0.0f,0.0f, //left
						1.0f,0.0f,
						1.0f,1.0f,
						0.0f,1.0f,
						0.0f,0.0f, //right
						1.0f,0.0f,
						1.0f,1.0f,
						0.0f,1.0f,
						0.0f,0.0f, //front
						1.0f,0.0f,
						1.0f,1.0f,
						0.0f,1.0f,
						0.0f,0.0f, //back
						1.0f,0.0f,
						1.0f,1.0f,
						0.0f,1.0f
	};

	return BufferData{vertices,indices,normals,textureCoord};
}

BufferData genPlane(int r, int c) {
	std::vector<float> vertices(r*c*3);
	std::vector<unsigned int> indices((r-1)*(c-1)*6);
	std::vector<float> normals(r*c*3);
	std::vector<float> textureCoord(r*c*2);
	for (int i = 0; i < c; i++) {
		for (int j = 0; j < r; j++) {
			vertices[(i*c+j)*3] = -1.0f+2*((float) i/(c-1));
			vertices[(i*c+j)*3+1] = 0.0f;
			vertices[(i*c+j)*3+2] = -1.0f+2*((float) j/(r-1));


			normals[i*c+j] = 0.0f;
			normals[i*c+j+1] = 1.0f;
			normals[i*c+j+2] = 0.0f;

			textureCoord[(i*c+j)*2] = (float) i/(c-1);
			textureCoord[(i*c+j)*2+1] = (float) j/(r-1);
		}
	}
	for (auto& val:vertices) {
		std::cout << val << std::endl;
	}

	for (int i = 0; i < c-1; i++) {
		for (int j = 0; j < r-1; j++) {
			indices[(i*(c-1)+j)*6] = i*c+j;
			indices[(i*(c-1)+j)*6+1] = (i+1)*c+j;
			indices[(i*(c-1)+j)*6+2] = (i)*c+j+1;
			indices[(i*(c-1)+j)*6+3] = (i)*c+j+1;
			indices[(i*(c-1)+j)*6+4] = (i+1)*c+j;
			indices[(i*(c-1)+j)*6+5] = (i+1)*c+j+1;
		}
	}
	for (auto& val:indices) {
		std::cout << val << std::endl;
	}
	return BufferData{vertices, indices, normals, textureCoord};
}


BufferData genSphere(int prec) {
	std::vector<float> vertices((prec+1)*(prec+1)*6*3);
	std::vector<unsigned int> indices(prec*prec*6*6);
	std::vector<float> normals((prec+1)*(prec+1)*6*3);
	std::vector<float> textureCoord((prec+1)*(prec+1)*6*2);
	auto offset = (prec+1)*(prec+1);
	auto offset2 = prec*prec*6;
	for (int i = 0; i < prec+1; i++) { //top
		for (int j = 0; j < prec+1; j++) {
			float horr = ((float) i)/(prec+1)/2-1;
			float depth = ((float) j)/(prec+1)/2-1;
			vertices[0*offset+i*prec+1+j] = horr;
			vertices[0*offset+i*prec+1+j+1] = 1.0f;
			vertices[0*offset+i*prec+1+j+2] = depth;

			normals[0*offset+i*prec+1+j] = 1.0f;
			normals[0*offset+i*prec+1+j+1] = 1.0f;
			normals[0*offset+i*prec+1+j+2] = 1.0f;

			textureCoord[0*offset+i*prec+1+j] = (horr+1)/2;
			textureCoord[0*offset+i*prec+1+j+1] = (depth+1)/2;
		}
	}
	for (int i = 0; i < prec; i++) {
		for (int j = 0; j < prec; j++) {
			indices[i*prec+j] = i*(prec+1)+j;
			indices[i*prec+j+1] = (i+1)*(prec+1)+j;
			indices[i*prec+j+2] = i*(prec+1)+j+1;
			indices[i*prec+j+3] = i*(prec+1)+j+1;
			indices[i*prec+j+4] = (i+1)*(prec+1)+j;
			indices[i*prec+j+5] = (i+1)*(prec+1)+j+1;
		}
	}
	return BufferData{vertices, indices, normals, textureCoord};
}
