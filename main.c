
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void consultarReserva(); //imprimir a reserva encontrada
int consultaCPF(char cpf[15]); //Consulta a reserva
void cancelarReserva(float* valorTotal, int* qntresv); //Cancela a reserva
void modificarReserva();
int consultaNome(char nome[50], char sobrenome[50]); //Consulta a reserva
int consultaAssento(char assento[4]); //Consulta a reserva
int monetarioExiste(); //Verifica se o arquivo monetario existe
void fecharVoo(float valortotal); //Fecha o voo
void alocaTemp(reserva* pessoa);
void liberarReserva(reserva* pessoa);

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

int VooExiste() {
    FILE *fs = fopen("AVoo.dat", "rb");
    if (fs) {
        fclose(fs);
        return 1;
    }
    return 0;
}

int RegistroExiste() {
    FILE *fs = fopen("RegistroPessoa.dat", "rb");
    if (fs) {
        fclose(fs);
        return 1;
    }
    return 0;
}

int monetarioExiste() {
    FILE *fs = fopen("monetario.dat", "rb");
    if (fs) {
        fclose(fs);
        return 1;
    }
    return 0;
}

void aberturaVoo() {
    if (VooExiste()) {
        printf("O voo já foi aberto!\n");
    } else {
        FILE *fs = fopen("AVoo.dat", "wb");
        int ndeassentos;
        float precoEconomica, precoExecutiva;
        scanf(" %d", &ndeassentos);
        scanf(" %f", &precoEconomica);
        scanf(" %f", &precoExecutiva);
        fwrite(&ndeassentos, sizeof(int), 1, fs);
        fwrite(&precoEconomica, sizeof(float), 1, fs);
        fwrite(&precoExecutiva, sizeof(float), 1, fs);
        fclose(fs);
    }
}

void realizarReserva(float *valorTotal, int *quantresv, int *voo) {
    if (!VooExiste()) {
        printf("Voo não foi aberto!\n");
        exit(0);
    } else {
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

void fecharDia(int quantresv, 
float valorTotal) {
    if (!VooExiste()) {
        printf("Voo não foi aberto!\n");
        exit(0);
    } else {
        FILE *fs = fopen("AVoo.dat", "rb");
        int ndeassentos;
        fread(&ndeassentos, sizeof(int), 1, fs);
        fclose(fs);

        if (!monetarioExiste()) {
            fs = fopen("monetario.dat", "wb");
            int assentos = ndeassentos - quantresv;
            fwrite(&assentos, sizeof(int), 1, fs);
            fwrite(&valorTotal, sizeof(float), 1, fs);
            fclose(fs);
        } else {
            fs = fopen("monetario.dat", "rb");
            FILE *fs2 = fopen("monetariotemp.dat", "wb");
            int assentos;
            float total_prev;
            fread(&assentos, sizeof(int), 1, fs);
            fread(&total_prev, sizeof(float), 1, fs);
            fclose(fs);

            assentos -= quantresv;
            valorTotal += total_prev;

            fwrite(&assentos, sizeof(int), 1, fs2);
            fwrite(&valorTotal, sizeof(float), 1, fs2);
            fclose(fs2);

            remove("monetario.dat");
            rename("monetariotemp.dat", "monetario.dat");
        }

        printf("Fechamento do dia:\n");
        printf("Quantidade de reservas: %d\n", quantresv);
        printf("Posição: %.2f\n", valorTotal);
        printf("--------------------------------------------------\n");
        exit(0);
    }
}

int consultaCPF(char cpf[15]) {
    if (!VooExiste() || !RegistroExiste()) {
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
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

void modificarReserva(){
    if (!VooExiste() || !RegistroExiste()) {
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
    } else {
        reserva pessoa;
        alocaTemp(&pessoa);
        FILE *fs = fopen("RegistroPessoa.dat", "rb");
        FILE *fs2 = fopen("RegistroPessoaTemp.dat", "wb");
        char nomee[50], sobrenome[50], cpf[15], assento[4];
        scanf(" %s", nomee);
        scanf(" %s", sobrenome);
        scanf(" %s", cpf);
        scanf(" %s", assento);
        if (consultaNome(nomee, sobrenome) != -1 || consultaCPF(cpf) != -1 || consultaAssento(assento) != -1) {
            int mod_line = consultaNome(nomee, sobrenome) != -1 ? consultaNome(nomee, sobrenome) : consultaCPF(cpf) > -1 ? consultaCPF(cpf) : consultaAssento(assento);
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
        } else {
            printf("Reserva não encontrada!\n");
        }
    }
}

void cancelarReserva(float *valorTotal, int* qntresv) {
    if (!VooExiste() || !RegistroExiste()) {
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
    } else {
        reserva pessoa;
        alocaTemp(&pessoa);
        FILE *fs = fopen("RegistroPessoa.dat", "rb");
        FILE *fs2 = fopen("RegistroPessoaTemp.dat", "wb");
        char cpf[15];
        scanf(" %s", cpf);
        int found = 0;

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
                found = 1;
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

        if (found) {
            printf("Reserva cancelada com sucesso!\n");
        } else {
            printf("Reserva não encontrada!\n");
        }
        liberarReserva(&pessoa);
    }
}

void consultarReserva() {
    FILE *fs = fopen("RegistroPessoa.dat", "rb");
    reserva pessoa;
    alocaTemp(&pessoa);
    char cpf[15];
    scanf(" %s", cpf);
    int pos = consultaCPF(cpf);
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

int consultaNome(char nome[50], char sobrenome[50]) {
    if (!VooExiste() || !RegistroExiste()) {
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
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
            if (strcmp(pessoa.nome, nome) == 0 && strcmp(pessoa.sobrenome, sobrenome) == 0) {
                return i;
            } else {
                i++;
            }
        }
        fclose(fs);
        liberarReserva(&pessoa);
        return -1;
    }
}

int consultaAssento(char assento[4]) {
    if (!VooExiste() || !RegistroExiste()) {
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
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
            if (strcmp(pessoa.assento, assento) == 0) {
                return i;
            } else {
                i++;
            }
        }
        fclose(fs);
        liberarReserva(&pessoa);
        return -1;
    }
}

void fecharVoo(float valortotal) {
    if (!VooExiste()) {
        printf("Voo não foi aberto!\n");
        exit(0);
    } else {
        FILE *fs = fopen("RegistroPessoa.dat", "rb");
        if(monetarioExiste()){
            FILE *fs2 = fopen("monetario.dat", "rb");
            float total_prev;
            fseek(fs2, sizeof(int), SEEK_SET);
            fread(&total_prev, sizeof(float), 1, fs2);
            fclose(fs2);
            valortotal += total_prev;
        }
        reserva pessoa;
        alocaTemp(&pessoa);

        printf("Voo Fechado!\n\n");
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
        printf("Valor total: %.2f\n", valortotal);
        
        printf("--------------------------------------------------\n");

        remove("AVoo.dat");
        remove("RegistroPessoa.dat");
        remove("monetario.dat");
        exit(0);
    }
}

int main() {
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
            printf("Comando inválido!\n");
        }
    }
    return 0;
}