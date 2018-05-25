#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#ifndef DINOSAUR_H
#define DINOSAUR_H

/* Classe Dinosaur
 *  Descrição: classe referente ao objeto dinossauro, personagem principal
 *   da corrida contra o meteoro.
 */
class Dinosaur{
public:
    void jumpAction(bool *jumping, bool *falling);

    // Prototipação dos Métodos Get:
    float getCoordinateX();
    float getCoordinateY();
    float getCoordinateZ();
    // Prototipação dos Métodos Set:
    void  setCoordinateX(float currentX);
    void  setCoordinateY(float currentY);
    void  setCoordinateX(float currentZ);

private:
    float coordinateX;
    float coordinateY;
    float coordinateZ;

    float falling;
};

// CONSTRUTOR //----------------------//

void Dinosaur::Dinosaur(){
    coordinateX = 0.0;
    coordinateY = 0.0;
    coordinateZ = 0.0;

    falling = false;
}

//------------//----------------------//


// MÈTODOS DE PROPÒSITO GERAL //---------------------------------------------------//

void Dinosaur::generate(){
    glPushMatrix();
        // Aqui o cubo faz o movimento contrário ao da translação
        // aplicada pela câmera para cancelar com o +moviment dela
        // e consequentemente permanecer parado
        glTranslatef(coordinateX-0.7, coordinateY, -1.0 coordinateZ);

        // Define a cor do objeto:
        glColor3f (0.5f, 0.5f, 0.5f);
        glutSolidCube(0.2); 
    glPopMatrix();
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
 */
void Dinosaur::jumpAction(bool *jumping){
    // Apenas entra aqui durante uma ação de pulo
    if(*jumping){
        // Enquanto o objeto não chegar até determinada altura
        // o valor de sua coordinateY (altura) é incrementada
        if(!falling){
            coordinateY += 0.035;
            //coordinateY = sqrt(coordinateY*coordinateY + coordinateY + 0.01);

        // Quando o objeto atingi o valor máximo de altura, ele
        // começa a cair, ou seja, sua coordinateY começa a ser decrementada
        }else{
            coordinateY -= 0.035;
            //coordinateY = -sqrt(coordinateY*coordinateY + coordinateY + 0.01);
        }

        // Aqui é feito a checagem se o objeto atingiu o valor máximo
        // da altura do pulo, se sim, falling é colocado como true
        if (coordinateY >= 0.6) falling = true;

        // Quando o objeto chega ao chão novamente é quando ele para
        // de cair e também deixa de pular. Ao apertar o botão de pulo
        // o valor de coordinateY é definido como 0.1, então essa condição
        // efetivamente só é verdadeira no fim do pulo.
        if(coordinateY <= 0.0){
            coordinateY = 0.0;      // Sua altura agora é definida como 0
            falling  = false;       // Sua queda agora é definida como falsa
            *jumping = false;       // assim como seu pulo
        }
    }
}

//----------------------------//---------------------------------------------------//


// MÈTODOS GET E SET //----------------------------//

// Métodos Get:
float Dinosaur::getCoordinateX()
    return coordinateX;
}

float Dinosaur::getCoordinateY(){
    return coordinateY;
}

float Dinosaur::getCoordinateZ(){
    return coordinateZ;
}

// Métodos Set:
void Dinosaur::setCoordinateX(float currentX){
    coordinateX = currentX;
}

void Dinosaur::setCoordinateY(float currentY){
    coordinateY = currentY;
}

void Dinosaur::setCoordinateX(float currentZ){
    coordinateZ = currentZ;
}

//-------------------//----------------------------//

#endif	// DINOSAUR_H