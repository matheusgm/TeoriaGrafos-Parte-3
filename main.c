#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Lista.h"
#include "FilaLista.h"
#include "PilhaLista.h"
#include "FuncoesAuxiliares.h"
#include "BibliotecaTeoGraf.h"
#include <sys/time.h>

#define NUMERO_LINHA 1000


int main(){
    FILE *arq;
    double initialPathCost;
    char Linha[NUMERO_LINHA];
    char *result;
    int numVertices;
    double **MatrizVertice;
    int *vetorMarcacao;
    // Abre um arquivo TEXTO para LEITURA
    arq = fopen("points-5.txt", "rt"); // points-5
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }

    // Le a primeira linha para descobrir a quantidade de vertices totais
    result = fgets(Linha, NUMERO_LINHA-1, arq);
    result[strlen(result)-1] = '\0';
    numVertices = atoi(result); //transforma a string em inteiro
    long start,end;
    int* path = (int*) malloc((numVertices+1)*sizeof(int));
//    start = getMicrotime();
    MatrizVertice = geraMatrizAdjacenciaBYTE(arq,Linha,result,numVertices);
//    imprimeMatrizBYTE(MatrizVertice,numVertices);
//    end = getMicrotime();
//    printf("Tempo para gerar a Matriz %f\n",(float)(end-start));
//    start = getMicrotime();
//    int i;
////    for(i=0;i<100000;i++){
//        buildPath(MatrizVertice,numVertices, path, 1);
////    }
//    end = getMicrotime();
////    printf("%f\n",(float)(end-start));
//    printf("Tempo para achar o caminho %.3f us\n",((float)(end-start)));
//    initialPathCost = calculateCost(MatrizVertice,numVertices, path);
//    printf("Custo solucao inicial 1: %d\n", initialPathCost);

    //printf("Solucao inicial: ");
    //printPath(numVertices + 1, path);

    start = getMicrotime();
    int i;
    for(i=0;i<1;i++){
        tspMstAprox(MatrizVertice,numVertices,path,1);
    }
    end = getMicrotime();
    printf("Tempo para achar o caminho 2 %.3f us\n",((float)(end-start)/1));
    //printf("Solucao inicial 2: ");
//    printPath(numVertices + 1, path);

    initialPathCost = calculateCost(MatrizVertice,numVertices, path);
    printf("Custo solucao inicial 2: %.2f", initialPathCost);
    free(path);
//    srand(1);

    printf("\n");

    /*
    int* bestPath = path;
    int lowestCost = initialPathCost;
    start = getMicrotime();
    for(int i = 0; i < 100; i++) {
        int *solucao_aleatoria = malloc((numVertices + 1) * sizeof(int));
        buildRandomPath(MatrizVertice,numVertices, solucao_aleatoria,1);
        int custo_solucao_aleatoria = calculateCost(MatrizVertice,numVertices, solucao_aleatoria);
//        printf("Solucao aleatoria: ");
//        printPath(numVertices + 1, solucao_aleatoria);
//        printf("Custo solucao aleatoria: %d\n", custo_solucao_aleatoria);
//        printf("\n");
        if(custo_solucao_aleatoria < lowestCost){
            bestPath = solucao_aleatoria;
            lowestCost = custo_solucao_aleatoria;
        }

    }
    end = getMicrotime();
    printf("Tempo para achar o MELHOR caminho %.3f us\n",((float)(end-start)));
    printf("Tempo para achar o caminho aleatorio %.3f us\n",((float)(end-start))/100);

    printPath(numVertices+1,bestPath);
    printf("Custo melhor caminho: %d\n", lowestCost);
*/

    fclose(arq);
    //system("pause");

    return 1;
}

