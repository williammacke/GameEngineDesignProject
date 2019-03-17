#ifndef CAMERA_H_
#define CAMERA_H_
#include "linAlg.h"

class Camera {
public:
	Camera();
	~Camera();
	Matrix<float, 4, 4> getCameraMat();
private:
	colVector<float, 3> cameraPos;
	quaternion<float> rotation;
};

#endif
