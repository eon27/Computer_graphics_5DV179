/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include "geometryrender.h"

using namespace std;

// Initialize OpenGL
void GeometryRender::initialize()
{
    // Enable depth test
    glEnable(GL_DEPTH_TEST);

    // Create and initialize a program object with shaders
    program = initProgram("vshader.glsl", "fshader.glsl");
    // Installs the program object as part of current rendering state
    glUseProgram(program);

    // Creat a vertex array object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create vertex buffer in the shared display list space and
    // bind it as VertexBuffer (GL_ARRAY_BUFFER)
    glGenBuffers( 1, &vBuffer);
    glBindBuffer( GL_ARRAY_BUFFER, vBuffer);

    /* Create buffer in the shared display list space and 
       bind it as GL_ELEMENT_ARRAY_BUFFER */
    glGenBuffers(1, &iBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

    // Get locations of the attributes in the shader
    locVertices = glGetAttribLocation( program, "vPosition");
    locModel = glGetUniformLocation(program,"M");

    glBindVertexArray(0);
    glUseProgram(0);

    vector<Vector3> vertex;
    vector<int> index;
    // Define vertices in array
    vertex.push_back(Vector3(-0.5f, -0.75f, 0.0f));
    vertex.push_back(Vector3( 0.0f,  0.75f, 0.0f));
    vertex.push_back(Vector3( 0.5f, -0.75f, 1.5f));
    vertex.push_back(Vector3( 0.1f, -0.90f, 0.0f));

    index.push_back(0);
    index.push_back(1);
    index.push_back(2);
    index.push_back(0);
    index.push_back(2);
    index.push_back(3);

    loadGeometry(vertex, index);

}

void GeometryRender::loadGeometry(vector<Vector3> vertexList, vector<int> indexList)
{
    printf("MAKING NEW GEOMEtRY\n");

    vertices.clear();
    indices.clear();

    for (long unsigned int i = 0; i < vertexList.size(); i++) {
        vertices.push_back(vertexList[i]);
    }

    for (long unsigned int i = 0; i < indexList.size(); i++) {
        indices.push_back(indexList[i]);
    }

    glUseProgram(program);
    glBindVertexArray(vao);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    centerAndScaleObject();
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel.mat);

    // Load object data to the array buffer and index array
    size_t vSize = vertices.size()*sizeof(Vector3);
    size_t iSize = indices.size()*sizeof(unsigned int);
    glBufferData( GL_ARRAY_BUFFER, vSize, vertices.data()->vec, GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW );

    glBindVertexArray(0);
    glUseProgram(0);
}

void GeometryRender::centerAndScaleObject() {
	
}

void GeometryRender::loadObjectGeometry() {
    if (objData.newData) {
        vector<Vector3> vertexList;
        vector<int> indexList;
        
        for (int i = 0; i < objData.vertexCount; i++) {
            vertexList.push_back(Vector3(objData.vertexList[i][0].e));
        }
        for (int i = 0; i < objData.faceCount; i++) {
            for (long unsigned int j = 0; j < 3; j++) {
                indexList.push_back(objData.faceList[i]->vertex_index[j]);
            }
        }
        loadGeometry(vertexList, indexList);
        objData.newData = false;
    }
}

// Check if any error has been reported from the shader
void GeometryRender::debugShader(void) const
{
    GLint  logSize;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize );
    if (logSize > 0) {
        std::cerr << "Failure in shader "  << std::endl;
        char logMsg[logSize+1];
        glGetProgramInfoLog( program, logSize, nullptr, &(logMsg[0]) );
        std::cerr << "Shader info log: " << logMsg << std::endl;
    }

}

// Render object
void GeometryRender::display()
{
    loadObjectGeometry();

    glUseProgram(program);
    glBindVertexArray(vao);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    printf("DRAWINg NEw FRAME\n");

    // Call OpenGL to draw the triangle
    glDrawElements(GL_LINE_LOOP, static_cast<int>(indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Not to be called in release...
    debugShader();

    glBindVertexArray(0);
    glUseProgram(0);

}

void GeometryRender::passAction(char action, char dir) {
    glUseProgram(program);
    switch (action)
    {
    case 'r':
        switch (dir) {
            case 'u':
                printf("ROTATE up\n");
                matModel.rotatex(-0.1745);
                break;
            case 'd':
                printf("ROTATE down\n");
                matModel.rotatex(0.1745);
                break;
            case 'l':
                printf("ROTATE left\n");
                matModel.rotatey(-0.1745);
                break;
            case 'r':
                printf("ROTATE right\n");
                matModel.rotatey(0.1745);
                break;
            default:
                break;
        }
        break;
    case 't':
        switch (dir) {
            case 'l':
                /* code */
                break;
            case 'r':
                /* code */
                break;
            default:
                break;
        }
        break;
    case 's':
        /* code */
        break;
    default:
        break;
    }
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel.mat);
    glUseProgram(0);
}
