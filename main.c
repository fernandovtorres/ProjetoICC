#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Casos teste
AV 200 1200.00 2500.00
RR Carlos Massa 555.555.333-99 12 12 2024 V001 A27 economica 1200.00 CGH RAO
RR Maria Massa 444.555.333-93 12 12 2024 V001 A31 economica 1200.00 CGH RAO
RR Roberto Carlos 555.333.333-89 12 12 2024 V001 P12 executiva 2500.00 CGH RAO
MR 555.555.333-99 Carlos Massa 555.555.333-99 A30
FD

*/

/*
*  Autores: Fernando Valentim Torres, Daniel Jorge Manzano, Henrique Vieira Lima
*  Número USP: 15452340, 15446861, 15459372 
*  Curso: Bacharelado em Ciências de Computação
*  Disciplina: SCC0221 - Introdução à Ciência de Computação I
*  Data de entrega: 06/06/2024
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
*  - int dia: armazena o dia da reserva
*  - int mes: armazena o mês da reserva
*  - int ano: armazena o ano da reserva
*  - char* id: armazena o ID do voo
*  - char* assento: armazena o assento do passageiro
*  - char* classe: armazena a classe do voo
*  - char* origem: armazena a origem do voo
*  - char* destino: armazena o destino do voo
*  - float valor: armazena o valor da reserva
*/
typedef struct {
    char* cpf;
    char* nome;
    char* sobrenome;
    int dia;
    int mes;
    int ano;
    char* id;
    char* assento;
    char* classe;
    char* origem;
    char* destino;
    float valor;
} reserva;

int lerArquivo(reserva **galera); //Lê o arquivo de registro
void alocaTemp(reserva* pessoa); //Aloca memória para os campos do struct
void liberarReserva(reserva* pessoa); //Libera memória alocada para os campos do struct
int VooExiste(); //Verifica se o arquivo do voo existe
int RegistroExiste(); //Verifica se o arquivo de registro existe
int monetarioExiste(); //Verifica se o arquivo monetario existe
int vooFechou(); //Verifica se o voo foi fechado
int consultaCPF(char cpf[15], reserva **galera, int reservasTotais); //Consulta a reserva via cpf
void aberturaVoo(reserva **galera, int reservasTotais); //Abre o voo
void realizarReserva(float *valorTotal, int *voo, reserva **galera, int *reservasTotais); //Realiza a reserva
void consultarReserva(reserva ** galera, int reservasTotais); //consulta e imprime a reserva
void cancelarReserva(float* valorTotal, int* reservasTotais, reserva **galera); //Cancela a reserva
void modificarReserva(reserva **galera, int reservasTotais); //Modifica a reserva
void fecharDia(int quantresv, float valorTotal, reserva **galera); //Fecha o dia
void fecharVoo(float valortotal, int reservasTotais, reserva **galera); //Fecha o voo
void printVooFechado(reserva **galera, int reservasTotais); //Faz o print formatado do voofechado

int lerArquivo(reserva **galera) {
    if(RegistroExiste()){
        FILE *fs = fopen("RegistroPessoa.dat", "rb");
        int i = 0;
        size_t tmnNome, tmnSobrenome;
        while (1) {
            reserva pessoa;
            alocaTemp(&pessoa);
            fread(&tmnNome, sizeof(size_t), 1, fs);
            pessoa.nome = (char *)malloc(tmnNome * sizeof(char));
            fread(pessoa.nome, tmnNome * sizeof(char), 1, fs);
            fread(&tmnSobrenome, sizeof(size_t), 1, fs);
            pessoa.sobrenome = (char *)malloc(tmnSobrenome * sizeof(char));
            fread(pessoa.sobrenome, tmnSobrenome * sizeof(char), 1, fs);
            fread(pessoa.cpf, 15 * sizeof(char), 1, fs);
            fread(&pessoa.dia, sizeof(int), 1, fs);
            fread(&pessoa.mes, sizeof(int), 1, fs);
            fread(&pessoa.ano, sizeof(int), 1, fs);
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
            *galera = realloc(*galera, (i+1) * sizeof(reserva));
            (*galera)[i] = pessoa;
            i++;
        }
        fclose(fs);
        return i;
    }
    return 0;
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
}

/*
* Função para verificar se o arquivo referente a abertura do voo existe
*
* @param void
* @return int: 1 se o arquivo existe, 0 caso não exista
*/

int VooExiste() {
    FILE *fs = fopen("AVoo.dat", "rb");
    if (fs) {
        fclose(fs);
        return 1;
    }
    return 0;
}

