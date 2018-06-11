#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include "controls.h"
#include "definitions.h"
#include "dinosaur.h"
#include "pterodactylus.h"
#include "cactus.h"
#include "object.h"
#include "random.h"
#include "bitmap.h"
#include "SOIL.h"

using namespace std;

Dinosaur *dino;	// Instanciação do dinossauro corredor

Cactus *cactiSceneBehind1[1];
Cactus *cactiSceneBehind2[1];
Cactus *cactiSceneBehind3[1];

Cactus *cactiSceneOnPath1[50];
Cactus *cactiSceneOnPath2[50];
Cactus *cactiSceneOnPath3[50];

Cactus *cactiSceneForward1[1];
Cactus *cactiSceneForward2[1];
Cactus *cactiSceneForward3[1];

Pterodactylus *pteros1[50];
Pterodactylus *pteros2[50];
Pterodactylus *pteros3[50];

int cactiSceneBehindSize  = sizeof(cactiSceneBehind1) /sizeof(cactiSceneBehind1[0] );
int cactiSceneOnPathSize  = sizeof(cactiSceneOnPath1) /sizeof(cactiSceneOnPath1[0] );
int cactiSceneForwardSize = sizeof(cactiSceneForward1)/sizeof(cactiSceneForward1[0]);
int cactiSceneBiggerSize;

int pterosSize = sizeof(pteros1) /sizeof(pteros1[0]);

unsigned int score;

unsigned short int beginPteros;

unsigned short int cactiLimit;
unsigned int cactiGenerateBegin;
short int cactiGenerateSwitch;

int fillBar;
int fillBarLimit;

int fillBarPteros;
int fillBarLimitPteros;
float widthCurrent = IMAGE_WIDTH;
float heightCurrent = IMAGE_HEIGHT;

/* Função Objects Initial Positions:  
 *   Descrição: 
 *     Define as posições e valores iniciais de variáveis no começo do jogo e, a partur
 *     dos construtores carrega as malhas de triângulos dos objetos, logo essa função será
 *     chamada apenas quando o jogo começar.
 */
