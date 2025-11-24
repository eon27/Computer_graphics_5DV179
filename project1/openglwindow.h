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

    void start();
    virtual void initialize() = 0;
    virtual void display() = 0;
    virtual void passAction(int action) = 0;
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

private:
    void DrawGui();

    GLFWwindow* glfwWindow;
    int windowWidth = 0;
    int windowHeight = 0;

    std::string objFileName;
    std::string objFilePath;

    float fov;
    float farplane;
    float top;
    float obliqueScale;
    float obliqueAngleRad;
};