/*
* Função para verificar se o arquivo referente ao registro das pessoas existe
*
* @param void
* @return int: 1 se o arquivo existe, 0 caso não exista
*/

int RegistroExiste() {
    FILE *fs = fopen("RegistroPessoa.dat", "rb");
    if (fs) {
        fclose(fs);
        return 1;
    }
    return 0;
}

/*
* Função para verificar se o arquivo referente ao lucro total e os assentos restantes existe
*
* @param void
* @return int: 1 se o arquivo existe, 0 caso não exista
*/

int monetarioExiste() {
    FILE *fs = fopen("monetario.dat", "rb");
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
    FILE *fs = fopen("AVoof.dat", "rb");
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
*        galera: ponteiro da struct reserva
*        int reservasTotais: número de reservas feitas
* @return int: posição da reserva no arquivo, -1 caso não seja encontrada
*/

int consultaCPF(char cpf[15], reserva **galera, int reservasTotais) {
    if ((!VooExiste() || (!RegistroExiste() && reservasTotais == 0)) && !vooFechou()) {
        return -1;
    } else {
        for(int i = 0; i < reservasTotais; i++){
            if (!strcmp((*galera)[i].cpf, cpf)) {
                return i;
            }
        }
        return -1;
    }
}

/*
* Função para abrir o voo.
* Caso o arquivo Avoo.dat não exista (o que indica que o voo não foi aberto), a função pede a quantidade de assentos e o preço da classe economica e executiva.
* Em seguida, os dados são escritos no arquivo criado Avoo.dat.
* @param galera: ponteiro da struct reserva
*        int reservasTotais: número de reservas feitas
* @return void
*/

void aberturaVoo(reserva **galera, int reservasTotais) {
    if (VooExiste()) {
        return;
    } else if(vooFechou())  {
        printVooFechado(galera, reservasTotais);
    } else {
        FILE *fs = fopen("AVoo.dat", "wb");
        int ndeassentos;
        float precoEconomica, precoExecutiva;
        scanf(" %d", &ndeassentos);
        if(ndeassentos == 0){
            fecharVoo(0.0, reservasTotais, galera);
        }
        scanf(" %f", &precoEconomica);
        scanf(" %f", &precoExecutiva);
        fwrite(&ndeassentos, sizeof(int), 1, fs);
        fwrite(&precoEconomica, sizeof(float), 1, fs);
        fwrite(&precoExecutiva, sizeof(float), 1, fs);
        fclose(fs);
    }
}

/*
* Função para realizar a reserva de pessoas e verificar se o voo pode ser fechado por conta da falta de novos assentos.
*
* @param float *valorTotal: ponteiro para o valor total das reservas
*        galera: ponteiro da struct reserva
*        int *reservasTotais: ponteiro para a quantidade de reservas existentes
*        int *voo: ponteiro para a variável que indica se o voo pode ser fechado por conta da falta de assentos restantes
* @return void
*/

void realizarReserva(float *valorTotal, int *voo, reserva **galera, int *reservasTotais) {
    if (!VooExiste() && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado(galera, *reservasTotais);
    }else {
        reserva pessoa;
        alocaTemp(&pessoa);
        FILE *fs = NULL;
        (*reservasTotais)++;
        
        int assentos = 0;
        if(VooExiste()) {
            fs = fopen("AVoo.dat", "rb");
            fread(&assentos, sizeof(int), 1, fs);
            fclose(fs);
        }

        if (*reservasTotais - assentos == 0) {
            *voo = 1;
        }

        pessoa.nome = calloc(200, sizeof(char));
        scanf(" %s", pessoa.nome);
        pessoa.nome = realloc(pessoa.nome, (strlen(pessoa.nome) + 1) * sizeof(char));
        pessoa.sobrenome = calloc(200, sizeof(char));
        scanf(" %s", pessoa.sobrenome);
        pessoa.sobrenome = realloc(pessoa.sobrenome, (strlen(pessoa.sobrenome) + 1) * sizeof(char));
        scanf(" %s", pessoa.cpf);
        scanf(" %d", &pessoa.dia);
        scanf(" %d", &pessoa.mes);
        scanf(" %d", &pessoa.ano);
        scanf(" %s", pessoa.id);
        scanf(" %s", pessoa.assento);
        scanf(" %s", pessoa.classe);
        scanf(" %f", &pessoa.valor);
        *valorTotal += pessoa.valor;
        scanf(" %s", pessoa.origem);
        scanf(" %s", pessoa.destino);
        *galera = realloc(*galera, (*reservasTotais) * sizeof(reserva));
        (*galera)[(*reservasTotais) - 1] = pessoa;
        printf("%s\n", (*galera)[0].nome);
    }
}

/*
* Função para realizar a consulta de uma reserva e imprimir os dados da mesma caso seja encontrada
*
* @param galera: ponteiro da struct reserva
*        int reservasTotais: número de reservas totais existentes
* @return void
*/

void consultarReserva(reserva **galera, int reservasTotais) {
    if(!VooExiste() && !vooFechou()){
        return;
    }else {
        FILE *fs;
        if((fs = fopen("RegistroPessoa.dat", "rb")) == NULL){
            printf("Nenhuma reserva foi feita\n");
            fclose(fs);
            return;
        }
        fclose(fs);
        char cpf[15];
        scanf(" %s", cpf);
        int pos = consultaCPF(cpf, galera, reservasTotais);
        if(pos == -1){
            return;
        }
        printf("%s\n", (*galera)[pos].cpf);
        printf("%s", (*galera)[pos].nome);
        printf(" %s\n", (*galera)[pos].sobrenome);
        printf("%d/%d/%d\n", (*galera)[pos].dia, (*galera)[pos].mes, (*galera)[pos].ano);
        printf("Voo: %s\n", (*galera)[pos].id);
        printf("Assento: %s\n", (*galera)[pos].assento);
        printf("Classe: %s\n", (*galera)[pos].classe);
        printf("Trecho: %s", (*galera)[pos].origem);
        printf(" %s\n", (*galera)[pos].destino);
        printf("Valor: %.2f\n", (*galera)[pos].valor);
        printf("--------------------------------------------------\n");
    }
}

/*
* Função para cancelar uma reserva via chave de busca (cpf)
*
* @param float *valorTotal: ponteiro para o valor total das reserva
*        int *reservasTotais: ponteiro para a quantidade de reservas totais existentes
*        galera: ponteiro da struct reserva
* @return void
*/

void cancelarReserva(float *valorTotal, int* reservasTotais, reserva **galera) {
    if ((!VooExiste() || !RegistroExiste()) && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado(galera, *reservasTotais); 
    }else {
        char cpf[15];
        scanf(" %s", cpf);
        int j = 0;
        for(int i = 0; i < (*reservasTotais); i++){
            if(strcmp((*galera)[i].cpf, cpf) && !j){
                continue;
            } else if(!strcmp((*galera)[i].cpf, cpf)) {
                j = 1;
                (*valorTotal) -= (*galera)[i].valor;
                if(monetarioExiste()){
                    FILE *fs3 = fopen("monetario.dat", "rb");
                    FILE *fs4 = fopen("monetariotemp.dat", "wb");
                    int assentos;
                    float total_prev;
                    fread(&assentos, sizeof(int), 1, fs3);
                    fread(&total_prev, sizeof(float), 1, fs3);
                    fclose(fs3);
                    assentos ++;
                    fwrite(&assentos, sizeof(int), 1, fs4);
                    fwrite(&total_prev, sizeof(float), 1, fs4);
                    fclose(fs4);
                    remove("monetario.dat");
                    rename("monetariotemp.dat", "monetario.dat");
                } else {
                    *reservasTotais -= 1;
                }
            }
            if(j){
                if(i == (*reservasTotais) - 1){
                    break;
                }
                (*galera)[i] = (*galera)[i + 1];
            }
        }
    }
}

/*
* Função para realizar a modificação de uma reserva via uma chave de busca (cpf)
*
* @param galera: ponteiro da struct reserva
*        int reservasTotais: quantidade de reservas totais existentes
* @return void
*/

void modificarReserva(reserva **galera, int reservasTotais){
    if ((!VooExiste() || (!RegistroExiste() && reservasTotais == 0)) && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado(galera, reservasTotais);
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
        int mod_line = consultaCPF(searchkey, galera, reservasTotais);
        if (mod_line != -1) {
            (*galera)[mod_line].nome = pessoa.nome;
            (*galera)[mod_line].sobrenome = pessoa.sobrenome;
            (*galera)[mod_line].cpf = pessoa.cpf;
            (*galera)[mod_line].assento = pessoa.assento;
            printf("Reserva Modificada:\n%s\n%s %s\n%d/%d/%d\nVoo: %s\nAssento: %s\nClasse: %s\nTrecho: %s %s\nValor: %.2f\n", (*galera)[mod_line].cpf, (*galera)[mod_line].nome, (*galera)[mod_line].sobrenome, (*galera)[mod_line].dia, (*galera)[mod_line].mes, (*galera)[mod_line].ano, (*galera)[mod_line].id, (*galera)[mod_line].assento, (*galera)[mod_line].classe, (*galera)[mod_line].origem, (*galera)[mod_line].destino, (*galera)[mod_line].valor);
            printf("--------------------------------------------------\n");
        }
    }
}

/*
* Função para realizar o fechamento do dia, calculando os assentos restantes e o valor total arrecadado até o dia.
*
* @param int quantresv: quantidade de reservas feitas no dia
*        float valorTotal: valor total arrecadado no dia
*        galera: ponteiro da struct reserva
* @return void
*/

void fecharDia(int quantresv, float valorTotal, reserva **galera) {
    if (!VooExiste() && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado(galera, quantresv);
    } else {
        FILE *fs = fopen("AVoo.dat", "rb");
        int assentosIniciais;
        int assentosRestantes;
        fread(&assentosIniciais, sizeof(int), 1, fs);
        fclose(fs);

        if (!monetarioExiste()) {
            fs = fopen("monetario.dat", "wb");
            assentosRestantes = assentosIniciais - quantresv;
            fwrite(&assentosRestantes, sizeof(int), 1, fs);
            fwrite(&valorTotal, sizeof(float), 1, fs);
            fclose(fs);
        } else {
            fs = fopen("monetario.dat", "rb");
            FILE *fs2 = fopen("monetariotemp.dat", "wb");
            float total_prev;
            fread(&assentosRestantes, sizeof(int), 1, fs);
            fread(&total_prev, sizeof(float), 1, fs);
            fclose(fs);

            assentosRestantes = assentosIniciais - quantresv;
            valorTotal += total_prev;

            fwrite(&assentosRestantes, sizeof(int), 1, fs2);
            fwrite(&valorTotal, sizeof(float), 1, fs2);
            fclose(fs2);

            remove("monetario.dat");
            rename("monetariotemp.dat", "monetario.dat");
        }
        if(!RegistroExiste()){
            fs = fopen("RegistroPessoa.dat", "wb");
            for(int i = 0; i < quantresv; i ++) {
                size_t tmn = strlen((*galera)[i].nome);
                tmn ++;
                fwrite(&tmn, sizeof(size_t), 1, fs);
                fwrite((*galera)[i].nome, tmn * sizeof(char), 1, fs);
                tmn = strlen((*galera)[i].sobrenome);
                tmn ++;
                fwrite(&tmn, sizeof(size_t), 1, fs);
                fwrite((*galera)[i].sobrenome, tmn * sizeof(char), 1, fs);
                fwrite((*galera)[i].cpf, 15 * sizeof(char), 1, fs);
                fwrite(&(*galera)[i].dia, sizeof(int), 1, fs);
                fwrite(&(*galera)[i].mes, sizeof(int), 1, fs);
                fwrite(&(*galera)[i].ano, sizeof(int), 1, fs);
                fwrite((*galera)[i].id, 5 * sizeof(char), 1, fs);
                fwrite((*galera)[i].assento, 4 * sizeof(char), 1, fs);
                fwrite((*galera)[i].classe, 10 * sizeof(char), 1, fs);
                fwrite(&(*galera)[i].valor, sizeof(float), 1, fs);
                fwrite((*galera)[i].origem, 4 * sizeof(char), 1, fs);
                fwrite((*galera)[i].destino, 4 * sizeof(char), 1, fs);
            }
            fclose(fs);
        } else {
            fs = fopen("RegistroPessoatemp.dat", "wb");
            for(int i = 0; i < quantresv; i++) {
                size_t tmn = strlen((*galera)[i].nome);
                tmn ++;
                fwrite(&tmn, sizeof(size_t), 1, fs);
                fwrite((*galera)[i].nome, tmn * sizeof(char), 1, fs);
                tmn = strlen((*galera)[i].sobrenome);
                tmn ++;
                fwrite(&tmn, sizeof(size_t), 1, fs);
                fwrite((*galera)[i].sobrenome, tmn * sizeof(char), 1, fs);
                fwrite((*galera)[i].cpf, 15 * sizeof(char), 1, fs);
                fwrite(&(*galera)[i].dia, sizeof(int), 1, fs);
                fwrite(&(*galera)[i].mes, sizeof(int), 1, fs);
                fwrite(&(*galera)[i].ano, sizeof(int), 1, fs);
                fwrite((*galera)[i].id, 5 * sizeof(char), 1, fs);
                fwrite((*galera)[i].assento, 4 * sizeof(char), 1, fs);
                fwrite((*galera)[i].classe, 10 * sizeof(char), 1, fs);
                fwrite(&(*galera)[i].valor, sizeof(float), 1, fs);
                fwrite((*galera)[i].origem, 4 * sizeof(char), 1, fs);
                fwrite((*galera)[i].destino, 4 * sizeof(char), 1, fs);
            }
            fclose(fs);
            remove("RegistroPessoa.dat");
            rename("RegistroPessoatemp.dat", "RegistroPessoa.dat");
        }
        int reservaTotais = assentosIniciais - assentosRestantes;
        printf("Fechamento do dia:\n");
        printf("Quantidade de reservas: %d\n", reservaTotais);
        printf("Posição: %.2f\n", valorTotal);
        printf("--------------------------------------------------\n");
        exit(0);
    }
}

/*
* Função para realizar o fechamento o voo, imprimindo os dados das reservas e o valor total arrecadado
*
* @param float valortotal: valor total arrecadado no dia que irá ser somado ao valor total arrecadado até o dia
*        int reservasTotais: quantidade de reservas totais existentes
*        galera: ponteiro da struct reserva
* @return void
*/

void fecharVoo(float valortotal, int reservasTotais, reserva **galera) {
    if (!VooExiste()  && !vooFechou()) {
        return;
    }else {
        if(monetarioExiste()){
            FILE *fs2 = fopen("monetario.dat", "rb");
            float total_prev;
            fseek(fs2, sizeof(int), SEEK_SET);
            fread(&total_prev, sizeof(float), 1, fs2);
            fclose(fs2);
            valortotal += total_prev;
        }
        float valorFinal;
        if (vooFechou()){
            FILE *fs2 = fopen("AVoof.dat", "rb");
            fread(&valorFinal, sizeof(float), 1, fs2);
            fclose(fs2);
            valortotal = valorFinal;
        } else {
            FILE *fs2 = fopen("AVoof.dat", "wb");
            fwrite(&valortotal, sizeof(float), 1, fs2);
            remove("AVoo.dat");
        }

        printf("Voo Fechado!\n\n");
        if(RegistroExiste()){
            for(int i = 0; i < reservasTotais; i++){
                printf("%s\n", (*galera)[i].cpf);
                printf("%s", (*galera)[i].nome);
                printf(" %s\n", (*galera)[i].sobrenome);
                printf("%s\n\n", (*galera)[i].assento);
            }
        }
        printf("Valor total: %.2f\n", valortotal);
        printf("--------------------------------------------------\n");
        exit(0);
    }
}

/*
* Função para repetição do print formatado avisando que o voo foi fechado
*
* @param galera: ponteiro da struct reserva
*        int reservasTotais: quantidade de reservas totais existentes
* @return void
*/

void printVooFechado(reserva **galera, int reservasTotais){
    float valorFinal;
    FILE *fs2 = fopen("AVoof.dat", "rb");
    fread(&valorFinal, sizeof(float), 1, fs2);
    fclose(fs2);
    printf("Voo Fechado!\n\n");
    if(RegistroExiste()){
        for(int i = 0; i < reservasTotais; i++){
            printf("%s\n", (*galera)[i].cpf);
            printf("%s", (*galera)[i].nome);
            printf(" %s\n", (*galera)[i].sobrenome);
            printf("%s\n\n", (*galera)[i].assento);
        }
    }
    printf("Valor total: %.2f\n", valorFinal);
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
    float valortotal = 0;
    reserva *galera = NULL;
    int reservasTotais = lerArquivo(&galera);
    while (1) {
        char comando[3];
        scanf(" %s", comando);
        if (!strcmp(comando, "AV\0")) {
            aberturaVoo(&galera, reservasTotais);
        } else if (!strcmp(comando, "RR\0")) {
            realizarReserva(&valortotal, &voo, &galera, &reservasTotais);
            if (voo) {
                fecharVoo(valortotal, reservasTotais, &galera);
            }
        } else if (!strcmp(comando, "CR\0")) {
            consultarReserva(&galera, reservasTotais);
        } else if (!strcmp(comando, "CA\0")) {
            cancelarReserva(&valortotal, &reservasTotais, &galera);
        } else if (!strcmp(comando, "MR\0")) {
            modificarReserva(&galera, reservasTotais);
        } else if (!strcmp(comando, "FD\0")) {
            fecharDia(reservasTotais, valortotal, &galera);
        } else if(!strcmp(comando, "FV\0")){
            fecharVoo(valortotal, reservasTotais, &galera);
        }
        else {
            continue;
        }
    }
    return 0;
}