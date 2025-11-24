#include <math.h>

#include "LinearAlgebra.hpp"
#include "mycamera.hpp"

MyCamera::MyCamera() {
	pos = Vector4(0, 0, 2, 1);
	refPoint = Vector4(0, 0, 0, 1);
	upVec = Vector4(0, 1, 0, 0);

	far = 500;
	fov = 60;
}

MyCamera::MyCamera(float farDistance, float fovAngle) {
	pos = Vector4(0, 0, 2, 1);
	refPoint = Vector4(0, 0, 0, 1);
	upVec = Vector4(0, 1, 0, 0);

	far = farDistance;
	fov = fovAngle;
}

MyCamera::~MyCamera() {
}

Matrix MyCamera::getViewMatrix() {
	Vector4 lookVec = pos - refPoint;
	Vector3 look3 = Vector3(lookVec.vec[0], lookVec.vec[1], lookVec.vec[2]).normalize();
	Vector3 up3 = Vector3(upVec.vec[0], upVec.vec[1], upVec.vec[2]).normalize();
	Vector3 right3 = look3.cross(up3).normalize();

	Matrix orientation = Matrix(Vector4(right3), upVec, lookVec, Vector4(0, 0, 0, 1));
	Matrix translation = Matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-look3.vec[0], -look3.vec[0], -look3.vec[0], 1
	);
	Matrix temp = orientation * translation;
	return temp;
}

Matrix MyCamera::getProjectionMatrix() {
	float halfTanFOV = tan((fov/2)*(M_PI/180));
	Matrix temp = Matrix(
		1/halfTanFOV, 0, 0, 0,
		0, 1/halfTanFOV, 0, 0,
		0, 0, (far+near)/(far-near), (-2*far*near)/(far-near),
		0, 0, 1, 0
	);
	return temp;
}

void MyCamera::move(float x, float y, float z) {
	pos = pos - Vector3(x,y,z);
}