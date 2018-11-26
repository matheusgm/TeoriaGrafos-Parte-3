#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "FilaLista.h"
#include "PilhaLista.h"
#include "Lista.h"
#include "FuncoesAuxiliares.h"
#include <stdbool.h>
#include <limits.h>
#include <float.h>
#include "Prim.h"
#define NUMERO_LINHA 1000
#define FALSE 0
#define TRUE 1

struct nodo {
    int indice;
    double valor;
};

typedef struct vertice Vertice;
struct vertice{
    int n;
    Elemento* adjancencia;
};

typedef struct ponto Ponto;
struct ponto{
    double x;
    double y;
};

typedef struct arvore Arvore;
struct arvore{
    int nivel;
    int pai;
};

typedef struct marcacao2 Marcacao2;
struct marcacao2{
    int numVertice;
    int numMarcacao;
};

typedef struct endVetorPrincipal EndVetorPrincipal;
struct endVetorPrincipal{
    int qntMarcacao;
    Marcacao2* endereco;
};

void printPath(int n, int* caminho) {
    int i;

    for(i = 0; i < n; i++) {
        printf("%d ", caminho[i]);
    }
    printf("\n");
}

double calculateCost(double** MatrizVertice, int numVertices, int* caminho) {
    double custo = 0;

    for(int i = 0; i < numVertices; i++) {
//        printf("%.2f ",MatrizVertice[caminho[i]-1][caminho[i + 1]-1]);
        custo = custo + MatrizVertice[caminho[i]-1][caminho[i + 1]-1];
    }
    printf("\n");
    return custo;
}

// ----------------------------------------------------- INICIO DE GERAR GRAFOS -----------------------------------------------------

double** geraMatrizAdjacenciaBYTE(FILE *arq,char *Linha,char *result, int numVertices){
    double **MatrizVertice;
    int i,j,ultimoCaracter;
    double pri,seg;

    MatrizVertice = (double**) malloc(numVertices*sizeof(double*));
    for(i = 0; i < numVertices; i++){
//        printf("%d\n",i);
        MatrizVertice[i] = (double*) malloc(numVertices*sizeof(double));
//        for(j = 0; j < numVertices; j++){
//            MatrizVertice[i][j] = '0';
//        }
    }
    i = 0;
    Ponto* pontos = (Ponto*) malloc(numVertices*sizeof(Ponto));
    while (!feof(arq))
    {
        // L� uma linha (inclusive com o '\n')
        result = fgets(Linha, NUMERO_LINHA-1, arq);  // o 'fgets' l� at� NUMERO_LINHA caracteres ou at� o '\n'

        ultimoCaracter = strlen(result)-1;
        // Verificar e tirar o \n caso exista
        if(result[ultimoCaracter] == '\n'){
            result[ultimoCaracter] = '\0';
        }
        // Transforma os numeros em inteiros
        pri = atof(strtok(result," "));
        seg = atof(strtok('\0'," "));

//        printf("%f %f\n",pri,seg);
        pontos[i].x = pri;
        pontos[i].y = seg;

        i++;
//        if(i == 50)
//         break;
    }

    for(i=0; i<numVertices-1;i++){
        for(j=i;j<numVertices;j++){
            if(j==i){
                MatrizVertice[i][j] = 0.0;
            }else{
                MatrizVertice[i][j] = sqrt(pow(abs(pontos[i].x-pontos[j].x),2)+pow(abs(pontos[i].y - pontos[j].y),2));
                MatrizVertice[j][i] = MatrizVertice[i][j];
            }
        }
    }
    free(pontos);
    return MatrizVertice;
}
/*
 A fim de obter o menor caminho para o caixeiro viajante, constatamos o seguinte fato: a remo��o de uma aresta do caminho faz com que o grafo se torne uma arvore geradora, isto � conecta todos os pontos, possui exatamente V-1 arestas (onde V eh o numero de vertices do grafo) e nao possui ciclos. Essa arvore, portanto, possui custo maior do que aquela gerada pelos algoritmos de Prim ou Kruskal. A heur�stica usada nesse algoritmo eh usar o maior numero poss�vel das arestas que constituem a MST a fim de obter o menor caminho poss�vel. Para tanto, efetuaremos uma DFS encima da arvore geradora obtida usando o algoritmo de Prim.
*/
void tspMstAprox(double** MatrizVertice,int numVertice, int* caminho, int inicio)
{
 int v,i,k;/*v eh o proximo vertice da pilha, i eh um iterador para percorrer a lista de pais da MST e K eh um outro iterador para o caminho encontrado*/
 prim *MST;
 MST = Prim(MatrizVertice, numVertice, inicio);/*Obtem o vetor de pais da MST. Usaremos esse dado para construir um ciclo Hamiltoniano, isto eh uma possivel solucao para o caixeiro viajante*/
 //printMST(MST, numVertice);
 Pilha *P = pilha_cria();
 pilha_push(P,inicio);//coloca o vertice de inicio na pilha
 k=0;//Ainda nao colocamos nenhum vertice no caminho, portanto comecamos com k = 0
 while(!pilha_vazia(P))
 {
  v = pilha_pop(P);//sempre que retirarmos um vertice da pilha, colocaremos o mesmo no caminho do caixeiro
  caminho[k] = v;
  k++;
  for(i = 0;i < numVertice;i++)
  {
   if(MST->parent[i] == v)/*itera por toda a lista de pais gerada pela MST e adciona na pilha os vertices cujo pai eh "v".
    � como se estivessemos realizando uma DFS na arvore geradora de menor custo obtida pelo algoritmo de Prim.
    Sempre que chegamos a um "n� folha", ao inves de subir na arvore (o problema nao permite que visitemos um
   vertice mais de uma vez), pulamos para o proximo vertice ainda nao explorado que se encontra no topo da pilha.
   Em seguida descemos na �rvore geradora partindo desse ponto. O algoritmo se repete at� que a pilha esteja vazia.
   Isso garante que todos os vertices ser�o colocados no  caminho. Note que, ao contrario da DFS padrao,
   nao precisamos manter um vetor de marcacao, j� que na arvore geradora cada vertice so pode ser alcancado por meio
   de um outro unico vertice.*/

   pilha_push(P,i+1);//Coloca os vizinhos daquele vertice na MST na pilha, isto �, os vertices que foram descobertos por v na MST.
  }
 }
 caminho[k]=inicio;
}

