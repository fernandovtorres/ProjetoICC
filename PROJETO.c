#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//parte do projeto de ICC feita por mim (daniel)

typedef struct{ //obs particular da variavel classe: pensei em deixar a classe com valor de 0 ou 1 pra economica (0) e executiva (1), fica mais facil de guardar
    int cpf, data, voo, classe, valor, existe; //variaveis um tanto quanto faceis de entender o que representam, nao vou explicar
    char nome[20], sobrenome[20], origem[4], destino[4], assento[4]; //só uma obs.: a variavel existe vai me dizer se tal pessoa existe ou não no registro
}pessoa;                                                             //quando usar a funçao de cancelar reserva eu passo ela pra zero e ja era (tem que pensar em um jeito de zerar as outras informações quando passo o existe pra zero, me ajudem aí)

int acha(pessoa galera[10]){ //uso essa função pra ler qual o cpf que eu quero localizar
    int cpf;
    scanf("%d", &cpf);
    for(int i = 0; i < 10; i++){ //a partir daí, saio procurando dentro do vetor de pessoas, na variavel cpf, aquele que foi digitado
        if(galera[i].cpf == cpf){
            cpf = i; //a variavel recebe a posição do vetor que o cpf ta localizado
            break;
        }
    }
    return cpf; //retorno a posição que me interessa
}

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

void ca(pessoa p){
    p.existe = 0; //cancelo tal reserva e os registros vao parar de existir (como vou fazer isso não sei, mas esse é o começo da ideia)
    //isso aqui não funciona de vdd, só fiz pra marcar a ideia que tive e lembrar de discutir como colocar isso em prática
} 

int main(void){
    pessoa galera[10]; //a galera aqui é o vetor de todos os clientes que vamos ter por aí (não sei quantos vão ser ent deixei 10)
    char comando[3];
    int cpf; //esse cpf aqui é pra localizar a pessoa que eu quero, vou usar ele só pra ler o cpf e procurar dentro do vetor
    scanf("%s", &comando);

    if(strcmp(comando, "CR\0")){
        cpf = acha(galera); //detalhe: o valor dessa variavel é meramente a posição no vetor da pessoa com tal cpf e não o proprio numero do cpf, só pra deixar claro
        cr(galera[cpf]); //chamo a função (fora do loop) com as informações da pessoa com o cpf que me foi dado
    }

    if(strcmp(comando, "CA\0")){ //exata mesma logica de cima, só repeti tudo e dividi os dois em duas parte diferente pra ficar mais organizadinho de ver
        cpf = acha(galera);
        ca(galera[cpf]);
    }

    if(strcmp(comando, "MR\0")){
        cpf = acha(galera);
        //parei aqui, em progresso!!!
        //a ideia agora é criar uma função em que eu posso alterar os dados da reserva (obviamente, talvez)
    }
}
//ate agora só fiz pra CR, vou tentar os outros comandos depois