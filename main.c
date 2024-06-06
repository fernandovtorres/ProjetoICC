#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int VooExiste(); //Verifica se o arquivo do voo existe
int RegistroExiste(); //Verifica se o arquivo de registro existe
void aberturaVoo(); //Abre o voo
void realizarReserva(float *valorTotal, int *quantresv, int *voo); //Realiza a reserva
void fecharDia(int quantresv, float valorTotal); //Fecha o dia
void consultarReserva(); //consulta e imprime a reserva
int consultaCPF(char cpf[15]); //Consulta a reserva via cpf
void cancelarReserva(float* valorTotal, int* qntresv); //Cancela a reserva
void modificarReserva(); //Modifica a reserva
int monetarioExiste(); //Verifica se o arquivo monetario existe
void fecharVoo(float valortotal); //Fecha o voo
void alocaTemp(reserva* pessoa); //Aloca memória para os campos do struct
void liberarReserva(reserva* pessoa); //Libera memória alocada para os campos do struct
void printVooFechado(); //Faz o print formatado do voofechado

/*
* Função para alocar dinamicamente os campos do struct
*
* @param pessoa: struct reserva
* @return void
*/

void alocaTemp(reserva* pessoa) {
    pessoa->nome = (char *)malloc(50 * sizeof(char));
    pessoa->sobrenome = (char *)malloc(50 * sizeof(char));
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
* @return int: posição da reserva no arquivo, -1 caso não seja encontrada
*/

int consultaCPF(char cpf[15]) {
    if ((!VooExiste() || !RegistroExiste()) && !vooFechou()) {
        return -1;
    } else {
        FILE *fs = fopen("RegistroPessoa.dat", "rb");
        reserva pessoa;
        alocaTemp(&pessoa);
        int i = 0;
        while (fread(pessoa.nome, 50 * sizeof(char), 1, fs) &&
               fread(pessoa.sobrenome, 50 * sizeof(char), 1, fs) &&
               fread(pessoa.cpf, 15 * sizeof(char), 1, fs) &&
               fread(&pessoa.dia, sizeof(int), 1, fs) &&
               fread(&pessoa.mes, sizeof(int), 1, fs) &&
               fread(&pessoa.ano, sizeof(int), 1, fs) &&
               fread(pessoa.id, 5 * sizeof(char), 1, fs) &&
               fread(pessoa.assento, 4 * sizeof(char), 1, fs) &&
               fread(pessoa.classe, 10 * sizeof(char), 1, fs) &&
               fread(&pessoa.valor, sizeof(float), 1, fs) &&
               fread(pessoa.origem, 4 * sizeof(char), 1, fs) &&
               fread(pessoa.destino, 4 * sizeof(char), 1, fs)) {
            if (strcmp(pessoa.cpf, cpf) != 0) {
                i++;
            } else {
                return i;
            }
        }
        fclose(fs);
        liberarReserva(&pessoa);
        return -1;
    }
}

/*
* Função para abrir o voo.
* Caso o arquivo Avoo.dat não exista (o que indica que o voo não foi aberto), a função pede a quantidade de assentos e o preço da classe economica e executiva.
* Em seguida, os dados são escritos no arquivo criado Avoo.dat.
* @param void
* @return void
*/

void aberturaVoo() {
    if (VooExiste()) {
        return;
    } else if(vooFechou())  {
        printVooFechado();
    } else {
        FILE *fs = fopen("AVoo.dat", "wb");
        int ndeassentos;
        float precoEconomica, precoExecutiva;
        scanf(" %d", &ndeassentos);
        if(ndeassentos == 0){
            fecharVoo(0.0);
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
*        int *quantresv: ponteiro para a quantidade de reservas
*        int *voo: ponteiro para a variável que indica se o voo pode ser fechado por conta da falta de assentos restantes
* @return void
*/

void realizarReserva(float *valorTotal, int *quantresv, int *voo) {
    if (!VooExiste() && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado();
    }else {
        reserva pessoa;
        alocaTemp(&pessoa);
        FILE *fs = fopen("RegistroPessoa.dat", "ab+");
        FILE *fs2;
        (*quantresv)++;
        
        int assentos = 0;
        if (monetarioExiste()) {
            fs2 = fopen("monetario.dat", "rb");
            fread(&assentos, sizeof(int), 1, fs2);
            fclose(fs2);
        } else {
            fs2 = fopen("AVoo.dat", "rb");
            fread(&assentos, sizeof(int), 1, fs2);
            fclose(fs2);
        }

        if (*quantresv - assentos == 0) {
            *voo = 1;
        }

        scanf(" %s", pessoa.nome);
        scanf(" %s", pessoa.sobrenome);
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

        fwrite(pessoa.nome, 50 * sizeof(char), 1, fs);
        fwrite(pessoa.sobrenome, 50 * sizeof(char), 1, fs);
        fwrite(pessoa.cpf, 15 * sizeof(char), 1, fs);
        fwrite(&pessoa.dia, sizeof(int), 1, fs);
        fwrite(&pessoa.mes, sizeof(int), 1, fs);
        fwrite(&pessoa.ano, sizeof(int), 1, fs);
        fwrite(pessoa.id, 5 * sizeof(char), 1, fs);
        fwrite(pessoa.assento, 4 * sizeof(char), 1, fs);
        fwrite(pessoa.classe, 10 * sizeof(char), 1, fs);
        fwrite(&pessoa.valor, sizeof(float), 1, fs);
        fwrite(pessoa.origem, 4 * sizeof(char), 1, fs);
        fwrite(pessoa.destino, 4 * sizeof(char), 1, fs);

        liberarReserva(&pessoa);
        fclose(fs);
    }
}

/*
* Função para realizar a consulta de uma reserva e imprimir os dados da mesma caso seja encontrada
*
* @param void
* @return void
*/

void consultarReserva() {
    if(!VooExiste() && !vooFechou()){
        return;
    }else {
        FILE *fs;
        if((fs = fopen("RegistroPessoa.dat", "rb")) == NULL){
            printf("Nenhuma reserva foi feita\n");
            return;
        }
        reserva pessoa;
        alocaTemp(&pessoa);
        char cpf[15];
        scanf(" %s", cpf);
        int pos = consultaCPF(cpf);
        if(pos == -1){
            return;
        }
        int i = 0;
        while (fread(pessoa.nome, 50 * sizeof(char), 1, fs) &&
            fread(pessoa.sobrenome, 50 * sizeof(char), 1, fs) &&
            fread(pessoa.cpf, 15 * sizeof(char), 1, fs) &&
            fread(&pessoa.dia, sizeof(int), 1, fs) &&
            fread(&pessoa.mes, sizeof(int), 1, fs) &&
            fread(&pessoa.ano, sizeof(int), 1, fs) &&
            fread(pessoa.id, 5 * sizeof(char), 1, fs) &&
            fread(pessoa.assento, 4 * sizeof(char), 1, fs) &&
            fread(pessoa.classe, 10 * sizeof(char), 1, fs) &&
            fread(&pessoa.valor, sizeof(float), 1, fs) &&
            fread(pessoa.origem, 4 * sizeof(char), 1, fs) &&
            fread(pessoa.destino, 4 * sizeof(char), 1, fs)) {
            if (i == pos) {
                printf("%s\n", pessoa.cpf);
                printf("%s", pessoa.nome);
                printf(" %s\n", pessoa.sobrenome);
                printf("%d/%d/%d\n", pessoa.dia, pessoa.mes, pessoa.ano);
                printf("Voo: %s\n", pessoa.id);
                printf("Assento: %s\n", pessoa.assento);
                printf("Classe: %s\n", pessoa.classe);
                printf("Trecho: %s", pessoa.origem);
                printf(" %s\n", pessoa.destino);
                printf("Valor: %.2f\n", pessoa.valor);
                printf("--------------------------------------------------\n");
            }
            i++;
        }
        fclose(fs);
        liberarReserva(&pessoa);
    }
}

/*
* Função para cancelar uma reserva via chave de busca (cpf)
*
* @param char cpf[15]: cpf utilizado para a consulta
* @return void
*/

void cancelarReserva(float *valorTotal, int* qntresv) {
    if ((!VooExiste() || !RegistroExiste()) && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado(); 
    }else {
        reserva pessoa;
        alocaTemp(&pessoa);
        FILE *fs = fopen("RegistroPessoa.dat", "rb");
        FILE *fs2 = fopen("RegistroPessoaTemp.dat", "wb");
        char cpf[15];
        scanf(" %s", cpf);

        while (fread(pessoa.nome, 50 * sizeof(char), 1, fs) &&
               fread(pessoa.sobrenome, 50 * sizeof(char), 1, fs) &&
               fread(pessoa.cpf, 15 * sizeof(char), 1, fs) &&
               fread(&pessoa.dia, sizeof(int), 1, fs) &&
               fread(&pessoa.mes, sizeof(int), 1, fs) &&
               fread(&pessoa.ano, sizeof(int), 1, fs) &&
               fread(pessoa.id, 5 * sizeof(char), 1, fs) &&
               fread(pessoa.assento, 4 * sizeof(char), 1, fs) &&
               fread(pessoa.classe, 10 * sizeof(char), 1, fs) &&
               fread(&pessoa.valor, sizeof(float), 1, fs) &&
               fread(pessoa.origem, 4 * sizeof(char), 1, fs) &&
               fread(pessoa.destino, 4 * sizeof(char), 1, fs)) {
            if (strcmp(pessoa.cpf, cpf) != 0) {
                fwrite(pessoa.nome, 50 * sizeof(char), 1, fs2);
                fwrite(pessoa.sobrenome, 50 * sizeof(char), 1, fs2);
                fwrite(pessoa.cpf, 15 * sizeof(char), 1, fs2);
                fwrite(&pessoa.dia, sizeof(int), 1, fs2);
                fwrite(&pessoa.mes, sizeof(int), 1, fs2);
                fwrite(&pessoa.ano, sizeof(int), 1, fs2);
                fwrite(pessoa.id, 5 * sizeof(char), 1, fs2);
                fwrite(pessoa.assento, 4 * sizeof(char), 1, fs2);
                fwrite(pessoa.classe, 10 * sizeof(char), 1, fs2);
                fwrite(&pessoa.valor, sizeof(float), 1, fs2);
                fwrite(pessoa.origem, 4 * sizeof(char), 1, fs2);
                fwrite(pessoa.destino, 4 * sizeof(char), 1, fs2);
            } else {
                (*valorTotal) -= pessoa.valor;
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
                    *qntresv -= 1;
                }
            }
        }
        fclose(fs);
        fclose(fs2);
        remove("RegistroPessoa.dat");
        rename("RegistroPessoaTemp.dat", "RegistroPessoa.dat");
        liberarReserva(&pessoa);
    }
}

/*
* Função para realizar a modificação de uma reserva via uma chave de busca (cpf)
*
* @param void
* @return void
*/

void modificarReserva(){
    if ((!VooExiste() || !RegistroExiste()) && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado();
    } else {
        reserva pessoa;
        alocaTemp(&pessoa);
        FILE *fs = fopen("RegistroPessoa.dat", "rb");
        FILE *fs2 = fopen("RegistroPessoaTemp.dat", "wb");
        char nomee[50], sobrenome[50], cpf[15], assento[4], searchkey[15];
        scanf(" %s", searchkey);
        scanf(" %s", nomee);
        scanf(" %s", sobrenome);
        scanf(" %s", cpf);
        scanf(" %s", assento);
        if (consultaCPF(searchkey) != -1) {
            int mod_line = consultaCPF(searchkey);
            int i = 0;
            while (fread(pessoa.nome, 50 * sizeof(char), 1, fs) &&
                fread(pessoa.sobrenome, 50 * sizeof(char), 1, fs) &&
                fread(pessoa.cpf, 15 * sizeof(char), 1, fs) &&
                fread(&pessoa.dia, sizeof(int), 1, fs) &&
                fread(&pessoa.mes, sizeof(int), 1, fs) &&
                fread(&pessoa.ano, sizeof(int), 1, fs) &&
                fread(pessoa.id, 5 * sizeof(char), 1, fs) &&
                fread(pessoa.assento, 4 * sizeof(char), 1, fs) &&
                fread(pessoa.classe, 10 * sizeof(char), 1, fs) &&
                fread(&pessoa.valor, sizeof(float), 1, fs) &&
                fread(pessoa.origem, 4 * sizeof(char), 1, fs) &&
                fread(pessoa.destino, 4 * sizeof(char), 1, fs)) {
                if (mod_line != i) {
                    fwrite(pessoa.nome, 50 * sizeof(char), 1, fs2);
                    fwrite(pessoa.sobrenome, 50 * sizeof(char), 1, fs2);
                    fwrite(pessoa.cpf, 15 * sizeof(char), 1, fs2);
                    fwrite(&pessoa.dia, sizeof(int), 1, fs2);
                    fwrite(&pessoa.mes, sizeof(int), 1, fs2);
                    fwrite(&pessoa.ano, sizeof(int), 1, fs2);
                    fwrite(pessoa.id, 5 * sizeof(char), 1, fs2);
                    fwrite(pessoa.assento, 4 * sizeof(char), 1, fs2);
                    fwrite(pessoa.classe, 10 * sizeof(char), 1, fs2);
                    fwrite(&pessoa.valor, sizeof(float), 1, fs2);
                    fwrite(pessoa.origem, 4 * sizeof(char), 1, fs2);
                    fwrite(pessoa.destino, 4 * sizeof(char), 1, fs2);
                    liberarReserva(&pessoa);
                    alocaTemp(&pessoa);
                } else {
                    free(pessoa.nome);
                    free(pessoa.sobrenome);
                    free(pessoa.cpf);
                    free(pessoa.assento);
                    pessoa.nome = (char *)malloc(50 * sizeof(char));
                    pessoa.sobrenome = (char *)malloc(50 * sizeof(char));
                    pessoa.cpf = (char *)malloc(15 * sizeof(char));
                    pessoa.assento = (char *)malloc(4 * sizeof(char));
                    strcpy(pessoa.nome, nomee);
                    strcpy(pessoa.sobrenome, sobrenome);
                    strcpy(pessoa.cpf, cpf);
                    strcpy(pessoa.assento, assento);

                    fwrite(pessoa.nome, 50 * sizeof(char), 1, fs2);
                    fwrite(pessoa.sobrenome, 50 * sizeof(char), 1, fs2);
                    fwrite(pessoa.cpf, 15 * sizeof(char), 1, fs2);
                    fwrite(&pessoa.dia, sizeof(int), 1, fs2);
                    fwrite(&pessoa.mes, sizeof(int), 1, fs2);
                    fwrite(&pessoa.ano, sizeof(int), 1, fs2);
                    fwrite(pessoa.id, 5 * sizeof(char), 1, fs2);
                    fwrite(pessoa.assento, 4 * sizeof(char), 1, fs2);
                    fwrite(pessoa.classe, 10 * sizeof(char), 1, fs2);
                    fwrite(&pessoa.valor, sizeof(float), 1, fs2);
                    fwrite(pessoa.origem, 4 * sizeof(char), 1, fs2);
                    fwrite(pessoa.destino, 4 * sizeof(char), 1, fs2);
                    printf("Reserva Modificada:\n%s\n%s %s\n%d/%d/%d\nVoo: %s\nAssento: %s\nClasse: %s\nTrecho: %s %s\nValor: %.2f\n", pessoa.cpf, pessoa.nome, pessoa.sobrenome, pessoa.dia, pessoa.mes, pessoa.ano, pessoa.id, pessoa.assento, pessoa.classe, pessoa.origem, pessoa.destino, pessoa.valor);
                    printf("--------------------------------------------------\n");
                    free(pessoa.id);
                    free(pessoa.classe);
                    free(pessoa.origem);
                    free(pessoa.destino);
                    free(pessoa.nome);
                    free(pessoa.sobrenome);
                    free(pessoa.cpf);
                    free(pessoa.assento);
                    alocaTemp(&pessoa);
                }
                i++;
            }
            liberarReserva(&pessoa);
            fclose(fs);
            fclose(fs2);
            remove("RegistroPessoa.dat");
            rename("RegistroPessoaTemp.dat", "RegistroPessoa.dat");
        }
    }
}

/*
* Função para realizar o fechamento do dia, calculando os assentos restantes e o valor total arrecadado até o dia.
*
* @param int quantresv: quantidade de reservas feitas no dia
*        float valorTotal: valor total arrecadado no dia
* @return void
*/

void fecharDia(int quantresv, float valorTotal) {
    if (!VooExiste() && !vooFechou()) {
        return;
    }else if(vooFechou()){
        printVooFechado();
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

            assentosRestantes -= quantresv;
            valorTotal += total_prev;

            fwrite(&assentosRestantes, sizeof(int), 1, fs2);
            fwrite(&valorTotal, sizeof(float), 1, fs2);
            fclose(fs2);

            remove("monetario.dat");
            rename("monetariotemp.dat", "monetario.dat");
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
* @return void
*/

void fecharVoo(float valortotal) {
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
        reserva pessoa;
        alocaTemp(&pessoa);

        printf("Voo Fechado!\n\n");
        if(RegistroExiste()){
            FILE *fs = fopen("RegistroPessoa.dat", "rb");
            while (fread(pessoa.nome, 50 * sizeof(char), 1, fs) &&
                fread(pessoa.sobrenome, 50 * sizeof(char), 1, fs) &&
                fread(pessoa.cpf, 15 * sizeof(char), 1, fs) &&
                fread(&pessoa.dia, sizeof(int), 1, fs) &&
                fread(&pessoa.mes, sizeof(int), 1, fs) &&
                fread(&pessoa.ano, sizeof(int), 1, fs) &&
                fread(pessoa.id, 5 * sizeof(char), 1, fs) &&
                fread(pessoa.assento, 4 * sizeof(char), 1, fs) &&
                fread(pessoa.classe, 10 * sizeof(char), 1, fs) &&
                fread(&pessoa.valor, sizeof(float), 1, fs) &&
                fread(pessoa.origem, 4 * sizeof(char), 1, fs) &&
                fread(pessoa.destino, 4 * sizeof(char), 1, fs)) {
                printf("%s\n", pessoa.cpf);
                printf("%s", pessoa.nome);
                printf(" %s\n", pessoa.sobrenome);
                printf("%s\n\n", pessoa.assento);
            } 
            fclose(fs);
        }
        printf("Valor total: %.2f\n", valortotal);
        printf("--------------------------------------------------\n");
        liberarReserva(&pessoa);
        exit(0);
    }
}

/*
* Função para repetição do print formatado avisando que o voo foi fechado
*
* @param void
* @return void
*/

void printVooFechado(){
    float valorFinal;
    FILE *fs2 = fopen("AVoof.dat", "rb");
    fread(&valorFinal, sizeof(float), 1, fs2);
    fclose(fs2);
    reserva pessoa;
    alocaTemp(&pessoa);
    printf("Voo Fechado!\n\n");
    if(RegistroExiste()){
        FILE *fs = fopen("RegistroPessoa.dat", "rb");
        while (fread(pessoa.nome, 50 * sizeof(char), 1, fs) &&
            fread(pessoa.sobrenome, 50 * sizeof(char), 1, fs) &&
            fread(pessoa.cpf, 15 * sizeof(char), 1, fs) &&
            fread(&pessoa.dia, sizeof(int), 1, fs) &&
            fread(&pessoa.mes, sizeof(int), 1, fs) &&
            fread(&pessoa.ano, sizeof(int), 1, fs) &&
            fread(pessoa.id, 5 * sizeof(char), 1, fs) &&
            fread(pessoa.assento, 4 * sizeof(char), 1, fs) &&
            fread(pessoa.classe, 10 * sizeof(char), 1, fs) &&
            fread(&pessoa.valor, sizeof(float), 1, fs) &&
            fread(pessoa.origem, 4 * sizeof(char), 1, fs) &&
            fread(pessoa.destino, 4 * sizeof(char), 1, fs)) {
            printf("%s\n", pessoa.cpf);
            printf("%s", pessoa.nome);
            printf(" %s\n", pessoa.sobrenome);
            printf("%s\n\n", pessoa.assento);
        } 
        fclose(fs);
    }
    printf("Valor total: %.2f\n", valorFinal);
    printf("--------------------------------------------------\n");
    liberarReserva(&pessoa);
}

/*
* Função principal do programa, que lê os comandos e chama as funções correspondentes
*
* @param void
* @return int 0 para finalização do programa sem erros 
*/

int main(void) {
    int  quantresv = 0, voo = 0;
    float valortotal = 0;
    while (1) {
        char comando[3];
        scanf(" %s", comando);

        if (!strcmp(comando, "AV\0")) {
            aberturaVoo();
        } else if (!strcmp(comando, "RR\0")) {
            realizarReserva(&valortotal, &quantresv, &voo);
            if (voo) {
                fecharVoo(valortotal);
            }
        } else if (!strcmp(comando, "CR\0")) {
            consultarReserva();
        } else if (!strcmp(comando, "CA\0")) {
            cancelarReserva(&valortotal, &quantresv);
        } else if (!strcmp(comando, "MR\0")) {
            modificarReserva();
        } else if (!strcmp(comando, "FD\0")) {
            fecharDia(quantresv, valortotal);
        } else if(!strcmp(comando, "FV\0")){
            fecharVoo(valortotal);
        }
        else {
            continue;
        }
    }
    return 0;
}