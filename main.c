#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* nome;
    char* sobrenome;
    char* cpf;
    int dia;
    int mes;
    int ano;
    char* id;
    char* assento;
    char* classe;
    float valor;
    char* origem;
    char* destino;
}reserva;


int VooExiste() {
    FILE *fs;
    if((fs = fopen("AVoo.dat", "rb"))) {
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

int realizarReserva(){
    reserva pessoa;
    FILE *fs;
    fs = fopen("RegistroPessoa.dat", "wb");

    if(pessoa.classe == "executiva"){
        return(1);
    }
    else{
        return(0);
    }
}

void fecharDia(int quantresv, int qtd_ec, int qtd_ex){
    FILE *fs;
    fs = fopen("AVoo.dat", "rb");

    float pec;
    float pex;

    fread(&ndeassentos, sizeof(int), 1, fs);
    fread(&pec, sizeof(float), 1, fs);
    fread(&pex, sizeof(float), 1, fs);

    fclose(fs);

    printf("Fechamento do dia:\n");
    printf("Quantidade de reservas: %d\n", quantresv);
    printf("Posição: %.2f\n", (qtd_ec*pec)+(qtd_ex*pex));
    printf("--------------------------------------------------\n");
}

int main() {
    int qtd_ec=0, qtd_ex=0, quantresv=0;
    while(1) {
        char comando[3];
        fgets(comando, 3, stdin);

        if(!strcmp(comando, "AV\0")) {
            aberturaVoo();
        } 

        else if(!strcmp(comando, "RR\0")){
            if(realizarReserva()){
                qtd_ex++; //return da função realizarReserva como 1 ocorre quando a classe é def. como executiva
            }
            else{
                qtd_ec++; //return da função realizarReserva como 0 ocorre quando a classe é def. como executiva
            }
            quantresv++;
        }

        else if(!strcmp(comando, "FD\0")){
            fecharDia(quantresv, qtd_ec, qtd_ex);
        }
    }
    return 0;
}
