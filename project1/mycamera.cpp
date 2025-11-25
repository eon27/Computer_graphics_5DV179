#include <math.h>

#include "LinearAlgebra.hpp"
#include "mycamera.hpp"


/**
 * Constructs a camera with standard 500 far and 60 fov.
 */
MyCamera::MyCamera() {
	pos = Vector4(0, 0, 2, 1);
	refPoint = Vector4(0, 0, 0, 1);
	upVec = Vector4(0, 1, 0, 0);

	far = 500;
	fov = 60;
}

/**
 * Construct a camera with given far and fov.
 */
MyCamera::MyCamera(float farDistance, float fovAngle) {
	pos = Vector4(0, 0, 2, 1);
	refPoint = Vector4(0, 0, 0, 1);
	upVec = Vector4(0, 1, 0, 0);

	far = farDistance;
	fov = fovAngle;
}

MyCamera::~MyCamera() {
}

/**
 * Gets a forward vector, upvector and right vector then constructs the view matrix from them.
 * @return a new view matrix
 */
Matrix MyCamera::getViewMatrix() {
	// Forward vector
	Vector4 lookVec = pos - refPoint;

	Vector3 look3 = Vector3(lookVec.vec[0], lookVec.vec[1], lookVec.vec[2]).normalize();
	Vector3 up3 = Vector3(upVec.vec[0], upVec.vec[1], upVec.vec[2]).normalize();
	Vector3 right3 = look3.cross(up3).normalize();

	Matrix orientation = Matrix(Vector4(right3, 0), Vector4(up3, 0), Vector4(look3, 0), Vector4(0, 0, 0, 1));

	Matrix translation = Matrix(
		1, 0, 0, -pos.vec[0],
		0, 1, 0, -pos.vec[1],
		0, 0, 1, -pos.vec[2],
		0, 0, 0, 1
	);
	Matrix temp = orientation * translation;
	return temp;
}

/**
 * Constructs the projection/perspectiv matrix from the fov and the near and far planes.
 * @return a new projection/perspectiv matrix
 */
Matrix MyCamera::getProjectionMatrix() {
	float halfTanFOV = tan((fov/2)*(M_PI/180));
	Matrix temp = Matrix(
		1/halfTanFOV, 0, 0, 0,
		0, 1/halfTanFOV, 0, 0,
		0, 0, -(far+near)/(far-near), (-2*far*near)/(far-near),
		0, 0, -1, 0
	);
	return temp;
}

/**
 * Moves the camera along its x,y,z axels relative to where it is looking.
 */
void MyCamera::move(float x, float y, float z) {
	pos = pos + Vector4(x,y,z,0);
	refPoint = refPoint + Vector4(x,y,z,0);
}

void MyCamera::updateView(float farDistance, float fovAngle) {
	far = farDistance;
	fov = fovAngle;
}