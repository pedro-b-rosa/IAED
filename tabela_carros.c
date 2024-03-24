/**
 * funções para a gestão de uma tabela de carros
 * @file tabela_carros.c
 * @author ist1106426
*/

#include "tabela_carros.h"
#include <stdlib.h>

static PonteiroCarro **cabeca;

/**
    Inicializa a tabela de carros
*/
void inicializaTabelaCarros() {
    cabeca = (PonteiroCarro **)malloc(sizeof(PonteiroCarro*) * TAMANHO);
    int i;
    for (i = 0; i < TAMANHO; i++) {
        cabeca[i] = NULL;
    }
}

/**
    gera e devolve o indice do carro através da matricula
    @param matricula a matricula do carro
    @return o indice do carro
*/
int geraIndiceCarro(char matricula[9]) {
    int indice = 0;
    int i;
    for (i = 0; i < 2; i++) {
        if (matricula[i] >= 'A' && matricula[i] <= 'Z') {
            indice += (matricula[i] - 'A');
        } else {
            indice += (matricula[i] - '0');
        }
    }
    return indice/2;
}

/**
    insere um carro na tabela de carros
    @param carro o carro a inserir
*/
void insereCarro(Carro *carro) {
    int indice = geraIndiceCarro(carro->matricula);
    PonteiroCarro *novo = (PonteiroCarro *)malloc(sizeof(PonteiroCarro));
    novo->carro = carro;
    novo->prox = cabeca[indice]; // coloca no inicio para o tempo ser constante
    cabeca[indice] = novo;
}

/**
    procura um carro na tabela de carros
    @param matricula a matricula do carro a procurar
    @return o carro com a matricula dada
*/
Carro* procuraCarro(char matricula[9]) {
    int indice = geraIndiceCarro(matricula);
    PonteiroCarro *atual = cabeca[indice];
    while (atual != NULL) {
        if (strcmp(atual->carro->matricula, matricula) == 0) {
            return atual->carro;
        }
        atual = atual->prox;
    }
    return NULL;
}

/**
    remove um carro da tabela de carros
    @param matricula a matricula do carro a remover
*/
void removeCarro(char matricula[9]) {
    int indice = geraIndiceCarro(matricula);
    PonteiroCarro *atual = cabeca[indice];
    PonteiroCarro *anterior = NULL;
    while (atual != NULL) {
        if (strcmp(atual->carro->matricula, matricula) == 0) {
            if (anterior == NULL) {
                cabeca[indice] = atual->prox;
            } else {
                anterior->prox = atual->prox;
            }
            free(atual);
            break;
        }
        anterior = atual;
        atual = atual->prox;
    }
}

/**
    liberta a memoria alocada para a tabela de carros
*/
void libertaTabelaCarros() {
    PonteiroCarro *atual;
    PonteiroCarro *temp;
    int i;
    for (i = 0; i < TAMANHO; i++) {
        atual = cabeca[i];
        while (atual != NULL) {
            temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(cabeca);
}