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

    MyCamera cam;

    Matrix matModel = Matrix();
    Matrix matView = Matrix();
    Matrix matProjection = Matrix();
};
