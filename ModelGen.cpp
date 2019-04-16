#include "ModelGen.h"
#include <iostream>
#include <cmath>

BufferData genCube() {
	colVector<float, 4> mat{1.0f,1.0f,1.0f,1.0f};
	StackArray<float> vertices = {
		-1.0f,-1.0f,1.0f, //front
		1.0f,-1.0f,1.0f,
		1.0f,1.0f,1.0f,
		-1.0f,1.0f,1.0f,
		-1.0f,-1.0f,-1.0f, //left
		-1.0f,-1.0f,1.0f,
		-1.0f,1.0f,1.0f,
		-1.0f,1.0f,-1.0f,
		1.0f,-1.0f,1.0f, //right
		1.0f,-1.0f,-1.0f,
		1.0f,1.0f,-1.0f,
		1.0f,1.0f,1.0f,
		1.0f,-1.0f,-1.0f, //back
		-1.0f,-1.0f,-1.0f,
		-1.0f,1.0f,-1.0f,
		1.0f,1.0f,-1.0f,
		-1.0f,-1.0f,1.0f, //bottom
		1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,1.0f,1.0f, //top
		1.0f,1.0f,1.0f,
		1.0f,1.0f,-1.0f,
		-1.0f,1.0f,-1.0f
	};
	StackArray<unsigned int> indices = {
		0,1,3, 3,1,2, //front
		4,5,7, 7,5,6, //left
		8,9,11, 11,9,10, //right
		12,13,15, 15,13,14, //back
		16,17,19, 19,17,18, //bottom
		20,21,23, 23,21,22 //top
	};
	StackArray<float> normals = {
		0.0f,0.0f,1.0f, //front
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,

		-1.0f,0.0f,0.0f, //left
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,

		1.0f,0.0f,0.0f, //right
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,

		0.0f,0.0f,-1.0f, //back
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,

		0.0f,-1.0f,0.0f, //bottom
		0.0f,-1.0f,0.0f, 
		0.0f,-1.0f,0.0f, 
		0.0f,-1.0f,0.0f, 

		0.0f,1.0f,0.0f, //top
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f
	};
	StackArray<float> textureCoord = {
		0.0f,0.0f, //front
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,
		
		0.0f,0.0f,//left
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,

		0.0f,0.0f, //right
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,

		0.0f,0.0f, //back
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,

		0.0f,0.0f, //bottom
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,

		0.0f,0.0f, //top
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f
	};

	return BufferData{vertices,indices,normals,textureCoord, mat};
}

BufferData genPlane(int r, int c) {
	colVector<float, 4> mat{1.0f,1.0f,1.0f,1.0f};
	StackArray<float> vertices(r*c*3);
	StackArray<unsigned int> indices((r-1)*(c-1)*6);
	StackArray<float> normals(r*c*3);
	StackArray<float> textureCoord(r*c*2);
	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			vertices[(i*c+j)*3] = -1.0f+2*((float) j/(c-1));
			vertices[(i*c+j)*3+1] = -1.0f;
			vertices[(i*c+j)*3+2] = -1.0f+2*((float) i/(r-1));


			normals[(i*c+j)*3] = 0.0f;
			normals[(i*c+j)*3+1] = 1.0f;
			normals[(i*c+j)*3+2] = 0.0f;

			textureCoord[(i*c+j)*2] = (float) j/(c-1);
			textureCoord[(i*c+j)*2+1] = (float) i/(r-1);
		}
	}


	for (int i = 0; i < r-1; i++) {
		for (int j = 0; j < c-1; j++) {
			indices[(i*(c-1)+j)*6] = i*c+j;
			indices[(i*(c-1)+j)*6+1] = (i+1)*c+j;
			indices[(i*(c-1)+j)*6+2] = (i)*c+j+1;
			indices[(i*(c-1)+j)*6+3] = (i)*c+j+1;
			indices[(i*(c-1)+j)*6+4] = (i+1)*c+j;
			indices[(i*(c-1)+j)*6+5] = (i+1)*c+j+1;
		}
	}
	return BufferData{vertices, indices, normals, textureCoord, mat};
}

