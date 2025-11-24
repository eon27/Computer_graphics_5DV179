#ifndef MY_CAMERA
#define MY_CAMERA
#include "LinearAlgebra.hpp"

class MyCamera
{
	private:
		Vector4 pos;
		Vector4 refPoint;
		Vector4 upVec;

		float far;
		float fov;
		float near = 1;
	public:
		MyCamera();
		MyCamera(float farDistance, float povAngle);
		~MyCamera();
		Matrix getViewMatrix();
		Matrix getProjectionMatrix();
		void move(float x, float y, float z);
};
#endif