/**
 * projecto de IAED em C.
 * @file main.c
 * @author ist1106426
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXPARQUES 20
#define NOME 40
#define FALSE 0
#define TRUE 1

typedef struct {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
} Data;

typedef struct Carro{
    char matricula[9]; // AA-00-AA\0
    Data data_entra;
    struct Carro *prox;
} Carro;

typedef struct Parque{
    char* nome;
    int cap;
    int livres;
    float valor_15;
    float valor_1hora;
    float valor_max;
    Carro *carros;
    struct Parque *prox;
} Parque;

Parque *lista_parques = NULL;
Data ult_data;

/**
    cria vetor para nome com o minimo de memoria
    @param vetor do nome para ser transformado
    @return pointer para um vetor com char
*/
char* criaNome(char vetor[NOME]) {
    char* vetor_novo = (char*) malloc(strlen(vetor) + 1);
    strcpy(vetor_novo, vetor);
    return vetor_novo;
}

/**
    da printf da lista de parques existentes por ordem de colucacao
*/
void mustrarParques(){
    Parque *aux = lista_parques;
    while (aux != NULL) {
        printf("%s %d %d\n", aux->nome, aux->cap, aux->livres);
        aux = aux->prox;
    }
}

/**
    Verifica se o parque pode ser adicionado.
    @param nome vetor com os caracteres do nome
    @param cap capacidade do parque
    @param val_15 valores a pagar pelo parque na primeira hora
    @param val_1h valores a pagar pelo parque depois da primeira hora
    @param val_max valore maximo diario
    @param lista_parques vetor para a lista dos parques
    @return inteiro 1 caso verifique, 0 caso falhe
 */
int verificaParque(char nome[NOME], int cap, float val_15, float val_1h, float val_max, Parque *lista_parques){
    int i = 0;
    Parque *aux = lista_parques;
    while (aux != NULL) {
        if (strcmp(aux->nome, nome) == 0) {
            printf("%s: parking already exists.\n", nome);
            return FALSE;
        }
        aux = aux->prox;
        i++;
    }
    if (cap <= 0) {
        printf("%d: invalid capacity.\n",cap);
        return FALSE;
    }
    if ((val_15 > val_1h) || (val_1h > val_max)) {
        printf("invalid cost.\n");
        return FALSE;
    }
    if (i >= MAXPARQUES) {
        printf("too many parks.\n");
        return FALSE;
    }
    return TRUE;
}

/**
    Cria um parque e coloca-o na lista.
    @param nome vetor com os caracteres do nome
    @param cap capacidade do parque
    @param val_15 valores a pagar pelo parque na primeira hora
    @param val_1h valores a pagar pelo parque depois da primeira hora
    @param val_max valore maximo diario
    @param lista_parques vetor para a lista dos parques
    @param val_15 valores a pagar pelo parque
 */
void criaParque(char nome[NOME], int cap, float val_15, float val_1h, float val_max, Parque **lista_parques){
    int verifica;
    char *nome_novo;
    Parque *parque_novo = (Parque *) malloc(sizeof(Parque)), *aux;

    verifica = verificaParque(nome,cap,val_15,val_1h,val_max,*lista_parques);
    nome_novo = criaNome(nome);
    if (verifica) {
        parque_novo->nome = nome_novo;
        parque_novo->cap = cap;
        parque_novo->livres = cap;
        parque_novo->valor_15 = val_15;
        parque_novo->valor_1hora = val_1h;
        parque_novo->valor_max = val_max;
        parque_novo->prox = NULL;

        if (*lista_parques == NULL) {
            *lista_parques = parque_novo;
        } else {
            aux = *lista_parques;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = parque_novo;
        }
    } else {
        free(parque_novo);
    }
}

/**
    Le o input do utilizador para o comando p
*/
void leArgumentosParque(){
    char nome[NOME], c;
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
        criaParque(nome, cap, val_15, val_1h, val_max, &lista_parques);
    }else{
        mustrarParques();
    }
}

/**
    segunda implementação______________________________________________________________
*/

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
    verifica se pode adicionar o carro
    @param matricula do carro
    @return inteiro 1 caso a matricula seja válida 0 caso falhe