BufferData genPyramid() {
	colVector<float, 4> mat{1.0f,1.0f,1.0f,1.0f};
	StackArray<float> vertices = {
		-1.0f,-1.0f,1.0f, //bottom
		1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,

		-1.0f,-1.0f,1.0f, //front
		1.0f,-1.0f,1.0f,
		0.0f,1.0f,0.0f,

		-1.0f,-1.0f,-1.0f, //left
		-1.0f,-1.0f,1.0f,
		0.0f,1.0f,0.0f,

		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		0.0f,1.0f,0.0f,

		1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,
		0.0f,1.0f,0.0f
	};

	StackArray<unsigned int> indices = {
		0,1,3, 3,1,2, 

		4,5,6,
		7,8,9,
		10,11,12,
		13,14,15
	};

	StackArray<float> normals {
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,

		0.0f,.707f,.707f,
		0.0f,.707f,.707f,
		0.0f,.707f,.707f,

		-.707f,.707f,0.0f,
		-.707f,.707f,0.0f,
		-.707f,.707f,0.0f,

		0.0f,.707f,-.707f,
		0.0f,.707f,-.707f,
		0.0f,.707f,-.707f,

		.707f,.707f,0.0f,
		.707f,.707f,0.0f,
		.707f,.707f,0.0f
	};

	StackArray<float> textureCoord = {
		0.0f,0.0f,
		1.0f,0.0f,
		1.0f,1.0f,
		0.0f,1.0f,

		0.0f,0.0f,
		1.0f,0.0f,
		0.5f,1.0f,

		0.0f,0.0f,
		1.0f,0.0f,
		0.5f,1.0f,
	
		0.0f,0.0f,
		1.0f,0.0f,
		0.5f,1.0f,

		0.0f,0.0f,
		1.0f,0.0f,
		0.5f,1.0f
	};

	return BufferData{vertices, indices, normals, textureCoord, mat};
}


BufferData genSphere(int rings, int sectors) {
	colVector<float, 4> mat{1.0f,1.0f,1.0f,1.0f};
	float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
	StackArray<float> vertices(rings*sectors*3);
	StackArray<unsigned int> indices(rings*sectors*6);
	StackArray<float> normals(rings*sectors*3);
	StackArray<float> textureCoord(rings*sectors*2);

	for (int r = 0; r < rings; r++) {
		for (int s = 0; s < sectors; s++) {
			float const y = sin( -M_PI_2 + M_PI * r * R );
                	float const x = cos(2*M_PI * s * S) * sin( M_PI * r * R );
                	float const z = sin(2*M_PI * s * S) * sin( M_PI * r * R );

			textureCoord[(r*sectors+s)*2] = s*S;
			textureCoord[(r*sectors+s)*2+1] = r*R;

			vertices[(r*sectors+s)*3] = x;
			vertices[(r*sectors+s)*3+1] = y;
			vertices[(r*sectors+s)*3+2] = z;

			normals[(r*sectors+s)*3] = x;
			normals[(r*sectors+s)*3+1] = y;
			normals[(r*sectors+s)*3+2] = z;
		}
	}


	for (int r = 0;  r < rings; r++) {
		for (int s = 0; s < sectors; s++) {
			indices[(r*sectors+s)*6] = r*sectors+s;
			indices[(r*sectors+s)*6+1] = (r+1)*sectors+s;
			indices[(r*sectors+s)*6+2] = r*sectors+s+1;
			indices[(r*sectors+s)*6+3] = r*sectors+s+1;
			indices[(r*sectors+s)*6+4] = (r+1)*sectors+s;
			indices[(r*sectors+s)*6+5] = (r+1)*sectors+s+1;
		}
	}

	return BufferData{vertices, indices, normals, textureCoord, mat};
}