void objectsInitialPositions(){
	dino = new Dinosaur(0.0f, 0.0f, -1.0f);
	dino->setCenterPositiveDistanceX(0.3f);
	dino->setCenterPositiveDistanceY(0.6f);
	dino->setCenterPositiveDistanceZ(0.5f);
	dino->setCenterNegativeDistanceX(0.3f);
	dino->setCenterNegativeDistanceY(0.3f);
	dino->setCenterNegativeDistanceZ(0.5f);

	cactiLimit = 50;
	cactiGenerateBegin = 50;
	cactiGenerateSwitch = 0;

	fillBar = 0;
	fillBarLimit = 20;
	fillBarPteros = 0;
	fillBarLimitPteros = 30;

	score = 0;

	beginPteros = 300;

	float randomX;

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
    	cactiSceneOnPath1[i] = new Cactus();
		cactiSceneOnPath2[i] = new Cactus();
		cactiSceneOnPath3[i] = new Cactus();

		// Define o tamanho da distância do centro dos cactos
		cactiSceneOnPath1[i]->setCenterPositiveDistanceX(-0.15f);
		cactiSceneOnPath1[i]->setCenterPositiveDistanceY( 0.67f);
		cactiSceneOnPath1[i]->setCenterPositiveDistanceZ( 0.0f );
		cactiSceneOnPath1[i]->setCenterNegativeDistanceX( 0.55f);
		cactiSceneOnPath1[i]->setCenterNegativeDistanceY( 0.0f );
		cactiSceneOnPath1[i]->setCenterNegativeDistanceZ( 0.5f );

		cactiSceneOnPath2[i]->setCenterPositiveDistanceX(-0.15f);
		cactiSceneOnPath2[i]->setCenterPositiveDistanceY( 0.67f);
		cactiSceneOnPath2[i]->setCenterPositiveDistanceZ( 0.0f );
		cactiSceneOnPath2[i]->setCenterNegativeDistanceX( 0.55f);
		cactiSceneOnPath2[i]->setCenterNegativeDistanceY( 0.0f );
		cactiSceneOnPath2[i]->setCenterNegativeDistanceZ( 0.5f );

		cactiSceneOnPath3[i]->setCenterPositiveDistanceX(-0.15f);
		cactiSceneOnPath3[i]->setCenterPositiveDistanceY( 0.67f);
		cactiSceneOnPath3[i]->setCenterPositiveDistanceZ( 0.0f );
		cactiSceneOnPath3[i]->setCenterNegativeDistanceX( 0.55f);
		cactiSceneOnPath3[i]->setCenterNegativeDistanceY( 0.0f );
		cactiSceneOnPath3[i]->setCenterNegativeDistanceZ( 0.5f );
    }

    for(unsigned short int i = 0; i < pterosSize; i++){
    	pteros1[i] = new Pterodactylus();
    	pteros2[i] = new Pterodactylus();
    	pteros3[i] = new Pterodactylus();

    	// Define o tamanho da distância do centro dos pterodáctilos
    	pteros1[i]->setCenterPositiveDistanceX(0.3f);
		pteros1[i]->setCenterPositiveDistanceY(0.2f);
		pteros1[i]->setCenterPositiveDistanceZ(0.5f);
		pteros1[i]->setCenterNegativeDistanceX(0.6f);
		pteros1[i]->setCenterNegativeDistanceY(0.1f);
		pteros1[i]->setCenterNegativeDistanceZ(0.5f);

		pteros2[i]->setCenterPositiveDistanceX(0.3f);
		pteros2[i]->setCenterPositiveDistanceY(0.2f);
		pteros2[i]->setCenterPositiveDistanceZ(0.5f);
		pteros2[i]->setCenterNegativeDistanceX(0.6f);
		pteros2[i]->setCenterNegativeDistanceY(0.1f);
		pteros2[i]->setCenterNegativeDistanceZ(0.5f);

		pteros3[i]->setCenterPositiveDistanceX(0.3f);
		pteros3[i]->setCenterPositiveDistanceY(0.2f);
		pteros3[i]->setCenterPositiveDistanceZ(0.5f);
		pteros3[i]->setCenterNegativeDistanceX(0.6f);
		pteros3[i]->setCenterNegativeDistanceY(0.1f);
		pteros3[i]->setCenterNegativeDistanceZ(0.5f);
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
			cactiSceneBehind1[i] = new Cactus(randomX-cactiLimit, 0.0, -(rand()%74 + 2));
			// A coordenada X é gerada no intervalo de [ 1, 150], Y fica em 0 (preso ao chão)
			// e Z recai no intervalo de [-75, -2]
			cactiSceneBehind2[i] = new Cactus(randomX           , 0.0, -(rand()%74 + 2));
			// A coordenada X é gerada no intervalo de [ 1, 150] + 150, ou seja, de [151, 300],
			// a coordenada Y fica em 0 (preso ao chão) e Z fica no intervalo de [-75, -2]
			cactiSceneBehind3[i] = new Cactus(randomX+cactiLimit, 0.0, -(rand()%74 + 2));
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

			cactiSceneForward1[i] = new Cactus(randomX-cactiLimit, 0.0, rand()%76);
			cactiSceneForward2[i] = new Cactus(randomX           , 0.0, rand()%76);
			cactiSceneForward3[i] = new Cactus(randomX+cactiLimit, 0.0, rand()%76);
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
			// Move o bloco mais atrás onde é gerado o chão para depois do último
			// bloco, fazendo com que o que estava atrás vá para frente sem haver alteração
			// na exibição do do meio (onde está a câmera)
			positionFactor1 = positionFactor3 + cactiLimit;

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
				if(15 < fillBarLimit) fillBarLimit--;

				// A alternação entre 3 cactiSceneOnPath é feita aqui novamente para 
				// gerar aleatoriamente sem que o jogador perceba e para que não sejam
				// usados infinitos arrays, já que o jogador pode correr infinitamente
				// se possuir uma maestria e habilidade digna de um jedi
				randomCactiOnPath(&j, dino->getCoordinateZ(), cactiSceneOnPath1, 
					              cactiSceneOnPathSize, cactiGenerateBegin,
                                  &fillBar, fillBarLimit);
				
				if(beginPteros <= dino->getCoordinateX())
					randomPterodactylusOnPath(i, dino->getCoordinateZ(), pteros1, pterosSize, cactiGenerateBegin, 
			                                  &fillBarPteros, fillBarLimitPteros);				

				if(i < cactiSceneForwardSize){
					// Define novas coordenadas para os cactos da frente:
					cactiSceneForward1[i]->setCoordinateX(randomX);
					cactiSceneForward1[i]->setCoordinateZ(rand()%76);
				}
			}

			cactiGenerateSwitch = 1;

			break;

		case 1:
			// Move o bloco mais atrás onde é gerado o chão para depois do último
			// bloco, fazendo com que o que estava atrás vá para frente sem haver alteração
			// na exibição do do meio (onde está a câmera)
			positionFactor2 = positionFactor1 + cactiLimit;

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
				
				if(beginPteros <= dino->getCoordinateX())
					randomPterodactylusOnPath(i, dino->getCoordinateZ(), pteros2, pterosSize, cactiGenerateBegin, 
			    	                          &fillBarPteros, fillBarLimitPteros);
				

				if(i < cactiSceneForwardSize){
					// Define novas coordenadas para os cactos da frente:
					cactiSceneForward2[i]->setCoordinateX(randomX);
					cactiSceneForward2[i]->setCoordinateZ(rand()%76);
				}
			}

			cactiGenerateSwitch = 2;

			break;

		case 2:
			// Move o bloco mais atrás onde é gerado o chão para depois do último
			// bloco, fazendo com que o que estava atrás vá para frente sem haver alteração
			// na exibição do do meio (onde está a câmera)
			positionFactor3 = positionFactor2 + cactiLimit;

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
					
				if(beginPteros <= dino->getCoordinateX())
					randomPterodactylusOnPath(i, dino->getCoordinateZ(), pteros3, pterosSize, cactiGenerateBegin, 
			    	                          &fillBarPteros, fillBarLimitPteros);


				if(i < cactiSceneForwardSize){
					// Define novas coordenadas para os cactos da frente:
					cactiSceneForward3[i]->setCoordinateX(randomX);
					cactiSceneForward3[i]->setCoordinateZ(rand()%76);
				}
			}

			cactiGenerateSwitch = 0;

			break;
	}

}

