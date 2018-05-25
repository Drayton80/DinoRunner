#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "definitions.h"

#ifndef _CONTROLS_H_
#define _CONTROLS_H_

#define SPACE_BAR_KEY 32	// Código ASCII para a barra de espaço
#define ESC_KEY 27			// Código ASCII para a tecla de escape
#define TAB_KEY 9			// Código ASCII para a tecla de TAB

bool  fpsActive = false;
float xRotation = 0;
float yRotation = 0;
float intensity = 0.1;
float height = 0.0;
bool jump = false;
bool objectFall = false;
float lastX, lastY;

/* Descrição: função criada para ser colocada como parâmetro de entrada
 * para a função glutKeyboardFunc() do Glut que registra as interações
 * das teclas padrões do teclado e faz com que sejam executadas quando 
 * pressionadas.
 *   Parâmetros de Entrada:
 *     key: Tecla pressionada;
 *     x: coordenada X atual do mouse no momento que a tecla hey foi pressionada.
 *     y: coordenada Y atual do mouse no momento que a tecla key foi pressionada.
 */
void keyboard (unsigned char key, int x, int y) {
    if (key == 'w' || key == SPACE_BAR_KEY){
    	// TO DO Jump
    	jump = true;
    	height += 0.1;

    	if(fpsActive){
    		// TO DO Jump moviment if camera Jump
    	}
    }

    // Se o TAB for pressionado muda entre os dois possíveis
    // modos de câmera:
    if (key == TAB_KEY){
    	// Alterna a variável fpsActive entre 0 e 1
    	// caso o TAB seja apertado, isso serve para trocar
    	// o como de câmera de FPS para o normal e vice versa
    	if(fpsActive){
    		fpsActive = false;
    	}else{
    		fpsActive = true;

    		// TO DO FPS version (Special)
    	}
    }

    // Ser for pressionado o botão de ESC o programa
    // será fechado
    if (key == ESC_KEY){
    	exit(0);
    }
}

/* Descrição: função criada para ser colocada como parâmetro de entrada
 * para a função glutSpecialFunc() do Glut que registra as interações
 * das teclas especiais do teclado (como F1, seta para cima, etc) e faz 
 * com que sejam executadas quando pressionadas.
 *   Parâmetros de Entrada:
 *     key: Tecla especial pressionada;
 *     x: coordenada X atual do mouse no momento que a tecla hey foi pressionada.
 *     y: coordenada Y atual do mouse no momento que a tecla key foi pressionada.
 */
void keyboardSpecial (int key, int x, int y){
	// Ao pressionar a Seta para Cima aumenta a Intenisdade da rotação da câmera
	if (key == GLUT_KEY_UP){
		// Se a intensidade for menor que 2 (valor máximo de
		// intensidade para o movimento da câmera) então aumenta seu
		// valor em 0.1
		if(intensity < 2){
			intensity += 0.1;
		}
	}

	// Ao pressionar a Seta para Cima aumenta a Intenisdade da rotação da câmera
	if (key == GLUT_KEY_DOWN){
		// Se a intensidade for maior que 0.1 (valor mínimo de
		// intensidade para o movimento da câmera) então diminui seu
		// valor em 0.1. Isso possibilita que a câmera nunca fique 
		// estagnada devido ao valor 0 de intensidade
		if(intensity > 0.1){
			intensity -= 0.1;
		}
	}
}

/* Descrição: função criada para ser colocada como parâmetro de entrada
 * para a função glutPassiveMotionFunc do Glut que registra a posição atual
 * do mouse à cada momento (esse momento é definido pela tava de atualização
 * do registro do próprio mouse).
 *   Parâmetros de Entrada:
 *     x: coordenada X atual do mouse.
 *     y: coordenada Y atual do mouse.
 */
void mouseMovement(int x, int y) {
	if(fpsActive){
		// Pega o resultado da diferença entre a posição em x atual do mouse e 
		// a posição em x anterior
	    int variationX = x - lastX;
	    // Pega o resultado da diferença entre a posição em y atual do mouse e 
		// a posição em y anterior  
	    int variationY = y - lastY;

	    // Atualiza a posição em X anterior com a atual: 
	    lastX = x;
	    // Atualiza a posição em Y anterior com a atual:
	    lastY = y; 

	    // Rotação da câmera será igual a variação do mouse vezes
	    // um número que define a intensidade da rotação
	    xRotation += (float) variationX * intensity; 
	    yRotation += (float) variationY * intensity;    
	}
}

/* Descrição: Passa as funções aqui criadas para suas respectivas chamadas nas funções
 * da biblioteca Glut.
 */
void controlsDefinitions(){
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboardSpecial);
	glutPassiveMotionFunc(mouseMovement);
}

#endif

