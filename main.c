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
    Data data_e; // data de entrada
    Data data_s; // data de saida
    float valor_pago; // valor pago
    struct Carro *prox; // ponteiro para o proximo carro
} Carro;

typedef struct Parque{
    char* nome; // nome do parque
    int cap; // capacidade do parque
    int livres; // lugares livres
    float valor_15; // valor a pagar pelo parque na primeira hora por 15 min
    float valor_1hora; // valor a pagar depois da primeira hora por 15 min
    float valor_max; // valor maximo diario
    Carro *carros; // lista de carros
    struct Parque *prox; // ponteiro para o proximo parque
} Parque;

Parque *lista_parques = NULL; // ponteir para lista de parques
int num_parques = 0; // numero de parques criados
Data ult_data; // ultima data usada

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
    procura se o parque existe
    @param vetor do nome para ser transformado
    @return ponteiro para o parque ou NULL caso não exista
*/
Parque* parqueExiste(char nome[NOME]){
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
    @return inteiro 1 caso verifique, 0 caso falhe
 */
int verificaParque(char nome[NOME], int cap, float val_15, float val_1h, float val_max){
    if(parqueExiste(nome) != NULL){
        printf("%s: parking already exists.\n", nome);
        return FALSE;
    }
    if (cap <= 0) {
        printf("%d: invalid capacity.\n",cap);
        return FALSE;
    }
    if ((val_15 > val_1h) || (val_1h > val_max)) {
        printf("invalid cost.\n");
        return FALSE;
    }
    if (num_parques >= MAXPARQUES) {
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
void criaParque(char nome[NOME], int cap, float val_15, float val_1h, float val_max){
    int verifica;
    char *nome_novo;
    Parque *parque_novo = (Parque *) malloc(sizeof(Parque)), *aux;

    verifica = verificaParque(nome,cap,val_15,val_1h,val_max);
    nome_novo = criaNome(nome);
    if (verifica) {
        parque_novo->nome = nome_novo;
        parque_novo->cap = cap;
        parque_novo->livres = cap;
        parque_novo->valor_15 = val_15;
        parque_novo->valor_1hora = val_1h;
        parque_novo->valor_max = val_max;
        parque_novo->prox = NULL;
        parque_novo->carros = NULL;

        if (lista_parques == NULL) {
            lista_parques = parque_novo;
        } else {
            aux = lista_parques;
            while (aux->prox != NULL) {
                aux = aux->prox;
            }
            aux->prox = parque_novo;
        }
        num_parques++;
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
        criaParque(nome, cap, val_15, val_1h, val_max);
    }else{
        mustrarParques();
    }
}

/**
    segunda implementação___________________________________________________________________________________
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
    @param matr matricula do carro
    @return inteiro 1 caso a matricula seja válida 0 caso falhe
*/
int carroNumParque(char *matr){
    Parque *aux = lista_parques;
    Carro *aux_carro;

    while (aux != NULL) {
        aux_carro = aux->carros;
        while (aux_carro != NULL) {
            if(strcmp(aux_carro->matricula, matr)==0 && aux_carro->data_s.ano==0){
                return TRUE;
            }
            aux_carro = aux_carro->prox;
        }
        aux = aux->prox;
    }
    return FALSE;
}

/**
    verifica se a data e maior que a ultima data
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
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int podeAdicionarCarro(char nome_par[NOME], char matricula[9], Data data){
    Parque *aux = lista_parques;

    aux = parqueExiste(nome_par);
    if (aux == NULL){
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
    if (carroNumParque(matricula)){
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
    adiciona um carro ah lista de carros do parque
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
*/
void adicionaListaCarros(char nome_par[NOME], char matricula[9], Data data){
    Parque *aux = lista_parques;
    Carro *aux_carro,*carro_novo = (Carro*)malloc(sizeof(Carro));

    aux = parqueExiste(nome_par);
    aux_carro = aux->carros;
    aux->livres--;
    printf("%s %d\n", aux->nome, aux->livres);

    strcpy(carro_novo->matricula,matricula);
    carro_novo->data_e.ano = data.ano;
    carro_novo->data_e.mes = data.mes;
    carro_novo->data_e.dia = data.dia;
    carro_novo->data_e.hora = data.hora;
    carro_novo->data_e.minuto = data.minuto;
    carro_novo->data_s.ano = 0;
    carro_novo->valor_pago = 0;
    carro_novo->prox = NULL;
    if (aux_carro == NULL){
        aux->carros = carro_novo;
    } else {
        while(aux_carro->prox != NULL){
            aux_carro = aux_carro->prox;
        }
        aux_carro->prox = carro_novo;
    }
}

/**
    Le o input do utilizador para o comando e
*/
void leArgumentosEntrada(){
    char nome_par[NOME], c, matricula[9];
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

    if (podeAdicionarCarro(nome_par, matricula, d)){
        adicionaListaCarros(nome_par, matricula, d);
    }
}

/**
    terceira implementação____________________________________________________________________________________
*/

/**
    verifica se pode adicionar o carro
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int podeRegistarSaida(char nome_par[NOME], char matricula[9], Data data){
    Parque *aux = lista_parques;

    aux = parqueExiste(nome_par);
    if (aux == NULL){
        printf("%s: no such parking.\n", nome_par);
        return FALSE;
    }
    if (!(matriculaValida(matricula))){
        printf("%s: invalid licence plate.\n", matricula);
        return FALSE;
    }
    if (!(carroNumParque(matricula))){
        printf("%s: invalid vehicle exit.\n", matricula);
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
    Calcula o numero de minutos entre duas datas
    @param data_e a data de entrada
    @param data_s a data de saida
    @return o numero de minutos entre as duas datas
*/
long int minutosEntreDatas(Data data_e, Data data_s){
    int dias_mes[13] = {0 ,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    long int t = 0, conta = 0;

    while(data_e.dia != data_s.dia || data_e.mes != data_s.mes || data_e.ano != data_s.ano){
        conta++;
        data_e.dia++;

        if(data_e.dia > dias_mes[data_e.mes]){
            data_e.dia = 1;
            data_e.mes++;
            if (data_e.mes > 12){
                data_e.mes = 1;
                data_e.ano++;
            }
        }
    }
    t += conta*24*60+(data_s.hora-data_e.hora)*60+data_s.minuto-data_e.minuto;

    return t;
}

/**
    Calcula o valor a pagar pelo carro
    @param minutos numero de minutos que o carro esteve no parque
    @param v_15 valor a pagar pelo parque na primeira hora
    @param v_1hora valor a pagar depois da primeira hora
    @param v_max valor maximo diario
    @return o valor a pagar pelo carro
*/
float valorAPagar(long int minutos, float v_15, float v_1hora, float v_max){
    float valor = 0;
    int dias = minutos/(24*60);
    int minutosResto = minutos%(24*60);

    valor = dias*v_max;
    if (minutosResto <= 60){
        valor += v_15*(minutosResto/15);
    }else{
        valor += v_15*4;
        minutosResto -= 60;
        valor += v_1hora*(minutosResto/15);
    }

    return valor;
}

/**
    Regista a saida do carro pondo a data de saida e valor pago
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de saida
*/
void RegistaSaida(char nome_par[NOME], char matricula[9], Data data){
    Parque *aux = lista_parques;
    Carro *aux_carro;
    float valor_pago;
    long int min;

    aux = parqueExiste(nome_par);
    aux_carro = aux->carros;
    while (aux_carro != NULL) {
        if(strcmp(aux_carro->matricula, matricula)==0 && aux_carro->data_s.ano==0){
            break;
        }
        aux_carro = aux_carro->prox;
    }
    aux->livres++;

    aux_carro->data_s.ano = data.ano;
    aux_carro->data_s.mes = data.mes;
    aux_carro->data_s.dia = data.dia;
    aux_carro->data_s.hora = data.hora;
    aux_carro->data_s.minuto = data.minuto;
    min = minutosEntreDatas(aux_carro->data_e, aux_carro->data_s);
    valor_pago=valorAPagar(min,aux->valor_15,aux->valor_1hora,aux->valor_max);
    aux_carro->valor_pago = valor_pago;

    printf("%s ", aux_carro->matricula);
    printf("%02d-%02d-%d ", aux_carro->data_e.dia, aux_carro->data_e.mes, aux_carro->data_e.ano);
    printf("%02d:%02d ", aux_carro->data_e.hora, aux_carro->data_e.minuto);
    printf("%02d-%02d-%d ", aux_carro->data_s.dia, aux_carro->data_s.mes, aux_carro->data_s.ano);
    printf("%02d:%02d ", aux_carro->data_s.hora, aux_carro->data_s.minuto);
    printf("%.2f\n", valor_pago);
}

/**
    Le o input do utilizador para o comando s
*/
void leArgumentosSaida(){
    char nome_par[NOME], c, matricula[9];
    Data d; // data de saida

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
    if (podeRegistarSaida(nome_par, matricula, d)){
        RegistaSaida(nome_par, matricula, d);
    }
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
            leArgumentosSaida();
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