/**
 * projecto de IAED em C.
 * @file main.c
 * @author ist1106426
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXPARQUES 20
#define NOME 40
#define FALSE 0
#define TRUE 1

typedef struct Carro{
    char matricula[9]; // AA-00-AA\0AXPARQUES 20
    char entry_data[11]; // DD-MM-YYYY\0
    char entry_hora[6]; // HH:MM\0
    char exit_data[11];
    char exit_hora[6];
    float total_cost;
    struct Carro *prox;
} Carro;

typedef struct Parque{
    char* nome;
    int cap;
    int livres;
    float valor_15;
    float valor_1hora;
    float valor_max;
    Carro *carros;
    struct Parque *prox;
} Parque;

Parque *lista_parques = NULL;
char ult_data[11];
char ult_hora[6];

/**
    cria vetor para nome com o minimo de memoria
    @param vetor do nome para ser transformado
    @return pointer para um vetor com char
*/
char* criaNome(char vetor[NOME]) {
    char* vetor_novo = (char*) malloc(strlen(vetor) + 1);
    strcpy(vetor_novo, vetor);
    return vetor_novo;
}

/**
    da printf da lista de parques existentes por ordem de colucacao
*/
void mustrarParques(){
    Parque *aux = lista_parques;
    while (aux != NULL) {
        printf("%s %d %d\n", aux->nome, aux->cap, aux->livres);
        aux = aux->prox;
    }
}

/**
 * Verifica se o parque pode ser adicionado.
 * @param nome vetor com os caracteres do nome
 * @param cap capacidade do parque
 * @param val_15 valores a pagar pelo parque
 * @return inteiro 1 caso verifique, 0 caso falhe
 */
int verificaParque(char nome[NOME], int cap, float val_15, float val_1h, float val_max, Parque *lista_parques) {
    int i = 0;
    Parque *aux = lista_parques;
    while (aux != NULL) {
        if (strcmp(aux->nome, nome) == 0) {
            printf("%s: parking already exists.\n", nome);
            return FALSE;
        }
        aux = aux->prox;
        i++;
    }
    if (cap <= 0) {
        printf("%d: invalid capacity.\n",cap);
        return FALSE;
    }
    if ((val_15 > val_1h) || (val_1h > val_max)) {
        printf("invalid cost.\n");
        return FALSE;
    }
    if (i >= MAXPARQUES) {
        printf("too many parks.\n");
        return FALSE;
    }
    return TRUE;
}

/**
 * Cria um parque e coloca-o na lista.
 * @param nome vetor com os caracteres do nome
 * @param cap capacidade do parque
 * @param val_15 valores a pagar pelo parque
 */
void criaParque(char nome[NOME], int cap, float val_15, float val_1h, float val_max, Parque **lista_parques) {
    int verifica;
    char *nome_novo;
    Parque *parque_novo = (Parque *) malloc(sizeof(Parque)), *aux;

    verifica = verificaParque(nome, cap, val_15, val_1h, val_max, *lista_parques);
    nome_novo = criaNome(nome);
    if (verifica) {
        parque_novo->nome = nome_novo;
        parque_novo->cap = cap;
        parque_novo->livres = cap;
        parque_novo->valor_15 = val_15;
        parque_novo->valor_1hora = val_1h;
        parque_novo->valor_max = val_max;
        parque_novo->prox = NULL;

        if (*lista_parques == NULL) {
            *lista_parques = parque_novo;
        } else {
            aux = *lista_parques;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = parque_novo;
        }
    } else {
        free(parque_novo);
    }
}

/**
    Le o input do utilizador para o comando p
*/
void leArgumentosParque(){
    char nome[NOME], c;
    int cap;
    float val_15, val_1h, val_max;
    if ((c = getchar()) != '\n'){
        while ((c = getchar()) == ' ');
        ungetc(c, stdin);
        if((c = getchar()) == '"'){
            ungetc(c, stdin);
            scanf("\"%[^\"]\"%d%f%f%f", nome, &cap, &val_15, &val_1h, &val_max);
        }else{
            ungetc(c, stdin);
            scanf("%s%d%f%f%f", nome, &cap, &val_15, &val_1h, &val_max);
        }
        criaParque(nome, cap, val_15, val_1h, val_max, &lista_parques);
    }else{
        mustrarParques();
    }

}

/**
    Le o input do utilizador para direionar para a funcao certa
    @return retorna sempre 0
*/
int main(){
    char c;

    while ((c = getchar()) != 'q'){
        switch(c){
            case 'p':
            leArgumentosParque();
            break;
            case 'e':
            // leArgumentosEntrada();
            break;
            case 's':
            break;
            case 'v':
            break;
            case 'f':
            break;
            case 'r':
            break;
        }
    }

    // funcão para limpar

    return 0;
}