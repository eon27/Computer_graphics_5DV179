#ifndef LINEARALGEBRA
#define LINEARALGEBRA

// 3D vector that has implemented vector multiplication, scalar multiplication, addition and subtraction.
// 
class Vector3 {
    public:
        float vec[3];

        Vector3();

        Vector3(float x, float y, float z);

        Vector3(double vector[3]);

        static Vector3 cross(const Vector3& lhs, const Vector3& rhs);
        
        Vector3 cross(const Vector3& rhs);

        float operator*(const Vector3& rhs);

        Vector3 operator*(const float rhs);

        Vector3 operator+(const Vector3& rhs);
        
        Vector3 operator-(const Vector3& rhs);
        
    };

/**
 * 4D vector that has implemented vector addition and subtraction. Has a mutlitude of constructors for ease of use.
 */
class Vector4 {
    public:
        float vec[4];

        Vector4();

        Vector4(float x, float y, float z);

        Vector4(Vector3 vector);

        Vector4(float x, float y, float z, float w);

        Vector4(float vector[4]);

        Vector4 operator+(const Vector4& rhs);

        Vector4 operator-(const Vector4& rhs);
};

/**
 * Matrix implementation with support for 4x4 matrices
 * Has matrix multiplication, addition and subtraction implemented with opperator overloading.
 * Can translate, scale and rotate.
 * Mutliplication works with Vector4 from the same library.
 */
class Matrix {
    public:
        float mat[16];
    
        Matrix();

        Matrix(float a1, float a2, float a3, float a4, float a5, float a6, float a7, float a8, float a9, float a10, float a11, float a12, float a13, float a14, float a15, float a16);

        Matrix(float arr[16]);

        Matrix(float temp_mat[4][4]);

        Matrix(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4);

        Matrix operator*(const Matrix& rhs);

        void translate(float dx, float dy, float dz);

        void scale(float sx, float sy, float sz);
        
        void rotatex(float a);

        void rotatey(float a);

        void rotatez(float a);

        Vector4 operator*(const Vector4& rhs);

        Matrix operator+(const Matrix& rhs);

        Matrix operator-(const Matrix& rhs);

        void printMatrix();
};

#endif