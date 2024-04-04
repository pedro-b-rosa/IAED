#ifndef TABELA_CARROS_H
#define TABELA_CARROS_H

#include "main.h"
#include <stdlib.h>
#include <string.h>

#define TAMANHO 338 // (26*26)/2 = 338 dois primeiros caracteres da matricula a dividir por 2

// Estrutura de dados para os ponteiros para os carros
typedef struct ponteiroCarro{
    Carro *carro;
    struct ponteiroCarro *prox;
} PonteiroCarro;

/**
    Inicializa a tabela de carros e inicializa os ponteiros a NULL
*/
void inicializaTabelaCarros();

/**
    insere um carro na tabela de carros
    @param carro o carro a inserir
*/
void insereCarro(Carro *carro);

/**
    procura um carro na tabela de carros
    @param matricula a matricula do carro a procurar
    @return o carro com a matricula dada
*/
Carro* procuraCarro(char matricula[9]);

/**
    remove um carro da tabela de carros
    @param matricula a matricula do carro a remover
*/
void removeCarro(char matricula[9]);

/**
    liberta a memoria alocada para a tabela de carros
*/
void libertaTabelaCarros();

#endif