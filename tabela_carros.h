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

void inicializaTabelaCarros();
void insereCarro(Carro *carro);
Carro* procuraCarro(char matricula[9]);
void removeCarro(char matricula[9]);
void libertaTabelaCarros();

#endif