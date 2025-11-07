#include <iostream>
#include <math.h>
#include <string>

#include "LinearAlgebra.hpp"
#include "objLoader/objLoader.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int load_new_object(objLoader* objData);

static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

int main() {
    // float rotation_array[4][4] = {
    //     {2,2,2,2},
    //     {2,2,2,2},
    //     {2,2,2,2},
    //     {2,2,2,2}};
    // Matrix mat(rotation_array);
    // mat.scale(2,2,2);
    // mat.printMatrix();
    //printf("------------------\n");
    //mat.translate(5,6,7);
    //mat.printMatrix();
    //printf("------------------\n");
    //mat.rotatex(3.1415);
    //mat.printMatrix();

    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Window creation failed");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);

    objLoader *objData = new objLoader();

    GLsizei num_vertices = 8;

    while (!glfwWindowShouldClose(window))
    {
        int state = glfwGetKey(window, GLFW_KEY_O);
        if (state == GLFW_PRESS)
        {
            int status = load_new_object(objData); // Returns 0 on error.
            if (status == 0) {
                break;
            }
            printf("Vertex count: %d\nVertices:\n", objData->vertexCount);
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 3; j++)
                {
                    printf("%f \t", objData->vertexList[i][0].e[j]);
                }
                printf("\n");
            }
        }
        // Keep running
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

int load_new_object(objLoader* objData) {
    std::string fileName = "";
    char c;

    std::cout << "Enter object filename: "; // Type a number and press enter
    
    while(true) {
        if (std::cin.peek() == '\n') {
            break;
        }
        else {
            std::cin >> c;
            fileName.push_back(c);
        }
    }
    char* namePointer;
    namePointer = &fileName[0];
    int status = objData->load(namePointer); // Returns 0 on error

    return status;
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}