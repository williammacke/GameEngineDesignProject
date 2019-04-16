#ifndef CAMERA_H_
#define CAMERA_H_
#include "linAlg.h"

class Camera {
public:
	Camera();
	~Camera();
	Matrix<float, 4, 4> getCameraMat();
	void move(colVector<float, 3> pos);
	void horizTurn(float amount);
	void vertTurn(float amount);
private:
	colVector<float, 3> cameraPos;
	quaternion<float> horizRotation;
	quaternion<float> vertRotation;
};

#endif
