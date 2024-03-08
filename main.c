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

typedef struct Carro{
    char matricula[9]; // AA-00-AA\0AXPARQUES 20
    char entry_data[11]; // DD-MM-YYYY\0
    char entry_hora[6]; // HH:MM\0
    char exit_data[11];
    char exit_hora[6];
    float total_cost;
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
char ult_data[11];
char ult_hora[6];

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
 * Verifica se o parque pode ser adicionado.
 * @param nome vetor com os caracteres do nome
 * @param cap capacidade do parque
 * @param val_15 valores a pagar pelo parque na primeira hora
 * @param val_1h valores a pagar pelo parque depois da primeira hora
 * @param val_max valore maximo diario
 * @param lista_parques vetor para a lista dos parques
 * @return inteiro 1 caso verifique, 0 caso falhe
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
 * Cria um parque e coloca-o na lista.
 * @param nome vetor com os caracteres do nome
 * @param cap capacidade do parque
 * @param val_15 valores a pagar pelo parque na primeira hora
 * @param val_1h valores a pagar pelo parque depois da primeira hora
 * @param val_max valore maximo diario
 * @param lista_parques vetor para a lista dos parques
 * @param val_15 valores a pagar pelo parque
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
    @return inteiro 1 caso a matricula seja válida 0 caso falhe
*/
int carroNumParque(){
    
}

/**
    verifica se pode adicionar o carro
    @param nome do parque, matricula do carro, data e hora de entrada
    @return inteiro com a posicao do parque na lista ou -1 caso falhe
*/
int podeAdicionarCarro(char nome_par[NOME], char matricula[9], char data[11], char hora[6]){
    int i;

    for(i = 0; i < parques_utilizados; i++){
        if (strcmp(lista_p[i].nome, nome_par) == 0){
            break;
        }
    }
    if (i == parques_utilizados){
        printf("%s: no such parking.\n", nome_par);
        return -1;
    }
    if (lista_p[i].livres == 0){
        printf("%s: parking is full.\n", nome_par);
        return -1;
    }
    if (!(matriculaValida(matricula))){
        printf("%s: invalid licence plate.", matricula);
        return -1;
    }
    if (!(carroNumParque())){
        printf("%s: invalid vehicle entry.", matricula);
        return -1;
    }

    return i;
}

/**
    adiciona um carro ao parque caso não acha erros
    @param nome_par nome do parque, matricula do carro, data e hora de entrada
*/
void adicionaCarro(char nome_par[NOME], char matricula[9], char data[11],char hora[6]){
    int n;

    n = podeAdicionarCarro(nome_par, matricula, data, hora);

    if (n != -1){

    }
}

/**
    Le o input do utilizador para o comando e
*/
void leArgumentosEntrada(){
    char nome_par[NOME], c, matricula[9], data[11], hora[6];

    while ((c = getchar()) == ' ');
    ungetc(c, stdin);
    if((c = getchar()) == '"'){
        ungetc(c, stdin);
        scanf("\"%[^\"]\"%s%s%s", nome_par, matricula, data, hora);
    }else{
        ungetc(c, stdin);
        scanf("%s%s%s%s", nome_par, matricula, data, hora);
    }
    adicionaCarro(nome_par, matricula, data, hora);
}

/**
    Le o input do utilizador para direionar para a funcao certa
    @return retorna sempre 0
*/
int main(){
    char c;

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