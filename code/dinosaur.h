#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

#ifndef DINOSAUR_H
#define DINOSAUR_H

/* CLASSE DINOSSAUR
 *  Descrição: 
 *    classe referente ao objeto dinossauro, personagem principal
 *    da corrida contra o meteoro.
 */
class Dinosaur{
public:
    // Prototipação do Construtor:
    Dinosaur();

    // Prototipação dos Métodos de Propósito Geral:
    void generate();
    void runAction();
    void jumpAction(bool *jumping);

    // Prototipação dos Métodos Get:
    float getCoordinateX();
    float getCoordinateY();
    float getCoordinateZ();
    // Prototipação dos Métodos Set:
    void setCoordinateX(float currentX);
    void setCoordinateY(float currentY);
    void setCoordinateZ(float currentZ);

private:
    // Coordenadas do objeto:
    float coordinateX;
    float coordinateY;
    float coordinateZ;

    // Atributos que guardam informações sobre a variação
    // do objeto em determinado instante
    float variationX;       // Variação Atual
    float maxVariationX;    // Variação Máxima
    float variationY;

    // Define se o objeto está caindo ou não:
    bool falling;

    // Apenas para exibição de testes:
    float lastCoordinateX = 0.0;
};


// CONSTRUTOR //----------------------//

Dinosaur::Dinosaur(){
    coordinateX = 0.0;
    coordinateY = 0.0;
    coordinateZ = 0.0;

    //variationX = 0.08;
    variationX = 0.2;
    maxVariationX = 0.5;

    variationY = 0.15;   

    falling = false;
}

//------------//----------------------//


// MÈTODOS DE PROPÒSITO GERAL //---------------------------------------------------//

/* Método Generate:  
 *   Descrição: 
 *     Faz a geração do objeto e define sua posição, tamanho, rotação, cor, etc, ou
 *     seja, todas as informações acerca de suas características no espaço do universo,
 *     usualmente sendo chamado a cada frame de exibição para poder gerar novamente
 *     sua exibição;
 */
void Dinosaur::generate(){
    glPushMatrix();
        // Aqui o cubo faz o movimento contrário ao da translação
        // aplicada pela câmera para cancelar com o +moviment dela
        // e consequentemente permanecer parado
        glTranslatef(coordinateX-1.7, coordinateY, coordinateZ);
        //glTranslatef(coordinateX, coordinateY, coordinateZ);

         // Define a cor do objeto:
        glColor3f (0.5f, 0.5f, 0.5f);
        // Exibe o objeto:
        glutSolidCube(0.5); 
    glPopMatrix();
}

/* Método Run Action:  
 *   Descrição: 
 *     Define o movimento do personagem ao longo do plano XoZ e sua respectiva
 *     aceleração (constante) e velocidade (aumenta conforme o tempo até estabilizar).
 */
void Dinosaur::runAction(){
    float velocity;

    if(variationX <= maxVariationX){
        // Faz com que a posição seja incrementada de acordo
        // com o valor da variationX:
        coordinateX  = coordinateX + variationX;
        // A variationX cresce linearmente aos poucos para
        // aumentar a velocidade de movimento:
        variationX += 0.0000055;

       //--------- Apenas para exibição ----------//
        velocity = coordinateX - lastCoordinateX;
        lastCoordinateX = coordinateX;

        cout << "velocity = " << velocity << "\n";
       //--------- -------------------- ----------//

    }else{
        // Estagna a variação da posição até aproximadamente
        // o valor de maxVariationX (variação máxima)
        coordinateX  = coordinateX + variationX;

       //--------- Apenas para exibição ----------//
        velocity = coordinateX - lastCoordinateX;
        lastCoordinateX = coordinateX;

        cout << "velocity = " << velocity << "\n";
       //--------- -------------------- ----------//
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
 */
void Dinosaur::jumpAction(bool *jumping){
    // Apenas entra aqui durante uma ação de pulo
    if(*jumping){
        // Enquanto o objeto não chegar até determinada altura
        // o valor de sua coordinateY (altura) é incrementada
        if(!falling){
            coordinateY += variationY;
            variationY -= 0.007;
            //coordinateY = sqrt(coordinateY*coordinateY + coordinateY + 0.01);

        // Quando o objeto atingi o valor máximo de altura, ele
        // começa a cair, ou seja, sua coordinateY começa a ser decrementada
        }

        // Aqui é feito a checagem se o objeto atingiu o valor máximo
        // da altura do pulo, se sim, falling é colocado como true
        //if (coordinateY >= 2.5) falling = true;

        // Quando o objeto chega ao chão novamente é quando ele para
        // de cair e também deixa de pular. Ao apertar o botão de pulo
        // o valor de coordinateY é definido como 0.1, então essa condição
        // efetivamente só é verdadeira no fim do pulo.
        if(coordinateY <= 0.0){
            coordinateY = 0.0;      // Sua altura agora é definida como 0
            variationY = 0.15;
            falling  = false;       // Sua queda agora é definida como falsa
            *jumping = false;       // assim como seu pulo
        }
    }
}

//----------------------------//---------------------------------------------------//


// MÈTODOS GET E SET //----------------------------//

// Métodos Get:
float Dinosaur::getCoordinateX(){
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

void Dinosaur::setCoordinateZ(float currentZ){
    coordinateZ = currentZ;
}

//-------------------//----------------------------//

#endif	// DINOSAUR_H