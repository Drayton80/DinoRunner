#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <stdlib.h>
#include "object.h"
#include "OBJ_Loader.h"
#include "SOIL.h"

using namespace std;

#ifndef DINOSAUR_H
#define DINOSAUR_H

/// CLASSE DINOSSAUR ///========================================================================================///
/*  Descrição: 
 *    classe referente ao objeto dinossauro, personagem principal
 *    da corrida contra o meteoro.
 */
///==================///========================================================================================///
class Dinosaur:public Object{
public:
    // Prototipação do Construtor:
    Dinosaur();
    Dinosaur(float currentX, float currentY, float currentZ);
    Dinosaur(float currentX, float currentY, float currentZ,
             bool  rotateX , bool  rotateY , bool  rotateZ , 
             float angle, float newSizeX, float newSizeY, float newSizeZ);

    // Prototipação dos Métodos de Propósito Geral:
    void generate(float red, float green, float blue);
    void runAction();
    void jumpAction(bool *jumping, float *descendForced);
    bool collisionCheck(float centerPositiveDistanceObjectX, float centerPositiveDistanceObjectY, 
                        float centerPositiveDistanceObjectZ, float centerNegativeDistanceObjectX, 
                        float centerNegativeDistanceObjectY, float centerNegativeDistanceObjectZ,  
                        float objectCoordinateX, float objectCoordinateY, float objectCoordinateZ);

    // Métodos Get:
    float getVariationX();
    float getMaxVariationX();
    float getVariationY();
    float getDecrementVariationY();

    // Métodos Set:
    void setVariationX(float newVariationX);
    void setMaxVariationX(float newMaxVariationX);
    void setVariationY(float newVariationY);
    void setDecrementVariationY(float newDecrementVariationY);

private:
    // Atributos que guardam informações sobre a variação do
    // deslocamento do objeto em determinado instante no respectivo eixo
    float variationX;          // Variação Atual do movimento
    float incrementVariationX; // Define o incremento da variação
    float maxVariationX;       // Variação Máxima
    float variationY;          // Variação atual do pulo
    float decrementVariationY; // Define o decremento da variação
    
    GLuint mesh;
    void loadMesh();

};


// CONSTRUTOR //---------------------------------------------------------------------------//

Dinosaur::Dinosaur(){
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

    // Atributos da Classe:
    variationX = 0;
    incrementVariationX = 0.00000055;
    maxVariationX = 0.5;

    variationY = 0.17;
    decrementVariationY = 0.007;

    loadMesh();
}

Dinosaur::Dinosaur(float currentX, float currentY, float currentZ){
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
    variationX = 0;
    incrementVariationX = 0.00000055;
    maxVariationX = 0.5;

    variationY = 0.17;
    decrementVariationY = 0.007;

    loadMesh();
}

Dinosaur::Dinosaur(float currentX, float currentY, float currentZ,
                   bool  rotateX , bool  rotateY , bool  rotateZ , 
                   float angle, float newSizeX, float newSizeY, float newSizeZ){
    // Atributos Herdados:
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
    variationX = 0;
    incrementVariationX = 0.00000055;
    maxVariationX = 0.5;

    variationY = 0.17;
    decrementVariationY = 0.007;  

    loadMesh();
}

//------------//---------------------------------------------------------------------------//


// MÈTODOS DE PROPÒSITO GERAL //-----------------------------------------------------------//

/* Método Run Action:  
 *   Descrição: 
 *     Define o movimento do personagem ao longo do plano XoZ e sua respectiva
 *     aceleração (constante) e velocidade (aumenta conforme o tempo até estabilizar).
 */
