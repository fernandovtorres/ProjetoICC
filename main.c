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
}reserva;

int VooExiste(); //Verifica se o arquivo do voo existe
int RegistroExiste(); //Verifica se o arquivo de registro existe
void aberturaVoo(); //Abre o voo
int realizarReserva(int *valorTotal, int *quantresv, int *voo); //Realiza a reserva
void fecharDia(int quantresv, int qtd_ec, int qtd_ex); //Fecha o dia
void consultarReserva(); //imprimir a reserva encontrada
int consultaCPF(char cpf[15]); //Consulta a reserva
void cancelarReserva();
void modificarReserva();
int consultaNome(char nome[50], char sobrenome[50]); //Consulta a reserva
int consultaAssento(char assento[4]); //Consulta a reserva
int monetarioExiste(); //Verifica se o arquivo monetario existe
void fecharVoo(int valortotal); //Fecha o voo

int VooExiste() {
    FILE *fs;
    if((fs = fopen("AVoo.dat", "rb"))) {
        fclose(fs);
        return 1;
    }
    return 0;
}

int RegistroExiste() {
    FILE *fs;
    if((fs = fopen("RegistroPessoa.dat", "rb"))) {
        fclose(fs);
        return 1;
    }
    return 0;
}

int monetarioExiste() {
    FILE *fs;
    if((fs = fopen("monetario.dat", "rb"))) {
        fclose(fs);
        return 1;
    }
    return 0;
}

void aberturaVoo(){
    if(VooExiste()) {
        printf("O voo já foi aberto!");
    } else {
        FILE *fs;
        fs = fopen("AVoo.dat", "wb");
        int ndeassentos;
        float precoEconomica, precoExecutiva;
        scanf(" %d %f %f", &ndeassentos, &precoEconomica, &precoExecutiva);
        fwrite(&ndeassentos, sizeof(int), 1, fs);
        fprintf(fs, "\t");
        fwrite(&precoEconomica, sizeof(float), 1, fs);
        fprintf(fs, "\t");
        fwrite(&precoExecutiva, sizeof(float), 1, fs);
        fprintf(fs, "\n");
        fclose(fs);
    }
}

int realizarReserva(int *valorTotal, int *quantresv, int *voo){
    if(!VooExiste()){
        printf("Voo não foi aberto!\n");
        exit(0);
    } else {
        reserva pessoa;
        FILE *fs, *fs2;
        fs = fopen("RegistroPessoa.dat", "wb");
        if(monetarioExiste()) {
            fs2 = fopen("monetario.dat", "rb");
            int assentos;
            fread(&assentos, sizeof(int), 1, fs2);
            (*quantresv) += 1;
            if(*quantresv - assentos == 0){
                *voo = 1;
            }
            fclose(fs2);
        }
        char nomee[50], sobrenome[50], cpf[15], id[5], assento[4], classe[10], origem[4], destino[4];
        fgets(nomee, sizeof(nomee), stdin);
        fgets(sobrenome, sizeof(sobrenome), stdin);
        fgets(cpf, sizeof(cpf), stdin);
        scanf("%d %d %d", &pessoa.dia, &pessoa.mes, &pessoa.ano);
        fgets(id, sizeof(id), stdin);
        fgets(assento, sizeof(assento), stdin);
        fgets(classe, sizeof(classe), stdin);
        scanf("%f", &pessoa.valor);
        *valorTotal += pessoa.valor;
        fgets(origem, sizeof(origem), stdin);
        fgets(destino, sizeof(destino), stdin);
        int tamanho = strlen(nomee);
        pessoa.nome = (char*)malloc(tamanho*sizeof(char));
        strcpy(pessoa.nome, nomee);

        tamanho = strlen(sobrenome);
        pessoa.sobrenome = (char*)malloc(tamanho*sizeof(char));
        strcpy(pessoa.sobrenome, sobrenome);

        tamanho = strlen(cpf);
        pessoa.cpf = (char*)malloc(tamanho*sizeof(char));
        strcpy(pessoa.cpf, cpf);

        tamanho = strlen(id);
        pessoa.id = (char*)malloc(tamanho*sizeof(char));
        strcpy(pessoa.id, id);

        tamanho = strlen(assento);
        pessoa.assento = (char*)malloc(tamanho*sizeof(char));
        strcpy(pessoa.assento, assento);

        tamanho = strlen(classe);
        pessoa.classe = (char*)malloc(tamanho*sizeof(char));
        strcpy(pessoa.classe, classe);

        tamanho = strlen(origem);
        pessoa.origem = (char*)malloc(tamanho*sizeof(char));
        strcpy(pessoa.origem, origem);

        tamanho = strlen(destino);
        pessoa.destino = (char*)malloc(tamanho*sizeof(char));
        strcpy(pessoa.destino, destino);   
        
        fseek(fs, 0, SEEK_END);
        fwrite(&pessoa, sizeof(reserva), 1, fs);
        fprintf(fs, "\n");

        if(!strcmp(pessoa.classe, "executiva\0")){
            fclose(fs);
            return (1);
        }else{
            fclose(fs);
            return(0);
        }
    }
}

