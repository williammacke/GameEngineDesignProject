#include "Camera.h"
#include <iostream>

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
	/*
	std::cout << X(0) << "\t" << X(1) << "\t" << X(2) << std::endl;
	std::cout << Y(0) << "\t" << Y(1) << "\t" << Y(2) << std::endl;
	std::cout << Z(0) << "\t" << Z(1) << "\t" << Z(2) << std::endl;
	*/
	Matrix<float, 4, 4> mat;
	mat << X(0),X(1),X(2),-1*X*cameraPos,
		Y(0),Y(1),Y(2),-1*Y*cameraPos,
		Z(0),Z(1),Z(2),-1*Z*cameraPos,
		0,0,0,1;
	return mat;
	/*
	colVector<float, 3> dir;
	dir << 0,0,1;
	colVector<float, 3> up;
	up << 0,1,0;
	colVector<float, 3> target = cameraPos+dir;
	return lookAt(cameraPos, target, up);
	*/

}

void Camera::move(colVector<float, 3> pos) {
	cameraPos += rotate(pos, rotation);
}

void Camera::turn(float amount) {
	colVector<float, 3> axis;
	axis << 0,1,0;
	auto quat = genQuaternion(axis, amount);
	rotation = quatMult(quat, rotation);
}
