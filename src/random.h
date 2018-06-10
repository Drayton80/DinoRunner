#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "object.h"
#include "cactus.h"

using namespace std;

#ifndef RANDOM_H
#define RANDOM_H

/// HEADER RANDOM ///==================================================================================================///
/*  Descrição: 
 *    Possuí todas as implementações relativas a funções de geração aleatória mais complexa, como
 *    uma geração controlada de cactos para que hajam obstáculos e que obrigatoriamente seja possível
 *    atravessá-los (pois se não houvesse como, o jogo estaria funcionando erroneamente)
 */
///==============///==================================================================================================///

/* Método Random Cacti on Path:  
 *   Descrição: 
 *     Gera cactos aleatoriamente no caminho especificado por onde o personagem passa, de forma a que
 *     os cactos nunca gerem repetidamente mais que três e, mesmo nesse caso, há uma porcentagem controlada
 *     para isso ocorrer, além de que, quando isso ocorre, há um espaço entre os 3 cactos e os próximos
 *     gerados.
 *
 *   Parâmetros de Entrada:
 *     int *positionArray:
 *       É necessário passar um positionArray que representa a posição atual do array que deseja ser alterada com
 *       um valor aleatório;
 *     float CharacterPositionZ:
 *       Define a posição em Z a qual estará o personagem para saber por onde ele estará se movientando
 *     Object **cactiSceneOnPath:
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
 *     int *randomFillBar:
 *       É uma espécie de barra que é preenchida com um valor aleatório (dentro de intervalo especificado)
 *       e vai se incrementado até atingir um limite, quando esse limite é atingido algo ocorre, enquanto
 *       não for outra coisa ocorre (como uma barra de energia)
 *    int randomFillBarLimit:
 *       É o limite definido para que, quando randomFillBar o atingir, algo aconteça, caso não, outra
 *       coisa ocorre.
 *
 *   Saídas:
 *     return 0:
 *       Retorna esse valor quando houve algum erro ao longo do código;
 *     return 1:
 *       Retorna esse valor quando tudo ocorreu corretamente;
 */
int randomCactiOnPath(int *positionArray, float CharacterPositionZ, Cactus **cactiSceneOnPath, 
                      int cactiSceneOnPathArraySize, int begin, int *randomFillBar,
                      int randomFillBarLimit){

    // Variável que define se haverá uma sequência de três cactos ou não
    bool randomThreeCacti;

    // Se positionArray ultrapassar o limite do array é retornado um valor que denota esse problema.
    if(cactiSceneOnPathArraySize < *positionArray){
        return 0;
    }

    // Assim como apenas preenche o array do cactiSceneOnPath enquanto i for menor que  o
    // tamanho do próprio array (senão haveria corrupção de memória). Aqui é usado o positionArray
    // em vez do i pois há uma alteração diferente nele dependendo se há uma sequência de
    // cactos ou não
    if(*positionArray < cactiSceneOnPathArraySize){
        // Inicia preenchendo de 0
        *randomFillBar += rand() % 2 + 1;

        // Se *randomFillBar ultrapassar seu limite, um cacto é gerado no caminho na
        // posição relativa ao i atual, senão ele é gerado em qualquer outro canto da cena.
        if(randomFillBarLimit <= *randomFillBar){
            // Define a coordenada X como o valor de positionArray (referenta a posição dentro do limite
            // de begin até begin+positionArray)
            cactiSceneOnPath[*positionArray]->setCoordinateX(*positionArray+begin);
            // Caso entre nesse if o cacto será gerado no posição Z por onde o personagem passa (constante),
            // ou seja, na linha de movimento do personagem
            cactiSceneOnPath[*positionArray]->setCoordinateZ(CharacterPositionZ);

            // rand()%4 é um random entre 0 e 3, ou seja, há 25% de cair em 1 e, por conseguinte,
            // randomThreeCacti ser true
            randomThreeCacti = (rand()%4 == 1);

            // Apenas entre nesse if se randomThreeCacti for true e se a soma de positionArray não
            // for ultrapassar os limites do array (sem a segunda opção poderia ocorrer
            // corrupção de memória)
            if(randomThreeCacti && ((*positionArray+2) < cactiSceneOnPathArraySize)){
                // Daí temos que há 25% de serem gerados três cactos em sequência
                cactiSceneOnPath[*positionArray+1]->setCoordinateX(*positionArray+begin+1);
                cactiSceneOnPath[*positionArray+1]->setCoordinateZ(CharacterPositionZ);

                cactiSceneOnPath[*positionArray+2]->setCoordinateX(*positionArray+begin+2);
                cactiSceneOnPath[*positionArray+2]->setCoordinateZ(CharacterPositionZ);

                *positionArray += 2;
            }

            // A contagem do fillBar volta para 0 novamente para outra rodada de aleatoriedade
            // assim é garantido que os cactos não fiquem se aglomerem em sequência (o que faria 
            // com que não fosse possível atravessá-los com um pulo)
            *randomFillBar = 0;

        }else{
            // Caso não caia na condição de gerar os cactos no meio do caminho, eles são
            // gerados na exata posição em X, mas com um Z aleatório no resto da cena
            cactiSceneOnPath[*positionArray]->setCoordinateX(*positionArray+begin);
            rand()%2 == 1 ? cactiSceneOnPath[*positionArray]->setCoordinateZ(-(rand()%40 + 5)):
                            cactiSceneOnPath[*positionArray]->setCoordinateZ( (rand()%40 + 5));
        }

        // OBS.: é usado um positionArray auxiliar dentro de um for de i também pois mesmo que não seja 
        //       feita qualquer sequência de cactos ainda sim será preservado o numero de positionArray
        //       necessário para preencher todo array, ou seja, sempre será
        //       positionArray <= tamanhoDoArrayOnPath
    }

    return 1;
}