void fecharDia(int quantresv, int qtd_ec, int qtd_ex){
    if(!VooExiste()){
        printf("Voo não foi aberto!\n");
        exit(0);
    } else {
        FILE *fs; FILE *fs2;
        fs = fopen("AVoo.dat", "rb");
        float pec;
        float pex;
        int ndeassentos;
        fseek(fs, 0, SEEK_SET);
        fread(&ndeassentos, sizeof(int), 1, fs);
        fseek(fs, sizeof(char), SEEK_CUR);
        fread(&pec, sizeof(float), 1, fs);
        fseek(fs, sizeof(char), SEEK_CUR);  
        fread(&pex, sizeof(float), 1, fs);
        float total = qtd_ex*pex + qtd_ec*pec;
        if(!monetarioExiste()){
            fs = fopen("monetario.dat", "wb");
            int assentos = ndeassentos - quantresv;
            fwrite(&assentos, sizeof(int), 1, fs);
            fprintf(fs, "\t");
            fwrite(&total, sizeof(float), 1, fs);
            fprintf(fs, "\n");
            fclose(fs);
        } else {
            fs = fopen("monetario.dat", "rb");
            fs2 = fopen("monetariotemp.dat", "wb");
            fread(&ndeassentos, sizeof(int), 1, fs);
            fseek(fs, sizeof(char), SEEK_CUR);
            fread(&total, sizeof(float), 1, fs);
            ndeassentos -= quantresv;
            total += qtd_ex*pex + qtd_ec*pec;
            fwrite(&ndeassentos, sizeof(int), 1, fs2);
            fprintf(fs2, "\t");
            fwrite(&total, sizeof(float), 1, fs2);
            fprintf(fs2, "\n");
            fclose(fs);
            fclose(fs2);
            remove("monetario.dat");
            rename("monetariotemp.dat", "monetario.dat");   
        }
        

        printf("Fechamento do dia:\n");
        printf("Quantidade de reservas: %d\n", quantresv);
        printf("Posição: %.2f\n", total);
        printf("--------------------------------------------------\n");
    }
}

int consultaCPF(char cpf[15]) {
    if(!VooExiste() || !RegistroExiste()){
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
    } else {
        FILE *fs;
        fs = fopen("RegistroPessoa.dat", "rb");
        reserva pessoa;
        fseek(fs, 0, SEEK_SET);
        int i = 0;
        while(fread(&pessoa, sizeof(reserva), 1, fs) && !feof(fs)){
            if(!strcmp(pessoa.cpf, cpf)){
                fclose(fs);
                return i;
            }
            i++;
            fseek(fs, i*sizeof(reserva), SEEK_SET);
        }
        fclose(fs);
        return -1;
    }
}

int consultaNome(char nome[50], char sobrenome[50]){
    if(!VooExiste() || !RegistroExiste()){
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
    } else {
        FILE *fs;
        fs = fopen("RegistroPessoa.dat", "rb");
        reserva pessoa;
        fseek(fs, 0, SEEK_SET);
        int i = 0;
        while(fread(&pessoa, sizeof(reserva), 1, fs) && !feof(fs)){
            if(!strcmp(pessoa.nome, nome) && !strcmp(pessoa.sobrenome, sobrenome)){
                fclose(fs);
                return i;
            }
            i++;
            fseek(fs, i*sizeof(reserva), SEEK_SET);
        }
        fclose(fs);
        return -1;
    }
}

