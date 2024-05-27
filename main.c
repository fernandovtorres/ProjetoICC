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

void realizarReserva(){
    reserva pessoa;
    FILE *fs;
    fs = fopen("RegistroPessoa.dat", "wb");
    
}

int main() {
    while(1) {
        char comando[3];
        fgets(comando, 3, stdin);
        if(!strcmp(comando, "AV\0")) {
            aberturaVoo();
        } else if(!strcmp(comando, "RR\0")){
            realizarReserva();
        }
    }
    return 0;
}