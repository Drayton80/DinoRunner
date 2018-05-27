#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "controls.h"
#include "definitions.h"
#include "dinosaur.h"
#include "object.h"
#include "random.h"

using namespace std;

Dinosaur *dino = new Dinosaur();	// Instanciação de nosso dinossauro corredor
Object *cactusManager = new Object();

Object *cactiSceneBehind1[100];
Object *cactiSceneBehind2[100];
Object *cactiSceneBehind3[100];

Object *cactiSceneOnPath1[150];
Object *cactiSceneOnPath2[150];
Object *cactiSceneOnPath3[150];

Object *cactiSceneForward1[5];
Object *cactiSceneForward2[5];
Object *cactiSceneForward3[5];

int cactiSceneBehindSize  = sizeof(cactiSceneBehind1) /sizeof(cactiSceneBehind1[0] );
int cactiSceneOnPathSize  = sizeof(cactiSceneOnPath1) /sizeof(cactiSceneOnPath1[0] );
int cactiSceneForwardSize = sizeof(cactiSceneForward1)/sizeof(cactiSceneForward1[0]);
int cactiSceneBiggerSize; 

unsigned short int cactiLimit = 150;
unsigned int cactiGenerateBegin = 150;
short int cactiGenerateSwitch = 0;

int fillBar = 0;
int fillBarLimit = 20;



void objectsInitialPositions(){
	// Define um X aleatório para posicionar algo:
	float randomX;

	dino->setCoordinateZ(-1.0);

	// Aqui é verificado qual o maior de todos os sizes, comparando primeiramente behind com onPath
	// e depois forward com o resultado anterior
	cactiSceneBehindSize > cactiSceneOnPathSize  ? cactiSceneBiggerSize = cactiSceneBehindSize
	                                             : cactiSceneBiggerSize = cactiSceneOnPathSize;
    cactiSceneBiggerSize > cactiSceneForwardSize ? cactiSceneBiggerSize = cactiSceneBiggerSize
	                                             : cactiSceneBiggerSize = cactiSceneForwardSize;

    // Aqui são instanciados os objetos da classe Object relativos à aqueles que ficam no caminho
    // pelo qual o dinossauro percorre, é preciso fazer isso antes do próximo for pois há um
    // método ali dentro (randomCactiOnPath) que altera o valor do k(j ou l) do for dentro de si
    // fazendo com que não dê para usá-los como posição de array para instanciar os objetos,
    // caso fosse feito haveriam elementos do array não instanciados que tentariam ser usados
    // o que provocaria erros
    for(unsigned short int i = 0; i < cactiSceneOnPathSize; i++){
    	cactiSceneOnPath1[i] = new Object();
		cactiSceneOnPath2[i] = new Object();
		cactiSceneOnPath3[i] = new Object();

    }

	for(int i = 0, j = 0, k = 0, l = 0; i < cactiSceneBiggerSize; i++, j++, k++, l++){
		// Faz com que a coordenada X aleatória vá de 1 até cactiGenerateBegin (que inicia com 150),
		// rand()%150 = 0~149
		randomX = rand()%cactiGenerateBegin + 1;

		// Aqui são gerados os cactos da parte detrás da cena:
		// Apenas só preenche cactiSceneBehind quando i for menor que o tamanho do array
		if(i < cactiSceneBehindSize){
			// A coordenada X é gerada no intervalo de [ 1, 150] - 150, ou seja, de [-149, 0],
			// a coordenada Y fica em 0 (preso ao chão) e Z fica no intervalo de [-75, -2]
			cactiSceneBehind1[i] = new Object(randomX-cactiLimit, 0.0, -(rand()%74 + 2));
			// A coordenada X é gerada no intervalo de [ 1, 150], Y fica em 0 (preso ao chão)
			// e Z recai no intervalo de [-75, -2]
			cactiSceneBehind2[i] = new Object(randomX           , 0.0, -(rand()%74 + 2));
			// A coordenada X é gerada no intervalo de [ 1, 150] + 150, ou seja, de [151, 300],
			// a coordenada Y fica em 0 (preso ao chão) e Z fica no intervalo de [-75, -2]
			cactiSceneBehind3[i] = new Object(randomX+cactiLimit, 0.0, -(rand()%74 + 2));
		}

		// Aqui são gerados os cactos que ficam no caminho pelo qual o dinossauro percorre:
		// O X desse primeiro vai de -153 até -3 (isso ocorre para que não sejam gerados cactos na posição em que
		// o personagem nasce e ele morra instantaneamente), é também instanciado esses cactos numa parte da cena
		// que nem se não fossem instanciados seria dado um erro na primeira vez que é chamado o Object->generate
		// OBS.: O intervalo de geração dos cactos é definido pelo valor máximo que j (k ou l) pode alcançar, que,
		//       nesse caso é igual a cactiSceneOnPathSize (tamanho máximo do array)
		randomCactiOnPath(&j, dino->getCoordinateZ(), cactiSceneOnPath1, cactiSceneOnPathSize,-153,
                          &fillBar, fillBarLimit);
		// O primeiro cacto só nasce numa posição além de 3 para que haja espaço para o personagem correr e para
		// garantir que ele não vá nascer na mesma posição que um cacto, como dito acima
		randomCactiOnPath(&k, dino->getCoordinateZ(), cactiSceneOnPath2, cactiSceneOnPathSize, 3,
                          &fillBar, fillBarLimit);
		// Gera cactos do intervalo de 150 até 300 na coordenada X relativa ao l atual
		randomCactiOnPath(&l, dino->getCoordinateZ(), cactiSceneOnPath3, cactiSceneOnPathSize, 153,
                          &fillBar, fillBarLimit);
		
		if(i < cactiSceneForwardSize){

			cactiSceneForward1[i] = new Object(randomX-cactiLimit, 0.0, -(rand()%1));
			cactiSceneForward2[i] = new Object(randomX           , 0.0, -(rand()%1));
			cactiSceneForward3[i] = new Object(randomX+cactiLimit, 0.0, -(rand()%1));
		}
	}
}

