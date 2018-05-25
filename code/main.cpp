#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "controls.h"
#include "definitions.h"

using namespace std;

int window_width  = IMAGE_WIDTH;
int window_height = IMAGE_HEIGHT;
float position = 0.0;
float lastPosition = 0.0;
float maxVariation = 0.4;
float variation = 0.1;
float velocity;

void camera (void) {
	// Define as opções da perspectiva, como a Field of View, o near plane, etc
	gluPerspective(60.0, (GLfloat)window_width/(GLfloat)window_height, 1.0, 100.0);

	// Se o modo fps estiver ativo a câmera fica livre para se movimentar:
	if(fpsActive){
		glRotatef(xRotation,1.0,0.0,0.0);  //rotate our camera on teh 
    	glRotatef(yRotation,0.0,1.0,0.0);  //rotate our camera on the 
	}

	// Faz com que a câmera se mova na direção horizontal, pois seu eixo x é inverso
	// ao do universo, já que ela olha de forma "contrária" a ele
    glTranslatef(-position, -0.3f, -0.5);
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

void testCube(){
	glPushMatrix();
		// Aqui o cubo faz o movimento contrário ao da translação
		// aplicada pela câmera para cancelar com o +moviment dela
		// e consequentemente permanecer parado
		glTranslatef(position-0.7, height, -1.0f);

		glColor3f (1.0f, 0.0f, 0.0f);
		glutSolidCube(0.2); 
	glPopMatrix();
}

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
	testCube();

    if(variation <= maxVariation){
		position  = position + variation;
		variation += 0.0000055;

		velocity = position - lastPosition;
		lastPosition = position;

    	cout << "velocity = " << velocity << "\n";
    }else{
    	position  = position + variation;

    	velocity = position - lastPosition;
		lastPosition = position;

    	cout << "velocity = " << velocity << "\n";
    }
    		

    if(jump){
    	if(!objectFall){
    		height += 0.035;
    		//height = sqrt(height*height + height + 0.01);
    	}else{
    		height -= 0.035;
    		//height = -sqrt(height*height + height + 0.01);
    	}

    	if (height >= 0.5) objectFall = true;

    	if(height <= 0.0){
    		height = 0.0;
    		velocity = 1.0;
    		objectFall = false;
    		jump = false;
    	}
    }

    glutSwapBuffers();
}

int main (int argumentsC, char **argumentsV){
	// Inicialização do Glut (definitions.h)
	initializations(argumentsC, argumentsV);
	//enables();

    glutDisplayFunc (display);
    glutIdleFunc (display);
    //glutReshapeFunc (reshape);

    controlsDefinitions();

    glutMainLoop ();
    return 0;
}