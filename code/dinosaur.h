#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "object.h"

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
             float angle   , float newSizeX, float newSizeY, float newSizeZ);

    // Prototipação dos Métodos de Propósito Geral:
    void runAction();
    void jumpAction(bool *jumping, float *descendForced);
    void collisionEffect();

    // Métodos Get:
    float getVariationX();
    float getMaxVariationX();
    float getVariationY();

    // Métodos Set:
    void setVariationX(float newVariationX);
    void setMaxVariationX(float newMaxVariationX);
    void setVariationY(float newVariationY);

private:
    // Atributos que guardam informações sobre a variação do
    // deslocamento do objeto em determinado instante no respectivo eixo
    float variationX;          // Variação Atual do movimento
    float incrementVariationX; // Define o incremento da variação
    float maxVariationX;       // Variação Máxima
    float variationY;          // Variação atual do pulo
    float decrementVariationY; // Define o decremento da variação

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

    planPositiveX =  sizeX/2 + coordinateX;
    planNegativeX = -sizeX/2 + coordinateX;
    planPositiveY =  sizeY/2 + coordinateY;
    planNegativeY = -sizeY/2 + coordinateY;
    planPositiveZ =  sizeZ/2 + coordinateZ;
    planNegativeZ = -sizeZ/2 + coordinateZ;

    collided = false; 

    // Atributos da Classe:
    variationX = 0;
    incrementVariationX = 0.00000055;
    maxVariationX = 0.5;

    variationY = 0.17;
    decrementVariationY = 0.007;
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

    planPositiveX =  sizeX/2 + coordinateX;
    planNegativeX = -sizeX/2 + coordinateX;
    planPositiveY =  sizeY/2 + coordinateY;
    planNegativeY = -sizeY/2 + coordinateY;
    planPositiveZ =  sizeZ/2 + coordinateZ;
    planNegativeZ = -sizeZ/2 + coordinateZ;

    collided = false; 

    // Atributos da Classe:
    variationX = 0;
    incrementVariationX = 0.00000055;
    maxVariationX = 0.5;

    variationY = 0.17;
    decrementVariationY = 0.007;  
}

Dinosaur::Dinosaur(float currentX, float currentY, float currentZ,
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

    // Atributos da Classe:
    variationX = 0;
    incrementVariationX = 0.00000055;
    maxVariationX = 0.5;

    variationY = 0.17;
    decrementVariationY = 0.007;  
}

//------------//---------------------------------------------------------------------------//


// MÈTODOS DE PROPÒSITO GERAL //-----------------------------------------------------------//

/* Método Run Action:  
 *   Descrição: 
 *     Define o movimento do personagem ao longo do plano XoZ e sua respectiva
 *     aceleração (constante) e velocidade (aumenta conforme o tempo até estabilizar).
 */
void Dinosaur::runAction(){
    if(!collided){
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
    if(*jumping && !collided){
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
            variationY = 0.17;      // Retorna variationY para seu valor default

            *descendForced = 0.0;   // Volta descendForced para seu valor default
            *jumping = false;       // O pulo acabou

            // O dinossauro inicia parado e apenas começa a correr dps do primeiro salto
            if(variationX <= 0){
                variationX = 0.08;
            }
        }
    }
}

void Dinosaur::collisionEffect(){
    if(collided){
        // Se o personagem colidir ele pare de se mover
        variationX = 0;
        variationY = 0;

        // TO DO - Mudança de textura?
    }
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

//-------------------//--------------------------------------------------------------------//

///=================///========================================================================================///

#endif	// DINOSAUR_H