void objectsRestartPositions(){
	dino->setCoordinateX( 0.0f);
	dino->setCoordinateY( 0.0f);
	dino->setCoordinateZ(-1.0f);
	dino->setCollidedX(false);
	dino->setCollidedY(false);
	dino->setCollidedZ(false);
	dino->setDecrementVariationY(0.007f);
	dino->setVariationY(0.2f);

	jump = false;
	descend = 0.0;

	cactiLimit = 50;
	cactiGenerateBegin = 50;
	cactiGenerateSwitch = 0;

	fillBar = 0;
	fillBarLimit = 20;
	fillBarPteros = 0;
	fillBarLimitPteros = 30;

	score = 0;

	float randomX;

	for(unsigned short int i = 0; i < pterosSize; i++){
		pteros1[i]->setCoordinateX(0.0f);
		pteros1[i]->setCoordinateY(0.0f);
		pteros1[i]->setCoordinateZ(0.0f);
		pteros1[i]->setConstantVariationX(-0.06);

		pteros2[i]->setCoordinateX(0.0f);
		pteros2[i]->setCoordinateY(0.0f);
		pteros2[i]->setCoordinateZ(0.0f);
		pteros2[i]->setConstantVariationX(-0.06);

		pteros3[i]->setCoordinateX(0.0f);
		pteros3[i]->setCoordinateY(0.0f);
		pteros3[i]->setCoordinateZ(0.0f);
		pteros3[i]->setConstantVariationX(-0.06);
	}

	// Aqui é verificado qual o maior de todos os sizes, comparando primeiramente behind com onPath
	// e depois forward com o resultado anterior
	cactiSceneBehindSize > cactiSceneOnPathSize  ? cactiSceneBiggerSize = cactiSceneBehindSize
	                                             : cactiSceneBiggerSize = cactiSceneOnPathSize;
    cactiSceneBiggerSize > cactiSceneForwardSize ? cactiSceneBiggerSize = cactiSceneBiggerSize
	                                             : cactiSceneBiggerSize = cactiSceneForwardSize;


	for(int i = 0, j = 0, k = 0, l = 0; i < cactiSceneBiggerSize; i++, j++, k++, l++){
		// Faz com que a coordenada X aleatória vá de 1 até cactiGenerateBegin (que inicia com 150),
		// rand()%150 = 0~149
		randomX = rand()%cactiGenerateBegin + 1;

		// Aqui são gerados os cactos da parte detrás da cena:
		// Apenas só preenche cactiSceneBehind quando i for menor que o tamanho do array
		if(i < cactiSceneBehindSize){
			// A coordenada X é gerada no intervalo de [ 1, 150] - 150, ou seja, de [-149, 0],
			// a coordenada Y fica em 0 (preso ao chão) e Z fica no intervalo de [-75, -2]
			cactiSceneBehind1[i]->setCoordinateX(randomX-cactiLimit);
			cactiSceneBehind1[i]->setCoordinateY(0.0);
			cactiSceneBehind1[i]->setCoordinateZ(-(rand()%38 + 2));
			// A coordenada X é gerada no intervalo de [ 1, 150], Y fica em 0 (preso ao chão)
			// e Z recai no intervalo de [-75, -2]
			cactiSceneBehind2[i]->setCoordinateX(randomX);
			cactiSceneBehind2[i]->setCoordinateY(0.0);
			cactiSceneBehind2[i]->setCoordinateZ(-(rand()%38 + 2));
			// A coordenada X é gerada no intervalo de [ 1, 150] + 150, ou seja, de [151, 300],
			// a coordenada Y fica em 0 (preso ao chão) e Z fica no intervalo de [-75, -2]
			cactiSceneBehind3[i]->setCoordinateX(randomX+cactiLimit);
			cactiSceneBehind3[i]->setCoordinateY(0.0);
			cactiSceneBehind3[i]->setCoordinateZ(-(rand()%38 + 2));
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
			cactiSceneForward1[i]->setCoordinateX(randomX-cactiLimit);
			cactiSceneForward1[i]->setCoordinateY(0.0);
			cactiSceneForward1[i]->setCoordinateZ(rand()%38 + 2);

			cactiSceneForward2[i]->setCoordinateX(randomX);
			cactiSceneForward2[i]->setCoordinateY(0.0);
			cactiSceneForward2[i]->setCoordinateZ(rand()%38 + 2);

			cactiSceneForward3[i]->setCoordinateX(randomX+cactiLimit);
			cactiSceneForward3[i]->setCoordinateY(0.0);
			cactiSceneForward3[i]->setCoordinateZ(rand()%38 + 2);
		}
	}
}

