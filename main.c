#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
*  Autores: Fernando Valentim Torres, Daniel Jorge Manzano, Henrique Vieira Lima
*  Número USP: 15452340, 15446861, 15459372 
*  Curso: Bacharelado em Ciências de Computação
*  Disciplina: SCC0221 - Introdução à Ciência de Computação I
*  Data de entrega: 16/06/2024
*  Descrição do projeto:
*  O projeto consiste em um sistema de gerenciamento de reservas de voos. 
*  O sistema permite a abertura de um voo, a realização, a consulta, o cancelamento e a modificação de reservas, o fechamento do dia e o fechamento do voo.
*/

/*
*  Foi utilizado um struct para armazenar os dados da reserva.
*  O struct possui os seguintes campos:
*  - char* cpf: armazena o CPF do passageiro
*  - char* nome: armazena o nome do passageiro
*  - char* sobrenome: armazena o sobrenome do passageiro
*  - char* dia: armazena o dia da reserva
*  - char* mes: armazena o mês da reserva
*  - char* ano: armazena o ano da reserva
*  - char* id: armazena o ID do voo
*  - char* assento: armazena o assento do passageiro
*  - char* classe: armazena a classe da passagem (economica ou executiva)
*  - char* origem: armazena a origem do voo
*  - char* destino: armazena o destino do voo
*  - float valor: armazena o valor da reserva
*/
typedef struct {
    char* cpf;
    char* nome;
    char* sobrenome;
    char* dia;
    char* mes;
    char* ano;
    char* id;
    char* assento;
    char* classe;
    char* origem;
    char* destino;
    float valor;
} reserva;

/*
* Foi também utilizado um struct para armazenar os dados do avião.
* O struct possui os seguintes campos:
* - int assentos: armazena a quantidade de assentos do avião
* - int reservasTotais: armazena a quantidade total de reservas feitas
* - int reservasAlocadas: armazena a quantidade de reservas alocadas
* - float precoEconomica: armazena o preço da classe economica
* - float precoExecutiva: armazena o preço da classe executiva
* - float valorTotall: armazena o valor total arrecadado
* - reserva* galera: um array de reservas
*/

typedef struct {
    int assentos;
    int reservasTotais;
    int reservasAlocadas;
    float precoEconomica;
    float precoExecutiva;
    float valorTotall;
    reserva * galera;
} aviao;


void lerArquivo(aviao *voo); //Lê o arquivo de registro
void exportarArquivo(aviao* voo); //Exporta o arquivo de registro
void alocaTemp(reserva* pessoa); //Aloca memória para os campos do struct
void liberarReserva(reserva* pessoa); //Libera memória alocada para os campos do struct
int RegistroExiste(); //Verifica se o arquivo de registro existe
int vooFechou(); //Verifica se o voo foi fechado
int consultaCPF(char cpf[15], aviao * voo); //Consulta a reserva via cpf
void aberturaVoo(aviao * voo); //Abre o voo
void realizarReserva(int *vooFechado, aviao * voo); //Realiza a reserva
void consultarReserva(aviao * voo); //consulta e imprime a reserva
void cancelarReserva(aviao *voo); //Cancela a reserva
void modificarReserva(aviao *voo); //Modifica a reserva
void fecharDia(aviao *voo); //Fecha o dia
void fecharVoo(aviao *voo); //Fecha o voo
void printVooFechado(aviao *voo); //Faz o print formatado do voofechado

/*
* Função para ler o arquivo de registro e armazenar essas informações no struct
* @param aviao * voo: ponteiro da struct aviao onde ficarão localizados as informações sobre o voo
* @return void
*/

