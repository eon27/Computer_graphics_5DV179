/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include "geometryrender.h"
#include "mycamera.hpp"

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
    locNormals = glGetAttribLocation(program, "vNormal");
    
    locModel = glGetUniformLocation(program,"M");
    locView = glGetUniformLocation(program,"V");
    locProjection = glGetUniformLocation(program,"P");

    glBindVertexArray(0);
    glUseProgram(0);

    cam = MyCamera(500, 60);
}

void GeometryRender::loadGeometry(vector<Vector3> vertexList, vector<Vector3> normalList, vector<int> indexList)
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
    
    size_t vSize = vertices.size()*sizeof(float)*3;
    size_t nSize = normalList.size()*sizeof(float)*3;
    size_t iSize = indices.size()*sizeof(unsigned int);

    glUseProgram(program);
    glBindVertexArray(vao);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    glVertexAttribPointer(locNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));
    glEnableVertexAttribArray(locNormals);
    
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel.mat);
    glUniformMatrix4fv(locView, 1, GL_TRUE, cam.getViewMatrix().mat);
    glUniformMatrix4fv(locProjection, 1, GL_TRUE, cam.getProjectionMatrix().mat);

    // Load object data to the array buffer and index array

    glBufferData(GL_ARRAY_BUFFER, vSize + nSize, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vertices.data()->vec);
    glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, normalList.data()->vec);
    
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
    moveObject *= longestLength; // Scale the movement with the shrinkage of the object.
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
    
    updateView();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(indices.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Not to be called in release...
    debugShader();

    glBindVertexArray(0);
    glUseProgram(0);

}

void GeometryRender::updateView() {
    // Must treat them as floats to so they don't get rounded
    float aspectRatio = ((float) width()) / ((float) height());
    cam.updateView(fov, farplane, top, obliqueScale, obliqueAngleRad, proj_current_idx, aspectRatio);
    glUniformMatrix4fv(locProjection, 1, GL_TRUE, cam.getProjectionMatrix().mat);
}

void GeometryRender::passAction(int action) {
    const float moveCamera = 0.1;
    glUseProgram(program);
    switch (action)
    {
    // Rotating the object
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
    // Moving the object in 3d
    case GLFW_KEY_J:
        matModel.translate(-moveObject, 0, 0);
        break;
    case GLFW_KEY_L:
        matModel.translate(moveObject, 0, 0);
        break;
    case GLFW_KEY_I:
        matModel.translate(0, moveObject, 0);
        break;
    case GLFW_KEY_K:
        matModel.translate(0, -moveObject, 0);
        break;
    case GLFW_KEY_N:
        matModel.translate(0, 0, moveObject);
        break;
    case GLFW_KEY_M:
        matModel.translate(0, 0, -moveObject);
        break;
    
    // Moving the camera in 3d
    case GLFW_KEY_W:
        cam.move(0,0,moveCamera);
        break;
    case GLFW_KEY_S:
        cam.move(0,0,-moveCamera);
        break;
    case GLFW_KEY_A:
        cam.move(-moveCamera,0,0);
        break;
    case GLFW_KEY_D:
        cam.move(moveCamera,0,0);
        break;
    case GLFW_KEY_Q:
        cam.move(0,-moveCamera,0);
        break;
    case GLFW_KEY_E:
        cam.move(0,moveCamera,0);
        break;
    
    case GLFW_KEY_P:
        printf("Material:\n");
        matModel.printMatrix();
        printf("View:\n");
        cam.getViewMatrix().printMatrix();
        printf("Projection:\n");
        cam.getProjectionMatrix().printMatrix();
        printf("\n\n");
        break;
    default:
        break;
    }
    glUniformMatrix4fv(locView, 1, GL_TRUE, cam.getViewMatrix().mat);
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel.mat);
    glUseProgram(0);
}

void GeometryRender::rotateCamera(float deltaX, float deltaY) {
    glUseProgram(program);
    cam.rotate(deltaX/180, deltaY/180);
    glUniformMatrix4fv(locView, 1, GL_TRUE, cam.getViewMatrix().mat);
    glUseProgram(0);
}

void GeometryRender::handleNewObject() {
    vector<Vector3> vertexList;
    vector<Vector3> normalList;
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
    
    for (int i = 0; i < objData.vertexCount; i++) {
        normalList.push_back(Vector3(0,0,0));
    }
    if (objData.normalCount) {
        for (int i = 0; i < objData.faceCount; i++) {
            // For face i, go trough all vertices in the face.
            for (int j = 0; j < objData.faceList[i]->vertex_count; j++) {

                int currentVertex = objData.faceList[i]->vertex_index[j];
                int normalIndexOfVertex = objData.faceList[i]->normal_index[j];
                
                normalList[currentVertex] = normalList[currentVertex] + Vector3(objData.normalList[normalIndexOfVertex]->e).normalize();
            }
        }
    } else {
        // Calculate vertex normals. To account for quads, the indexList and vertexList must be used insted of the 
        for (size_t i = 0; i < indexList.size(); i += 3) {
            Vector3 vector1 = vertexList[indexList[i]] - vertexList[indexList[i+1]];
            Vector3 vector2 = vertexList[indexList[i]] - vertexList[indexList[i+2]];
            Vector3 normal = vector1.cross(vector2).normalize();

            normalList[indexList[i]] = normalList[indexList[i]] + normal;
            normalList[indexList[i+1]] = normalList[indexList[i+1]] + normal;
            normalList[indexList[i+2]] = normalList[indexList[i+2]] + normal;
        }
    }

    //Normalize the normal values
    for (size_t i = 0; i < normalList.size(); i++){
        normalList[i] = normalList[i].normalize();
    }
    
    
    loadGeometry(vertexList, normalList, indexList);
    objData.newData = false;
}