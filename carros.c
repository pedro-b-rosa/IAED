/**
 * funções para alterar e mostrar as listas dos carros.
 * @file carros.c
 * @author ist1106426
*/

#include "carros.h"

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
    verifica se pode adicionar o carro
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
    @param lista_parques ponteiro para a lista dos parques
    @return inteiro TRUE caso passe nos criterios ou FALSE caso nao passe
*/
int podeAdicionarCarro(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques){
    Parque *aux = lista_parques;

    aux = parqueExiste(nome_par, lista_parques);
    if (aux == NULL){
        printf("%s%s\n", nome_par,  ERROPARQUENAOEXISTE);
        return FALSE;
    }
    if (aux->livres == 0){
        printf("%s%s\n", nome_par, ERROESTACIONAMENTOCHEIO);
        return FALSE;
    }
    if (!(matriculaValida(matricula))){
        printf("%s%s\n", matricula, ERROMATRICULAINVALIDA);
        return FALSE;
    }
    if (procuraCarro(matricula) != NULL){
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
void adicionaListaCarros(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques){
    Parque *aux = lista_parques;
    Carro *carro_novo = (Carro*)malloc(sizeof(Carro));

    aux = parqueExiste(nome_par, lista_parques);
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
    insereCarro(carro_novo);
}

/**
    verifica se pode adicionar o carro
    @param nome_par nome do parque
    @param matricula do carro
    @param data a data de entrada
    @param lista_parques ponteiro para a lista dos parques
    @return ponteiro para o carro ou NULL caso não exista
*/
Carro* podeRegistarSaida(char nome_par[BUFSIZ], char matricula[9], Data data, Parque *lista_parques){
    Parque *aux;
    Carro* carro;

    aux = parqueExiste(nome_par, lista_parques);
    if (aux == NULL){
        printf("%s%s\n", nome_par, ERROPARQUENAOEXISTE);
        return NULL;
    }
    if (!(matriculaValida(matricula))){
        printf("%s%s\n", matricula, ERROMATRICULAINVALIDA);
        return NULL;
    }
    if ((carro = procuraCarro(matricula)) == NULL){
        printf("%s%s\n", matricula, ERROVEICULOSAIDA);
        return NULL;
    }
    if (!(dataValida(data))){ 
        printf("%s\n", ERRODATA);
        return NULL;
    }
    atualizarData(data);
    return carro;
}

/**
    Calcula o valor a pagar pelo carro
    @param minutos numero de minutos que o carro esteve no parque
    @param v_15 valor a pagar pelo parque na primeira hora
    @param v_1hora valor a pagar depois da primeira hora
    @param v_max valor maximo diario
    @return o valor a pagar pelo carro
*/
float valorAPagar(Data data_e, Data data_s, float v_15, float v_1hora, float v_max){
    float valor = 0;
    int dias, minutosResto;
    long int minutos;

    minutos = minutosEntreDatas(data_e, data_s);
    minutosResto = minutos%(24*60);
    dias = minutos/(24*60);

    if (minutosResto <= 60){
        valor += v_15*(minutosResto/15);
        if (minutosResto%15 != 0)
            valor += v_15;
    }else{
        valor += v_15*4;
        minutosResto -= 60;
        valor += v_1hora*(minutosResto/15);
        if (minutosResto%15 != 0)
            valor += v_1hora;
        if (valor > v_max)
            valor = v_max;
    }
    valor += dias*v_max;
    return valor;
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
    @param carro ponteiro para o carro
    @param data a data de saida
    @param lista_parques ponteiro para a lista dos parques
    @param nome_par nome do parque
    @param data a data de saida
*/
void RegistaSaida(Carro *carro, Data data, Parque *lista_parques, char nome_par[BUFSIZ]){
    Parque *aux = lista_parques;
    float valor_pago;

    aux = parqueExiste(nome_par, lista_parques);
    aux->livres++;

    carro->data_s.ano = data.ano;
    carro->data_s.mes = data.mes;
    carro->data_s.dia = data.dia;
    carro->data_s.hora = data.hora;
    carro->data_s.minuto = data.minuto;
    valor_pago=valorAPagar(carro->data_e,carro->data_s,aux->valor_15,aux->valor_1hora,aux->valor_max);
    carro->valor_pago = valor_pago;
    adicionaHistorico(aux, carro->data_s, valor_pago);
    removeCarro(carro->matricula);

    printf("%s ", carro->matricula);
    printf("%02d-%02d-%d ",carro->data_e.dia,carro->data_e.mes,carro->data_e.ano);
    printf("%02d:%02d ", carro->data_e.hora, carro->data_e.minuto);
    printf("%02d-%02d-%d ",carro->data_s.dia,carro->data_s.mes,carro->data_s.ano);
    printf("%02d:%02d ", carro->data_s.hora, carro->data_s.minuto);
    printf("%.2f\n", valor_pago);
}

/**
    funcao que procura e dá print dos carros 
    @param lista_ord vetor para lista de parques
    @param lista_parques ponteiro para a lista dos parques
*/
int procuraEMostraCarro(char matricula[9], Parque *lista_parques){
    Parque *lista_ord = NULL, *aux;
    Carro *aux_carro;
    int i = TRUE;
    criaListaOrdParques(&lista_ord, lista_parques);
    while (lista_ord != NULL) {
        aux_carro = lista_ord->carros;
        while (aux_carro != NULL) {
            if(strcmp(aux_carro->matricula, matricula)==0){
                i = FALSE;
                printf("%s ", lista_ord->nome);
                printf("%02d-%02d-%d ", aux_carro->data_e.dia, aux_carro->data_e.mes, aux_carro->data_e.ano);
                printf("%02d:%02d", aux_carro->data_e.hora, aux_carro->data_e.minuto);
                if (aux_carro->data_s.ano!=0){
                    printf(" %02d-%02d-%d ", aux_carro->data_s.dia, aux_carro->data_s.mes, aux_carro->data_s.ano);
                    printf("%02d:%02d\n", aux_carro->data_s.hora, aux_carro->data_s.minuto);
                } else {
                    printf("\n");
                    break;
                }
            }
            aux_carro = aux_carro->prox;
        }
        aux = lista_ord;
        lista_ord = lista_ord->prox;
        free(aux);
    }
    return i;
}

/**
    da printf da lista de carros existentes por ordem de parque e de entrada
    @param lista_parques ponteiro para a lista dos parques
*/
void mostrarCarro(Parque *lista_parques){
    char matricula[9];

    scanf("%s", matricula);

    if (!(matriculaValida(matricula))){
        printf("%s%s\n", matricula, ERROMATRICULAINVALIDA);
    }else{
        if (procuraEMostraCarro(matricula, lista_parques)){
            printf("%s%s\n", matricula, ERROHISTORICO);
        }
    }
}

void insereCarroNoMeio(Carro *aux_carro, Carro **lista_carros, Carro *carro){
    Carro *aux_ant, *aux_pos;
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

/**
    cria uma lista de carros por ordem de hora de saida
    @param aux_carro carro a adicionar
    @param lista_carros vetor para lista de carros
*/
void criaListaOrd(Carro *aux_carro,Carro **lista_carros){
    Carro *aux_ant, *carro = (Carro*) malloc(sizeof(Carro));
    
    strcpy(carro->matricula, aux_carro->matricula);
    carro->data_s.hora = aux_carro->data_s.hora;
    carro->data_s.minuto = aux_carro->data_s.minuto;
    carro->valor_pago = aux_carro->valor_pago;
    carro->prox = NULL;
    aux_ant = *lista_carros;
    if(*lista_carros == NULL){
        *lista_carros = carro;
    }else if (horaMaior((*lista_carros)->data_s, aux_carro->data_s)){
            *lista_carros = carro;
            carro->prox = aux_ant;
    }else{
        insereCarroNoMeio(aux_carro, lista_carros, carro);
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
    guia para a funcao mostrarFaturaCarros caso não de erro
    @param matricula do carro
    @param data a data a consultar
*/
void mostrarFaturaCarros(char nome_par[BUFSIZ], Data data, Parque *lista_parques){
    Parque *aux = lista_parques;
    Carro *aux_carro = NULL, *lista_carros = NULL;

    aux = parqueExiste(nome_par, lista_parques);
    if (aux == NULL){
        printf("%s%s\n", nome_par,  ERROPARQUENAOEXISTE);
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