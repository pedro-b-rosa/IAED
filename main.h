#ifndef MAIN_H
#define MAIN_H

#include "data.h"

#define MAXPARQUES 20 // numero maximo de parques
#define FALSE 0
#define TRUE 1

#define ERROPARQUEEXISTE ": parking already exists."
#define ERROCAPACIDADE ": invalid capacity."
#define ERROVALOR "invalid cost."
#define ERROPARQUES "too many parks."
#define ERROVEICULO ": invalid vehicle entry."
#define ERRODATA "invalid date."
#define ERROVEICULOSAIDA ": invalid vehicle exit."
#define ERROPARQUENAOEXISTE ": no such parking."
#define ERROMATRICULAINVALIDA ": invalid licence plate."
#define ERROESTACIONAMENTOCHEIO ": parking is full."
#define ERROHISTORICO ": no entries found in any parking."

typedef struct Historico{
    Data data;
    float valor;
    struct Historico *prox;
} Historico;

typedef struct Carro{
    char matricula[9]; // AA-00-AA\0
    Data data_e; // data de entrada
    Data data_s; // data de saida
    float valor_pago; // valor pago
    struct Carro *prox; // ponteiro para o proximo carro
} Carro;

typedef struct Parque{
    char* nome; // nome do parque
    int cap; // capacidade do parque
    int livres; // lugares livres
    float valor_15; // valor a pagar pelo parque na primeira hora por 15 min
    float valor_1hora; // valor a pagar depois da primeira hora por 15 min
    float valor_max; // valor maximo diario
    Carro *carros; // lista de carros
    Carro *ultimo_carro; // ponteiro para o ultimo carro
    Historico *historico; // historico de faturacao
    Historico *ultimo_historico; // ponteiro para o ultimo historico
    struct Parque *prox; // ponteiro para o proximo parque
} Parque;

#endif