void Dinosaur::runAction(){
    if(!collidedX || !collidedY || !coordinateZ){
        // O dinossauro só corre se estiver em movimento, pois ele inicia o jogo
        // parado
        if(0 < variationX && variationX <= maxVariationX){
            // Faz com que a posição seja incrementada de acordo
            // com o valor da variationX:
            coordinateX  = coordinateX + variationX;
            // A variationX cresce linearmente aos poucos para
            // aumentar a velocidade de movimento:
            variationX += incrementVariationX;

            //|cout << "Coordenada X = " << coordinateX << "\n";

        }else if(maxVariationX <= variationX){
            // Estagna a variação da posição até aproximadamente
            // o valor de maxVariationX (variação máxima)
            coordinateX  = coordinateX + variationX;

            //|cout << "Coordenada X = " << coordinateX << "\n";
        }
    }
}

/* Método Jump Action:  
 *   Descrição: 
 *     faz com que o dinossauro aumente sua coordenada Y para gerar uma
 *     sensação de pulo em seu movimento.
 *
 *   Parâmetros de Entrada:
 *     bool *jumping:
 *       É um boolean que define se o objeto está pulando ou não, ele
 *       definido como um ponteiro pois o estado de pulo deve ser alterado
 *       também em controls.h, já que é lá onde fica definido as teclas e
 *       suas respectivas consequências quando pressionadas
 *     float *descendForced:
 *       Define uma velocidade de descida forçada pelo objeto durante o salto,
 *       sendo um ponteiro pois no fim do pulo é necessário redefinir descendForced
 *       para o seu valor original de 0.0
 */
void Dinosaur::jumpAction(bool *jumping, float *descendForced){
    // Apenas entra aqui durante uma ação de pulo
    if(*jumping && (!collidedX || !collidedY || !coordinateZ)){
        // A coordenada Y é incrementada de acordo com a variationY, que, por sua
        // vez, é decrementada linearmente a cada interação
        coordinateY += variationY + *descendForced;
        variationY -= decrementVariationY;

        // Quando o objeto chega ao chão novamente é quando ele para
        // de cair e também deixa de pular. Ao apertar o botão de pulo
        // o valor de coordinateY é definido como diferente de 0, então essa condição
        // efetivamente só é verdadeira no fim do pulo.
        if(coordinateY <= 0.0){
            coordinateY = 0.0;      // Sua altura agora é definida como 0
            variationY = 0.2;      // Retorna variationY para seu valor default

            *descendForced = 0.0;   // Volta descendForced para seu valor default
            *jumping = false;       // O pulo acabou

            // O dinossauro inicia parado e apenas começa a correr dps do primeiro salto
            if(variationX <= 0){
                variationX = 0.08;
            }
        }
    }
}

