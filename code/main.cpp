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
				// Sim, cacto no plural em inglês é cacti

Cactus *cactiSceneBehind1[100];
Cactus *cactiSceneBehind2[100];
Cactus *cactiSceneBehind3[100];

Cactus *cactiSceneOnPath1[150];
Cactus *cactiSceneOnPath2[150];
Cactus *cactiSceneOnPath3[150];

Cactus *cactiSceneForward1[5];
Cactus *cactiSceneForward2[5];
Cactus *cactiSceneForward3[5];

unsigned short int cactiSceneBehindSize  = sizeof(cactiSceneBehind1) /sizeof(cactiSceneBehind1[0] );
unsigned short int cactiSceneOnPathSize  = sizeof(cactiSceneOnPath1) /sizeof(cactiSceneOnPath1[0] );
unsigned short int cactiSceneForwardSize = sizeof(cactiSceneForward1)/sizeof(cactiSceneForward1[0]);
unsigned short int cactiSceneBiggerSize; 
unsigned short int cactiLimit = 150;
unsigned int cactiGenerateEnd = 150;
short int cactiGenerateSwitch = 0;
float randomX;

unsigned short int randomFillBar = 0;
unsigned short int randomFillBarLimit = 20;
bool randomThreeCacti;

/* Método Random Cacti on Path:  
 *   Descrição: 
 *     Gera cactos aleatoriamente no caminho especificado por onde o personagem passa, de forma a que
 *     os cactos nunca gerem repetidamente mais que três e, mesmo nesse caso, há uma porcentagem controlada
 *     para isso ocorrer, além de que, quando isso ocorre, há um espaço entre os 3 cactos e os próximos
 *     gerados.
 *
 *   Parâmetros de Entrada:
 *     unsigned short int *j:
 *       É necessário passar um j que representa a posição atual do array que deseja ser alterada com
 *       um valor aleatório;
 *     Cactus **cactiSceneOnPath:
 *       É passado também um array de cactos para que seja gerado a alteração em determinado elemento
 *       desse array (OBS.: O valor é um ponteiro para um ponteiro porque uma classe em C++ é definida
 *       como um ponteiro e era necessário um ponteiro que apontasse para a classe para poder fazer
 *       alterações nela, já que o valor das coordenadas de determinado objeto são alteradas aleato-
 *       riamente dentro daqui);
 *     int cactiSceneOnPathArraySize:
 *       É preciso especificar o tamanho do array que está sendo passado para garantir que seus limites
 *       não sejam ultrapassados e, caso isso ocorra, haja uma notificação sem corrupção de memória
 *       (OBS.: eu tentei fazer isso usando sizeof no cactiSceneOnPath, mas os valores retornados por
 *       ele eram sempre relativo ao ponteiro e não ao array em si).
 *     int beginX:
 *       É preciso saber onde iniciará a geração dos blocos (o fim é 150 blocos depois do inicio).
 *
 *   Saídas:
 *     return 0:
 *       Retorna esse valor quando houve algum erro ao longo do código;
 *     return 1:
 *       Retorna esse valor quando tudo ocorreu corretamente;
 */
int randomCactiOnPath(unsigned short int *j, Cactus **cactiSceneOnPath, 
	                  int cactiSceneOnPathArraySize, int begin){

	// Se j ultrapassar o limite do array é retornado um valor que denota esse problema.
	if(cactiSceneOnPathArraySize < *j){
		return 0;
	}

	// Assim como apenas preenche o array do cactiSceneOnPath enquanto i for menor que  o
	// tamanho do próprio array (senão haveria corrupção de memória). Aqui é usado o j
	// em vez do i pois há uma alteração diferente nele dependendo se há uma sequência de
	// cactos ou não
	if(*j < cactiSceneOnPathArraySize){
		// Inicia preenchendo de 0
		randomFillBar += rand() % 2 + 1;

		// Se randomFillBar ultrapassar seu limite, um cacto é gerado no caminho na
		// posição relativa ao i atual, senão ele é gerado em qualquer outro canto da cena.
		if(randomFillBarLimit <= randomFillBar){
			// As posições iniciais (0, 1, 2 e 3) nunca devem ser ocupadas por cactus para 
			// garantir que o personagem não nassa em cima de um e que possua espaço para 
			// correr de inicio.
			cactiSceneOnPath[*j] = new Cactus(*j+3+begin, 0.0, dino->getCoordinateZ());

			// Apenas entre nesse if se randomThreeCacti for true e se a soma de j não
			// for ultrapassar os limites do array (sem a segunda opção poderia ocorrer
			// corrupção de memória)
			if(randomThreeCacti && ((*j+2) < cactiSceneOnPathArraySize)){
				// Daí temos que há 25% de serem gerados
				cactiSceneOnPath[*j+1] = new Cactus(*j+3+1+begin, 0.0, dino->getCoordinateZ());
				cactiSceneOnPath[*j+2] = new Cactus(*j+3+2+begin, 0.0, dino->getCoordinateZ());

				*j += 2;
			}

			// A contagem do fillBar volta para 0 novamente para outra rodada de aleatoriedade
			// assim é garantido que os cactos não fiquem se aglomerem em sequência (o que faria 
			// com que não fosse possível atravessá-los com um pulo)
			randomFillBar = 0;

		}else{
			// Caso não caia na condição de gerar os cactos no meio do caminho, eles são
			// gerados na exata posição em X, mas com um Z aleatório no resto da cena
			cactiSceneOnPath[*j] = new Cactus(*j+3+begin, 0.0, -(rand()%70 + 2));
		}

		// OBS.: é usado um j auxiliar dentro de um for de i também pois mesmo que não seja 
		// 		 feita qualquer sequência de cactos ainda sim será preservado o numero de j
		//       necessário para preencher todo array, ou seja, sempre será
		//       j <= tamanhoDoArrayOnPath
	}

	return 1;
}