void lerArquivo(aviao *voo){
    char nomeDoArquivo[20];
    if(RegistroExiste()) {
        strcpy(nomeDoArquivo, "SuperRegistro.dat");
    } else if (vooFechou()){
        strcpy(nomeDoArquivo, "SuperRegistroF.dat");
    } else {
        voo->galera = NULL;
        return;
    }
    FILE *fs = fopen(nomeDoArquivo, "rb");
    fread(&voo->assentos, sizeof(int), 1, fs);
    fread(&voo->reservasTotais, sizeof(int), 1, fs);
    fread(&voo->reservasAlocadas, sizeof(int), 1, fs);
    fread(&voo->precoEconomica, sizeof(float), 1, fs);
    fread(&voo->precoExecutiva, sizeof(float), 1, fs);
    fread(&voo->valorTotall, sizeof(float), 1, fs);
    voo->galera = (reserva *)malloc(voo->reservasAlocadas * sizeof(reserva));
    size_t tmnNome, tmnSobrenome;
    for(int j = 0; j < (voo->reservasTotais); j++) {
        reserva pessoa;
        alocaTemp(&pessoa);
        fread(&tmnNome, sizeof(size_t), 1, fs);
        pessoa.nome = (char *)malloc(tmnNome * sizeof(char));
        fread(pessoa.nome, tmnNome * sizeof(char), 1, fs);
        fread(&tmnSobrenome, sizeof(size_t), 1, fs);
        pessoa.sobrenome = (char *)malloc(tmnSobrenome * sizeof(char));
        fread(pessoa.sobrenome, tmnSobrenome * sizeof(char), 1, fs);
        fread(pessoa.cpf, 15 * sizeof(char), 1, fs);
        fread(pessoa.dia, 3 * sizeof(char), 1, fs);
        fread(pessoa.mes, 3 * sizeof(char), 1, fs);
        fread(pessoa.ano, 5 * sizeof(char), 1, fs);
        fread(pessoa.id, 5 * sizeof(char), 1, fs);
        fread(pessoa.assento, 4 * sizeof(char), 1, fs);
        fread(pessoa.classe, 10 * sizeof(char), 1, fs);
        fread(&pessoa.valor, sizeof(float), 1, fs);
        fread(pessoa.origem, 4 * sizeof(char), 1, fs);
        fread(pessoa.destino, 4 * sizeof(char), 1, fs);
        if (feof(fs)) {
            liberarReserva(&pessoa);
            break;
        }
        voo->galera[j] = pessoa;
    }
    fclose(fs);
}

/*
* Função para exportar as informações do struct para o arquivo de registro
*
* @param aviao * voo: ponteiro da struct aviao onde estão localizados as informações sobre o voo
* @return void
*/

void exportarArquivo(aviao* voo){
    if(RegistroExiste()){
        FILE *fs2 = fopen("SuperRegistroTemp.dat", "wb");
        fwrite(&voo->assentos, sizeof(int), 1, fs2);
        fwrite(&voo->reservasTotais, sizeof(int), 1, fs2);
        fwrite(&voo->reservasAlocadas, sizeof(int), 1, fs2);
        fwrite(&voo->precoEconomica, sizeof(float), 1, fs2);
        fwrite(&voo->precoExecutiva, sizeof(float), 1, fs2);
        fwrite(&voo->valorTotall, sizeof(float), 1, fs2);
        for(int i = 0; i < (voo->reservasTotais); i ++) {
            size_t tmn = strlen(voo->galera[i].nome);
            tmn ++;
            fwrite(&tmn, sizeof(size_t), 1, fs2);
            fwrite(voo->galera[i].nome, tmn * sizeof(char), 1, fs2);
            tmn = strlen(voo->galera[i].sobrenome);
            tmn ++;
            fwrite(&tmn, sizeof(size_t), 1, fs2);
            fwrite(voo->galera[i].sobrenome, tmn * sizeof(char), 1, fs2);
            fwrite(voo->galera[i].cpf, 15 * sizeof(char), 1, fs2);
            fwrite(voo->galera[i].dia, 3 * sizeof(char), 1, fs2);
            fwrite(voo->galera[i].mes, 3 * sizeof(char), 1, fs2);
            fwrite(voo->galera[i].ano, 5 * sizeof(char), 1, fs2);
            fwrite(voo->galera[i].id, 5 * sizeof(char), 1, fs2);
            fwrite(voo->galera[i].assento, 4 * sizeof(char), 1, fs2);
            fwrite(voo->galera[i].classe, 10 * sizeof(char), 1, fs2);
            fwrite(&voo->galera[i].valor, sizeof(float), 1, fs2);
            fwrite(voo->galera[i].origem, 4 * sizeof(char), 1, fs2);
            fwrite(voo->galera[i].destino, 4 * sizeof(char), 1, fs2);
        }
        fclose(fs2);
        remove("SuperRegistro.dat");
        rename("SuperRegistroTemp.dat", "SuperRegistro.dat");
    } else {
        FILE *fs = fopen("SuperRegistro.dat", "wb");
        fwrite(&voo->assentos, sizeof(int), 1, fs);
        fwrite(&voo->reservasTotais, sizeof(int), 1, fs);
        fwrite(&voo->reservasAlocadas, sizeof(int), 1, fs);
        fwrite(&voo->precoEconomica, sizeof(float), 1, fs);
        fwrite(&voo->precoExecutiva, sizeof(float), 1, fs);
        fwrite(&voo->valorTotall, sizeof(float), 1, fs);
        for(int i = 0; i < (voo->reservasTotais); i ++) {
            size_t tmn = strlen(voo->galera[i].nome);
            tmn ++;
            fwrite(&tmn, sizeof(size_t), 1, fs);
            fwrite(voo->galera[i].nome, tmn * sizeof(char), 1, fs);
            tmn = strlen(voo->galera[i].sobrenome);
            tmn ++;
            fwrite(&tmn, sizeof(size_t), 1, fs);
            fwrite(voo->galera[i].sobrenome, tmn * sizeof(char), 1, fs);
            fwrite(voo->galera[i].cpf, 15 * sizeof(char), 1, fs);
            fwrite(voo->galera[i].dia, 3 * sizeof(char), 1, fs);
            fwrite(voo->galera[i].mes, 3 * sizeof(char), 1, fs);
            fwrite(voo->galera[i].ano, 5 * sizeof(char), 1, fs);
            fwrite(voo->galera[i].id, 5 * sizeof(char), 1, fs);
            fwrite(voo->galera[i].assento, 4 * sizeof(char), 1, fs);
            fwrite(voo->galera[i].classe, 10 * sizeof(char), 1, fs);
            fwrite(&voo->galera[i].valor, sizeof(float), 1, fs);
            fwrite(voo->galera[i].origem, 4 * sizeof(char), 1, fs);
            fwrite(voo->galera[i].destino, 4 * sizeof(char), 1, fs);
        }
        fclose(fs);
    }
}
/*
* Função para alocar dinamicamente os campos do struct
*
* @param pessoa: struct reserva
* @return void
*/

