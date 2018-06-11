#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "object.h"
#include "OBJ_Loader.h"
#include "SOIL.h"

using namespace std;

#ifndef PTERODACTYLUS_H
#define PTERODACTYLUS_H

/// CLASSE PTERODACTYLUS ///====================================================================================///
/*  Descrição: 
 *    classe referente ao objeto pterodáctilo, um dos inimigos mais ferozes de qualquer dinossauro que
 *    deseje cruzar o deserto correndo.
 */
///==================///========================================================================================///
class Pterodactylus:public Object{
public:
    // Prototipação do Construtor:
    Pterodactylus();
    Pterodactylus(float currentX, float currentY, float currentZ);
    Pterodactylus(float currentX, float currentY, float currentZ,
                  bool  rotateX , bool  rotateY , bool  rotateZ , 
                  float angle, float newSizeX, float newSizeY, float newSizeZ);

    // Prototipação dos Métodos de Propósito Geral:
    void generate(float red, float green, float blue);
    void flyAction();

    // Métodos Get:
    float getConstantVariationX();

    // Métodos Set:
    void setConstantVariationX(float newVariationX);

private:
    // Atributos que guardam informações sobre a variação
    // do deslocamento objeto em determinado instante no respectivo eixo
    float constantVariationX;       // Variação Atual do movimento
    GLuint mesh;
    void loadMesh();
};


// CONSTRUTOR //---------------------------------------------------------------------------//

Pterodactylus::Pterodactylus(){
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

    centerPositiveDistanceX = 0.5;
    centerPositiveDistanceY = 0.5;
    centerPositiveDistanceZ = 0.5;
    centerNegativeDistanceX = 0.5;
    centerNegativeDistanceY = 0.5;
    centerNegativeDistanceZ = 0.5;

    collidedX = false;
    collidedY = false;
    collidedZ = false; 

    // Atributos da classe:
    constantVariationX = -0.06;

    loadMesh();
}

Pterodactylus::Pterodactylus(float currentX, float currentY, float currentZ){
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

    centerPositiveDistanceX = 0.5;
    centerPositiveDistanceY = 0.5;
    centerPositiveDistanceZ = 0.5;
    centerNegativeDistanceX = 0.5;
    centerNegativeDistanceY = 0.5;
    centerNegativeDistanceZ = 0.5;

    collidedX = false;
    collidedY = false;
    collidedZ = false;  

    // Atributos da Classe:
    constantVariationX = -0.06;

    loadMesh();
}

Pterodactylus::Pterodactylus(float currentX, float currentY, float currentZ,
                             bool  rotateX , bool  rotateY , bool  rotateZ , 
                             float angle, float newSizeX, float newSizeY, float newSizeZ){

    // Atributos herdados:
    coordinateX = currentX;
    coordinateY = currentY;
    coordinateZ = currentZ;

    rotationX = rotateX;
    rotationY = rotateY;
    rotationZ = rotateZ;
    rotationAngle = angle;

    centerPositiveDistanceX = 0.5;
    centerPositiveDistanceY = 0.5;
    centerPositiveDistanceZ = 0.5;
    centerNegativeDistanceX = 0.5;
    centerNegativeDistanceY = 0.5;
    centerNegativeDistanceZ = 0.5;

    collidedX = false;
    collidedY = false;
    collidedZ = false; 

    // Atributos da Classe:
    constantVariationX = -0.06;

    loadMesh();
}

//------------//---------------------------------------------------------------------------//


// MÈTODOS DE PROPÒSITO GERAL //-----------------------------------------------------------//

/* Método Fly Action:  
 *   Descrição: 
 *     Define o movimento constante do pterodáctilo ao longo do plano XoZ.
 */
void Pterodactylus::flyAction(){
    // Faz com que a posição seja decrementada de acordo com a variation X:
    coordinateX  = coordinateX + constantVariationX;
}

//----------------------------//-----------------------------------------------------------//


// MÈTODOS GET E SET //--------------------------------------------------------------------//

// Métodos Get:
float Pterodactylus::getConstantVariationX(){
    return constantVariationX;
}

// Métodos Set:
void Pterodactylus::setConstantVariationX(float newConstantVariationX){
    constantVariationX = newConstantVariationX;
}

//-------------------//--------------------------------------------------------------------//

void Pterodactylus::generate(float red, float green, float blue){
    glPushMatrix();
        glTranslatef(coordinateX, coordinateY, coordinateZ);
        glColor3f(1,1,1);
        glCallList(mesh);
    glPopMatrix();
}

void Pterodactylus::loadMesh(){

    objl::Loader Loader;
    Loader.LoadFile("assets/ptero.obj");
    objl::Mesh curMesh = Loader.LoadedMeshes[0];

    GLuint ptero_tex =   SOIL_load_OGL_texture
      (
        "assets/E_Texture.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
      );

    mesh = glGenLists(1);
    glNewList(mesh, GL_COMPILE);
    {
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, ptero_tex);
        glRotatef(90, 0.0f, 1.0f, 0.0f);
        glScalef(0.2, 0.2, 0.2);
        glBegin(GL_TRIANGLES);
            for (int j = 0; j < curMesh.Indices.size(); j+=3){
                glTexCoord2d(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
                glVertex3f(curMesh.Vertices[curMesh.Indices[j]].Position.X, curMesh.Vertices[curMesh.Indices[j]].Position.Y, curMesh.Vertices[curMesh.Indices[j]].Position.Z);

                glTexCoord2d(curMesh.Vertices[curMesh.Indices[j+1]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j+1]].TextureCoordinate.Y);
                glVertex3f(curMesh.Vertices[curMesh.Indices[j+1]].Position.X, curMesh.Vertices[curMesh.Indices[j+1]].Position.Y, curMesh.Vertices[curMesh.Indices[j+1]].Position.Z);

                glTexCoord2d(curMesh.Vertices[curMesh.Indices[j+2]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j+2]].TextureCoordinate.Y);
                glVertex3f(curMesh.Vertices[curMesh.Indices[j+2]].Position.X, curMesh.Vertices[curMesh.Indices[j+2]].Position.Y, curMesh.Vertices[curMesh.Indices[j+2]].Position.Z);
            }
        glEnd();
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glEndList();
}
///=================///========================================================================================///

#endif  // PTERODACTYLUS_H