void camera (void) {
	// Se o modo fps estiver ativo a câmera fica livre para se movimentar:
	if(fpsActive){
		glTranslatef(-dino->getCoordinateZ(), -dino->getCoordinateY() - 0.7, dino->getCoordinateX()-0.5);
		// É preciso primeiramente rotacionar a camera 90º em torno do eixo Y para que ela comece
		// a apontar para onde o dinossauro aponta
		// OBS.: No OpenGL a chamada das matrizes mais abaixo ocorre primeiro, sendo isso devido ao fato
		//       de que, na multiplicação de matrizes para ocorrer transformações geomêtricas, a transformação
		//       que é colocada mais à direita ocorre primeiro, ou seja, em M = Mt * Ms * Ma, Ma ocorrerá
		//       primeiro, depois será Ms e, por fim, Mt. Ao colocarmos isso em termos de OpenGL aqui nessas
		//       duas transformações obteriamos M = glTranslatef(...) * glRotatef(...)
		glRotatef(90, 0.0f, 1.0f, 0.0f);
	}else{
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
}

void reshape (int width, int height) {  
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);   
	glMatrixMode(GL_PROJECTION);   
	glLoadIdentity();   

	// Define as opções da perspectiva, como a Field of View, o near plane, etc
	gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 50.0); 
	glMatrixMode(GL_MODELVIEW);   
}

void gameOver(){
	setOrtho(widthCurrent, heightCurrent);
		glLoadIdentity();

		glTranslatef( 0.0, 0.0, -2.0);

		glColor4f(0.0, 0.0, 0.0, 0.5);
		glBegin(GL_POLYGON);
			glVertex3f(widthCurrent, heightCurrent, -2.0);
			glVertex3f(         0.0, heightCurrent, -2.0);
			glVertex3f(         0.0,           0.0, -2.0);
			glVertex3f(widthCurrent,           0.0, -2.0);
		glEnd();

		drawGameOver(widthCurrent, heightCurrent);
	restoreView();

	for(unsigned short int i = 0; i < pterosSize; i++){
		pteros1[i]->setConstantVariationX(0.0f);
		pteros2[i]->setConstantVariationX(0.0f);
		pteros3[i]->setConstantVariationX(0.0f);
	}

	if(restart){
		positionFactor1 =   0.0;
		positionFactor2 =  50.0;
		positionFactor3 = 100.0;

		objectsRestartPositions();

		restart   = false;
		collision = false;
	}
}