int randomPterodactylusOnPath(int positionArray, float CharacterPositionZ, Pterodactylus **pterosOnPath, 
                              int pterosOnPathSize, int begin, int *randomFillBar,
                              int randomFillBarLimit){

    // Se positionArray ultrapassar o limite do array é retornado um valor que denota esse problema.
    if(pterosOnPathSize < positionArray){
        return 0;
    }

    // Assim como apenas preenche o array do pterosOnPath enquanto i for menor que  o
    // tamanho do próprio array (senão haveria corrupção de memória). Aqui é usado o positionArray
    // em vez do i pois há uma alteração diferente nele dependendo se há uma sequência de
    // cactos ou não
    if(positionArray < pterosOnPathSize){
        // Inicia preenchendo de 0
        *randomFillBar += rand() % 2 + 1;

        // Se *randomFillBar ultrapassar seu limite, um cacto é gerado no caminho na
        // posição relativa ao i atual, senão ele é gerado em qualquer outro canto da cena.
        if(randomFillBarLimit <= *randomFillBar){
            // Define a coordenada X como o valor de positionArray (referenta a posição dentro do limite
            // de begin até begin+positionArray)
            // OBS.: O rand()%151 serve para espalhar os 50 pteros ao longo de 150 blocos
            pterosOnPath[positionArray]->setCoordinateX(positionArray + begin + 60 + rand()%50);
            // Há 33% de chance do pterodáctilo vir voando mais alto do que o normal
            // O 1 + (rand()%11 + 1)/10.0f garante que essa variação será de 1.0 até 2.0, pois o
            // rand()%11 + 1 faz com que o intervalo seja de 1 até 10 e a divisão por 10.0f
            // normaliza num float que vai de 0.0 até 1.0
            rand()%3 == 1 ? 
            pterosOnPath[positionArray]->setCoordinateY(1.0 + (rand()%11 + 1)/10.0f):
            pterosOnPath[positionArray]->setCoordinateY(1.0);
            // Caso entre nesse if o cacto será gerado no posição Z por onde o personagem passa (constante),
            // ou seja, na linha de movimento do personagem
            pterosOnPath[positionArray]->setCoordinateZ(CharacterPositionZ);

            // A contagem do fillBar volta para 0 novamente para outra rodada de aleatoriedade
            // assim é garantido que os cactos não fiquem se aglomerem em sequência (o que faria 
            // com que não fosse possível atravessá-los com um pulo)
            *randomFillBar = 0;

        }else{
            // Caso não caia na condição de gerar os cactos no meio do caminho, eles são
            // gerados na exata posição em X, mas com um Z aleatório no resto da cena
            pterosOnPath[positionArray]->setCoordinateX(positionArray + begin + 60 + rand()%50);
            // Aqui a variação da altura fica entre 1 e 5 para dar mais variação na altitude
            // já que ele não está voando no caminho do dino, não há necessidade de colocar
            // uma altura que ele alcance
            pterosOnPath[positionArray]->setCoordinateY(1.0 + (rand()%40 + 1)/10.0f);
            // Faz um rand de 0 ou 1 para ter 50% de chance dos pterodáctilos cairem na frente
            // ou atrás da cena 
            rand()%2 == 1 ? pterosOnPath[positionArray]->setCoordinateZ(-(rand()%35 + 5)):
                            pterosOnPath[positionArray]->setCoordinateZ( (rand()%35 + 5));
        }

        //|std::cout << "Posição X do Ptero: " << pterosOnPath[positionArray]->getCoordinateX() << " | ";
        //|std::cout << "Posição Y do Ptero: " << pterosOnPath[positionArray]->getCoordinateY() << " | ";
        //|std::cout << "Posição Z do Ptero: " << pterosOnPath[positionArray]->getCoordinateZ() << " \n";

        // OBS.: é usado um positionArray auxiliar dentro de um for de i também pois mesmo que não seja 
        //       feita qualquer sequência de cactos ainda sim será preservado o numero de positionArray
        //       necessário para preencher todo array, ou seja, sempre será
        //       positionArray <= tamanhoDoArrayOnPath
    }

    return 1;
}

///==============///==================================================================================================///

#endif  // RANDOM_H