void buildPath(double** MatrizVertice,int numVertice, int* caminho, int inicio) {
    int *inseridos = malloc(numVertice * sizeof(int));

    for(int i = 0; i < numVertice; i++) {
        inseridos[i] = FALSE;//mantem um vetor com os vertices ja visitados
    }

    caminho[0] = inicio;
    inseridos[inicio-1] = TRUE;//Coloca o vertice de inicio pra comecar a sequencia de vertices que compoe o caminho e marca ele como visitado

    for(int i = 0; i < numVertice; i++) {
        double valor_referencia = DBL_MAX;/*Inicialmente todos os vertices sao tidos como estando no infinito. Note que precisamos visitar todas as cidades, da� o loop tem de ser a quantidade de vertices existentes do grafo*/
        int vizinho_selecionado = 0;

        for(int j = 0; j < numVertice; j++) {
            if(!inseridos[j] && valor_referencia > MatrizVertice[i][j]) {
                vizinho_selecionado = j;
                valor_referencia = MatrizVertice[i][j];/*Cada vez que visitamos um vertice, vemos qual eh o vizinho mais proximo dele. Trata-se portanto de um algoritmo guloso para o problema do caixeiro viajante onde a gula est� em pegar cidades mais pr�ximas*/
            }
        }

        caminho[i + 1] = vizinho_selecionado+1;
        inseridos[vizinho_selecionado] = TRUE;
    }

    caminho[numVertice] = inicio;
    free(inseridos);
}

