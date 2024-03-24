#ifndef PARQUES_H
#define PARQUES_H

#include "main.h"
#include "tabela_carros.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void criaParque(char nome[BUFSIZ], int cap, float val_15, float val_1h, float val_max, Parque **lista_parques, int *num);
void mostrarParques(Parque *lista_parques);
Parque* parqueExiste(char nome[BUFSIZ], Parque *lista_parques);
void removeListas(Parque *parque);
void removeParque(Parque *parque, Parque **lista_parques, int *num_parques);
void mostrarParques2(Parque *lista_parques);
void criaListaOrdParques(Parque **lista_ord, Parque *lista_parques);

#endif