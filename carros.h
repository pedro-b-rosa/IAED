#ifndef CARROS_H
#define CARROS_H

#include "main.h"
#include "tabela_carros.h"
#include "parques.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int podeAdicionarCarro(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques);
void adicionaListaCarros(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques);
Carro* podeRegistarSaida(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques);
void RegistaSaida(Carro *carro, Data data, Parque *lista_parques, char nome_par[BUFSIZ]);
void mostrarFaturaCarros(char nome_par[BUFSIZ], Data data, Parque *lista_parques);
void mostrarCarro(Parque *lista_parques);

#endif