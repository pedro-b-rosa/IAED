#ifndef DATA_H
#define DATA_H

#define FALSE 0
#define TRUE 1

// Estrutura de dados para armazenar a data
typedef struct {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
} Data;

void atualizarData(Data data);
int dataValida(Data data);
long int minutosEntreDatas(Data data_e, Data data_s);
int dataIgual(Data d1, Data d2);
int validarDataAnterior(Data data);
int horaMaior(Data d1, Data d2);

#endif