void ground(){
	glPushMatrix();

	GLfloat orange[] = {1.0, 0.45, 0.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, orange);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, orange);

	for(unsigned short int i = 0; i < 5; i++){
		for(unsigned short int j = 0; j < 5; j++){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, ground_tex);

			glBegin(GL_QUADS);
					glTexCoord2d(0.0,0.0); 
					glVertex3f(positionFactor1 - i*10, -0.2f,  j*10);

					glTexCoord2d(1.0,0.0); 
					glVertex3f(positionFactor1 + i*10, -0.2f,  j*10);

					glTexCoord2d(1.0,1.0); 
					glVertex3f(positionFactor1 + i*10, -0.2f, -j*10);

					glTexCoord2d(0.0,1.0);
					glVertex3f(positionFactor1 - i*10, -0.2f, -j*10);			
					
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, ground_tex);

			glBegin(GL_QUADS);
					glTexCoord2d(0.0,0.0); 
					glVertex3f(positionFactor2 - i*10, -0.2f,  j*10);

					glTexCoord2d(1.0,0.0); 
					glVertex3f(positionFactor2 + i*10, -0.2f,  j*10);

					glTexCoord2d(1.0,1.0); 
					glVertex3f(positionFactor2 + i*10, -0.2f, -j*10);

					glTexCoord2d(0.0,1.0);
					glVertex3f(positionFactor2 - i*10, -0.2f, -j*10);			
					
			glEnd();
			glDisable(GL_TEXTURE_2D);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, ground_tex);

			glBegin(GL_QUADS);
					glTexCoord2d(0.0,0.0); 
					glVertex3f(positionFactor3 - i*10, -0.2f,  j*10);

					glTexCoord2d(1.0,0.0); 
					glVertex3f(positionFactor3 + i*10, -0.2f,  j*10);

					glTexCoord2d(1.0,1.0); 
					glVertex3f(positionFactor3 + i*10, -0.2f, -j*10);

					glTexCoord2d(0.0,1.0);
					glVertex3f(positionFactor3 - i*10, -0.2f, -j*10);			
					
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}
	}
	glPopMatrix();
}

