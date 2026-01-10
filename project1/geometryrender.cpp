/*
 *  Workshop 1
 *  Computer Graphics course
 *  Dept Computing Science, Umea University
 *  Stefan Johansson, stefanj@cs.umu.se
 */

#include "geometryrender.h"
#include "mycamera.hpp"

#define CAMERA_MOVE_SPEED 0.001

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
    glGenVertexArrays(OBJECTS, vao);
    glBindVertexArray(vao[0]);

    // Create vertex buffer in the shared display list space and
    // bind it as VertexBuffer (GL_ARRAY_BUFFER)
    glGenBuffers(OBJECTS, vBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vBuffer[0]);

    /* Create buffer in the shared display list space and 
       bind it as GL_ELEMENT_ARRAY_BUFFER */
    glGenBuffers(OBJECTS, iBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer[0]);

    // Get locations of the attributes in the shader
    locVertices = glGetAttribLocation( program, "vPosition");
    locNormals = glGetAttribLocation(program, "vNormal");
    locTexturePos = glGetAttribLocation(program, "vTexture");
    
    locModel = glGetUniformLocation(program,"M");
    locView = glGetUniformLocation(program,"V");
    locProjection = glGetUniformLocation(program,"P");

    locCameraPos = glGetUniformLocation(program,"camPos");

    locLightPos = glGetUniformLocation(program,"lightPos");
    locLightColor = glGetUniformLocation(program,"lightColor");
    locAmbientColor = glGetUniformLocation(program,"ambientColor");

    locMaterialAmbient = glGetUniformLocation(program,"materialAmbient");
    locMaterialDiffuse = glGetUniformLocation(program,"materialDiffuse");
    locMaterialSpecular = glGetUniformLocation(program,"materialSpecular");
    locMaterialShininess = glGetUniformLocation(program,"materialShininess");

    locUseTexture = glGetUniformLocation(program, "useTexture");

    glBindVertexArray(0);
    glUseProgram(0);

    cam = MyCamera(500, 60);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

void GeometryRender::loadGeometry()
{  
    size_t vSize = vertexList.size()*sizeof(float)*3;
    size_t nSize = normalList.size()*sizeof(float)*3;
    size_t tSize = texCoords.size()*sizeof(float)*2;
    size_t iSize = indexList.size()*sizeof(unsigned int);
    

    glUseProgram(program);
    glBindVertexArray(vao[0]);

    // Set the pointers of locVertices to the right places
    glVertexAttribPointer(locVertices, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(locVertices);

    glVertexAttribPointer(locNormals, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));
    glEnableVertexAttribArray(locNormals);

    glVertexAttribPointer(locTexturePos, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize + nSize));
    glEnableVertexAttribArray(locTexturePos);
    
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel.mat);
    glUniformMatrix4fv(locView, 1, GL_TRUE, cam.getViewMatrix().mat);
    glUniformMatrix4fv(locProjection, 1, GL_TRUE, cam.getProjectionMatrix().mat);

    // Load object data to the array buffer and index array

    glBufferData(GL_ARRAY_BUFFER, vSize + nSize + tSize, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vertexList.data()->vec);
    glBufferSubData(GL_ARRAY_BUFFER, vSize, nSize, normalList.data()->vec);
    glBufferSubData(GL_ARRAY_BUFFER, vSize + nSize, tSize, texCoords.data()->vec);
    
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, iSize, indexList.data(), GL_STATIC_DRAW );

    glBindVertexArray(0);
    glUseProgram(0);
}

