/**
 * funções para alterar e mostrar as listas dos parques.
 * @file parques.c
 * @author ist1106426
*/

#include "parques.h"

/**
    cria vetor para nome com o minimo de memoria
    @param vetor do nome para ser transformado
    @return pointer para um vetor com char
*/
char* criaNome(char vetor[BUFSIZ]) {
    char* vetor_novo = (char*) malloc(strlen(vetor) + 1);
    strcpy(vetor_novo, vetor);
    return vetor_novo;
}

/**
    procura se o parque existe
    @param vetor do nome para ser transformado
    @param lista_parques ponteiro para a lista dos parques
    @return ponteiro para o parque ou NULL caso não exista
*/
Parque* parqueExiste(char nome[BUFSIZ], Parque *lista_parques){
    Parque *aux = lista_parques;

    while (aux != NULL) {
        if (strcmp(aux->nome, nome) == 0) {
            return aux;
            break;
        }
        aux = aux->prox;
    }
    return NULL;
}

/**
    @param lista_parques ponteiro para a lista dos parques
    da printf da lista de parques existentes por ordem de colucacao
*/
void mostrarParques(Parque *lista_parques){
    Parque *aux = lista_parques;
    while (aux != NULL) {
        printf("%s %d %d\n", aux->nome, aux->cap, aux->livres);
        aux = aux->prox;
    }
}

/**
    Verifica se os valores sao validos
    @param val_15 valores a pagar pelo parque na primeira hora
    @param val_1h valores a pagar pelo parque depois da primeira hora
    @param val_max valore maximo diario
    @return TRUE caso verifique, FALSE caso falhe
 */
int valoresValidos(float val_15, float val_1h, float val_max){
    if (val_15 < 0 || val_1h < 0 || val_max < 0)
        return FALSE;
    if ((val_15 > val_1h) || (val_1h > val_max)) {
        return FALSE;
    }
    return TRUE;
}

/**
    Verifica se o parque pode ser adicionado.
    @param nome vetor com os caracteres do nome
    @param cap capacidade do parque
    @param val_15 valores a pagar pelo parque na primeira hora
    @param val_1h valores a pagar pelo parque depois da primeira hora
    @param val_max valore maximo diario
    @param lista_parques ponteiro para a lista dos parques
    @return inteiro 1 caso verifique, 0 caso falhe
 */
int verificaParque(char nome[BUFSIZ], int cap, float val_15, float val_1h, float val_max, Parque *lista_parques, int num){
    if(parqueExiste(nome, lista_parques) != NULL){
        printf("%s%s\n", nome, ERROPARQUEEXISTE);
        return FALSE;
    }
    if (cap <= 0) {
        printf("%d%s\n", cap, ERROCAPACIDADE);
        return FALSE;
    }
    if (valoresValidos(val_15, val_1h, val_max) == FALSE) {
        printf("%s\n", ERROVALOR);
        return FALSE;
    }
    if (num >= MAXPARQUES) {
        printf("%s\n", ERROPARQUES);
        return FALSE;
    }
    return TRUE;
}

void colocaParque(Parque **lista_parques, char *nome_novo, int cap, float val_15, float val_1h, float val_max){
    Parque *parque_novo = (Parque *) malloc(sizeof(Parque)), *aux;

    parque_novo->nome = nome_novo;
    parque_novo->cap = cap;
    parque_novo->livres = cap;
    parque_novo->valor_15 = val_15;
    parque_novo->valor_1hora = val_1h;
    parque_novo->valor_max = val_max;
    parque_novo->prox = NULL;
    parque_novo->carros = NULL;
    parque_novo->ultimo_carro = NULL;
    parque_novo->historico = NULL;
    parque_novo->ultimo_historico = NULL;

    if (*lista_parques == NULL) {
        *lista_parques = parque_novo;
    } else {
        aux = *lista_parques;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = parque_novo;
    }
}

/**
    Cria um parque e coloca-o na lista.
    @param nome vetor com os caracteres do nome
    @param cap capacidade do parque
    @param val_15 valores a pagar pelo parque na primeira hora
    @param val_1h valores a pagar pelo parque depois da primeira hora
    @param val_max valore maximo diario
    @param lista_parques ponteiro para a lista dos parques
    @param val_15 valores a pagar pelo parque
 */
