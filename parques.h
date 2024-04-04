#ifndef PARQUES_H
#define PARQUES_H

#include "main.h"
#include "tabela_carros.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
    Cria um parque e coloca-o na lista.
    @param nome vetor com os caracteres do nome
    @param cap capacidade do parque
    @param val_15 valores a pagar pelo parque na primeira hora
    @param val_1h valores a pagar pelo parque depois da primeira hora
    @param val_max valore maximo diario
    @param lista_parques ponteiro para a lista dos parques
*/
void criaParque(char nome[BUFSIZ], int cap, float val_15, float val_1h, float val_max, Parque **lista_parques, int *num);

/**
    @param lista_parques ponteiro para a lista dos parques
    da printf da lista de parques existentes por ordem de colucacao
*/
void mostrarParques(Parque *lista_parques);

/**
    procura se o parque existe
    @param vetor do nome para ser transformado
    @param lista_parques ponteiro para a lista dos parques
    @return ponteiro para o parque ou NULL caso não exista
*/
Parque* parqueExiste(char nome[BUFSIZ], Parque *lista_parques);

/**
    remove as listas ligadas ao parque
    @param parque ponteiro para o parque a eliminar
*/
void removeListas(Parque *parque);

/**
    remove o parque
    @param parque ponteiro para o parque a eliminar
*/
void removeParque(Parque *parque, Parque **lista_parques, int *num_parques);

/**
    da printf da lista de parques existentes por ordem de colucacao
*/
void mostrarParques2(Parque *lista_parques);

/**
    coloca o parque na lista de parques por ordem alfabetica
    @param lista_ord vetor para lista de parques
    @param lista_parques ponteiro para a lista dos parques
*/
void criaListaOrdParques(Parque **lista_ord, Parque *lista_parques);

#endif