*/
int matriculaValida(char matricula[9]){
    int letras = 0, digitos = 0, pare_letras = 0, pare_digitos = 0, i;

    for (i = 0; i < 9; i++) {
        if (matricula[i] == '-' || matricula[i] == '\0'){
            // Verifica se ha pares ou se nao ha
            if (letras == 2){
                pare_letras++;
            }else if (digitos == 2){
                pare_digitos++;
            }else{
                return FALSE;
            }
            letras = 0;
            digitos = 0;
        }
        // Verifica se o caractere eh uma letra
        else if (isalpha(matricula[i]))
            letras++;
        // Verifica se o caractere eh um digito
        else if (isdigit(matricula[i]))
            digitos++;
        // Se nao for letra nem digito, a matricula eh invalida
        else return FALSE;
    }
    if(pare_letras >= 1 && pare_digitos >= 1)
        return TRUE;
    else return FALSE;
}

/**
    verifica se o carro ja está em algum parque
    @param matricula do carro
    @param lista_parques vetor para a lista dos parques
    @return inteiro 1 caso a matricula seja válida 0 caso falhe
*/
int carroNumParque(Parque *lista_parques,char *matricula){
    Parque *aux = lista_parques;
    Carro *aux_carro = aux->carros;

    while (aux != NULL) {
        while (aux_carro != NULL) {
            if (strcmp(aux_carro->matricula, matricula) == 0) {
                return TRUE;
            }
            if (strcmp(aux_carro->matricula, matricula) > 0) {
                break;
            }
            aux_carro = aux_carro->prox;
        }
        aux = aux->prox;
    }
    return FALSE;
}

/**
    verifica se a data e valida
    @param data a data de entrada
    @param ult_data ultima data que foi inserida
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int dataValida(Data data, Data ult_data){
    int n;
    if (data.ano != ult_data.ano)
        n = (data.ano > ult_data.ano) ? TRUE : FALSE;
    else if (data.mes != ult_data.mes)
        n = (data.mes > ult_data.mes) ? TRUE : FALSE;
    else if (data.dia != ult_data.dia)
        n = (data.dia > ult_data.dia) ? TRUE : FALSE;
    else if (data.hora != ult_data.hora)
        n = (data.hora > ult_data.hora) ? TRUE : FALSE;
    else if (data.minuto != ult_data.minuto)
        n = (data.minuto > ult_data.minuto) ? TRUE : FALSE;
    return n;
}

/**
    verifica se pode adicionar o carro
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
    @param lista_parques vetor para a lista dos parques
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int podeAdicionarCarro(char nome_par[NOME], char matricula[9], Data data, Parque *lista_parques){
    int i = TRUE;
    Parque *aux = lista_parques;

    while (aux != NULL) {
        if (strcmp(aux->nome, nome_par) == 0) {
            i = FALSE;
            break;
        }
        aux = aux->prox;
    }
    if (i){
        printf("%s: no such parking.\n", nome_par);
        return FALSE;
    }
    if (aux->livres == 0){
        printf("%s: parking is full.\n", nome_par);
        return FALSE;
    }
    if (!(matriculaValida(matricula))){
        printf("%s: invalid licence plate.\n", matricula);
        return FALSE;
    }
    if (carroNumParque(lista_parques, matricula)){
        printf("%s: invalid vehicle entry.\n", matricula);
        return FALSE;
    }
    if (!(dataValida(data, ult_data))){
        printf("invalid date.\n");
        return FALSE;
    }
    atualizarData(data);
    return TRUE;
}

/**
    adiciona um carro ao parque caso não acha erros
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
*/
void adicionaCarro(char nome_par[NOME], char matricula[9], Data data){
    int n;

    n = podeAdicionarCarro(nome_par, matricula, data, lista_parques);

    if (n){
        
    }
}

/**
    Le o input do utilizador para o comando e
*/
void leArgumentosEntrada(){
    char nome_par[NOME], c, matricula[9];
    Data data;

    while ((c = getchar()) == ' ');
    ungetc(c, stdin);
    if((c = getchar()) == '"'){
        ungetc(c, stdin);
        scanf("\"%[^\"]\"%s", nome_par, matricula);
    }else{
        ungetc(c, stdin);
        scanf("%s%s", nome_par, matricula);
    }
    scanf("%d-%d-%d %d:%d",&data.dia,&data.mes,&data.ano,&data.hora,&data.minuto);
    adicionaCarro(nome_par, matricula, data);
}

/**
    Le o input do utilizador para direionar para a funcao certa
    @return retorna sempre 0
*/
int main(){
    char c;
    ult_data.ano = 0;

    while ((c = getchar()) != 'q'){
        switch(c){
            case 'p':
            leArgumentosParque();
            break;
            case 'e':
            leArgumentosEntrada();
            break;
            case 's':
            break;
            case 'v':
            break;
            case 'f':
            break;
            case 'r':
            break;
        }
    }

    // funcão para limpar

    return 0;
}