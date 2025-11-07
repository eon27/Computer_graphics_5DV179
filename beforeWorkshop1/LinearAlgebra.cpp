#include <math.h>
#include <stdio.h>

#include "LinearAlgebra.hpp"

Matrix::Matrix() {
    float temp_mat[4][4] = {
        {1.0,0,0,0},
        {0,1,0,0},
        {0,0,1,0},
        {0,0,0,1}
    };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = temp_mat[i][j];
        }
    }
}

Matrix::Matrix(float r1[4], float r2[4], float r3[4], float r4[4]) {
    for (int i = 0; i < 4; i++) {
        mat[0][i] = r1[i];
    }
    for (int i = 0; i < 4; i++) {
        mat[1][i] = r1[i];
    }
    for (int i = 0; i < 4; i++) {
        mat[2][i] = r1[i];
    }
    for (int i = 0; i < 4; i++) {
        mat[3][i] = r1[i];
    }
}

Matrix::Matrix(float temp_mat[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mat[i][j] = temp_mat[i][j];
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
                result_matrix[r][c] += this->mat[r][i] * rhs.mat[i][c]; 
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
			vector[r] += this->mat[r][c] * rhs.vec[r];
		}
	}
	Vector4 result(vector);
	return result;
}

Matrix Matrix::operator+(const Matrix& rhs) {
	float result_matrix[4][4];
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
			result_matrix[r][c] = this->mat[r][c] + rhs.mat[r][c]; 
        }
    }
    Matrix result(result_matrix);
    return result;
}

Matrix Matrix::operator-(const Matrix& rhs) {
	float result_matrix[4][4];
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
			result_matrix[r][c] = this->mat[r][c] - rhs.mat[r][c]; 
        }
    }
    Matrix result(result_matrix);
    return result;
}

void Matrix::printMatrix() {
    for (int r = 0; r < 4; ++r) {
        printf("%f, %f, %f, %f\n",
            mat[r][0],
            mat[r][1],
            mat[r][2],
            mat[r][3]
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