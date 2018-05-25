#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include "controls.h"
#include "definitions.h"
#include "dinosaur.h"

using namespace std;

int window_width  = IMAGE_WIDTH;
int window_height = IMAGE_HEIGHT;
float maxVariation = 0.4;
float velocity;

Dinosaur *dino = new Dinosaur();

void objectsInitialPositions(){
	dino->setCoordinateZ(-1.0);
}

void camera (void) {
	// Define as opções da perspectiva, como a Field of View, o near plane, etc
	gluPerspective(60.0, (GLfloat)window_width/(GLfloat)window_height, 1.0, 100.0);

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
    // momento do pulo (mas diminuido pelo 0.5) assim conferindo um aspecto de suavidade durante o pulo.
    glTranslatef( -(dino->getCoordinateX()), -(dino->getCoordinateY())*0.5 - 0.3, -0.5);
}

void testLines(){
	// Push e Pop matrix servem para isolar uma transformação das demais, ou seja,
	// fazer uma transformação focar em apenas uma
	glPushMatrix();
		float redIncrement   = 0.3;
		float blueIncrement  = 0.0;
		float greenIncrement = 0.0;

		int rgbSwitch = 1;

		glTranslatef(0.0f, -0.5f, -3.0f);

		int maxAndMinLines = 10000;

		for(int i = -maxAndMinLines; i <= maxAndMinLines; i += 1){
			// Aqui é feito a transição de cores das linhas verticais:
			switch(rgbSwitch){
				case 1:
					redIncrement  += 0.2;
					blueIncrement  = 0.0;
					greenIncrement = 0.0;

					if(redIncrement > 1.0){
						blueIncrement = 0.3;
						rgbSwitch = 2;
					}

					break;
				case 2:
					redIncrement   = 0.0;
					blueIncrement += 0.2;
					greenIncrement = 0.0;

					if(blueIncrement > 1.0){
						greenIncrement = 0.3;
						rgbSwitch = 3;
					}

					break;
				case 3:
					redIncrement    = 0.0;
					blueIncrement   = 0.0;
					greenIncrement += 0.2;

					if(greenIncrement > 1.0){
						redIncrement = 0.3;
						rgbSwitch = 1;
					}

					break;
			}

			// Exibição das linhas:
			glBegin(GL_LINES);
				// Linhas Verticais:
				glColor3f (redIncrement, blueIncrement, greenIncrement);
				glVertex3f( i, -0.5f, -1000.0f);
				glVertex3f( i, -0.5f,  1000.0f);

				// Linhas Horizontais:
				glColor3f (0.4f, 0.4f, 0.4f);
				glVertex3f(-1000.0f, -0.5f, i);
				glVertex3f( 1000.0f, -0.5f, i);			
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

	//axis();
	testLines();
	
	dino->generate();

    dino->runAction(maxVariation);
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