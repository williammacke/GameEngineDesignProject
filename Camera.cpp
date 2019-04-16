#include "Camera.h"
#include <iostream>

Camera::Camera() {
	colVector<float, 3> axis;
	axis << 0,1,0;
	horizRotation = genQuaternion(axis, 0);
	axis << 1,0,0;
	vertRotation = genQuaternion(axis, 0);
}

Camera::~Camera() {
}

Matrix<float, 4, 4> Camera::getCameraMat() {
	/*
	colVector<float, 3> X, Y, Z;
	X << 1,0,0;
	Y << 0,1,0;
	Z << 0,0,1;
	auto rotation = quatMult(horizRotation, vertRotation);
	X = rotate(X, rotation);
	Y = rotate(Y, rotation);
	Z = rotate(Z, rotation);
	Matrix<float, 4, 4> mat;
	mat << X(0),X(1),X(2),-1*X*cameraPos,
		Y(0),Y(1),Y(2),-1*Y*cameraPos,
		Z(0),Z(1),Z(2),-1*Z*cameraPos,
		0,0,0,1;
	return mat;
	*/
	colVector<float, 3> dir;
	dir << 0,0,1;
	auto rotation = quatMult(horizRotation, vertRotation);
	dir = rotate(dir, rotation);
	colVector<float, 3> up;
	up << 0,1,0;
	colVector<float, 3> target = cameraPos+dir;
	return lookAt(cameraPos, target, up);

}

void Camera::move(colVector<float, 3> pos) {
	cameraPos += rotate(pos, horizRotation);
}

void Camera::horizTurn(float amount) {
	colVector<float, 3> axis;
	axis << 0,1,0;
	auto quat = genQuaternion(axis, amount);
	horizRotation = quatMult(quat, horizRotation);
}

void Camera::vertTurn(float amount) {
	colVector<float, 3> axis;
	axis << 1,0,0;
	auto quat = genQuaternion(axis, amount);
	vertRotation = quatMult(quat, vertRotation);
}
