#include "Camera.h"

Camera::Camera() {
	colVector<float, 3> axis;
	axis << 0,1,0;
	rotation = genQuaternion(axis, 0);
}

Camera::~Camera() {
}

Matrix<float, 4, 4> Camera::getCameraMat() {
	colVector<float, 3> X, Y, Z;
	X << 1,0,0;
	Y << 0,1,0;
	Z << 0,0,1;
	X = rotate(X, rotation);
	Y = rotate(Y, rotation);
	Z = rotate(Z, rotation);
	Matrix<float, 4, 4> mat;
	mat << X(0),X(1),X(2),-1*X*cameraPos,
		Y(0),Y(1),Y(2),-1*Y*cameraPos,
		Z(0),Z(1),Z(2),-1*Z*cameraPos,
		0,0,0,1;
	return mat;

}
