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
}

void GeometryRender::loadGeometry(vector<Vector3> vertexList, vector<int> indexList)
{
    vertices.clear();
    indices.clear();
    vector<Vector3> updatedVertices = centerAndScaleObject(vertexList);
    
    for (long unsigned int i = 0; i < updatedVertices.size(); i++) {
        vertices.push_back(updatedVertices[i]);
    }

    for (long unsigned int i = 0; i < indexList.size(); i++) {
        indices.push_back(indexList[i]);
    }

    glUseProgram(program);
    glBindVertexArray(vao);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);
    
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel.mat);

    // Load object data to the array buffer and index array
    size_t vSize = vertices.size()*sizeof(float)*3;
    size_t iSize = indices.size()*sizeof(unsigned int);
    glBufferData( GL_ARRAY_BUFFER, vSize, vertices.data()->vec, GL_STATIC_DRAW );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indices.data(), GL_STATIC_DRAW );

    glBindVertexArray(0);
    glUseProgram(0);
}

vector<Vector3> GeometryRender::centerAndScaleObject(vector<Vector3> vertexList) {
	if (vertexList.empty()) return vertexList;
	// Reset the matrix	
	matModel = Matrix();

	float lowestX = vertexList[0].vec[0];
	float lowestY = vertexList[0].vec[1];
	float lowestZ = vertexList[0].vec[2];

	float highestX = vertexList[0].vec[0];
	float highestY = vertexList[0].vec[1];
	float highestZ = vertexList[0].vec[2];

	// Find the lowest and highest x,y,z values.
	for (long unsigned int i = 0; i < vertexList.size(); i++) {
		lowestX = min<float>(lowestX, vertexList[i].vec[0]);
		lowestY = min<float>(lowestY, vertexList[i].vec[1]);
		lowestZ = min<float>(lowestZ, vertexList[i].vec[2]);
		
		highestX = max<float>(highestX, vertexList[i].vec[0]);
		highestY = max<float>(highestY, vertexList[i].vec[1]);
		highestZ = max<float>(highestZ, vertexList[i].vec[2]);
	}

	float middleX = (lowestX + highestX) / 2;
	float middleY = (lowestY + highestY) / 2;
	float middleZ = (lowestZ + highestZ) / 2;
	
	
	float lengthX = highestX - lowestX;
	float lengthY = highestY - lowestY;
	float lengthZ = highestZ - lowestZ;
	
	// Avoid division by 0, (0 == false so !0 == true)
	if (!lengthX) lengthX = 1;
	if (!lengthY) lengthY = 1;
	if (!lengthZ) lengthZ = 1;
	
	
	float longestLength = max<float>(lengthX, max<float>(lengthY, lengthZ));
	float scale = 1/longestLength;
    
	matModel.scale(scale, scale, scale);
    //matModel.translate(-middleX * scale, -middleY * scale, -middleZ * scale);
	for (long unsigned int i = 0; i < vertexList.size(); i++) {
		vertexList[i].vec[0] -= middleX;
		vertexList[i].vec[1] -= middleY;
		vertexList[i].vec[2] -= middleZ;
	}

    return vertexList;
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
    glUseProgram(program);
    glBindVertexArray(vao);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Not to be called in release...
    debugShader();

    glBindVertexArray(0);
    glUseProgram(0);

}

void GeometryRender::passAction(int action) {
    glUseProgram(program);
    switch (action)
    {
    case GLFW_KEY_UP:
        matModel.rotatex(-M_PI/18);
        break;
    case GLFW_KEY_DOWN:
        matModel.rotatex(M_PI/18);
        break;
    case GLFW_KEY_LEFT:
        matModel.rotatey(M_PI/18);
        break;
    case GLFW_KEY_RIGHT:
        matModel.rotatey(-M_PI/18);
        break;
    case GLFW_KEY_J:
        matModel.translate(-0.1, 0, 0);
        break;
    case GLFW_KEY_L:
        matModel.translate(0.1, 0, 0);
        break;
    case GLFW_KEY_I:
        matModel.translate(0, 0.1, 0);
        break;
    case GLFW_KEY_K:
        matModel.translate(0, -0.1, 0);
        break;
    default:
        break;
    }
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel.mat);
    glUseProgram(0);
}

void GeometryRender::handleNewObject() {
    vector<Vector3> vertexList;
    vector<int> indexList;
    
    for (int i = 0; i < objData.vertexCount; i++) {
        vertexList.push_back(Vector3(objData.vertexList[i][0].e));
    }
    for (int i = 0; i < objData.faceCount; i++) {
        for (int j = 2; j < objData.faceList[i]->vertex_count; j++) {
            indexList.push_back(objData.faceList[i]->vertex_index[0]);
            indexList.push_back(objData.faceList[i]->vertex_index[j-1]);
            indexList.push_back(objData.faceList[i]->vertex_index[j]);
        }
    }
    loadGeometry(vertexList, indexList);
    objData.newData = false;
}