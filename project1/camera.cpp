#include "LinearAlgebra.hpp"
#include "camera.hpp"

Camera::Camera() {
	pos = Vector4(0, 0, 2, 1);
	refPoint = Vector4(0, 0, 0, 1);
	upVec = Vector4(0, 1, 0, 0);
}

Camera::~Camera() {
}

Matrix Camera::getMatrix() {
	Matrix temp = Matrix(pos, refPoint, upVec, Vector4(0, 0, 0, 1));
	return temp;
}
