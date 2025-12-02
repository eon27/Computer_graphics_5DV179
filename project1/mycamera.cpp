#include <math.h>
#include <stdio.h>

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
	Vector4 lookVec = refPoint - pos;
	Vector3 look3 = Vector3(lookVec.vec[0], lookVec.vec[1], lookVec.vec[2]).normalize();

	Vector3 up3 = Vector3(upVec.vec[0], upVec.vec[1], upVec.vec[2]).normalize();
	Vector3 right3 = look3.cross(up3).normalize();
	up3 = right3.cross(look3).normalize();

	Matrix orientation = Matrix(Vector4(right3, 0), Vector4(up3,0), Vector4(-look3.vec[0], -look3.vec[1], -look3.vec[2], 0), Vector4(0, 0, 0, 1));

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
	if (parallellPerspective) {
		Matrix temp = Matrix(
			1/aspectRatio,0,obliqueScale*cos(obliqueAngleRad),0,
			0,1,obliqueScale*sin(obliqueAngleRad),0,
			0,0,1,0,
			0,0,0,1
		);
		temp.translate(0,0,(-2*far*near)/(far-near));
		temp.scale(1/top,1/top,-(far+near)/(far-near));
		return temp;

	} else {
		float halfTanFOV = tan((fov/2)*(M_PI/180));
		Matrix temp = Matrix(
			(1/halfTanFOV)/aspectRatio, 0, 0, 0,
			0, 1/halfTanFOV, 0, 0,
			0, 0, -(far+near)/(far-near), (-2*far*near)/(far-near),
			0, 0, -1, 0
		);
		return temp;		
	}
}

/**
 * Moves the camera along its x,y,z axels relative to where it is looking.
 */
void MyCamera::move(float x, float y, float z) {
	// Forward vector
	Vector4 lookVec = refPoint - pos;

	Vector3 look3 = Vector3(lookVec.vec[0], lookVec.vec[1], lookVec.vec[2]).normalize();
	Vector3 up3 = Vector3(upVec.vec[0], upVec.vec[1], upVec.vec[2]).normalize();
	Vector3 right3 = look3.cross(up3).normalize();
	 
	Vector4 movement = Vector4(right3,0)*x + upVec*y + Vector4(look3,0)*z;
	pos = pos + movement;
	refPoint = refPoint + movement;
}

void MyCamera::updateView(float fovAngle, float farDistance, float planeTop, float oScale, float oAngleRad, int proj_current_idx, float ratio) {
	far = farDistance;
	fov = fovAngle;
	top = planeTop;
	obliqueScale = oScale;
	obliqueAngleRad = oAngleRad;
	parallellPerspective = proj_current_idx;
	aspectRatio = ratio;
}

void MyCamera::rotate(float deltaX, float deltaY) {
	
	Matrix rotationMatrix = Matrix();

	rotationMatrix.translate(pos.vec[0], pos.vec[1], pos.vec[2]);
	rotationMatrix.rotatey(deltaX);

	// Forward vector
	Vector4 lookVec = refPoint - pos;
	Vector3 look3 = Vector3(lookVec.vec[0], lookVec.vec[1], lookVec.vec[2]).normalize();
	Vector3 up3 = Vector3(upVec.vec[0], upVec.vec[1], upVec.vec[2]).normalize();
	Vector3 right3 = look3.cross(up3).normalize();

	rotationMatrix.rotateAroundAxis(right3, -deltaY);
	rotationMatrix.translate(-pos.vec[0], -pos.vec[1], -pos.vec[2]);

	refPoint = rotationMatrix * refPoint;
}