bool Dinosaur::collisionCheck(float centerPositiveDistanceObjectX, float centerPositiveDistanceObjectY, 
                              float centerPositiveDistanceObjectZ, float centerNegativeDistanceObjectX, 
                              float centerNegativeDistanceObjectY, float centerNegativeDistanceObjectZ,  
                              float objectCoordinateX, float objectCoordinateY, float objectCoordinateZ){
    // Define os planos que limitam a hit box do objeto que será testado o
    float objectPositivePlanX = objectCoordinateX + centerPositiveDistanceObjectX;
    float objectNegativePlanX = objectCoordinateX - centerNegativeDistanceObjectX;
    float objectPositivePlanY = objectCoordinateY + centerPositiveDistanceObjectY;
    float objectNegativePlanY = objectCoordinateY - centerNegativeDistanceObjectY;
    float objectPositivePlanZ = objectCoordinateZ + centerPositiveDistanceObjectZ;
    float objectNegativePlanZ = objectCoordinateZ - centerNegativeDistanceObjectZ;

    // Define os planos que limitam a hit box do dinossauro:
    float dinoPositivePlanX = coordinateX + centerPositiveDistanceX;
    float dinoNegativePlanX = coordinateX - centerNegativeDistanceX; 
    float dinoPositivePlanY = coordinateY + centerPositiveDistanceY; 
    float dinoNegativePlanY = coordinateY - centerNegativeDistanceY; 
    float dinoPositivePlanZ = coordinateZ + centerPositiveDistanceZ; 
    float dinoNegativePlanZ = coordinateZ - centerNegativeDistanceZ;  



    // e, ao mesmo tempo, o Plano Negativo Y (parte
    // de baixo) do dinossauro estiver abaixo do Plano Positivo Y do objeto (parte de cima), então isso significará
    // que o cubo de delimitação do dinossauro adentrou os limites do cubo do objeto e, por conseguinte, haverá
    // colisão

    // Se o Plano Positivo X (parte da frente) do dinossauro estiver dentro dos limites dos planos de delimitação
    // (hit box) do objeto ou o Plano Negativo X (parte de trás) dele estiver, a colisão em X é detectada
    if((objectNegativePlanX <= dinoPositivePlanX && dinoPositivePlanX <= objectPositivePlanX)||
       (objectNegativePlanX <= dinoNegativePlanX && dinoNegativePlanX <= objectPositivePlanX)){
        //|std::cout << "Colidiu em X na posição ( " << coordinateX << " ) \n";
        collidedX = true;
    }

    if((objectNegativePlanY <= dinoPositivePlanY && dinoPositivePlanY <= objectPositivePlanY)||
       (objectNegativePlanY <= dinoNegativePlanY && dinoNegativePlanY <= objectPositivePlanY)){
        //|std::cout << "Colidiu em Y na posição ( " << coordinateY << " ) \n";
        collidedY = true;
    }

    // OBS.: A soma com +0.5 vem devido ao deslocamento em Z do dinossauro para corrigir o defeito causado
    //       pelo load mesh
    if((objectNegativePlanZ <= (dinoPositivePlanZ+0.5) && (dinoPositivePlanZ+0.5) <= objectPositivePlanZ)||
       (objectNegativePlanZ <= (dinoNegativePlanZ+0.5) && (dinoNegativePlanZ+0.5) <= objectPositivePlanZ)){
        //|std::cout << "Colidiu em Z na posição ( " << coordinateZ << " ) \n";
        collidedZ = true;
    }

    // Consequência da colisão:
    if(collidedX && collidedY && collidedZ){
        // Se o personagem colidir ele pare de se mover
        variationX = 0;
        variationY = 0;

        return true;

        // TO DO - Mudança de textura?
    }else{
        // Reseta os valores para o próximo teste:
        collidedX = false;
        collidedY = false;
        collidedZ = false;

        return false;
    }
}


void Dinosaur::generate(float red, float green, float blue){
    glPushMatrix();
        glTranslatef(coordinateX, coordinateY, coordinateZ-1);
        glColor3f(1,1,1);
        glCallList(mesh);
    glPopMatrix();
}

void Dinosaur::loadMesh(){

    objl::Loader Loader;
    Loader.LoadFile("assets/Dino.obj");
    objl::Mesh curMesh = Loader.LoadedMeshes[0];

    GLuint dino_tex =   SOIL_load_OGL_texture
      (
        "assets/Texture.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
      );

    mesh = glGenLists(1);
    glNewList(mesh, GL_COMPILE);
    {
        glPushMatrix();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, dino_tex);
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

//----------------------------//-----------------------------------------------------------//


// MÈTODOS GET E SET //--------------------------------------------------------------------//

// Métodos Get:
float Dinosaur::getVariationX(){
    return variationX;
}

float Dinosaur::getMaxVariationX(){
    return maxVariationX;
}

float Dinosaur::getVariationY(){
    return variationY;
}


float Dinosaur::getDecrementVariationY(){
    return decrementVariationY;
}

// Métodos Set:
void Dinosaur::setVariationX(float newVariationX){
    variationX = newVariationX;
}

void Dinosaur::setMaxVariationX(float newMaxVariationX){
    maxVariationX = newMaxVariationX;
}

void Dinosaur::setVariationY(float newVariationY){
    variationY = newVariationY;
}

void Dinosaur::setDecrementVariationY(float newDecrementVariationY){
    decrementVariationY = newDecrementVariationY;
}

//-------------------//--------------------------------------------------------------------//

///=================///========================================================================================///

#endif  // DINOSAUR_H