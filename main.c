/**
 * projecto de IAED em C.
 * @file main.c
 * @author ist1106426
*/

#include "main.h"
#include "tabela_carros.h"
#include "carros.h"
#include "parques.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/**
    Le o input do utilizador para o comando p
*/
void leArgumentosParque(Parque **lista_parques, int *num_parques){
    char nome[BUFSIZ], c;
    int cap;
    float val_15, val_1h, val_max;

    if ((c = getchar()) != '\n'){
        while ((c = getchar()) == ' ');
        ungetc(c, stdin);
        if((c = getchar()) == '"'){
            ungetc(c, stdin);
            scanf("\"%[^\"]\"%d%f%f%f",nome, &cap, &val_15, &val_1h, &val_max);
        }else{
            ungetc(c, stdin);
            scanf("%s%d%f%f%f", nome, &cap, &val_15, &val_1h, &val_max);
        }
        criaParque(nome, cap, val_15, val_1h, val_max, lista_parques, num_parques);
    }else{
        mostrarParques(*lista_parques);
    }
}

/**
    Le o input do utilizador para o comando e
*/
void leArgumentosEntrada(Parque *lista_parques){
    char nome_par[BUFSIZ], c, matricula[9];
    Data d; // data de entrada

    while ((c = getchar()) == ' ');
    ungetc(c, stdin);
    if((c = getchar()) == '"'){
        ungetc(c, stdin);
        scanf("\"%[^\"]\"%s", nome_par, matricula);
    }else{
        ungetc(c, stdin);
        scanf("%s%s", nome_par, matricula);
    }
    scanf("%d-%d-%d %d:%d",&d.dia,&d.mes,&d.ano,&d.hora,&d.minuto);

    if (podeAdicionarCarro(nome_par, matricula, d, lista_parques)){
        adicionaListaCarros(nome_par, matricula, d, lista_parques);
    }
}

/**
    Le o input do utilizador para o comando s
*/
void leArgumentosSaida(Parque *lista_parques){
    char nome_par[BUFSIZ], c, matricula[9];
    Data d; // data de saida
    Carro *carro;

    while ((c = getchar()) == ' ');
    ungetc(c, stdin);
    if((c = getchar()) == '"'){
        ungetc(c, stdin);
        scanf("\"%[^\"]\"%s", nome_par, matricula);
    }else{
        ungetc(c, stdin);
        scanf("%s%s", nome_par, matricula);
    }
    scanf("%d-%d-%d %d:%d",&d.dia,&d.mes,&d.ano,&d.hora,&d.minuto);
    if ((carro = podeRegistarSaida(nome_par, matricula, d, lista_parques)) != NULL){
        RegistaSaida(carro, d, lista_parques, nome_par);
    }
}

/**
    mostra o historico se não der erro
    @param nome_par nome do parque
*/
void mostrarHistorico(char nome_par[BUFSIZ], Parque *lista_parques){
    Parque *aux = lista_parques;
    Historico *aux_historico;

    aux = parqueExiste(nome_par, lista_parques);
    if (aux == NULL){
        printf("%s%s\n", nome_par,  ERROPARQUENAOEXISTE);
    }else{
        aux_historico = aux->historico;
        while (aux_historico != NULL) {
            printf("%02d-", aux_historico->data.dia);
            printf("%02d-%d ", aux_historico->data.mes, aux_historico->data.ano);
            printf("%.2f\n", aux_historico->valor);
            aux_historico = aux_historico->prox;
        }
    }
}

/**
    Le o input do utilizador para o comando f
*/
void leArgumentosFaturacao(Parque *lista_parques){
    char nome_par[BUFSIZ], c;
    Data d; // data a consultar

    while ((c = getchar()) == ' ');
    ungetc(c, stdin);
    if((c = getchar()) == '"'){
        ungetc(c, stdin);
        scanf("\"%[^\"]\"", nome_par);
    }else{
        ungetc(c, stdin);
        scanf("%s", nome_par);
    }
    if ((c = getchar()) != '\n'){
        scanf("%d-%d-%d",&d.dia,&d.mes,&d.ano);
        d.hora = 0;
        d.minuto = 0;
        mostrarFaturaCarros(nome_par, d, lista_parques);
    }else{
        mostrarHistorico(nome_par, lista_parques);
    }
}

/**
    le o nome do parque a remover
*/
void leArgumentosRemove(Parque **lista_parques, int *num_parques){
    char c, nome_par[BUFSIZ];
    Parque *aux;

    while ((c = getchar()) == ' ');
    ungetc(c, stdin);
    if((c = getchar()) == '"'){
        ungetc(c, stdin);
        scanf("\"%[^\"]\"", nome_par);
    }else{
        ungetc(c, stdin);
        scanf("%s", nome_par);
    }
    aux = parqueExiste(nome_par, *lista_parques);
    if (aux == NULL){
        printf("%s%s\n", nome_par,  ERROPARQUENAOEXISTE);
    } else {
        removeParque(aux, lista_parques, num_parques);
        mostrarParques2(*lista_parques);
    }
}

/**
    Limpa a memoria alocada
    @param lista_parques ponteiro para a lista de parques
*/
void limparTudo(Parque *lista_parques){
    Parque *aux = lista_parques, *aux_ant = lista_parques;
    while (aux != NULL) {
        aux_ant = aux;
        aux = aux->prox;
        removeListas(aux_ant);
        free(aux_ant->nome);
        free(aux_ant);
    }
    libertaTabelaCarros();
}

/**
    Le o input do utilizador para direionar para a funcao certa
    @return retorna sempre 0
*/
int main(){
    Parque *lista_parques = NULL; // ponteir para lista de parques
    int num_parques = 0; // numero de parques criados
    char c;
    inicializaTabelaCarros();
    while ((c = getchar()) != 'q'){
        switch(c){
            case 'p':
            leArgumentosParque(&lista_parques, &num_parques);
            break;
            case 'e':
            leArgumentosEntrada(lista_parques);
            break;
            case 's':
            leArgumentosSaida(lista_parques);
            break;
            case 'v':
            mostrarCarro(lista_parques);
            break;
            case 'f':
            leArgumentosFaturacao(lista_parques);
            break;
            case 'r':
            leArgumentosRemove(&lista_parques, &num_parques);
            break;
        }
    }
    limparTudo(lista_parques);
    return 0;
}