void objectsInitialPositions(){
	dino->setCoordinateZ(-1.0);
	dino->setCoordinateX(-1.7);

	// Aqui é verificado qual o maior de todos os sizes, comparando primeiramente behind com onPath
	// e depois forward com o resultado anterior
	cactiSceneBehindSize > cactiSceneOnPathSize  ? cactiSceneBiggerSize = cactiSceneBehindSize
	                                             : cactiSceneBiggerSize = cactiSceneOnPathSize;
    cactiSceneBiggerSize > cactiSceneForwardSize ? cactiSceneBiggerSize = cactiSceneBiggerSize
	                                             : cactiSceneBiggerSize = cactiSceneForwardSize;

	for(unsigned short int i = 0, j = 0, k = 0, l = 0; i < cactiSceneBiggerSize; i++, j++, k++, l++){
		randomX = rand()%cactiGenerateEnd + 1;
		// rand()%4 é um random entre 0 e 3, ou seja, há 25% de cair em 1 e, por conseguinte,
		// randomThreeCacti ser true
		randomThreeCacti = (rand()%4 == 1);

		// Apenas só preenche cactiSceneBehind quando i for menor que o tamanho do array
		if(i < cactiSceneBehindSize){
			cactiSceneBehind1[i] = new Cactus(randomX-cactiLimit, 0.0, -(rand()%70 + 2));
			cactiSceneBehind2[i] = new Cactus(randomX           , 0.0, -(rand()%70 + 2));
			cactiSceneBehind3[i] = new Cactus(randomX+cactiLimit, 0.0, -(rand()%70 + 2));
		}

		int test1 = randomCactiOnPath(&j, cactiSceneOnPath1, cactiSceneOnPathSize,-153);
		int test2 = randomCactiOnPath(&k, cactiSceneOnPath2, cactiSceneOnPathSize,   0);
		int test3 = randomCactiOnPath(&l, cactiSceneOnPath3, cactiSceneOnPathSize, 150);
		std::cout << "teste 1: " << test1 << "  n" << j <<  "\n";
		std::cout << "teste 2: " << test2 << "  n" << k <<  "\n";
		std::cout << "teste 3: " << test3 << "  n" << l <<  "\n";

		if(i < cactiSceneForwardSize){

			cactiSceneForward1[i] = new Cactus(randomX-cactiLimit, 0.0, -(rand()%1));
			cactiSceneForward2[i] = new Cactus(randomX           , 0.0, -(rand()%1));
			cactiSceneForward3[i] = new Cactus(randomX+cactiLimit, 0.0, -(rand()%1));
		}
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
				for(unsigned short int i = 0, j = 0; i < cactiSceneBiggerSize; i++, j++){
					randomX = rand()%cactiLimit + 1 + cactiGenerateEnd;
					// rand()%4 é um random entre 0 e 3, ou seja, há 25% de cair em 1 e, por conseguinte,
					// randomThreeCacti ser true
					randomThreeCacti = (rand()%4 == 1);

					if(i < cactiSceneBehindSize){

						cactiSceneBehind1[i]  = new Cactus(randomX, 0.0, -(rand()%70 + 2));
					}

					if(j < cactiSceneOnPathSize){
						// Após os primeiros 100 blocos, randomFillBarLimit é decrementado à cada
						// 300 blocos até no mínimo de 10, isso garante que o número de cactos
						// no caminho aumento cada vez mais até um limite (necessário para que não
						// haja só cactos no caminho em determinado ponto)
						if(10 < randomFillBarLimit) randomFillBarLimit--;

						// A alternação entre 3 cactiSceneOnPath é feita aqui novamente para 
						// gerar aleatoriamente sem que o jogador perceba e para que não sejam
						// usados infinitos arrays, já que o jogador pode correr infinitamente
						// se possuir uma maestria e habilidade digna de um jedi
						int test = randomCactiOnPath(&j, cactiSceneOnPath1, cactiSceneOnPathSize, cactiGenerateEnd);
						std::cout << "teste switch 0: " << test << "\n";
					}
				

					if(i < cactiSceneForwardSize){

						cactiSceneForward1[i] = new Cactus(randomX, 0.0, -(rand()%1));
					}
				}

				cactiGenerateSwitch = 1;

				break;

			case 1:
				for(unsigned short int i = 0, j = 0; i < cactiSceneBiggerSize; i++, j++){
					randomX = rand()%cactiLimit + 1 + cactiGenerateEnd;
					// rand()%4 é um random entre 0 e 3, ou seja, há 25% de cair em 1 e, por conseguinte,
					// randomThreeCacti ser true
					randomThreeCacti = (rand()%4 == 1);

					if(i < cactiSceneBehindSize){

						cactiSceneBehind2[i]  = new Cactus(randomX, 0.0, -(rand()%70 + 2));
					}

					if(j < cactiSceneOnPathSize){
						// A alternação entre 3 cactiSceneOnPath é feita aqui novamente para 
						// gerar aleatoriamente sem que o jogador perceba e para que não sejam
						// usados infinitos arrays, já que o jogador pode correr infinitamente
						// se possuir uma maestria e habilidade digna de jedi
						int test = randomCactiOnPath(&j, cactiSceneOnPath2, cactiSceneOnPathSize, cactiGenerateEnd);
						std::cout << "teste switch 1: " << test << "\n";
					}

					if(i < cactiSceneForwardSize){

						cactiSceneForward2[i] = new Cactus(randomX, 0.0, -(rand()%1));
					}
				}

				cactiGenerateSwitch = 2;

				break;

			case 2:
				for(unsigned short int i = 0, j = 0; i < cactiSceneBiggerSize; i++, j++){
					randomX = rand()%cactiLimit + 1 + cactiGenerateEnd;
					// rand()%4 é um random entre 0 e 3, ou seja, há 25% de cair em 1 e, por conseguinte,
					// randomThreeCacti ser true
					randomThreeCacti = (rand()%4 == 1);

					if(i < cactiSceneBehindSize){

						cactiSceneBehind3[i]  = new Cactus(randomX, 0.0, -(rand()%70 + 2));
					}

					if(j < cactiSceneOnPathSize){
						// A alternação entre 3 cactiSceneOnPath é feita aqui novamente para 
						// gerar aleatoriamente sem que o jogador perceba e para que não sejam
						// usados infinitos arrays, já que o jogador pode correr infinitamente
						// se possuir uma maestria e habilidade digna de jedi
						int test = randomCactiOnPath(&j, cactiSceneOnPath3, cactiSceneOnPathSize, cactiGenerateEnd);
						std::cout << "teste switch 2: " << test << "\n";
					}
					

					if(i < cactiSceneForwardSize){

						cactiSceneForward3[i] = new Cactus(randomX, 0.0, -(rand()%1));
					}
				}

				cactiGenerateSwitch = 0;

				break;
		}

	}

	for(unsigned short int i = 0; i < cactiSceneBiggerSize; i++){

		if(i < cactiSceneBehindSize){

			cactiSceneBehind1[i]->generate();
			cactiSceneBehind2[i]->generate();
			cactiSceneBehind3[i]->generate();
		}

		if(i < cactiSceneOnPathSize){

			cactiSceneOnPath1[i]->generate();
			cactiSceneOnPath2[i]->generate();
		 	cactiSceneOnPath3[i]->generate();
		}
		

		if(i < cactiSceneForwardSize){

			cactiSceneForward1[i]->generate();
			cactiSceneForward2[i]->generate();
			cactiSceneForward3[i]->generate();
		}

	}
	
	dino->generate(0.4f, 0.4f, 0.4f);

	for(unsigned short int i = 0; i < cactiSceneForwardSize; i++){

		cactiSceneForward1[i]->generate();
		cactiSceneForward2[i]->generate();
		cactiSceneForward3[i]->generate();
	}

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