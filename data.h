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

/**
    Atualiza a ultima data usada
    @param data a data de entrada
*/
void atualizarData(Data data);

/**
    verifica se a data e valida
    @param data a data de entrada
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int dataValida(Data data);

/**
    Calcula o numero de minutos entre duas datas
    @param d_e a data de entrada
    @param d_s a data de saida
    @return o numero de minutos entre as duas datas
*/
long int minutosEntreDatas(Data data_e, Data data_s);

/**
    verifica se a data de d1 é igual a de d2
    @param d1 data 1
    @param d2 data 2
    @return inteiro TRUE caso a data de d1 seja igual a de d2 ou FALSE caso contrario
*/
int dataIgual(Data d1, Data d2);

/**
    verifica se a data é anterior a ultima data usada
    @param data a data de entrada
    @return inteiro TRUE caso a data de d1 seja maior que a de d2 
    ou FALSE caso contrario
*/
int validarDataAnterior(Data data);

/**
    verifica se a hora de d1 é maior que a de d2
    @param d1 data 1
    @param d2 data 2
    @return inteiro TRUE caso a hora de d1 seja maior que a de d2 ou FALSE caso contrario
*/
int horaMaior(Data d1, Data d2);

#endif