int consultaAssento(char assento[4]){
    if(!VooExiste() || !RegistroExiste()){
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
    } else {
        FILE *fs;
        fs = fopen("RegistroPessoa.dat", "rb");
        reserva pessoa;
        fseek(fs, 0, SEEK_SET);
        int i = 0;
        while(fread(&pessoa, sizeof(reserva), 1, fs) && !feof(fs)){
            if(!strcmp(pessoa.assento, assento)){
                fclose(fs);
                return i;
            }
            i++;
            fseek(fs, i*sizeof(reserva), SEEK_SET);
        }
        fclose(fs);
        return -1;
    }
}

void consultarReserva(){
    if(!VooExiste() || !RegistroExiste()){
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
    } else {
        FILE *fs;
        fs = fopen("RegistroPessoa.dat", "rb");
        reserva pessoa;
        char cpf[15];
        fgets(cpf, sizeof(cpf), stdin);
        if(consultaCPF(cpf) == -1){
            printf("CPF não encontrado!\n");
            return;
        } else {
            fseek(fs, consultaCPF(cpf)*sizeof(reserva), SEEK_SET);
            fread(&pessoa, sizeof(reserva), 1, fs);
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
        fseek(fs, 0, SEEK_SET);
        fclose(fs);
    }
}

void cancelarReserva(){
    if(!VooExiste() || !RegistroExiste()){
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
    } else {
        FILE *fs; FILE *fs2;
        fs = fopen("RegistroPessoa.dat", "rb");
        fs2 = fopen("RegistroPessoatemp.dat", "wb");
        reserva pessoa;
        char cpf[15];
        fgets(cpf, sizeof(cpf), stdin);
        int del_num = consultaCPF(cpf);
        if(del_num == -1){
            printf("CPF não encontrado!\n");
            return;
        } else {
            int line_num = 0;
            while(fread(&pessoa, sizeof(reserva), 1, fs) && !feof(fs)){
                if(line_num != del_num){
                    fwrite(&pessoa, sizeof(reserva), 1, fs2);
                    fprintf(fs2, "\n");
                }
                line_num++;
            }
        }
        fclose(fs);
        fseek(fs2, 0, SEEK_SET);
        fclose(fs2);
        remove("RegistroPessoa.dat");
        rename("RegistroPessoatemp.dat", "RegistroPessoa.dat");
    }
}

void modificarReserva() {
    if(!VooExiste() || !RegistroExiste()){
        printf("Voo não foi aberto ou nenhum registro existe!\n");
        exit(0);
    } else {
        FILE *fs; FILE *fs2;
        fs = fopen("RegistroPessoa.dat", "rb");
        fs2 = fopen("RegistroPessoatemp.dat", "wb");
        reserva pessoa;
        reserva pessoa2;
        char nomee[50], sobrenome[50], cpf[15], assento[4];
        fgets(nomee, sizeof(nomee), stdin);
        fgets(sobrenome, sizeof(sobrenome), stdin);
        fgets(cpf, sizeof(cpf), stdin);
        fgets(assento, sizeof(assento), stdin);
        if(consultaNome(nomee, sobrenome) != -1 || consultaCPF(cpf) != -1 || consultaAssento(assento) != -1){
            int mod_line = consultaNome(nomee, sobrenome) != -1 ? consultaNome(nomee, sobrenome) : consultaCPF(cpf) > -1 ? consultaCPF(cpf) : consultaAssento(assento);
            int line_num = 0;
            while(fread(&pessoa, sizeof(reserva), 1, fs) && !feof(fs)){
                if(line_num != mod_line){
                    fwrite(&pessoa, sizeof(reserva), 1, fs2);
                    fprintf(fs2, "\n");
                } else {
                    int tamanho = strlen(nomee);
                    pessoa2.nome = (char*)malloc(tamanho*sizeof(char));
                    strcpy(pessoa2.nome, nomee);

                    tamanho = strlen(sobrenome);
                    pessoa2.sobrenome = (char*)malloc(tamanho*sizeof(char));
                    strcpy(pessoa2.sobrenome, sobrenome);
                    
                    tamanho = strlen(cpf);
                    pessoa2.cpf = (char*)malloc(tamanho*sizeof(char));
                    strcpy(pessoa2.cpf, cpf);

                    tamanho = strlen(assento);
                    pessoa2.assento = (char*)malloc(tamanho*sizeof(char));
                    strcpy(pessoa2.assento, assento);

                    pessoa2.dia = pessoa.dia; pessoa2.mes = pessoa.mes; pessoa2.ano = pessoa.ano; pessoa2.valor = pessoa.valor;
                    pessoa2.id = pessoa.id; pessoa2.classe = pessoa.classe; pessoa2.origem = pessoa.origem; pessoa2.destino = pessoa.destino;
                    fwrite(&pessoa2, sizeof(reserva), 1, fs2);
                    printf("Reserva modificada:\n%s\n%s %s\n%d/%d/%d\nVoo: %s\nAssento: %s\nClasse: %sTrecho: %s %s\nValor: %.2f\n", pessoa2.cpf, pessoa2.nome, pessoa2.sobrenome, pessoa2.dia, pessoa2.mes, pessoa2.ano, pessoa2.id, pessoa2.assento, pessoa2.classe, pessoa2.origem, pessoa2.destino, pessoa2.valor);
                    printf("--------------------------------------------------\n");
                }
            }
        }
        fclose(fs);
        fseek(fs2, 0, SEEK_SET);
        fclose(fs2);
        remove("RegistroPessoa.dat");
        rename("RegistroPessoatemp.dat", "RegistroPessoa.dat"); 
    }
}

void fecharVoo(int valortotal){
    printf("Voo fechado!\n\n");
    reserva pessoa;
    FILE *fs, *fs2;
    fs = fopen("RegistroPessoa.dat", "rb");
    fs2 = fopen("monetario.dat", "rb");
    while(fread(&pessoa, sizeof(reserva), 1, fs) && !feof(fs)){
        printf("%s\n", pessoa.cpf);
        printf("%s", pessoa.nome);
        printf(" %s\n", pessoa.sobrenome);
        printf("Assento: %s\n\n", pessoa.assento);
    }
    fseek(fs2, sizeof(int), SEEK_SET);
    fseek(fs2, sizeof(char), SEEK_CUR);
    float total;
    fread(&total, sizeof(float), 1, fs2);
    total += valortotal;
    printf("Valor Total: %.2f\n", total);
    printf("--------------------------------------------------\n");
    fclose(fs);
    fclose(fs2);
    remove("AVoo.dat");
    remove("RegistroPessoa.dat");
    remove("monetario.dat");
    exit(0);
}

int main() {
    int qtd_ec=0, qtd_ex=0, quantresv=0, valortotal=0,  voo = 0;
    while(1) {
        char comando[3];
        fgets(comando, 3, stdin);

        if(!strcmp(comando, "AV\0")) {
            aberturaVoo();
        }
        else if(!strcmp(comando, "RR\0")){
            if(realizarReserva(&valortotal, &quantresv, &voo)){
                qtd_ex++; //return da função realizarReserva como 1 ocorre quando a classe é def. como executiva
            }
            else{
                qtd_ec++; //return da função realizarReserva como 0 ocorre quando a classe é def. como executiva
            }
            if(voo) {
                fecharVoo(valortotal);
            }
        }
        else if(!strcmp(comando, "CR\0")){
            consultarReserva();
        }
        else if(!strcmp(comando, "CA\0")){
            cancelarReserva();
        }
        else if(!strcmp(comando, "MR\0")){
            modificarReserva();
        }
        else if(!strcmp(comando, "FD\0")){
            fecharDia(quantresv, qtd_ec, qtd_ex);
            break;
        } else if(!strcmp(comando, "FV\0")){
            fecharVoo(valortotal);
        }
        else {
            printf("Comando inválido!\n");
        }
    }
    return 0;
}