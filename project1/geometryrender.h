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
    virtual void handleNewTexture(unsigned char *data, int width, int height, int nrChannels) override;

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
    GLuint locNormals;
    GLuint locTexturePos;

    GLuint locCameraPos;

    GLuint locLightPos;
    GLuint locLightColor;
    GLuint locAmbientColor;

    GLuint locMaterialAmbient;
    GLuint locMaterialDiffuse;
    GLuint locMaterialSpecular;
    GLuint locMaterialShininess;

    // Geometry data
    std::vector<Vector3> vertexList;
    std::vector<Vector3> normalList;
    std::vector<unsigned int> indexList;
    std::vector<Vector2> texCoords;

    float moveObject = 0.1;
    float pointMaxDistance;

    void debugShader(void) const;

    void centerAndScaleObject();
    void loadGeometry();
    void loadObjectGeometry();

    void updateView();

    MyCamera cam;

    Matrix matModel = Matrix();
};
