/**
 * projecto de IAED em C.
 * @file main.c
 * @author ist1106426
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAXPARQUES 20 // numero maximo de parques
#define FALSE 0
#define TRUE 1
#define ERROPARQUEEXISTE ": parking already exists."
#define ERROCAPACIDADE ": invalid capacity."
#define ERROVALOR ": invalid cost."
#define ERROPARQUES ": too many parks."
#define ERROVEICULO ": invalid vehicle entry."
#define ERRODATA "invalid date."
#define ERROVEICULOSAIDA ": invalid vehicle exit."
#define ERROPARQUENAOEXITE ": no such parking."
#define ERROMATRICULAINVALIDA ": invalid licence plate."
#define ERROESTACIONAMENTOCHEIO ": parking is full."
#define ERROHISTORICO ": no entries found in any parking."

typedef struct {
    int dia;
    int mes;
    int ano;
    int hora;
    int minuto;
} Data;

typedef struct Historico{
    Data data;
    float valor;
    struct Historico *prox;
} Historico;

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
    Carro *ultimo_carro; // ponteiro para o ultimo carro
    Historico *historico; // historico de faturacao
    Historico *ultimo_historico; // ponteiro para o ultimo historico
    struct Parque *prox; // ponteiro para o proximo parque
} Parque;

Parque *lista_parques = NULL; // ponteir para lista de parques
int num_parques = 0; // numero de parques criados
int dias_mes[13] = {0 ,31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
Data ult_data; // ultima data usada

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
    @return ponteiro para o parque ou NULL caso não exista
*/
Parque* parqueExiste(char nome[BUFSIZ]){
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
int verificaParque(char nome[BUFSIZ], int cap, float val_15, float val_1h, float val_max){
    if(parqueExiste(nome) != NULL){
        printf("%s%s\n", nome, ERROPARQUEEXISTE);
        return FALSE;
    }
    if (cap <= 0) {
        printf("%d%s\n", cap, ERROCAPACIDADE);
        return FALSE;
    }
    if ((val_15 > val_1h) || (val_1h > val_max)) {
        printf("%s\n", ERROVALOR);
        return FALSE;
    }
    if (num_parques >= MAXPARQUES) {
        printf("%s\n", ERROPARQUES);
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
void criaParque(char nome[BUFSIZ], int cap, float val_15, float val_1h, float val_max){
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
        parque_novo->ultimo_carro = NULL;
        parque_novo->historico = NULL;
        parque_novo->ultimo_historico = NULL;

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
    verifica se a data e valida
    @param data a data de entrada
    @param ult_data ultima data que foi inserida
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int dataValida(Data data){
    int n;

    // Verifica se o mês eh valido
    if (data.mes < 1 || data.mes > 12)
        return FALSE;
    // Verifica se o dia e valido
    if (data.dia < 1 || data.dia > dias_mes[data.mes])
        return FALSE;
    // Verifica se a data nova eh maior que a ultima data
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
int podeAdicionarCarro(char nome_par[BUFSIZ], char matricula[9], Data data){
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
        printf("%s%s\n", matricula, ERROMATRICULAINVALIDA);
        return FALSE;
    }
    if (carroNumParque(matricula)){
        printf("%s%s\n", matricula, ERROVEICULO);
        return FALSE;
    }
    if (!(dataValida(data))){ 
        printf("%s\n", ERRODATA);
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
void adicionaListaCarros(char nome_par[BUFSIZ], char matricula[9], Data data){
    Parque *aux = lista_parques;
    Carro *carro_novo = (Carro*)malloc(sizeof(Carro));

    aux = parqueExiste(nome_par);
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
    if (aux->carros == NULL){
        aux->carros = carro_novo;
    } else {
        aux->ultimo_carro->prox = carro_novo;
    }
    aux->ultimo_carro = carro_novo;
}

/**
    Le o input do utilizador para o comando e
*/
void leArgumentosEntrada(){
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
int podeRegistarSaida(char nome_par[BUFSIZ], char matricula[9], Data data){
    Parque *aux = lista_parques;

    aux = parqueExiste(nome_par);
    if (aux == NULL){
        printf("%s: no such parking.\n", nome_par);
        return FALSE;
    }
    if (!(matriculaValida(matricula))){
        printf("%s%s\n", matricula, ERROMATRICULAINVALIDA);
        return FALSE;
    }
    if (!(carroNumParque(matricula))){
        printf("%s: invalid vehicle exit.\n", matricula);
        return FALSE;
    }
    if (!(dataValida(data))){ 
        printf("%s\n", ERRODATA);
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
    adiciona um historico ah lista de historico do parque
    @param aux parque
    @param data a data de entrada
    @param valor valor a pagar
*/
void adicionaHistorico(Parque *aux, Data data, float valor){
    Historico *historico_novo;

    if(aux->historico == NULL){
        historico_novo = (Historico*)malloc(sizeof(Historico));
        historico_novo->data.ano = data.ano;
        historico_novo->data.mes = data.mes;
        historico_novo->data.dia = data.dia;
        historico_novo->valor = valor;
        historico_novo->prox = NULL;
        aux->historico = historico_novo;
        aux->ultimo_historico = historico_novo;
    } else if(dataIgual(aux->ultimo_historico->data, data)){
        aux->ultimo_historico->valor += valor;
    } else {
        historico_novo = (Historico*)malloc(sizeof(Historico));
        historico_novo->data.ano = data.ano;
        historico_novo->data.mes = data.mes;
        historico_novo->data.dia = data.dia;
        historico_novo->valor = valor;
        historico_novo->prox = NULL;
        aux->ultimo_historico->prox = historico_novo;
        aux->ultimo_historico = historico_novo;
    }
}

/**
    Regista a saida do carro pondo a data de saida e valor pago
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de saida
*/
void RegistaSaida(char nome_par[BUFSIZ], char matricula[9], Data data){
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
    adicionaHistorico(aux, aux_carro->data_s, valor_pago);

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
    char nome_par[BUFSIZ], c, matricula[9];
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
    quarta implementação____________________________________________________________________________________
*/

/**
    da printf da lista de carros existentes por ordem de parque e de entrada
*/
void mustrarCarro(){
    Parque *aux = lista_parques;
    Carro *aux_carro;
    char matricula[9];
    int i = TRUE;

    scanf("%s", matricula);

    if (!(matriculaValida(matricula))){
        printf("%s%s\n", matricula, ERROMATRICULAINVALIDA);
    }else{
        while (aux != NULL) {
            aux_carro = aux->carros;
            while (aux_carro != NULL) {
                if(strcmp(aux_carro->matricula, matricula)==0 && aux_carro->data_s.ano!=0){
                    i = FALSE;
                    printf("%s ", aux->nome);
                    printf("%02d-%02d-%d ", aux_carro->data_e.dia, aux_carro->data_e.mes, aux_carro->data_e.ano);
                    printf("%02d:%02d ", aux_carro->data_e.hora, aux_carro->data_e.minuto);
                    printf("%02d-%02d-%d ", aux_carro->data_s.dia, aux_carro->data_s.mes, aux_carro->data_s.ano);
                    printf("%02d:%02d\n", aux_carro->data_s.hora, aux_carro->data_s.minuto);
                }else if(strcmp(aux_carro->matricula, matricula)==0 && aux_carro->data_s.ano==0){
                    i = FALSE;
                    printf("%s ", aux->nome);
                    printf("%02d-%02d-%d ", aux_carro->data_e.dia, aux_carro->data_e.mes, aux_carro->data_e.ano);
                    printf("%02d:%02d\n", aux_carro->data_e.hora, aux_carro->data_e.minuto);
                    break;
                }
                aux_carro = aux_carro->prox;
            }
            aux = aux->prox;
        }
        if (i){
            printf("%s: no entries found in any parking.\n", matricula);
        }
    }
}

/**
    quinta implementação____________________________________________________________________________________
*/

int validarDataAnterior(Data data){
    // Verifica se o mes eh valido
    if (data.mes < 1 || data.mes > 12)
        return FALSE;
    // Verifica se o dia e valido
    if (data.dia < 1 || data.dia > dias_mes[data.mes])
        return FALSE;
    // Verifica se a data nova eh menor que a ultima data
    if (data.ano != ult_data.ano)
        return (data.ano <= ult_data.ano) ? TRUE : FALSE;
    else if (data.mes != ult_data.mes)
        return (data.mes <= ult_data.mes) ? TRUE : FALSE;
    else if (data.dia != ult_data.dia)
        return (data.dia <= ult_data.dia) ? TRUE : FALSE;
    return TRUE;
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

/**
    cria uma lista de carros por ordem de hora de saida
    @param aux_carro carro a adicionar
    @param lista_carros vetor para lista de carros
*/
void criaListaOrd(Carro *aux_carro,Carro **lista_carros){
    Carro *aux_ant, *aux_pos, *carro = (Carro*) malloc(sizeof(Carro));
    
    strcpy(carro->matricula, aux_carro->matricula);
    carro->data_s.hora = aux_carro->data_s.hora;
    carro->data_s.minuto = aux_carro->data_s.minuto;
    carro->valor_pago = aux_carro->valor_pago;
    carro->prox = NULL;
    if(*lista_carros == NULL){
        *lista_carros = carro;
    }else{
        aux_ant = *lista_carros;
        aux_pos = (*lista_carros)->prox;
        while(aux_pos != NULL){
            if(horaMaior(aux_pos->data_s, aux_carro->data_s)){
                aux_ant->prox = carro;
                carro->prox = aux_pos;
                break;
            }else{
                aux_ant = aux_pos;
                aux_pos = aux_pos->prox;
            }
        }
        if(aux_pos == NULL){
            if(horaMaior(aux_ant->data_s, aux_carro->data_s)){
                *lista_carros = carro;
                carro->prox = aux_ant;
            }else{
                aux_ant->prox = carro;
            }
        }
    }
}

/**
    da printf da lista de carros existentes por ordem de parque e de entrada
    @param lista_carros lista de carros
*/
void mustraAlista(Carro *lista_carros){
    Carro *aux = lista_carros;

    while (aux != NULL) {
        printf("%s ", aux->matricula);
        printf("%02d:%02d ", aux->data_s.hora, aux->data_s.minuto);
        printf("%.2f\n", aux->valor_pago);
        aux = aux->prox;
    }
}

/**
    da free a lista de carros
    @param lista_carros lista de carros
*/
void darFreeListaOrd(Carro *lista_carros){
    Carro *aux = lista_carros, *aux_pos = lista_carros;

    while (aux_pos != NULL) {
        aux_pos = aux->prox;
        free(aux);
        aux = aux_pos;
    }
}

/**
    guia para a funcao mustrarFaturaCarros caso não de erro
    @param matricula do carro
    @param data a data a consultar
*/
void mustrarFaturaCarros(char nome_par[BUFSIZ], Data data){
    Parque *aux = lista_parques;
    Carro *aux_carro = NULL, *lista_carros = NULL;

    aux = parqueExiste(nome_par);
    if (aux == NULL){
        printf("%s: no such parking.\n", nome_par);
    }else if (!(validarDataAnterior(data))){
        printf("%s\n", ERRODATA);
    }else{
        aux_carro = aux->carros;
        while (aux_carro != NULL) {
            if(dataIgual(data, aux_carro->data_s)){
                criaListaOrd(aux_carro, &lista_carros);
            }
            aux_carro = aux_carro->prox;
        }
        if (lista_carros != NULL) { 
            mustraAlista(lista_carros); 
            darFreeListaOrd(lista_carros); 
        }
    }
}

/**
    mostra o historico se não der erro
    @param nome_par nome do parque
*/
void mustrarHistorico(char nome_par[BUFSIZ]){
    Parque *aux = lista_parques;
    Historico *aux_historico;

    aux = parqueExiste(nome_par);
    if (aux == NULL){
        printf("%s: no such parking.\n", nome_par);
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
void leArgumentosFaturacao(){
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
        mustrarFaturaCarros(nome_par, d);
    }else{
        mustrarHistorico(nome_par);
    }
}

/**
    sexta implementação____________________________________________________________________________________
*/

/**
    remove as listas ligadas ao parque
    @param parque ponteiro para o parque a eliminar
*/
void removeListas(Parque *parque){
    Carro *aux_carro_ant = parque->carros, *aux_carro_pos = parque->carros;
    Historico *auxHis_ant = parque->historico, *auxHis_pos = parque->historico;

    while (aux_carro_pos != NULL) {
        aux_carro_pos = aux_carro_ant->prox;
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
void removeParque(Parque *parque){
    Parque *aux = lista_parques, *aux_ant = lista_parques;

    if (parque == lista_parques){
        lista_parques = parque->prox;
    }else{
        while (aux != parque) {
            aux_ant = aux;
            aux = aux->prox;
        }
        aux_ant->prox = parque->prox;
    }
    removeListas(parque);
    free(parque->nome);
    free(parque);
    num_parques--;
}

/**
    da printf da lista de parques existentes por ordem de colucacao
*/
void mustrarParques2(){
    Parque *aux = lista_parques;
    while (aux != NULL) {
        printf("%s\n", aux->nome);
        aux = aux->prox;
    }
}

/**
    le o nome do parque a remover
*/
void leArgumentosRemove(){
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
    aux = parqueExiste(nome_par);
    if (aux == NULL){
        printf("%s: no such parking.\n", nome_par);
    } else {
        removeParque(aux);
    }
    mustrarParques2();
}

/**
    sétima implementação____________________________________________________________________________________
*/
void limparTudo(){
    Parque *aux = lista_parques, *aux_ant = lista_parques;

    while (aux != NULL) {
        aux_ant = aux;
        aux = aux->prox;
        removeListas(aux_ant);
        free(aux_ant->nome);
        free(aux_ant);
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
            mustrarCarro();
            break;
            case 'f':
            leArgumentosFaturacao();
            break;
            case 'r':
            leArgumentosRemove();
            break;
        }
    }
    limparTudo();

    return 0;
}