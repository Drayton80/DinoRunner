#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "SOIL.h"

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define PI 3.141592653589793
#define IMAGE_WIDTH  768
#define IMAGE_HEIGHT 512

GLuint ground_tex;
GLuint background_tex;

float positionFactor1 =   0.0;
float positionFactor2 =  50.0;
float positionFactor3 = 100.0;

float descend = 0.0;

bool menu = true;
bool startGame = false;
bool collision = false;
bool restart = false;
bool jump = false;

// Habilita certas definições e caracteristícas do OpenGL, como luz e 
// profundidade da cena:
void enableLight(){

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat ambientLight[]  = {0.2, 0.2, 0.2, 1.0};
    GLfloat diffuseLight[]  = {0.8, 0.8, 0.8, 1.0};
    GLfloat specularLight[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    GLfloat lightPosition[] = {0.5, 0.5, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

// Inicializa o Glut e abre a janela do programa:
void initializations(int argumentsC, char **argumentsV){
    // Inicializa o Glut:
    glutInit(&argumentsC, argumentsV);
    // Define os modos de exibição como Double Buffer e ativa o
    // depth buffer também
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);  

    // Inicializa a janela em que será exibido o programa com
    // o respectivo tamanho informado como entrada
    glutInitWindowSize (IMAGE_WIDTH, IMAGE_HEIGHT);
    // Faz com que a janela seja exibida no centro da tela ao subtrair o tamanho da
    // tela pelo da janela e dividir a subtração por 2
    glutInitWindowPosition ((glutGet(GLUT_SCREEN_WIDTH)  - IMAGE_WIDTH )/2,
                            (glutGet(GLUT_SCREEN_HEIGHT) - IMAGE_HEIGHT)/2);
    // Define o nome que será exibido no topo da janela
    glutCreateWindow ("Dino Runner");

    ground_tex =   SOIL_load_OGL_texture
      (
        "assets/ground3.jpg",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
      );

      background_tex =   SOIL_load_OGL_texture
      (
        "assets/background.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
);
}

#endif  // DEFINITIONS_H