void alocaTemp(reserva* pessoa) {
    pessoa->cpf = (char *)malloc(15 * sizeof(char));
    pessoa->id = (char *)malloc(5 * sizeof(char));
    pessoa->assento = (char *)malloc(4 * sizeof(char));
    pessoa->classe = (char *)malloc(10 * sizeof(char));
    pessoa->origem = (char *)malloc(4 * sizeof(char));
    pessoa->destino = (char *)malloc(4 * sizeof(char));
    pessoa->dia = (char *)malloc(3 * sizeof(char));
    pessoa->mes = (char *)malloc(3 * sizeof(char));
    pessoa->ano = (char *)malloc(5 * sizeof(char));
}

/*
* Função para desalocar os campos do struct
*
* @param pessoa: struct reserva
* @return void
*/

void liberarReserva(reserva* pessoa) {
    free(pessoa->nome);
    free(pessoa->sobrenome);
    free(pessoa->cpf);
    free(pessoa->id);
    free(pessoa->assento);
    free(pessoa->classe);
    free(pessoa->origem);
    free(pessoa->destino);
    free(pessoa->dia);
    free(pessoa->mes);
    free(pessoa->ano);
}

/*
* Função para verificar se o arquivo referente ao registro das informações existe
*   
* @param void
* @return int: 1 se o arquivo existe, 0 caso não exista
*/

int RegistroExiste() {
    FILE *fs = fopen("SuperRegistro.dat", "rb");
    if (fs) {
        fclose(fs);
        return 1;

    }
    return 0;
}

/*
* Função para verificar se o arquivo referente ao fechamento do voo existe
*
* @param void
* @return int: 1 se o arquivo existe, 0 caso não exista
*/

int vooFechou(){
    FILE *fs = fopen("SuperRegistroF.dat", "rb");
    if (fs) {
        fclose(fs);
        return 1;
    }
    return 0;
}

/*
* Função para realizar a consulta de uma reserva via cpf
*
* @param char cpf[15]: cpf utilizado para a consulta
*        aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return int: posição da reserva no arquivo, -1 caso não seja encontrada
*/

int consultaCPF(char cpf[15], aviao * voo) {
    if ((!RegistroExiste() || voo->reservasTotais == 0) && !vooFechou()) {
        return -1;
    } else {
        for(int i = 0; i < voo->reservasTotais; i++){
            if (!strcmp(voo->galera[i].cpf, cpf)) {
                return i;
            }
        }
        return -1;
    }
}