void buildRandomPath(double** MatrizVertice,int numVertice, int* caminho, int inicio) {
    int *inseridos = malloc(numVertice * sizeof(int));
    struct nodo *vizinhos = malloc(numVertice * sizeof(struct nodo));/*Nessa abordagem selecionaremos um caminho aleat�rio. Para tal manteremos uma lista com os vizinhos que ainda nao foram incluidos e sempre que visitarmos uma cidade selecionaremos aleatoriamente um vertice de saida a partir dessa lista*/

    for(int i = 0; i < numVertice; i++) {
        inseridos[i] = FALSE;
    }

    caminho[0] = inicio;
    inseridos[inicio-1] = TRUE;

    for(int i = 0; i < numVertice; i++) {
        int iv = 0;//iv eh a quantidade de cidades que faltam ser visitadas

        for(int j = 0; j < numVertice; j++) {
            if(!inseridos[j]) {//verifica se cada uma das cidades j� foi visitada
                vizinhos[iv].indice = j;/*em caso negativo adciona a cidade na lista de nodos que faltam, isto �, o numero do vertice que a representa no grafo*/
                vizinhos[iv].valor = MatrizVertice[i][j];//Adciona, tambem, a distancia para aquele vertice caso ele seja escolhido
                iv++;//a todo momento, iv guarda quantas cidades faltam para serem visitadas
            }
        }

        if(iv == 0) {
            caminho[i + 1] = inicio;//caso em que nao existem mais cidades a serem visitadas, adciona o vertice de inicio no final do caminho
        } else {
            int vizinho_selecionado = rand() % iv;/*seleciona randomicamente um dos vertices da lista. Como a lista tem tamanho iv, ent�o o numero randomico tem de estar na faixa de 0-iv*/
            caminho[i + 1] = vizinhos[vizinho_selecionado].indice+1;
            inseridos[vizinhos[vizinho_selecionado].indice] = TRUE;
        }
    }

    free(inseridos);
    free(vizinhos);
}

Vertice* geraListaAdjacencia(FILE *arq,char *Linha,char *result, int numVertices, int *numArestas, int *vetorGraus, bool *isUnweighted,bool *gotNegativeEdges){

    int i,pri,seg,ultimoCaracter;
    float peso;
    Vertice *vetorVertice;
    Elemento *lst;
    char* cha;
    vetorVertice = (Vertice*) malloc(numVertices * sizeof(Vertice));
    for(i = 0; i < numVertices; i++){
        vetorVertice[i].n = i+1;
        vetorVertice[i].adjancencia = lst_cria();
        vetorGraus[i] = 0;
    }

    if(numVertices > 10500000){
        cha = "	";
    }else{
        cha = " ";
    }

    printf("Vertices: %d\n",numVertices);
    i = 0;
    int r=0;
    *isUnweighted = true;
    *gotNegativeEdges = false;
    while (!feof(arq))
    {
        // L� uma linha (inclusive com o '\n')
        result = fgets(Linha, NUMERO_LINHA-1, arq);  // o 'fgets' l� at� NUMERO_LINHA caracteres ou at� o '\n'
        ultimoCaracter = strlen(result)-1;
        // Verificar e tirar o \n caso exista
        if(result[ultimoCaracter] == '\n'){
            result[ultimoCaracter] = '\0';
        }

        pri = atoi(strtok(result,cha));
        seg = atoi(strtok('\0',cha));
        peso = strtof(strtok('\0',cha),NULL);
        if(peso != 0)
         *isUnweighted = false;
        if(peso < 0)
         *gotNegativeEdges = true;
        //printf("%d %d %.2f\n",pri,seg,peso);
        if(pri > numVertices || seg > numVertices){
//            printf("Erro! %s com numero incorreto\n", result);
            r++;
            continue;
        }

        // Insere o segundo vertice na lista adjacencia do primeiro vertice
        lst = vetorVertice[pri-1].adjancencia;
        lst = lst_insere(lst,seg,peso);
        //lst_imprime(lst);
        vetorVertice[pri-1].adjancencia = lst;

        // Insere o primeiro vertice na lista adjacencia do segundo vertice
        lst = vetorVertice[seg-1].adjancencia;
        lst = lst_insere(lst,pri,peso);
        vetorVertice[seg-1].adjancencia = lst;

        vetorGraus[pri-1]++;
        vetorGraus[seg-1]++;

        i++;
        (*numArestas)++;
        if(i == 10000001)
         break;
    }
//    printf("Chegou Aqui\n");
    printf("Linhas erradas: %d\n",r);
    return vetorVertice;
}

// ----------------------------------------------------- FIM DE GERAR GRAFOS -----------------------------------------------------