void criaParque(char nome[BUFSIZ], int cap, float val_15, float val_1h, float val_max, Parque **lista_parques, int *num){
    int verifica;
    char *nome_novo;

    verifica = verificaParque(nome,cap,val_15,val_1h,val_max, *lista_parques, *num);
    nome_novo = criaNome(nome);
    if (verifica) {
        colocaParque(lista_parques, nome_novo, cap, val_15, val_1h, val_max);
        (*num)++;
    }
}

/**
    remove as listas ligadas ao parque
    @param parque ponteiro para o parque a eliminar
*/
void removeListas(Parque *parque){
    Carro *aux_carro_ant = parque->carros, *aux_carro_pos = parque->carros;
    Historico *auxHis_ant = parque->historico, *auxHis_pos = parque->historico;

    while (aux_carro_pos != NULL) {
        aux_carro_pos = aux_carro_ant->prox;
        removeCarro(aux_carro_ant->matricula); // atualiza a tabela de carros
        free(aux_carro_ant);
        aux_carro_ant = aux_carro_pos;
    }
    while (auxHis_pos != NULL) {
        auxHis_pos = auxHis_ant->prox;
        free(auxHis_ant);
        auxHis_ant = auxHis_pos;
    }
}

/**
    remove o parque
    @param parque ponteiro para o parque a eliminar
*/
void removeParque(Parque *parque, Parque **lista_parques, int *num_parques){
    Parque *aux = *lista_parques, *aux_ant = *lista_parques;

    if (parque == *lista_parques){
        *lista_parques = parque->prox;
    }else{
        while (aux != parque) {
            aux_ant = aux;
            aux = aux->prox;
        }
        aux_ant->prox = parque->prox;
    }
    removeListas(parque);
    (*num_parques)--;
    free(parque->nome);
    free(parque);
}

/**
    coloca o parque na lista de parques por ordem alfabetica
    @param lista_ord vetor para lista de parques
    @param parque parque a adicionar
    @param parque_novo parque a adicionar
*/
void colocaNoMeio(Parque *lista_ord, Parque *parque, Parque *parque_novo){
    Parque *aux_ant, *aux_pos;

    aux_ant = lista_ord;
    aux_pos = lista_ord->prox;
    while(aux_pos != NULL){
        if(strcmp(aux_pos->nome, parque->nome) > 0){
            aux_ant->prox = parque_novo;
            parque_novo->prox = aux_pos;
            break;
        }else{
            aux_ant = aux_pos;
            aux_pos = aux_pos->prox;
        }
    }
    if(aux_pos == NULL){
        if(strcmp(aux_ant->nome, parque->nome) > 0){
            lista_ord = parque_novo;
            parque_novo->prox = aux_ant;
        }else{
            aux_ant->prox = parque_novo;
        }
    }
}

/**
    coloca o parque na lista de parques por ordem alfabetica
    @param lista_ord vetor para lista de parques
    @param parque parque a adicionar
*/
void colocaParqueOrd(Parque **lista_ord, Parque *parque){
    Parque *parque_novo = (Parque*) malloc(sizeof(Parque));

    parque_novo->nome = parque->nome;
    parque_novo->carros = parque->carros;
    parque_novo->prox = NULL;
    if(*lista_ord == NULL){
        *lista_ord = parque_novo;
    }else if (strcmp((*lista_ord)->nome, parque->nome) > 0){
        parque_novo->prox = *lista_ord;
        *lista_ord = parque_novo;
    }else{
        colocaNoMeio(*lista_ord, parque, parque_novo);
    }
}

/**
    coloca o parque na lista de parques por ordem alfabetica
    @param lista_ord vetor para lista de parques
    @param lista_parques ponteiro para a lista dos parques
*/
void criaListaOrdParques(Parque **lista_ord, Parque *lista_parques){
    Parque *aux = lista_parques;
    while (aux != NULL){
        colocaParqueOrd(lista_ord, aux);
        aux = aux->prox;
    }
}

/**
    da printf da lista de parques existentes por ordem de colucacao
*/
void mostrarParques2(Parque *lista_parques){
    Parque *aux, *lista_ord = NULL;

    criaListaOrdParques(&lista_ord, lista_parques);
    while (lista_ord != NULL) {
        printf("%s\n", lista_ord->nome);
        aux = lista_ord;
        lista_ord = lista_ord->prox;
        free(aux);
    }
}