void GeometryRender::centerAndScaleObject() {
	if (vertexList.empty()) return;
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

    // Maximum possible distance from a point on the object to the origin (used in texture coordinate calculation, to avoid looping over vertices twice)
    pointMaxDistance = longestLength/2 * sqrt(2);
    
	matModel.scale(scale, scale, scale);
    moveObject *= longestLength; // Scale the movement with the shrinkage of the object.
    //matModel.translate(-middleX * scale, -middleY * scale, -middleZ * scale);
	for (long unsigned int i = 0; i < vertexList.size(); i++) {
		vertexList[i].vec[0] -= middleX;
		vertexList[i].vec[1] -= middleY;
		vertexList[i].vec[2] -= middleZ;
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
    
    glUseProgram(program);
    glBindVertexArray(vao[0]);
    
    updateView();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Call OpenGL to draw the triangle
    glDrawElements(GL_TRIANGLES, static_cast<int>(indexList.size()), GL_UNSIGNED_INT, BUFFER_OFFSET(0));

    // Not to be called in release...
    debugShader();

    glBindVertexArray(0);
    glUseProgram(0);
}

/**
 * Takes the inputs from the GUI and utdates the values in the shader.
 */
void GeometryRender::updateView() {

    // Must treat them as floats to so result doesn't get rounded
    float aspectRatio = ((float) width()) / ((float) height());
    cam.updateView(fov, farplane, top, obliqueScale, obliqueAngleRad, proj_current_idx, aspectRatio);
    glUniformMatrix4fv(locProjection, 1, GL_TRUE, cam.getProjectionMatrix().mat);

    glUniform3f(locCameraPos, cam.getPosition().vec[0], cam.getPosition().vec[1], cam.getPosition().vec[2]);

    glUniform3f(locLightPos, lightPos[0], lightPos[1], lightPos[2]);
    glUniform3f(locLightColor, lightColor[0], lightColor[1], lightColor[2]);
    glUniform3f(locAmbientColor, ambientColor[0], ambientColor[1], ambientColor[2]);

    glUniform3f(locMaterialAmbient, materialAmbient[0], materialAmbient[1], materialAmbient[2]);
    glUniform3f(locMaterialDiffuse, materialDiffuse[0], materialDiffuse[1], materialDiffuse[2]);
    glUniform3f(locMaterialSpecular, materialSpecular[0], materialSpecular[1], materialSpecular[2]);
    glUniform1f(locMaterialShininess, materialShininess);

    glUniform1i(locUseTexture, textureShow);
}

void GeometryRender::controlls(int action) {
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
    glUniformMatrix4fv(locModel, 1, GL_TRUE, matModel.mat);
}

void GeometryRender::rotateCamera(float deltaX, float deltaY) {
    glUseProgram(program);
    cam.rotate(deltaX/180, deltaY/180);
    glUniformMatrix4fv(locView, 1, GL_TRUE, cam.getViewMatrix().mat);
    glUseProgram(0);
}

void GeometryRender::moveCamera(int action, float deltaTIme) {
    glUseProgram(program);
    switch (action)
    {
    // Moving the camera in 3d
    case GLFW_KEY_W:
        cam.move(0,0,CAMERA_MOVE_SPEED*deltaTIme);
        break;
    case GLFW_KEY_S:
        cam.move(0,0,-CAMERA_MOVE_SPEED*deltaTIme);
        break;
    case GLFW_KEY_A:
        cam.move(-CAMERA_MOVE_SPEED*deltaTIme,0,0);
        break;
    case GLFW_KEY_D:
        cam.move(CAMERA_MOVE_SPEED*deltaTIme,0,0);
        break;
    case GLFW_KEY_Q:
        cam.move(0,-CAMERA_MOVE_SPEED*deltaTIme,0);
        break;
    case GLFW_KEY_E:
        cam.move(0,CAMERA_MOVE_SPEED*deltaTIme,0);
        break;
    }
    glUniformMatrix4fv(locView, 1, GL_TRUE, cam.getViewMatrix().mat);
    glUseProgram(0);
}

void GeometryRender::handleNewObject() {
    vertexList.clear();
    indexList.clear();
    normalList.clear();
    texCoords.clear();

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
    
    centerAndScaleObject();
    
    // Calculate texture coordinates for vertices
    for (size_t i = 0; i < vertexList.size(); i++) {
        float b = 2 * (vertexList[i] * normalList[i]);
        float c = (vertexList[i] * vertexList[i]) - pointMaxDistance * pointMaxDistance; // pointMaxDistance = maximum possible distance from vertex to origin 
        float q = -0.5 * (b + (signbit(b) * 2 - 1) * sqrt(b*b - 4*c)); // Signbit = true/false (1/0), 1 * 2 - 1 = 1, 0 * 2 - 1 = -1

        float d1 = q;
        float d2 = c/q;

        float dPlus = d1;
        if (d2 > d1) dPlus = d2;

        Vector3 intersect = vertexList[i] + normalList[i] * dPlus;
        intersect = intersect.normalize();

        float s = acos(intersect.vec[0]/pointMaxDistance) / M_1_PI;
        float t = atan(intersect.vec[2]/intersect.vec[1]) / M_1_PI + 0.5;
        texCoords.push_back(Vector2(s, t));
    }
    

    loadGeometry();
}

void GeometryRender::handleNewTexture(unsigned char *data, int width, int height, int nrChannels) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// load and generate the texture
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}