// ----------------------------------------------------- INICIO DE IMPRIMIR GRAFOS -----------------------------------------------------
// Imprime o Grafo na forma de Lista de Adjacencia
void imprimeListaAdjacencia(Vertice *vetorVertice, int numVertices){
    int i;
    for(i = 0; i < numVertices; i++){
        printf("Vertice %d: ",i+1);
        lst_imprime(vetorVertice[i].adjancencia);
        printf("\n");
    }
}

void imprimeMatrizBYTE(double** MatrizVertice, int numVertices)
{
 int i,j;
 for(i = 0; i < numVertices; i++){
  for(j = 0; j < numVertices; j++){
    printf("|%.2f|",MatrizVertice[i][j]);
//    if(MatrizVertice[i][j]==0){
//        printf("%d - %d\n",i+1,j+1);
//    }
  }
  printf("\n");
 }

}
// ----------------------------------------------------- FIM DE IMPRIMIR GRAFOS -----------------------------------------------------

// ----------------------------------------------------- INICIO ARQUIVOS DE SAIDA -----------------------------------------------------
// Funcao para gerar o arquivo de Arvore
void gerarArquivoArvore(Arvore *arvore, int tam){
    FILE *arqSaida;
    int i;
    if((arqSaida = fopen("Arvore.txt", "wb")) == NULL)
    {
        printf("Erro na abertura do arquivo");
        exit(1);
    }
    for(i=0;i<tam;i++){
        if(arvore[i].pai == 0){
            fprintf(arqSaida,"Vertice: %d -> Nivel: %d -> Pai: Raiz\r\n",i+1,arvore[i].nivel);
        }else if(arvore[i].pai != -1){
            fprintf(arqSaida,"Vertice: %d -> Nivel: %d -> Pai: %d\r\n",i+1,arvore[i].nivel,arvore[i].pai);
        }
    }

    fclose(arqSaida);

}

// Gera um Arquivo de Saida com o Numero Total de Vertices, Numero Total de Arestas, Grau Maximo, Grau Minimo, Grau Medio e Mediana dos Graus
void gerarArquivoSaida(int numeroVertice, int numArestas, int gMax, int gMin, float gMediana, float gMedio, char* nomeArqSaida){
    FILE *arqSaida;
    if((arqSaida = fopen(nomeArqSaida, "wb")) == NULL)
    {
        printf("Erro na abertura do arquivo");
        exit(1);
    }

    fprintf(arqSaida,"Numero de Vertices: %d\r\n",numeroVertice);
    fprintf(arqSaida,"Numero de Arestas: %d\r\n", numArestas);
    fprintf(arqSaida,"Grau Minimo: %d\r\n",gMin);
    fprintf(arqSaida,"Grau Maximo: %d\r\n",gMax);
    fprintf(arqSaida,"Grau Media: %.2f\r\n",gMedio);
    fprintf(arqSaida,"Mediana do Grau: %.2f\r\n",gMediana);

    fclose(arqSaida);

}
// ----------------------------------------------------- FIM ARQUIVOS DE SAIDA -----------------------------------------------------

// ----------------------------------------------------- INICIO DE BUSCA -----------------------------------------------------
// BFS de Lista de Adjacencia
int* BFSListaAdjacencia(Vertice *Grafo, int s, int tam){
    int v,i;
    Elemento *p;
    Arvore *vetorArvore = malloc(tam*sizeof(Arvore));
    int *vetorMarcacao = malloc(tam*sizeof(int));
    for(i = 0; i < tam; i++){
        vetorMarcacao[i] = 0;
        vetorArvore[i].nivel = -1;
        vetorArvore[i].pai = -1;
    }
    vetorMarcacao[s-1]=1;
    vetorArvore[s-1].nivel = 0;
    vetorArvore[s-1].pai = 0;
    Fila *Q = fila_cria();
    fila_insere(Q,s);
    while(!fila_vazia(Q)){
        v = fila_retira(Q);
        for(p = Grafo[v-1].adjancencia; p!= NULL; p=proximaLista(p)){
            int w = getItem(p);
            if(vetorMarcacao[w-1]==0){
//                printf("%d\n",w);
                vetorMarcacao[w-1]=1;
                fila_insere(Q,w);
                vetorArvore[w-1].nivel = vetorArvore[v-1].nivel + 1;
                vetorArvore[w-1].pai = v;
            }
        }
    }

    gerarArquivoArvore(vetorArvore,tam);
    free(vetorArvore);
//    free(vetorMarcacao);
    return vetorMarcacao;
}

