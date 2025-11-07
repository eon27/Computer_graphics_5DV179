#ifndef LINEARALGEBRA
#define LINEARALGEBRA

// 3D vector that has logic for vector multiplication, scalar multiplication and addition
// the standard vector is (0,0,0).
class Vector3 {
    public:
        float vec[3];

        Vector3();

        Vector3(float x, float y, float z);

        static Vector3 cross(const Vector3& lhs, const Vector3& rhs);

        float operator*(const Vector3& rhs);

        Vector3 operator+(const Vector3& rhs);
        
        Vector3 operator-(const Vector3& rhs);
        
    };

/**
 * 4D vector that has logic for 
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

class Matrix {
    private:
        float mat[4][4];
    public:

        Matrix();

        Matrix(float r1[4], float r2[4], float r3[4], float r4[4]);

        Matrix(float temp_mat[4][4]);

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