/*
* Função para abrir o voo.
* Caso o arquivo SuperRegistro.dat não exista (o que indica que o voo não foi aberto), a função pede a quantidade de assentos e o preço da classe economica e executiva.
* Em seguida, os dados são escritos no arquivo criado SuperRegistro.dat.
* @param aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return void
*/

void aberturaVoo(aviao * voo) {
    if (RegistroExiste() || vooFechou()) {
        return;
    } else {
        int assentos;
        float precoEconomica, precoExecutiva;
        scanf(" %d", &assentos);
        scanf(" %f", &precoEconomica);
        scanf(" %f", &precoExecutiva);
        FILE *fs = fopen("SuperRegistro.dat", "wb");
        voo->valorTotall = 0.0;
        voo->reservasTotais = 0;
        voo->reservasAlocadas = 0;  
        voo->assentos = assentos;
        voo->precoEconomica = precoEconomica;
        voo->precoExecutiva = precoExecutiva;
        fwrite(&(voo->assentos), sizeof(int), 1, fs);
        fwrite(&(voo->reservasTotais), sizeof(int), 1, fs);
        fwrite(&(voo->precoEconomica), sizeof(float), 1, fs);
        fwrite(&(voo->precoExecutiva), sizeof(float), 1, fs);
        fwrite(&(voo->valorTotall), sizeof(float), 1, fs);
        fclose(fs);
        if(voo->assentos == 0){
            fecharVoo(voo);
        }
    }
}

/*
* Função para realizar a reserva de pessoas e verificar se o voo pode ser fechado por conta da falta de novos assentos.
*
* @param int *vooFechado: ponteiro para a variável que indica se o voo pode ser fechado por conta da falta de assentos restantes
*        aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return void
*
* Detalhe sobre a otimização da memória:
*   Decidimos, para o array de pessoas, alocar sempre um número fixo maior do que 1 para evitar a realocação constante de memória.
*   Um sistema parecido com o vector do c++, porém sem o fator multiplicativo.
*/

void realizarReserva(int *vooFechado, aviao * voo) {
    if (!RegistroExiste()) {
        return;
    } else {
        reserva pessoa;
        alocaTemp(&pessoa);
        pessoa.nome = calloc(200, sizeof(char));
        scanf(" %s", pessoa.nome);
        pessoa.nome = realloc(pessoa.nome, (strlen(pessoa.nome) + 1) * sizeof(char));
        pessoa.sobrenome = calloc(200, sizeof(char));
        scanf(" %s", pessoa.sobrenome);
        pessoa.sobrenome = realloc(pessoa.sobrenome, (strlen(pessoa.sobrenome) + 1) * sizeof(char));
        scanf(" %s", pessoa.cpf);
        scanf(" %s", pessoa.dia);
        scanf(" %s", pessoa.mes);
        scanf(" %s", pessoa.ano);
        scanf(" %s", pessoa.id);
        scanf(" %s", pessoa.assento);
        scanf(" %s", pessoa.classe);
        scanf(" %f", &pessoa.valor);
        scanf(" %s", pessoa.origem);
        scanf(" %s", pessoa.destino);
        (voo->reservasTotais)++;
        voo->valorTotall += pessoa.valor;

        if((voo->assentos) - (voo->reservasTotais) == 0){
            *vooFechado = 1;
        }
        if(voo->reservasTotais > voo->reservasAlocadas){
            if(((voo->reservasAlocadas) + 10) <= voo->assentos){
                voo->reservasAlocadas += 10;
                voo->galera = realloc(voo->galera, voo->reservasAlocadas * sizeof(reserva));
            } else {
                voo->reservasAlocadas = voo->assentos;
                voo->galera = realloc(voo->galera, voo->reservasAlocadas * sizeof(reserva));
            }
        }
        voo->galera[voo->reservasTotais - 1] = pessoa;
    }
}

/*
* Função para realizar a consulta de uma reserva e imprimir os dados da mesma caso seja encontrada
*
* @param aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return void
*/

