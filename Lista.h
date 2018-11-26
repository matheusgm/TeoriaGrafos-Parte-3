typedef struct elemento Elemento;

/* Fun��o de cria��o: retorna uma lista vazia */
Elemento* lst_cria();

/* Inser��o no inicio: retorna a lista atualizada */
Elemento* lst_insere(Elemento *lst, int val,float peso);

/* fun��o imprime: imprime valores dos elementos */
void lst_imprime(Elemento *lst);

/* fun��o vazia: retorna 1 se vazia ou 0 se n�o vazia */
int lst_vazia(Elemento *lst);

/* fun��o busca: busca um elemento na lista */
Elemento* busca(Elemento *lst, int v);

void lst_libera(Elemento *lst);

Elemento* proximaLista(Elemento *lst);

int getItem(Elemento *lst);

float getWeight(Elemento *lst);

int getTamanhoLista(Elemento *lst);

