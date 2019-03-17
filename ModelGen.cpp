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
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			vertices[i*c+j] = -1.0f+2*((float) i/(r-1));
			vertices[i*c+j+1] = 0.0f;
			vertices[i*c+j+2] = -1.0f+2*((float) j/(c-1));


			normals[i*c+j] = 0.0f;
			normals[i*c+j+1] = 1.0f;
			normals[i*c+j+2] = 0.0f;

			textureCoord[i*c+j] = (float) i/(r-1);
			textureCoord[i*c+j+1] = (float) j/(c-1);
		}
	}

	for (int i = 0; i < r-1; i++) {
		for (int j = 0; j < c-1; j++) {
			indices[i*(c-1)+j] = i*c+j;
			indices[i*(c-1)+j+1] = (i+1)*c+j;
			indices[i*(c-1)+j+2] = i*c+j+1;
			indices[i*(c-1)+j+3] = i*c+j+1;
			indices[i*(c-1)+j+4] = (i+1)*c+j;
			indices[i*(c-1)+j+5] = (i+1)*c+j+1;
		}
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
