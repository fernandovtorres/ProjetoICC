#include <stdio.h>
#include <stdlib.h>
//parte do projeto de ICC feita por mim

typedef struct{ //obs particular da variavel classe: pensei em deixar a classe com valor de 0 ou 1 pra economica (0) e executiva (1), fica mais facil de guardar
    int cpf, data, voo, classe, valor; //variaveis um tanto quanto faceis de entender o que representam, nao vou explicar
    char nome[20], sobrenome[20], origem[4], destino[4], assento[4];
}pessoa;

void cr(pessoa p){ //funçao chamada pro comando de consultar reserva
    printf("%d\n", p.cpf);

    printf("%s %s\n", p.nome, p.sobrenome);

    printf("%d\n", p.data);

    printf("Voo: %d\n", p.voo);

    printf("Assento: %s\n", p.assento);

    printf("Classe: ");
    if(p.classe) printf("executiva\n");
    else printf("economica\n");

    printf("Trecho: %s %s\n", p.origem, p.destino);

    printf("Valor: %d\n", p.valor);

    printf("--------------------------------------------------\n");
}

int main(void){
    pessoa galera[10]; //a galera aqui é o vetor de todos os clientes que vamos ter por aí (não sei quantos vão ser ent deixei 10)
    char comando[3];
    int cpf1; //esse cpf aqui é pra localizar a pessoa que eu quero, vou usar ele só pra ler o cpf e procurar dentro do vetor
    scanf("%s", &comando);

    if(comando == 'CR'){
        scanf("%d", &cpf1); //leio o cpf da respectiva pessoa de quem quero consultar a reserva
        for(int i = 0; i < 10; i++){
            if(galera[i].cpf == cpf1){ //procuro a pessoa com esse cpf dentro do vetor
                cpf1 = i; //marco qual a posição da pessoa com esse cpf la do vetor na variavel
                break; //se eu ja achei, saio do loop
            }
        }
        cr(galera[cpf1]); //chamo a função (fora do loop) com as informações da pessoa com o cpf que eu peguei
    }
}
//ate agora só fiz pra CR, vou tentar os outros comandos depois