#include <math.h>
#include <stdio.h>

#include "LinearAlgebra.hpp"

Matrix::Matrix() {
    float temp_mat[16] = {
        1.0,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };
    for (int i = 0; i < 16; i++) {
        mat[i] = temp_mat[i];
    }
}

Matrix::Matrix(float a0, float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, float a11, float a12, float a13, float a14, float a15) {
    mat[0] = a0;
    mat[1] = a1;
    mat[2] = a2;
    mat[3] = a3;

    mat[4] = a4;
    mat[5] = a5;
    mat[6] = a6;
    mat[7] = a7;

    mat[8] = a8;
    mat[9] = a9;
    mat[10] = a10;
    mat[11] = a11;

    mat[12] = a12;
    mat[13] = a13;
    mat[14] = a14;
    mat[15] = a15;
}

Matrix::Matrix(float arr[16]) {
    for (int i = 0; i < 16; i++) {
        mat[i] = arr[i];
    }
}

Matrix::Matrix(float temp_mat[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[(i*4)+j] = temp_mat[i][j];
        }
    }
}

Matrix Matrix::operator*(const Matrix& rhs) {
    float result_matrix[4][4] = {
		{0,0,0,0},
		{0,0,0,0},
        {0,0,0,0},
        {0,0,0,0}
    };
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            for (int i = 0; i < 4; i++) {
                result_matrix[r][c] += this->mat[(r*4)+i] * rhs.mat[(i*4)+c]; 
            }
        }
    }
    Matrix result(result_matrix);
    return result;
}

void Matrix::translate(float dx, float dy, float dz) {
    float translation_array[4][4] = {{1,0,0,dx},{0,1,0,dy},{0,0,1,dz},{0,0,0,1}};
    Matrix translation_matrix(translation_array);
    
    *this = (*this) * translation_matrix;
}

void Matrix::scale(float sx, float sy, float sz) {
    float scale_array[4][4] = {{sx,0,0,0},{0,sy,0,0},{0,0,sz,0},{0,0,0,1}};
    Matrix scale_matrix(scale_array);
    
    *this = (*this) * scale_matrix;
}
        
void Matrix::rotatex(float a) {
    float rotation_array[4][4] = {
        {1,0,0,0},
        {0,cos(a),-sin(a),0},
        {0,sin(a),cos(a),0},
        {0,0,0,1}};
    Matrix rotation_matrix(rotation_array);
    
    *this = (*this) * rotation_matrix;
}
 
void Matrix::rotatey(float a) {
    float rotation_array[4][4] = {
        {cos(a),0,-sin(a),0},
        {0,1,0,0},
        {sin(a),0,cos(a),0},
        {0,0,0,1}};
    Matrix rotation_matrix(rotation_array);

    *this = (*this) * rotation_matrix;
}

void Matrix::rotatez(float a) {
    float rotation_array[4][4] = {
        {cos(a),-sin(a),0,0},
        {sin(a),cos(a),0,0},
        {0,0,1,0},
        {0,0,0,1}};
    Matrix rotation_matrix(rotation_array);

    *this = (*this) * rotation_matrix;
}

Vector4 Matrix::operator*(const Vector4& rhs) {
	float vector[4] = {0,0,0,0};
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			vector[r] += this->mat[(r*4)+c] * rhs.vec[r];
		}
	}
	Vector4 result(vector);
	return result;
}

Matrix Matrix::operator+(const Matrix& rhs) {
	float result_matrix[16];
    for (int i = 0; i < 16; i++) {
        result_matrix[i] = this->mat[i] + rhs.mat[i]; 
    }
    Matrix result(result_matrix);
    return result;
}

Matrix Matrix::operator-(const Matrix& rhs) {
	float result_matrix[16];
    for (int i = 0; i < 16; i++) {
        result_matrix[i] = this->mat[i] - rhs.mat[i]; 
    }
    Matrix result(result_matrix);
    return result;
}

void Matrix::printMatrix() {
    for (int r = 0; r < 4; ++r) {
        printf("%f, %f, %f, %f\n",
            mat[(r*4)],
            mat[(r*4)+1],
            mat[(r*4)+2],
            mat[(r*4)+3]
        );
    }
}

Vector3::Vector3() {
    for (int i = 0; i < 3; i++){
        vec[i] = 0;
    }
}

Vector3::Vector3(float x, float y, float z) {
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
}

Vector3::Vector3(double vector[3]) {
    vec[0] = (float)vector[0];
    vec[1] = (float)vector[1];
    vec[2] = (float)vector[2];
}

float Vector3::operator*(const Vector3& rhs) {
    return (this->vec[0] * rhs.vec[0] + this->vec[1] * rhs.vec[1] + this->vec[2] * rhs.vec[2]);
}

Vector3 Vector3::cross(const Vector3& lhs, const Vector3& rhs) {
	float row1 = lhs.vec[1] * rhs.vec[2] - lhs.vec[2] * rhs.vec[1];
	float row2 = lhs.vec[2] * rhs.vec[0] - lhs.vec[0] * rhs.vec[2];
	float row3 = lhs.vec[0] * rhs.vec[1] - lhs.vec[1] * rhs.vec[0];
	Vector3 result({row1, row2, row3});
	return result;
}

Vector3 Vector3::operator+(const Vector3& rhs) {
	Vector3 result({this->vec[0] + rhs.vec[0], this->vec[1] + rhs.vec[1], this->vec[2] + rhs.vec[2]});
	return result;
}

Vector3 Vector3::operator-(const Vector3& rhs) {
	Vector3 result({this->vec[0] - rhs.vec[0], this->vec[1] - rhs.vec[1], this->vec[2] - rhs.vec[2]});
	return result;
}

Vector4::Vector4() {
    for (int i = 0; i < 4; i++){
        vec[i] = 0;
    }
}

Vector4::Vector4(float x, float y, float z) {
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
    vec[3] = 1;
}

Vector4::Vector4(Vector3 vector) {
    vec[0] = vector.vec[0];
    vec[1] = vector.vec[1];
    vec[2] = vector.vec[2];
    vec[3] = 1;
}

Vector4::Vector4(float x, float y, float z, float w) {
    vec[0] = x;
    vec[1] = y;
    vec[2] = z;
    vec[3] = w;
}

Vector4::Vector4(float vector[4]) {
    vec[0] = vector[0];
    vec[1] = vector[1];
    vec[2] = vector[2];
    vec[3] = vector[3];
}

Vector4 Vector4::operator+(const Vector4& rhs) {
	Vector4 result({this->vec[0] + rhs.vec[0], this->vec[1] + rhs.vec[1], this->vec[2] + rhs.vec[2], this->vec[3] + rhs.vec[3]});
	return result;
}

Vector4 Vector4::operator-(const Vector4& rhs) {
	Vector4 result({this->vec[0] - rhs.vec[0], this->vec[1] - rhs.vec[1], this->vec[2] - rhs.vec[2], this->vec[3] - rhs.vec[3]});
	return result;
}