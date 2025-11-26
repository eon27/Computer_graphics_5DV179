/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include "openglwindow.h"
#include "mycamera.hpp"

typedef float Mat4x4[16];

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    virtual void display() override;
    virtual void passAction(int action) override;
    virtual void rotateCamera(float deltaX, float deltaY);
    virtual void handleNewObject() override;

private:
    GLuint program;

    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;
    GLuint locModel;
    GLuint locView;
    GLuint locProjection;

    // OpenGL attribute locations
    GLuint locVertices;

    // Geometry data
    std::vector<Vector3> vertices;
    std::vector<unsigned int> indices;

    void debugShader(void) const;

    std::vector<Vector3> centerAndScaleObject(std::vector<Vector3> vertexList);
    void loadGeometry(std::vector<Vector3> vertexList, std::vector<int> indexList);
    void loadObjectGeometry();

    void updateView();

    MyCamera cam;

    Matrix matModel = Matrix();
    Matrix matView = Matrix(
        1,0,0,0,
        0,1,0,0,
        0,0,1,-2,
        0,0,0,1
    );
    Matrix matProjection = Matrix(
        1.28,0,0,0,
        0,2.75,0,0,
        0,0,-1.67,-53.33,
        0,0,-1,0
    );
};
