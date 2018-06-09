#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include "object.h"
#include "definitions.h"
#include "OBJ_Loader.h"
#include "SOIL.h"

using namespace std;

#ifndef CACTUS_H
#define CACTUS_H

class Cactus:public Object{
public:
    // Prototipação do Construtor:
    Cactus();
    Cactus(float currentX, float currentY, float currentZ);
    Cactus(float currentX, float currentY, float currentZ,
             bool  rotateX , bool  rotateY , bool  rotateZ ,
             float angle   , float newSizeX, float newSizeY, float newSizeZ);

    // Prototipação dos Métodos de Propósito Geral:
    void generate(float red, float green, float blue);

private:
    
    int random;
    GLuint mesh;
    void loadMesh();

};


// CONSTRUTOR //---------------------------------------------------------------------------//

Cactus::Cactus(){
    // Atributos herdados:
    coordinateX = 0.0;
    coordinateY = 0.0;
    coordinateZ = 0.0;

    rotationX = false;
    rotationY = false;
    rotationZ = false;
    rotationAngle = 0.0;

    sizeX = 0.5;
    sizeY = 0.5;
    sizeZ = 0.5;

    planPositiveX =  sizeX/2 + coordinateX;
    planNegativeX = -sizeX/2 + coordinateX;
    planPositiveY =  sizeY/2 + coordinateY;
    planNegativeY = -sizeY/2 + coordinateY;
    planPositiveZ =  sizeZ/2 + coordinateZ;
    planNegativeZ = -sizeZ/2 + coordinateZ;

    collided = false;

    srand(time(NULL));
    random = rand() % 4;

    loadMesh();
}

Cactus::Cactus(float currentX, float currentY, float currentZ){
    // Atributos herdados:
    coordinateX = currentX;
    coordinateY = currentY;
    coordinateZ = currentZ;

    rotationX = false;
    rotationY = false;
    rotationZ = false;
    rotationAngle = 0.0;

    sizeX = 0.5;
    sizeY = 0.5;
    sizeZ = 0.5;

    planPositiveX =  sizeX/2 + coordinateX;
    planNegativeX = -sizeX/2 + coordinateX;
    planPositiveY =  sizeY/2 + coordinateY;
    planNegativeY = -sizeY/2 + coordinateY;
    planPositiveZ =  sizeZ/2 + coordinateZ;
    planNegativeZ = -sizeZ/2 + coordinateZ;

    collided = false;

    srand(time(NULL));
    random = rand() % 4;

    loadMesh();
}

Cactus::Cactus(float currentX, float currentY, float currentZ,
                   bool  rotateX , bool  rotateY , bool  rotateZ ,
                   float angle   , float newSizeX, float newSizeY, float newSizeZ){
    // Atributos Herdados:
    coordinateX = currentX;
    coordinateY = currentY;
    coordinateZ = currentZ;

    rotationX = rotateX;
    rotationY = rotateY;
    rotationZ = rotateZ;
    rotationAngle = angle;

    sizeX = newSizeX;
    sizeY = newSizeY;
    sizeZ = newSizeZ;

    planPositiveX =  sizeX/2 + coordinateX;
    planNegativeX = -sizeX/2 + coordinateX;
    planPositiveY =  sizeY/2 + coordinateY;
    planNegativeY = -sizeY/2 + coordinateY;
    planPositiveZ =  sizeZ/2 + coordinateZ;
    planNegativeZ = -sizeZ/2 + coordinateZ;

    collided = false;  

    srand(time(NULL));
    random = rand() % 4;

    loadMesh();
}

void Cactus::generate(float red, float green, float blue){
    glPushMatrix();
    glTranslatef(coordinateX, coordinateY, coordinateZ);
    glColor3f(1,1,1);
    glCallList(mesh);
    glPopMatrix();
}

void Cactus::loadMesh(){

    ostringstream model;
    model << "assets/cactus" << random << ".obj";
    cout << model.str();

    objl::Loader Loader;
    Loader.LoadFile(model.str());
    objl::Mesh curMesh = Loader.LoadedMeshes[0];

    mesh = glGenLists(1);
    glNewList(mesh, GL_COMPILE);
    {
        glPushMatrix();
        glColor3f(curMesh.MeshMaterial.Kd.X, curMesh.MeshMaterial.Kd.Y, curMesh.MeshMaterial.Kd.Z);
        glRotatef(90, 0.0f, 1.0f, 0.0f);
        glScalef(0.05, 0.05, 0.05);
        glBegin(GL_TRIANGLES);
            for (int j = 0; j < curMesh.Indices.size(); j+=3){
                glVertex3f(curMesh.Vertices[curMesh.Indices[j]].Position.X, curMesh.Vertices[curMesh.Indices[j]].Position.Y, curMesh.Vertices[curMesh.Indices[j]].Position.Z);
                glVertex3f(curMesh.Vertices[curMesh.Indices[j+1]].Position.X, curMesh.Vertices[curMesh.Indices[j+1]].Position.Y, curMesh.Vertices[curMesh.Indices[j+1]].Position.Z);
                glVertex3f(curMesh.Vertices[curMesh.Indices[j+2]].Position.X, curMesh.Vertices[curMesh.Indices[j+2]].Position.Y, curMesh.Vertices[curMesh.Indices[j+2]].Position.Z);
            }
        glEnd();
    }
    glPopMatrix();
    glEndList();
}

//-------------------//--------------------------------------------------------------------//

///=================///========================================================================================///

#endif  // DINOSAUR_H