void consultarReserva(aviao * voo) {
    if(!RegistroExiste() && !vooFechou()){
        return;
    }else {
        char cpf[15];
        scanf(" %s", cpf);
        int pos = consultaCPF(cpf, voo);
        if(pos == -1){
            return;
        }
        printf("%s\n", voo->galera[pos].cpf);
        printf("%s", voo->galera[pos].nome);
        printf(" %s\n", voo->galera[pos].sobrenome);
        printf("%s/%s/%s\n", voo->galera[pos].dia, voo->galera[pos].mes, voo->galera[pos].ano);
        printf("Voo: %s\n", voo->galera[pos].id);
        printf("Assento: %s\n", voo->galera[pos].assento);
        printf("Classe: %s\n", voo->galera[pos].classe);
        printf("Trecho: %s", voo->galera[pos].origem);
        printf(" %s\n", voo->galera[pos].destino);
        printf("Valor: %.2f\n", voo->galera[pos].valor);
        printf("--------------------------------------------------\n");
    }
}

/*
* Função para cancelar uma reserva via chave de busca (cpf)
*
* @param aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return void
*/

void cancelarReserva(aviao *voo) {
    if (!RegistroExiste()) {
        return;
    } else {
        char cpf[15];
        scanf(" %s", cpf);
        int eraseLine = consultaCPF(cpf, voo);
        if(eraseLine != -1){
            int j = 0;
            for(int i = 0; i < (voo->reservasTotais); i++) {
                if (i != eraseLine) {
                    voo->galera[j] = voo->galera[i];
                    j++;
                    
                } else {
                    voo->valorTotall -= voo->galera[i].valor;
                    liberarReserva(&voo->galera[i]);
                }
            }
            (voo->reservasTotais)--;
            voo->galera = realloc(voo->galera, voo->reservasTotais * sizeof(reserva));
            voo->reservasAlocadas = voo->reservasTotais;
        }
    }
}

/*
* Função para realizar a modificação de uma reserva via uma chave de busca (cpf)
*
* @param aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return void
*/

void modificarReserva(aviao *voo){
    if (!RegistroExiste()) {
        return;
    } else {
        reserva pessoa;
        char searchkey[15];
        alocaTemp(&pessoa);
        scanf(" %s", searchkey);
        pessoa.nome = calloc(200, sizeof(char));
        scanf(" %s", pessoa.nome);
        pessoa.nome = realloc(pessoa.nome, (strlen(pessoa.nome) + 1) * sizeof(char));
        pessoa.sobrenome = calloc(200, sizeof(char));
        scanf(" %s", pessoa.sobrenome);
        pessoa.sobrenome = realloc(pessoa.sobrenome, (strlen(pessoa.sobrenome) + 1) * sizeof(char));
        scanf(" %s", pessoa.cpf);
        scanf(" %s", pessoa.assento);
        if(vooFechou()){
            liberarReserva(&pessoa);
            return;
        }
        int mod_line = consultaCPF(searchkey, voo);
        if (mod_line != -1) {
            voo->galera[mod_line].nome = pessoa.nome;
            voo->galera[mod_line].sobrenome = pessoa.sobrenome;
            voo->galera[mod_line].cpf = pessoa.cpf;
            voo->galera[mod_line].assento = pessoa.assento;
            printf("Reserva Modificada:\n%s\n%s %s\n%s/%s/%s\nVoo: %s\nAssento: %s\nClasse: %s\nTrecho: %s %s\nValor: %.2f\n", voo->galera[mod_line].cpf, voo->galera[mod_line].nome, voo->galera[mod_line].sobrenome, voo->galera[mod_line].dia, voo->galera[mod_line].mes, voo->galera[mod_line].ano, voo->galera[mod_line].id, voo->galera[mod_line].assento, voo->galera[mod_line].classe, voo->galera[mod_line].origem, voo->galera[mod_line].destino, voo->galera[mod_line].valor);
            printf("--------------------------------------------------\n");
        } else {
            liberarReserva(&pessoa);
        }
    }
}

/*
* Função para realizar o fechamento do dia, exportando as informações para um arquivo e dando free.
*
* @param aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return void
*/

