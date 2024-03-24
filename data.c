/**
 * funções que utilizão as datas
 * @file data.c
 * @author ist1106426
*/
#include "data.h"

static int dias_mes[13] = {0 ,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static Data ult_data; // ultima data usada

/**
    Atualiza a ultima data usada
    @param data a data de entrada
*/
void atualizarData(Data data){
    ult_data.minuto = data.minuto;
    ult_data.hora = data.hora;
    ult_data.dia = data.dia;
    ult_data.mes = data.mes;
    ult_data.ano = data.ano;
}

/**
    verifica se a data existe
    @param data a data de entrada
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int dataPossivel(Data data){
    // Verifica se o mes eh valido
    if (data.mes < 1 || data.mes > 12)
        return FALSE;
    // Verifica se o dia e valido
    if (data.dia < 1 || data.dia > dias_mes[data.mes])
        return FALSE;
    if (data.hora < 0 || data.hora > 23)
        return FALSE;
    if (data.minuto < 0 || data.minuto > 59)
        return FALSE;
    return TRUE;
}

/**
    verifica se a data e menor
    @param data a data de entrada
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int dataMaior(Data data){
    // Verifica se a data nova eh maior que a ultima data
    if (data.ano != ult_data.ano)
        return (data.ano > ult_data.ano) ? TRUE : FALSE;
    else if (data.mes != ult_data.mes)
        return (data.mes > ult_data.mes) ? TRUE : FALSE;
    else if (data.dia != ult_data.dia)
        return (data.dia > ult_data.dia) ? TRUE : FALSE;
    else if (data.hora != ult_data.hora)
        return (data.hora > ult_data.hora) ? TRUE : FALSE;
    else if (data.minuto != ult_data.minuto)
        return (data.minuto > ult_data.minuto) ? TRUE : FALSE;
    return TRUE;
}

/**
    verifica se a data e valida
    @param data a data de entrada
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int dataValida(Data data){
    if (dataPossivel(data))
        return dataMaior(data);
    else
        return FALSE;
}

/**
    Calcula o numero de minutos entre duas datas
    @param d_e a data de entrada
    @param d_s a data de saida
    @return o numero de minutos entre as duas datas
*/
long int minutosEntreDatas(Data d_e, Data d_s){
    long int t = 0, conta = 0;

    while(d_e.dia != d_s.dia || d_e.mes != d_s.mes || d_e.ano != d_s.ano){
        conta++;
        d_e.dia++;

        if(d_e.dia > dias_mes[d_e.mes]){
            d_e.dia = 1;
            d_e.mes++;
            if (d_e.mes > 12){
                d_e.mes = 1;
                d_e.ano++;
            }
        }
    }
    t += conta*24*60+(d_s.hora-d_e.hora)*60+d_s.minuto-d_e.minuto;

    return t;
}

/**
    verifica se a data de d1 é igual a de d2
    @param d1 data 1
    @param d2 data 2
    @return inteiro TRUE caso a data de d1 seja igual a de d2 
    ou FALSE caso contrario
*/
int dataIgual(Data d1, Data d2){
    if(d1.ano == d2.ano && d1.mes == d2.mes && d1.dia == d2.dia)
        return TRUE;
    else
        return FALSE;
}

/**
    verifica se a data é anterior a ultima data usada
    @param data a data de entrada
    @return inteiro TRUE caso a data de d1 seja maior que a de d2 
    ou FALSE caso contrario
*/
int validarDataAnterior(Data data){
    if (dataPossivel(data))
        return !(dataMaior(data));
    else
        return FALSE;
}

/**
    verifica se a hora de d1 é maior que a de d2
    @param d1 data 1
    @param d2 data 2
    @return inteiro TRUE caso a hora de d1 seja maior que a de d2 
    ou FALSE caso contrario
*/
int horaMaior(Data d1, Data d2){
    if(d1.hora > d2.hora)
        return TRUE;
    else if(d1.hora == d2.hora && d1.minuto > d2.minuto)
        return TRUE;
    else
        return FALSE;
}