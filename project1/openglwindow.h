/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */
#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "3dstudio.h"
#include "lib/objLoader/objLoader.h"

#include "lib/ImGui/imgui.h"
#include "lib/ImGui/imgui_impl_glfw.h"
#include "lib/ImGui/imgui_impl_opengl3.h"
#include "lib/ImGuiFileDialog/ImGuiFileDialog.h"

class OpenGLWindow
{
public:
    OpenGLWindow(std::string title, int width, int height);
    ~OpenGLWindow();

    GLFWwindow* window() const;
    virtual void errorCallback(int error, const char* desc);
    virtual void resizeCallback(GLFWwindow* window, int width, int height);
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    virtual void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    virtual void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    void start();
    virtual void initialize() = 0;
    virtual void display() = 0;
    virtual void passAction(int action) = 0;
    virtual void rotateCamera(float deltaX, float deltaY) = 0;
    void displayNow();

    void openNewObject(std::string filename);
    virtual void handleNewObject() = 0;

    objLoader objData;

protected:
    bool checkOpenGLError() const;
    int width() const;
    int height() const;

    std::string readShaderSource(const std::string shaderFile) const;
    GLuint initProgram(const std::string vShaderFile, const std::string fShaderFile) const;

    void reshape(const int width, const int height) const;

    float fov = 60.0f;
    float farplane = 500.0f;
    float top = 1.0f;
    float obliqueScale = 0.0f;
    float obliqueAngleRad = M_PI/4.0f;
    int proj_current_idx = 0;

    float lightPos[3] = {0.0f, 0.0f, 0.0f};
    float lightColor[3] = {1.0f, 1.0f, 1.0f};
    float ambientColor[3] = {0.2f, 0.2f, 0.2f};

    float materialAmbient[3] = {.5f, .5f, .5f};
    float materialDiffuse[3] = {.5f, .5f, .5f};
    float materialSpecular[3] = {.5f, .5f, .5f};
    float materialShininess = 16.0f;
private:
    void DrawGui();

    GLFWwindow* glfwWindow;
    int windowWidth = 0;
    int windowHeight = 0;

    double mousePosX;
    double mousePosY;

    std::string objFileName;
    std::string objFilePath;
    std::string textureFileName;
    std::string textureFilePath;
    bool textureShow = false;
};