// DFS de Lista de Adjacencia
int* DFSListaAdjacencia(Vertice *Grafo, int s, int tam){
    int v,i;
    Pilha *P = pilha_cria();
    Arvore *vetorArvore = malloc(tam*sizeof(Arvore));
    int *vetorMarcacao = malloc(tam*sizeof(int));
    Elemento *p;
    for(i = 0; i < tam; i++){
        vetorMarcacao[i] = 0;
    }
    vetorArvore[s-1].nivel = 0;
    vetorArvore[s-1].pai = 0;
    pilha_push(P,s);
    while(!pilha_vazia(P)){
        v = pilha_pop(P);
        if(vetorMarcacao[v-1] != 1){
            vetorMarcacao[v-1] = 1;
            for(p = Grafo[v-1].adjancencia; p!= NULL; p=proximaLista(p)){
                int w = getItem(p);
                pilha_push(P,w);
                if(vetorMarcacao[w-1] != 1){
                    vetorArvore[w-1].nivel = vetorArvore[v-1].nivel + 1;
                    vetorArvore[w-1].pai = v;
                }
            }
        }
    }
//    gerarArquivoArvore(vetorArvore,tam);
    free(vetorArvore);
    return vetorMarcacao;
}

// BFS de Lista de Adjacencia para utilizar na busca de Componentes Conexos
void BFSListaAdjacencia02(Vertice *Grafo, int s, int tam, Marcacao2* vetorMarcacao, int numMarcado){
    int v;
    Elemento *p;
    Arvore *vetorArvore = malloc(tam*sizeof(Arvore));

    vetorMarcacao[s-1].numMarcacao=numMarcado;
    vetorArvore[s-1].nivel = 0;
    vetorArvore[s-1].pai = 0;
    Fila *Q = fila_cria();
    fila_insere(Q,s);
    while(!fila_vazia(Q)){
        v = fila_retira(Q);
        for(p = Grafo[v-1].adjancencia; p!= NULL; p=proximaLista(p)){
            int w = getItem(p);
            if(vetorMarcacao[w-1].numMarcacao==0){
//                printf("%d\n",w);
                vetorMarcacao[w-1].numMarcacao=numMarcado;
                fila_insere(Q,w);
                vetorArvore[w-1].nivel = vetorArvore[v-1].nivel + 1;
                vetorArvore[w-1].pai = v;
            }
        }
    }
//    gerarArquivoArvore(vetorArvore,tam);
    free(vetorArvore);
    return;
}

int BFSListaAdjacencia03(Vertice *Grafo, int s, int tam){
    int v,i;
    int distancia = 0;
    Elemento *p;
    Arvore *vetorArvore = malloc(tam*sizeof(Arvore));
    int *vetorMarcacao = malloc(tam*sizeof(int));
    for(i = 0; i < tam; i++){
        vetorMarcacao[i] = 0;
        vetorArvore[i].nivel = -1;
        vetorArvore[i].pai = -1;
    }
    vetorMarcacao[s-1]=1;
    vetorArvore[s-1].nivel = 0;
    vetorArvore[s-1].pai = 0;
    Fila *Q = fila_cria();
    fila_insere(Q,s);
    while(!fila_vazia(Q)){
        v = fila_retira(Q);
        for(p = Grafo[v-1].adjancencia; p!= NULL; p=proximaLista(p)){
            int w = getItem(p);
            if(vetorMarcacao[w-1]==0){
//                printf("%d\n",w);
                vetorMarcacao[w-1]=1;
                fila_insere(Q,w);
                vetorArvore[w-1].nivel = vetorArvore[v-1].nivel + 1;
                vetorArvore[w-1].pai = v;
                if(vetorArvore[w-1].nivel > distancia){
                    distancia = vetorArvore[w-1].nivel;
                }
            }
        }
    }

//    gerarArquivoArvore(vetorArvore,tam);
    free(vetorArvore);
    free(vetorMarcacao);
    return distancia;
}
// ----------------------------------------------------- FIM DE BUSCA -----------------------------------------------------