void background(){
	// Push e Pop matrix servem para isolar uma ou mais transformações das demais ao
	// mudar para um estado que é englobado pelo anterior, mas não interfere nele, ou seja,
	// fazer uma ou mais transformações focarem em apenas um escopo de glPushMatrix()[...]glPopMatrix()
	// OBS.: É similar ao funcionamento de um abre e fecha {}
	glPushMatrix();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, background_tex);
		glBegin(GL_QUADS);
				glTexCoord2d(0.0,0.0); 
				glVertex3f(dino->getCoordinateX() - 100.0f, -20.0f, -40.0f);

				glTexCoord2d(1.0,0.0); 
				glVertex3f(dino->getCoordinateX() + 100.0f, -20.0f, -40.0f);

				glTexCoord2d(1.0,1.0); 
				glVertex3f(dino->getCoordinateX() + 100.0f, 60.0f, -40.0f);

				glTexCoord2d(0.0,1.0);
				glVertex3f(dino->getCoordinateX() - 100.0f, 60.0f, -40.0f);			
		glEnd();
		glDisable(GL_TEXTURE_2D);
	

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, background_tex);

		glBegin(GL_QUADS);
				glTexCoord2d(0.0,0.0); 
				glVertex3f(dino->getCoordinateX() + 40.0f, -50.0f, -50.0f);

				glTexCoord2d(1.0,0.0); 
				glVertex3f(dino->getCoordinateX() + 40.0f, -50.0f, 50.0f);

				glTexCoord2d(1.0,1.0); 
				glVertex3f(dino->getCoordinateX() + 40.0f, 150.0f, 50.0f);

				glTexCoord2d(0.0,1.0);
				glVertex3f(dino->getCoordinateX() + 40.0f, 150.0f, -50.0f);			
				
		glEnd();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void display(void){
	if(startGame){
		// Define as posições iniciais de cada objeto no mundo:
		objectsInitialPositions();

		startGame = false;
	}

	if(menu){
		// Define a cor de limpeza do:
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Limpa o color buffer e o depth buffer com a cor indicada:
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Carrega a matriz identidade no conjunto de matrizes para
		// poder limpar as matrizes de mudança
		glLoadIdentity();

		setOrtho(widthCurrent, heightCurrent);
			drawMenuTitle(widthCurrent, heightCurrent);
		restoreView();

		// Troca os buffers
	    glutSwapBuffers();
	}else{
		// Define a cor de limpeza do:
		glClearColor(1.0f, 0.45f, 0.0f, 1.0f);
		// Limpa o color buffer e o depth buffer com a cor indicada:
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Carrega a matriz identidade no conjunto de matrizes para
		// poder limpar as matrizes de mudança
		glLoadIdentity();

		camera();

		background();
		ground();

		// A cada 50 unidades de distância são atribuidos novos valores para as coordenadas X e Z
		// de um conjunto de cactos
		if(cactiGenerateBegin <= dino->getCoordinateX()){
			objectsNextPositions();
		}

		// Aqui ocorre a geração dos objetos atrás do dinossauro em relação à câmera (no modo câmera padrão)
		for(unsigned short int i = 0; i < cactiSceneBehindSize; i++){

			cactiSceneBehind1[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneBehind2[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneBehind3[i]->generate(0.0f, 0.4f, 0.0f);

		}

		if(jump && fpsActive){
			//OBS.: É necessário um for separado pois as diversas iterações de um for conjunto fazem com
			//      que alguns cactos de trás da cena fiquem à frente dos on Path
			for(unsigned short int i = 0; i < cactiSceneOnPathSize; i++){
				if(cactiSceneOnPath1[i]->getCoordinateZ() < -1.5)
					cactiSceneOnPath1[i]->generate(0.0f, 0.4f, 0.0f);
				
				if(cactiSceneOnPath2[i]->getCoordinateZ() < -1.5)
					cactiSceneOnPath2[i]->generate(0.0f, 0.4f, 0.0f);
				
				if(cactiSceneOnPath3[i]->getCoordinateZ() < -1.5)
				 	cactiSceneOnPath3[i]->generate(0.0f, 0.4f, 0.0f);
			}

			// Para que não seja visualizada a primeira renderização deles, os pteros são gerados 300 blocos além
			// da posição definida nesse if, ou seja, o jogador só os vê aproximadamente à partir da posição 450
			if(beginPteros <= dino->getCoordinateX()){
				for(unsigned short int i = 0; i < pterosSize; i++){

			    	pteros1[i]->generate(0.4f, 0.4f, 0.4f);
			    	pteros1[i]->flyAction();
			    	collision = dino->collisionCheck(pteros1[i]->getCenterPositiveDistanceX(),
		                                             pteros1[i]->getCenterPositiveDistanceY(),
		                                             pteros1[i]->getCenterPositiveDistanceZ(),
		                                             pteros1[i]->getCenterNegativeDistanceX(),
		                                             pteros1[i]->getCenterNegativeDistanceY(),
		                                             pteros1[i]->getCenterNegativeDistanceZ(), 
		                                             pteros1[i]->getCoordinateX(), 
		                                             pteros1[i]->getCoordinateY(), 
		                                             pteros1[i]->getCoordinateZ());

			    	pteros2[i]->generate(0.4f, 0.4f, 0.4f);
			    	pteros2[i]->flyAction();
			    	collision = dino->collisionCheck(pteros2[i]->getCenterPositiveDistanceX(),
		                                             pteros2[i]->getCenterPositiveDistanceY(),
		                                             pteros2[i]->getCenterPositiveDistanceZ(),
		                                             pteros2[i]->getCenterNegativeDistanceX(),
		                                             pteros2[i]->getCenterNegativeDistanceY(),
		                                             pteros2[i]->getCenterNegativeDistanceZ(), 
		                                             pteros2[i]->getCoordinateX(), 
		                                             pteros2[i]->getCoordinateY(), 
		                                             pteros2[i]->getCoordinateZ());

			    	pteros3[i]->generate(0.4f, 0.4f, 0.4f);
			    	pteros3[i]->flyAction();
			    	collision = dino->collisionCheck(pteros3[i]->getCenterPositiveDistanceX(),
		                                             pteros3[i]->getCenterPositiveDistanceY(),
		                                             pteros3[i]->getCenterPositiveDistanceZ(),
		                                             pteros3[i]->getCenterNegativeDistanceX(),
		                                             pteros3[i]->getCenterNegativeDistanceY(),
		                                             pteros3[i]->getCenterNegativeDistanceZ(), 
		                                             pteros3[i]->getCoordinateX(), 
		                                             pteros3[i]->getCoordinateY(), 
		                                             pteros3[i]->getCoordinateZ());
			    }
			}
		}else{
			if(beginPteros <= dino->getCoordinateX()){
				for(unsigned short int i = 0; i < pterosSize; i++){

			    	pteros1[i]->generate(0.4f, 0.4f, 0.4f);
			    	pteros1[i]->flyAction();
			    	collision = dino->collisionCheck(pteros1[i]->getCenterPositiveDistanceX(),
		                                             pteros1[i]->getCenterPositiveDistanceY(),
		                                             pteros1[i]->getCenterPositiveDistanceZ(),
		                                             pteros1[i]->getCenterNegativeDistanceX(),
		                                             pteros1[i]->getCenterNegativeDistanceY(),
		                                             pteros1[i]->getCenterNegativeDistanceZ(), 
		                                             pteros1[i]->getCoordinateX(), 
		                                             pteros1[i]->getCoordinateY(), 
		                                             pteros1[i]->getCoordinateZ());

			    	pteros2[i]->generate(0.4f, 0.4f, 0.4f);
			    	pteros2[i]->flyAction();
			    	collision = dino->collisionCheck(pteros2[i]->getCenterPositiveDistanceX(),
		                                             pteros2[i]->getCenterPositiveDistanceY(),
		                                             pteros2[i]->getCenterPositiveDistanceZ(),
		                                             pteros2[i]->getCenterNegativeDistanceX(),
		                                             pteros2[i]->getCenterNegativeDistanceY(),
		                                             pteros2[i]->getCenterNegativeDistanceZ(), 
		                                             pteros2[i]->getCoordinateX(), 
		                                             pteros2[i]->getCoordinateY(), 
		                                             pteros2[i]->getCoordinateZ());

			    	pteros3[i]->generate(0.4f, 0.4f, 0.4f);
			    	pteros3[i]->flyAction();
			    	collision = dino->collisionCheck(pteros3[i]->getCenterPositiveDistanceX(),
		                                             pteros3[i]->getCenterPositiveDistanceY(),
		                                             pteros3[i]->getCenterPositiveDistanceZ(),
		                                             pteros3[i]->getCenterNegativeDistanceX(),
		                                             pteros3[i]->getCenterNegativeDistanceY(),
		                                             pteros3[i]->getCenterNegativeDistanceZ(), 
		                                             pteros3[i]->getCoordinateX(), 
		                                             pteros3[i]->getCoordinateY(), 
		                                             pteros3[i]->getCoordinateZ());
			    }
			}

			for(unsigned short int i = 0; i < cactiSceneOnPathSize; i++){

				if(cactiSceneOnPath1[i]->getCoordinateZ() < -1.5)
					cactiSceneOnPath1[i]->generate(0.0f, 0.4f, 0.0f);
				
				if(cactiSceneOnPath2[i]->getCoordinateZ() < -1.5)
					cactiSceneOnPath2[i]->generate(0.0f, 0.4f, 0.0f);
				
				if(cactiSceneOnPath3[i]->getCoordinateZ() < -1.5)
				 	cactiSceneOnPath3[i]->generate(0.0f, 0.4f, 0.0f);
			}
		}
		
		// Aqui é gerado o dinossauro e definido suas ações:
		dino->generate(0.4f, 0.4f, 0.4f);
	    dino->runAction();
	    dino->jumpAction(&jump, &descend);

	    // Apenas os cactos gerados exatamente no caminho terâo a colisão testada e serão exibidos na frente
	    // do personagem
	    for(unsigned short int i = 0; i < cactiSceneOnPathSize; i++){
	    	if(cactiSceneOnPath1[i]->getCoordinateZ() > -1.5){
				cactiSceneOnPath1[i]->generate(0.0f, 0.4f, 0.0f);
				collision = dino->collisionCheck(cactiSceneOnPath1[i]->getCenterPositiveDistanceX(),
				                                 cactiSceneOnPath1[i]->getCenterPositiveDistanceY(),
				                                 cactiSceneOnPath1[i]->getCenterPositiveDistanceZ(),
				                                 cactiSceneOnPath1[i]->getCenterNegativeDistanceX(),
				                                 cactiSceneOnPath1[i]->getCenterNegativeDistanceY(),
				                                 cactiSceneOnPath1[i]->getCenterNegativeDistanceZ(), 
				                                 cactiSceneOnPath1[i]->getCoordinateX(), 
				                                 cactiSceneOnPath1[i]->getCoordinateY(), 
				                                 cactiSceneOnPath1[i]->getCoordinateZ());
			}
			
			if(cactiSceneOnPath2[i]->getCoordinateZ() > -1.5){
				cactiSceneOnPath2[i]->generate(0.0f, 0.4f, 0.0f);
				collision = dino->collisionCheck(cactiSceneOnPath2[i]->getCenterPositiveDistanceX(),
				                                 cactiSceneOnPath2[i]->getCenterPositiveDistanceY(),
				                                 cactiSceneOnPath2[i]->getCenterPositiveDistanceZ(),
				                                 cactiSceneOnPath2[i]->getCenterNegativeDistanceX(),
				                                 cactiSceneOnPath2[i]->getCenterNegativeDistanceY(),
				                                 cactiSceneOnPath2[i]->getCenterNegativeDistanceZ(), 
				                                 cactiSceneOnPath2[i]->getCoordinateX(), 
				                                 cactiSceneOnPath2[i]->getCoordinateY(), 
				                                 cactiSceneOnPath2[i]->getCoordinateZ());
			}
			
			if(cactiSceneOnPath3[i]->getCoordinateZ() > -1.5){
			 	cactiSceneOnPath3[i]->generate(0.0f, 0.4f, 0.0f);
			 	collision = dino->collisionCheck(cactiSceneOnPath3[i]->getCenterPositiveDistanceX(),
				                                 cactiSceneOnPath3[i]->getCenterPositiveDistanceY(),
				                                 cactiSceneOnPath3[i]->getCenterPositiveDistanceZ(),
				                                 cactiSceneOnPath3[i]->getCenterNegativeDistanceX(),
				                                 cactiSceneOnPath3[i]->getCenterNegativeDistanceY(),
				                                 cactiSceneOnPath3[i]->getCenterNegativeDistanceZ(), 
				                                 cactiSceneOnPath3[i]->getCoordinateX(), 
				                                 cactiSceneOnPath3[i]->getCoordinateY(), 
				                                 cactiSceneOnPath3[i]->getCoordinateZ());
			}
	    }

		// Aqui ocorre a geração dos objetos à frente do dinossauro em relação à câmera (no modo câmera padrão)
		for(unsigned short int i = 0; i < cactiSceneForwardSize; i++){

			cactiSceneForward1[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneForward2[i]->generate(0.0f, 0.4f, 0.0f);
			cactiSceneForward3[i]->generate(0.0f, 0.4f, 0.0f);
		}
		
		if(collision || restart){
			gameOver();
		}

		unsigned int s = dino->getCoordinateX() * 1.5;
		//Carregar o label "SCORE" e o score que cresce com a coodernada X e
		//Conversão para string
		string score = to_string(s);

		// Carrega a projeção ortogonal
		setOrtho(widthCurrent, heightCurrent);
			// Chama a função que desenha o score
			drawScore(score, widthCurrent);
		// Voltando para a perspectiva anterior
		restoreView();
	    
	    // Troca os buffers
	    glutSwapBuffers();
		
	}
}

int main (int argumentsC, char **argumentsV){
	// Inicialização do Glut (definitions.h)
	enableLight();
	initializations(argumentsC, argumentsV);
	//enables();

	// Passa a display tanto para glutDisplayFunc quanto para
	// a glutIdleFunc, pois, pelas minhas pesquisas em tutoriais,
	// forçar essa chamada tanto em Display quanto em Idle gera uma
	// pequena melhora de performance no desempenho da aplicação com glut
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);

    // Chama a função do header controls.h que define as teclas que
    // serão usadas e o que farão ao serem pressionadas
    controlsDefinitions();

    // Inicia o loop de renderização
    glutMainLoop ();

    return 0;
}