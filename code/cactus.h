#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

using namespace std;

#ifndef CACTUS_H
#define CACTUS_H

/* CLASSE DINOSSAUR
 *  Descrição: 
 *    classe referente ao objeto cacto, que costuma ser encontrado espalhado
 *    por todo o mapa e é conhecido por ser um imigo jurado de qualquer dinossauro
 *    que deseje cruzar o deserto correndo.
 */
class Cactus{
public:
    // Prototipação dos Construtores:
    Cactus();
    Cactus(float currentX, float currentY, float currentZ);
    Cactus(float currentX, float currentY, float currentZ, float changeSize, float rotation);

    // Prototipação dos Métodos de Propósito Geral:
    void generate();

    // Prototipação dos Métodos Get:
    float getCoordinateX();
    float getCoordinateY();
    float getCoordinateZ();
    // Prototipação dos Métodos Set:
    void setCoordinateX(float currentX);
    void setCoordinateY(float currentY);
    void setCoordinateZ(float currentZ);

private:
    // Coordenadas da posição do objeto:
    float coordinateX;
    float coordinateY;
    float coordinateZ;

    // Definição da distorção de tamanho do objeto:
    float size;

    // Ângulo de rotação do objeto:
    float rotationAngle;

};


// CONSTRUTORES //----------------------//

Cactus::Cactus(){
    coordinateX = 0.0;
    coordinateY = 0.0;
    coordinateZ = 0.0;

    size = 1.0;
    
    rotationAngle = 0.0;
}

Cactus::Cactus(float currentX, float currentY, float currentZ){
    coordinateX = currentX;
    coordinateY = currentY;
    coordinateZ = currentZ;

    size = 1.0;
    
    rotationAngle = 0.0;
}

Cactus::Cactus(float currentX  , float currentY, float currentZ,
               float changeSize, float rotation){
    coordinateX = currentX;
    coordinateY = currentY;
    coordinateZ = currentZ;

    size = changeSize;
    
    rotationAngle = rotation;
}

//--------------//----------------------//


// MÈTODOS DE PROPÒSITO GERAL //---------------------------------------------------//

/* Método Generate:  
 *   Descrição: 
 *     Faz a geração do objeto e define sua posição, tamanho, rotação, cor, etc, ou
 *     seja, todas as informações acerca de suas características no espaço do universo,
 *     usualmente sendo chamado a cada frame de exibição para poder gerar novamente
 *     sua exibição;
 */
void Cactus::generate(){
    glPushMatrix();
        // Muda o tamanho do objeto ao aplicar uma escala isotrópica em X, Y e Z
        glScalef(size, size, size);
        // Faz a rotação em torno do eixo Y, sendo essa a única rotação possível
        // já que o Cacto deve sempre permanecer em pé
        glRotatef(rotationAngle, 0.0f, 1.0f, 0.0f);
        // Aqui o cubo faz o movimento contrário ao da translação
        // aplicada pela câmera para cancelar com o +moviment dela
        // e consequentemente permanecer parado
        glTranslatef(coordinateX, coordinateY, coordinateZ);
        // OBS.: A translação deve sempre ocorrer após as outras mudanças pois a partir
        //       do momento em que o objeto sai da origem, qualquer rotação faria com que
        //       ele movesse em torno do eixo rotacionado (deslocando-o da posição correta)
        //       e qualquer mudança de escala o deslocaria implicitamente

        // Define a cor do objeto:
        glColor3f (0.0f, 1.0f, 0.0f);
        // Exibe o objeto:
        glutSolidCube(0.5); 
    glPopMatrix();
}

//----------------------------//---------------------------------------------------//


// MÈTODOS GET E SET //----------------------------//

// Métodos Get:
float Cactus::getCoordinateX(){
    return coordinateX;
}

float Cactus::getCoordinateY(){
    return coordinateY;
}

float Cactus::getCoordinateZ(){
    return coordinateZ;
}

// Métodos Set:
void Cactus::setCoordinateX(float currentX){
    coordinateX = currentX;
}

void Cactus::setCoordinateY(float currentY){
    coordinateY = currentY;
}

void Cactus::setCoordinateZ(float currentZ){
    coordinateZ = currentZ;
}

//-------------------//----------------------------//

#endif	// CACTUS_H