void fecharDia(aviao *voo) {
    if (!RegistroExiste() && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado(voo);
        for (int i = 0; i < voo->reservasTotais; i++) {
            free(voo->galera[i].nome);
            free(voo->galera[i].sobrenome);
            free(voo->galera[i].assento);
            free(voo->galera[i].classe);
            free(voo->galera[i].origem);
            free(voo->galera[i].destino);
            free(voo->galera[i].cpf);
            free(voo->galera[i].id);
            free(voo->galera[i].dia);
            free(voo->galera[i].mes);
            free(voo->galera[i].ano);
        }
        free(voo->galera);
        voo->galera = NULL;
        exit(0);
    } else {
        exportarArquivo(voo);
        printf("Fechamento do dia:\n");
        printf("Quantidade de reservas: %d\n", voo->reservasTotais);
        printf("Posição: %.2f\n", voo->valorTotall);
        printf("--------------------------------------------------\n");
        for (int i = 0; i < (voo->reservasTotais); i++) {
            free(voo->galera[i].nome);
            free(voo->galera[i].sobrenome);
            free(voo->galera[i].assento);
            free(voo->galera[i].classe);
            free(voo->galera[i].origem);
            free(voo->galera[i].destino);
            free(voo->galera[i].cpf);
            free(voo->galera[i].id);
            free(voo->galera[i].dia);
            free(voo->galera[i].mes);
            free(voo->galera[i].ano);
        }
        free(voo->galera);
        voo->galera = NULL;
        exit(0);
    }
}   

/*
* Função para realizar o fechamento o voo, imprimindo os dados das reservas e o valor total arrecadado
*
* @param aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return void
*/

void fecharVoo(aviao *voo) {
    if (!RegistroExiste()  && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado(voo);
        for (int i = 0; i < (voo->reservasTotais); i++) {
            free(voo->galera[i].nome);
            free(voo->galera[i].sobrenome);
            free(voo->galera[i].assento);
            free(voo->galera[i].classe);
            free(voo->galera[i].origem);
            free(voo->galera[i].destino);
            free(voo->galera[i].cpf);
            free(voo->galera[i].id);
            free(voo->galera[i].dia);
            free(voo->galera[i].mes);
            free(voo->galera[i].ano);
        }
        free(voo->galera);
        voo->galera = NULL;
        exit(0);
    }else {
        exportarArquivo(voo);
        rename("SuperRegistro.dat", "SuperRegistroF.dat");


        printVooFechado(voo);


        for (int i = 0; i < (voo->reservasTotais); i++) {
            free(voo->galera[i].nome);
            free(voo->galera[i].sobrenome);
            free(voo->galera[i].assento);
            free(voo->galera[i].classe);
            free(voo->galera[i].origem);
            free(voo->galera[i].destino);
            free(voo->galera[i].cpf);
            free(voo->galera[i].id);
            free(voo->galera[i].dia);
            free(voo->galera[i].mes);
            free(voo->galera[i].ano);
        }
        free(voo->galera);
        voo->galera = NULL;
        exit(0);
    }
}

/*
* Função para repetição do print formatado avisando que o voo foi fechado
*
* @param aviao * voo: ponteiro da struct aviao onde está localizado todas as informações do voo
* @return void
*/

void printVooFechado(aviao *voo){
    printf("Voo Fechado!\n\n");
    if(voo->reservasTotais > 0){
        for(int i = 0; i < (voo->reservasTotais); i++){            
            printf("%s\n", voo->galera[i].cpf);
            printf("%s", voo->galera[i].nome);
            printf(" %s\n", voo->galera[i].sobrenome);
            printf("%s\n\n", voo->galera[i].assento);
        }
    }
    printf("Valor total: %.2f\n", voo->valorTotall);
    printf("--------------------------------------------------\n");
}

/*
* Função principal do programa, que lê os comandos e chama as funções correspondentes
*
* @param void
* @return int 0 para finalização do programa sem erros 
*/

int main(void) {
    int voo = 0;
    int c;
    aviao infos;
    lerArquivo(&infos);
    if(vooFechou()){
        printVooFechado(&infos);
    }
    while (1) {
        char comando[3];
        scanf(" %2s", comando);
        if (!strcmp(comando, "AV\0")) {
            aberturaVoo(&infos);
        } else if (!strcmp(comando, "RR\0")) {
            realizarReserva(&voo, &infos);
            if (voo) {
                fecharVoo(&infos);
            }
        } else if (!strcmp(comando, "CR\0")) {
            consultarReserva(&infos);
        } else if (!strcmp(comando, "CA\0")) {
            cancelarReserva(&infos);
        } else if (!strcmp(comando, "MR\0")) {
            modificarReserva(&infos);
        } else if (!strcmp(comando, "FD\0")) {
            fecharDia(&infos);
        } else if(!strcmp(comando, "FV\0")){
            fecharVoo(&infos);
        }
        else {
            while ((c = fgetc(stdin)) != '\n' && c != EOF);
        }
    }
    return 0;
}