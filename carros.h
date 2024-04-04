#ifndef CARROS_H
#define CARROS_H

#include "main.h"
#include "tabela_carros.h"
#include "parques.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
    verifica se pode adicionar o carro
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
    @param lista_parques ponteiro para a lista dos parques
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int podeAdicionarCarro(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques);

/**
    adiciona um carro ah lista de carros do parque
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
*/
void adicionaListaCarros(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques);

/**
    verifica se pode adicionar o carro
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
    @param lista_parques ponteiro para a lista dos parques
    @return ponteiro para o carro ou NULL caso não exista
*/
Carro* podeRegistarSaida(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques);

/**
    Regista a saida do carro pondo a data de saida e valor pago
    @param carro ponteiro para o carro
    @param data a data de saida
    @param lista_parques ponteiro para a lista dos parques
    @param nome_par nome do parque
    @param data a data de saida
*/
void RegistaSaida(Carro *carro, Data data, Parque *lista_parques, char nome_par[BUFSIZ]);

/**
    da print das faturados carros
    @param matricula do carro
    @param data a data a consultar
    @param lista_parques ponteiro para a lista dos parques
*/
void mostrarFaturaCarros(char nome_par[BUFSIZ], Data data, Parque *lista_parques);

/**
    da printf da lista de carros existentes por ordem de parque e de entrada
    @param lista_parques ponteiro para a lista dos parques
*/
void mostrarCarro(Parque *lista_parques);

#endif