// ----------------------------------------------------- INICIO CICLO 1000X -----------------------------------------------------
// Executa, para Lista Adjacencia, a BFS(0) ou DFS(1) 1000x exibindo o tempo media e gerando um arquivo com o tempo em cada ciclo
void milCiclosListaAdjacencia(Vertice *vetorVertice, int numVertices, char* nomeArquivo, int tipo){
    long start,end;
    int i;
    int *vetorMarcacao;
//    double somaTempo = 0;

    FILE* s = fopen(nomeArquivo, "wb");
    fprintf(s,"Ciclo Tempo\r\n");
    if(tipo==0){
        for(i=0;i<1000;i++){
            printf("%d\n",i+1);
            start = getMicrotime();
            vetorMarcacao = BFSListaAdjacencia(vetorVertice,1,numVertices);
            end = getMicrotime();
            free(vetorMarcacao);
//            somaTempo += (double) end-start;
            fprintf(s,"%d %ld\r\n",i+1,end-start);
//          printf("Ciclo: %d -> %ld\r\n",i+1,end-start);
        }
    }
    else if(tipo==1){
        for(i=0;i<1000;i++){
            printf("%d\n",i+1);
            start = getMicrotime();
            vetorMarcacao = DFSListaAdjacencia(vetorVertice,1,numVertices);
            end = getMicrotime();
            free(vetorMarcacao);
//            somaTempo += (double) (end-start);
            fprintf(s,"%d %ld\r\n",i+1,end-start);
//          printf("Ciclo: %d -> %ld\r\n",i+1,end-start);
        }
    }else{
        fprintf(s,"Erro ao inserir o tipo de Busca. Tipos Validos: 0 (BFS) ou 1(DFS)\r\n");
        printf("Erro ao inserir o tipo de Busca. Tipos Validos: 0 (BFS) ou 1(DFS)\n");
    }

    fclose(s);
//    printf("Tempo Medio: %.2f\n",somaTempo/1000);
}

// ----------------------------------------------------- FIM CICLO 1000X -----------------------------------------------------

// ----------------------------------------------------- OUTRAS FUN��ES -----------------------------------------------------
void componenteConexa(Vertice *Grafo, int tam){
    int i,h,qntTotalConexo;
    int j = 1;
    int numVerticeComponenteConexa = 0;
    printf("Iniciando Componente Conexa!\n");
    // Inicializa o Vetor de Marca��o que contem o numero da marca��o e o numero do vertice
    Marcacao2* vetorMarcacaoCC = (Marcacao2*) malloc(tam*sizeof(Marcacao2));
    for(i = 0; i < tam; i++){
        vetorMarcacaoCC[i].numMarcacao=0;
        vetorMarcacaoCC[i].numVertice=i+1;
    }
    printf("Executando BFS!\n");
    // Executa a BFS varias vezes, marcando com o numero da componente conexa
    for(i = 0; i < tam; i++){
        if(vetorMarcacaoCC[i].numMarcacao == 0){
//            printf("%d\n",i+1);
            BFSListaAdjacencia02(Grafo,i+1,tam,vetorMarcacaoCC,j);
            j++;
        }
    }
    printf("Liberando Memoria!\n");
    // Liberar Espaco do Grafo, pois s� utilizaremos o vetor de marca��o
    for(i=0;i<tam;i++){
        lst_libera(Grafo[i].adjancencia);
    }
    free(Grafo);

    printf("1 MergeSort!\n");
    // Ordena o vetor de marca��o em ordem crescente do numero da componente conexa
    mergeSortStruct(vetorMarcacaoCC,0,tam-1);
    qntTotalConexo = vetorMarcacaoCC[tam-1].numMarcacao;

    printf("Guardando Endereco!\n");
    // Cria um vetor para guardar a quantidade de elementos de cada componente conexa e o
    // endereco do inicio da componente conexa no vetor de marca�ao
    EndVetorPrincipal *vet = malloc(qntTotalConexo*sizeof(EndVetorPrincipal));
    for(i = 0; i < tam; i++){
        if(i == 0 || vetorMarcacaoCC[i].numMarcacao != vetorMarcacaoCC[i-1].numMarcacao){
            vet[vetorMarcacaoCC[i].numMarcacao - 1].endereco = &(vetorMarcacaoCC[i]);
        }
        numVerticeComponenteConexa++;
        if(i == tam - 1 || vetorMarcacaoCC[i].numMarcacao != vetorMarcacaoCC[i+1].numMarcacao){
            vet[vetorMarcacaoCC[i].numMarcacao - 1].qntMarcacao = numVerticeComponenteConexa;
            numVerticeComponenteConexa=0;
        }
    }

    printf("2 MergeSort!\n");
    // Ordena o vetor em ordem crescente do numero de quantidade de elemento de cada componente
    mergeSortStructEnd(vet,0,qntTotalConexo-1);

//    printf("Imprimindo...\n");
    // Imprime o numero e a quantidade de vertices de cada componente
//    for(i = qntTotalConexo - 1; i>=0; i--){
//        printf("Total: %d \n",vet[i].qntMarcacao);
//        for(h = 0; h < vet[i].qntMarcacao; h++){
//            printf("%d ",vet[i].endereco[h].numVertice);
//        }
//        printf("\n");
//    }
        printf("Qnt Total Conexo: %d\n",qntTotalConexo);
        printf("Maior Conexo: %d\n",vet[qntTotalConexo-1].qntMarcacao);
        printf("Menor Conexo: %d\n",vet[0].qntMarcacao);
//    for(i = 0; i< tam; i++){
//        printf("%d -> ",vetorMarcacaoCC[i].numMarcacao);
//    }
//    printf("\n");

}

