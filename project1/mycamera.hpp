#ifndef MY_CAMERA
#define MY_CAMERA
#include "LinearAlgebra.hpp"

class MyCamera
{
	private:
		Vector4 pos;
		Vector4 refPoint;
		Vector4 upVec;

		float near = 1;
		float far;
		float fov;
		float top;
		float obliqueScale = 0.0f;
	    float obliqueAngleRad = M_PI/4.0f;
		bool parallellPerspective = true;
	public:
		MyCamera();
		MyCamera(float farDistance, float povAngle);
		~MyCamera();
		Matrix getViewMatrix();
		Matrix getProjectionMatrix();
		void move(float x, float y, float z);
		void updateView(float fovAngle, float farDistance, float planeTop, float oScale, float oAngleRad, int proj_current_idx);
};
#endif