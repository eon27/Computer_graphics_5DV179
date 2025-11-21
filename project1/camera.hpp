#include "LinearAlgebra.hpp"

class Camera
{
private:
	Vector4 pos;
	Vector4 refPoint;
	Vector4 upVec;
public:
	Camera();
	~Camera();
	Matrix getMatrix();
};