void gerarGraus(int* vetorGraus, int numVertices, int numArestas, char* nomeArqSaida){

    int gMax,gMin;
    float gMedio, gMediana;

    mergeSort(vetorGraus,0,numVertices-1);
    gMax = vetorGraus[numVertices-1];
    gMin = vetorGraus[0];

    if(numVertices % 2 == 0){
        gMediana = (float)(vetorGraus[(int)(numVertices/2)] + vetorGraus[(int)(numVertices/2) + 1])/2;
    }else{
        gMediana = (float)vetorGraus[(int)(numVertices/2)];
    }

    gMedio = (numArestas*2)/(float)numVertices;

    gerarArquivoSaida(numVertices, numArestas, gMax, gMin, gMediana, gMedio, nomeArqSaida);

}

int diametroGrafo(Vertice *Grafo, int tam ){
    int maiorDiametro = 0;
    int i,diametro;

    for(i=0;i<tam;i++){
        diametro = BFSListaAdjacencia03(Grafo,i+1,tam);
        if(diametro > maiorDiametro){
            maiorDiametro = diametro;
        }
    }
    return maiorDiametro;

}

char** pesquisadores(int tam){

    int indice,ultimoCaracter;
    char *nome;
    FILE* arq;
    char Linha[NUMERO_LINHA];
    char *result;
    char **nomes;


    // Abre um arquivo TEXTO para LEITURA
    arq = fopen("rede_colaboracao_vertices.txt", "rt");
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("Problemas na abertura do arquivo\n");
        return 0;
    }

    nomes = (char**) malloc((tam+1)*sizeof(char*));

    while (!feof(arq))
    {
        // L� uma linha (inclusive com o '\n')
        result = fgets(Linha, NUMERO_LINHA-1, arq);  // o 'fgets' l� at� NUMERO_LINHA caracteres ou at� o '\n'
        ultimoCaracter = strlen(result)-1;
        // Verificar e tirar o \n caso exista
        if(result[ultimoCaracter] == '\n'){
            result[ultimoCaracter] = '\0';
        }

        indice = atoi(strtok(result,","));
        nome = strtok('\0',",");

        int l = strlen(nome);
        char *n = (char*) malloc((l+1)*sizeof(char));
        strcpy(n,nome);
        nome[l-1] = '\0';
        nomes[indice] = n;

    }

//    int j;
//    for(j=1;j<=tam;j++){
//        printf("%s\n",nomes[j]);
//    }
    return nomes;

}
