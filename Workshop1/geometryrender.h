/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include "openglwindow.h"

typedef float Mat4x4[16];

class GeometryRender : public OpenGLWindow
{
public:
    template<typename... ARGS>
    GeometryRender(ARGS&&... args) : OpenGLWindow{ std::forward<ARGS>(args)... }
    {}

    void initialize();
    virtual void display() override;

private:
    GLuint program;

    // OpenGL buffers
    GLuint vao;
    GLuint vBuffer;
    GLuint iBuffer;
    GLuint locModel;

    // OpenGL attribute locations
    GLuint locVertices;

    // Geometry data
    std::vector<Vec3> vertices;
    std::vector<unsigned int> indices;

    void debugShader(void) const;

    void loadGeometry(void);

    Mat4x4 matModel = {2.0, 0.0, 0.0, 0.0,
                        0.0, 1.0, 0.0, 0.5,
                        0.0, 0.0, 1.0, 0.0,
                        0.0, 0.0, 0.0, 1.0};
};