void objectsNextPositions(){
	// Define um X aleatório para posicionar algo:
	float randomX;

	// Incrementa a distância final da geração
	cactiGenerateBegin += cactiLimit;

	switch(cactiGenerateSwitch){
		case 0:
			for(int i = 0, j = 0; i < cactiSceneBiggerSize; i++, j++){
				// Cria um novo valor aleatório começando de uma posição 150
				// blocos mais à frente do dinossauro (como o far plane da câmera
				// é 100, a geração fica impercetível ao usuário)
				randomX = rand()%cactiLimit + 1 + cactiGenerateBegin;

				if(i < cactiSceneBehindSize){
					// Define novas coordenadas para os cactos de trás:
					cactiSceneBehind1[i]->setCoordinateX(randomX);
					cactiSceneBehind1[i]->setCoordinateZ(-(rand()%74 + 2));
				}

			
				// Após os primeiros 100 blocos, fillBarLimit é decrementado à cada
				// 300 blocos até no mínimo de 10, isso garante que o número de cactos
				// no caminho aumento cada vez mais até um limite (necessário para que não
				// haja só cactos no caminho em determinado ponto)
				if(10 < fillBarLimit) fillBarLimit--;

				// A alternação entre 3 cactiSceneOnPath é feita aqui novamente para 
				// gerar aleatoriamente sem que o jogador perceba e para que não sejam
				// usados infinitos arrays, já que o jogador pode correr infinitamente
				// se possuir uma maestria e habilidade digna de um jedi
				randomCactiOnPath(&j, dino->getCoordinateZ(), cactiSceneOnPath1, 
					              cactiSceneOnPathSize, cactiGenerateBegin,
                                  &fillBar, fillBarLimit);
				
				
			

				if(i < cactiSceneForwardSize){
					// Define novas coordenadas para os cactos da frente:
					cactiSceneForward1[i]->setCoordinateX(randomX);
					cactiSceneForward1[i]->setCoordinateZ(-(rand()%1));
				}
			}

			cactiGenerateSwitch = 1;

			break;

		case 1:
			for(int i = 0, j = 0; i < cactiSceneBiggerSize; i++, j++){
				randomX = rand()%cactiLimit + 1 + cactiGenerateBegin;

				if(i < cactiSceneBehindSize){
					// Define novas coordenadas para os cactos de trás:
					cactiSceneBehind2[i]->setCoordinateX(randomX);
					cactiSceneBehind2[i]->setCoordinateZ(-(rand()%74 + 2));
				}

			
				// A alternação entre 3 cactiSceneOnPath é feita aqui novamente para 
				// gerar aleatoriamente sem que o jogador perceba e para que não sejam
				// usados infinitos arrays, já que o jogador pode correr infinitamente
				// se possuir uma maestria e habilidade digna de jedi
				randomCactiOnPath(&j, dino->getCoordinateZ(), cactiSceneOnPath2, 
						          cactiSceneOnPathSize, cactiGenerateBegin,
                                  &fillBar, fillBarLimit);
					
				

				if(i < cactiSceneForwardSize){
					// Define novas coordenadas para os cactos da frente:
					cactiSceneForward2[i]->setCoordinateX(randomX);
					cactiSceneForward2[i]->setCoordinateZ(-(rand()%1));
				}
			}

			cactiGenerateSwitch = 2;

			break;

		case 2:
			for(int i = 0, j = 0; i < cactiSceneBiggerSize; i++, j++){
				randomX = rand()%cactiLimit + 1 + cactiGenerateBegin;

				if(i < cactiSceneBehindSize){
					// Define novas coordenadas para os cactos de trás:
					cactiSceneBehind3[i]->setCoordinateX(randomX);
					cactiSceneBehind3[i]->setCoordinateZ(-(rand()%74 + 2));
				}

			
				// A alternação entre 3 cactiSceneOnPath é feita aqui novamente para 
				// gerar aleatoriamente sem que o jogador perceba e para que não sejam
				// usados infinitos arrays, já que o jogador pode correr infinitamente
				// se possuir uma maestria e habilidade digna de jedi
				randomCactiOnPath(&j, dino->getCoordinateZ(), cactiSceneOnPath3, 
						          cactiSceneOnPathSize, cactiGenerateBegin,
                                  &fillBar, fillBarLimit);
					
				
				

				if(i < cactiSceneForwardSize){
					// Define novas coordenadas para os cactos da frente:
					cactiSceneForward3[i]->setCoordinateX(randomX);
					cactiSceneForward3[i]->setCoordinateZ(-(rand()%1));
				}
			}

			cactiGenerateSwitch = 0;

			break;
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
    // Como a câmera é fixa no dinossauro também em relação ao X, para fazer com que ele fique um pouco deslocado
    // para a esquerda (posicionado no canto esquerdo da tela) em relação à câmera basta somar uma constante
    // com o getCoordinateX
    glTranslatef( -(dino->getCoordinateX()+1.7), -(dino->getCoordinateY())*0.3 - 0.7, -2.5);
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

	// A cada 150 unidades de distância são atribuidos novos valores para as coordenadas X e Z
	// de um conjunto de cactos
	if(cactiGenerateBegin <= dino->getCoordinateX()){
		objectsNextPositions();
	}

	// Aqui ocorre a geração dos objetos atrás do dinossauro em relação à câmera (no modo câmera padrão)
	for(unsigned short int i = 0; i < cactiSceneBiggerSize; i++){

		if(i < cactiSceneBehindSize){

			cactiSceneBehind1[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneBehind2[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneBehind3[i]->generate(0.0f, 0.4f, 0.0f);
		}

		if(i < cactiSceneOnPathSize){

			cactiSceneOnPath1[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneOnPath2[i]->generate(0.0f, 0.4f, 0.0f);
		 	cactiSceneOnPath3[i]->generate(0.0f, 0.4f, 0.0f);
		}
		

		if(i < cactiSceneForwardSize){

			cactiSceneForward1[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneForward2[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneForward3[i]->generate(0.0f, 0.4f, 0.0f);
		}

	}
	
	// Aqui é gerado o dinossauro:
	dino->generate(0.4f, 0.4f, 0.4f);

	// Aqui ocorre a geração dos objetos à frente do dinossauro em relação à câmera (no modo câmera padrão)
	for(unsigned short int i = 0; i < cactiSceneForwardSize; i++){

		cactiSceneForward1[i]->generate(0.0f, 0.4f, 0.0f);
		cactiSceneForward2[i]->generate(0.0f, 0.4f, 0.0f);
		cactiSceneForward3[i]->generate(0.0f, 0.4f, 0.0f);
	}

	// Aqui é feito a checagem das ações de movimento dos objetos:
    dino->runAction();
    dino->jumpAction(&jump, &descend);

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