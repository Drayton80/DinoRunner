#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "controls.h"
#include "definitions.h"
#include "dinosaur.h"
#include "cactus.h"

using namespace std;

Dinosaur *dino = new Dinosaur();	// Instanciação de nosso dinossauro corredor
Cactus *cactiPath[5];				// Sim, cacto no plural em inglês é cacti
Cactus *cactiScene1[50];
Cactus *cactiScene2[50];
Cactus *cactiScene3[50];

unsigned short int cactiSceneArraySize = sizeof(cactiScene1)/sizeof(cactiScene1[0]);
unsigned short int cactiLimit = 150;
unsigned int cactiGenerateEnd = 150;
short int cactiGenerateSwitch = 0;
float randomX;

void objectsInitialPositions(){
	dino->setCoordinateZ(-1.0);

	for(unsigned short int i = 0; i < cactiSceneArraySize; i++){
		randomX = rand()%cactiGenerateEnd + 1;

		cactiScene1[i] = new Cactus(randomX-cactiLimit, 0.0, -(rand()%70 + 2));
		cactiScene2[i] = new Cactus(randomX           , 0.0, -(rand()%70 + 2));
		cactiScene3[i] = new Cactus(randomX+cactiLimit, 0.0, -(rand()%70 + 2));
	}
}

void camera (void) {
	// Define as opções da perspectiva, como a Field of View, o near plane, etc
	gluPerspective(60.0, (GLfloat)IMAGE_WIDTH/(GLfloat)IMAGE_HEIGHT, 1.0, 100.0);

	// Se o modo fps estiver ativo a câmera fica livre para se movimentar:
	if(fpsActive){
		glRotatef(xRotation,1.0,0.0,0.0);  //rotate our camera on teh 
    	glRotatef(yRotation,0.0,1.0,0.0);  //rotate our camera on the 
	}

	// Faz com que a câmera se mova na direção do dinossauro, ou seja, para a direita (sentido positivo de x),
	// pois, a partir do momento em que a câmera é fixa na origem, para que ela se mova para um
	// lado é necessário que todo o resto seja movido na direção oposta a esse lado. Por isso o
	// sinal negativo em getCoordinate e em todas as outras coordenadas que queremos deslocá-la
    // É colocado getCoordinate Y aqui no deslocamento em y para dar uma sensação de movimento conjunto no
    // momento do pulo (mas diminuido pelo vezes 0.3) assim conferindo um aspecto de suavidade durante o pulo.
    glTranslatef( -(dino->getCoordinateX()), -(dino->getCoordinateY())*0.3 - 0.7, -2.5);
}

void testLines(){
	// Push e Pop matrix servem para isolar uma transformação das demais, ou seja,
	// fazer uma transformação focar em apenas uma
	glPushMatrix();
		float redIncrement   = 0.3;
		float blueIncrement  = 0.0;
		float greenIncrement = 0.0;

		int rgbSwitch = 1;

		//glTranslatef(0.0f, -0.5f, -3.0f);

		int maxAndMinLines = 10000;

		for(int i = -maxAndMinLines; i <= maxAndMinLines; i += 1){
			// Aqui é feito a transição de cores das linhas verticais:
			switch(rgbSwitch){
				case 1:
					redIncrement  += 0.2;
					greenIncrement = 0.0;
					blueIncrement  = 0.0;

					if(redIncrement > 1.0){
						greenIncrement = 0.3;
						rgbSwitch = 2;
					}

					break;
				case 2:
					redIncrement    = 0.0;
					greenIncrement += 0.2;
					blueIncrement   = 0.0;

					if(greenIncrement > 1.0){
						blueIncrement = 0.3;
						rgbSwitch = 3;
					}

					break;
				case 3:
					redIncrement   = 0.0;
					greenIncrement = 0.0;
					blueIncrement += 0.2;

					if(blueIncrement > 1.0){
						redIncrement = 0.3;
						rgbSwitch = 1;
					}

					break;
					
			}

			// Exibição das linhas:
			glBegin(GL_LINES);
				// Linhas Verticais:
				glColor3f (redIncrement, greenIncrement, blueIncrement);
				glVertex3f( (float) i, -0.2f, -1000.0f);
				glVertex3f( (float) i, -0.2f,  1000.0f);

				// Linhas Horizontais:
				glColor3f (0.4f, 0.4f, 0.4f);
				glVertex3f(-1000.0f, -0.2f, (float) i);
				glVertex3f( 1000.0f, -0.2f, (float) i);			
			glEnd();

		}

	glPopMatrix();
}

/*
void axis(){
	glPushMatrix();
		glTranslatef(-position-0.6, height, -1.0f);

		// Desenhando os eixos de coordenadas:
		glBegin(GL_LINES);
			// Eixo X:
			glColor3f (0.4f, 0.4f, 0.4f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f (1.0f, 0.0f, 0.0f);
			glVertex3f(3.0f, 0.0f, 0.0f);
			// Eixo Y:
			glColor3f (0.4f, 0.4f, 0.4f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f (0.0f, 1.0f, 0.0f);
			glVertex3f(0.0f, 3.0f, 0.0f);
			// Eixo Z:
			glColor3f (0.4f, 0.4f, 0.4f);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glColor3f (0.0f, 0.0f, 1.0f);
			glVertex3f(0.0f, 0.0f, 3.0f);
		glEnd();
	glPopMatrix();
}
*/

void display(void){
	// Define a cor de limpeza do:
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Limpa o color buffer e o depth buffer com a cor indicada:
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Carrega a matriz identidade no conjunto de matrizes para
	// poder limpar as matrizes de mudança
	glLoadIdentity();

	camera();

	testLines();

	if(cactiGenerateEnd <= dino->getCoordinateX()){
		cactiGenerateEnd += cactiLimit;

		switch(cactiGenerateSwitch){
			case 0:
				for(unsigned short int i = 0; i < cactiSceneArraySize; i++){
					randomX = rand()%cactiLimit + 1 + cactiGenerateEnd;

					cactiScene1[i] = new Cactus(randomX, 0.0, -(rand()%70 + 2));
				}

				cactiGenerateSwitch = 1;

				break;

			case 1:
				for(unsigned short int i = 0; i < cactiSceneArraySize; i++){
					randomX = rand()%cactiLimit + 1 + cactiGenerateEnd;

					cactiScene2[i] = new Cactus(randomX, 0.0, -(rand()%70 + 2));
				}

				cactiGenerateSwitch = 2;

				break;

			case 2:
				for(unsigned short int i = 0; i < cactiSceneArraySize; i++){
					randomX = rand()%cactiLimit + 1 + cactiGenerateEnd;

					cactiScene3[i] = new Cactus(randomX, 0.0, -(rand()%70 + 2));
				}

				cactiGenerateSwitch = 0;

				break;

		}

	}

	for(int i = 0; i < cactiSceneArraySize; i++){
		cactiScene1[i]->generate();
		cactiScene2[i]->generate();
		cactiScene3[i]->generate();
	}
	
	dino->generate();

    dino->runAction();
    dino->jumpAction(&jump);

    glutSwapBuffers();
}

int main (int argumentsC, char **argumentsV){
	// Inicialização do Glut (definitions.h)
	initializations(argumentsC, argumentsV);
	//enables();

	// Define as posições iniciais de cada objeto no mundo:
	objectsInitialPositions();

    glutDisplayFunc (display);
    glutIdleFunc (display);
    //glutReshapeFunc (reshape);

    controlsDefinitions();

    glutMainLoop ();
    return 0;
}