#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//parte do projeto de ICC feita por mim (daniel)

typedef struct{ //obs particular da variavel classe: pensei em deixar a classe com valor de 0 ou 1 pra economica (0) e executiva (1), fica mais facil de guardar
    int data, voo, classe, valor, existe; //variaveis um tanto quanto faceis de entender o que representam, nao vou explicar
    char *nome, *sobrenome, *origem, *destino, *assento, *cpf; //só uma obs.: a variavel existe vai me dizer se tal pessoa existe ou não no registro
}pessoa;                                                             //quando usar a funçao de cancelar reserva eu passo ela pra zero e ja era (tem que pensar em um jeito de zerar as outras informações quando passo o existe pra zero, me ajudem aí)

int acha(pessoa galera[10]){ //uso essa função pra ler qual o cpf que eu quero localizar
    int cont;
    char cpf[15];
    scanf(" %s", &cpf);
    for(int i = 0; i < 10; i++){ //a partir daí, saio procurando dentro do vetor de pessoas, na variavel cpf, aquele que foi digitado
        if(!strcmp(cpf, galera[i].cpf)){
            cont = i; //a variavel recebe a posição do vetor que o cpf ta localizado
            break;
        }
    }
    return cont; //retorno a posição que me interessa
}

void cr(pessoa p){ //funçao chamada pro comando de consultar reserva
    printf("%s\n", p.cpf);

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
    char comando[3], nome[20], sobrenome[20], assento[4], cpf[15];
    int index; //esse index aqui é pra localizar a pessoa que eu quero, vou usar ele só pra procurar a posição que me interessa dentro do vetor
    scanf("%s", &comando);

    if(!strcmp(comando, "CR\0")){
        index = acha(galera); 
        cr(galera[index]); //chamo a função (fora do loop) com as informações da pessoa com o cpf que me foi dado
    }

    if(!strcmp(comando, "CA\0")){ //exata mesma logica de cima, só repeti tudo e dividi os dois em duas parte diferente pra ficar mais organizadinho de ver
        index = acha(galera);
        ca(galera[index]);
    }

    if(!strcmp(comando, "MR\0")){ //obs.:pro comando MR não criei função pq tem que puxar muita string, não valeria fazer 50 funçao pra isso
        pessoa Nova;
        fgets(nome, 20, stdin); //recebo o nome a ser modificado e guardo numa variavel string
        fgets(sobrenome, 20, stdin); //recebo o sobrenome a ser modificado e mema coisa de cima
        fgets(cpf, 15, stdin);  //recebo o cpf em que quero mexer
        index = acha(galera);  //localizo o cpf que vai receber a mudança
        Nova.nome = calloc(strlen(nome), sizeof(char));
        Nova.nome = nome; //coloco o nome que recebi
        Nova.sobrenome = calloc(strlen(sobrenome), sizeof(char));
        Nova.sobrenome = sobrenome; //coloco o sobrenome que recebi
        fgets(assento, 4, stdin); //recebo o assento a ser modificado etc. etc.
        Nova.assento = calloc(strlen(assento), sizeof(char));
        Nova.assento = assento; //coloco o assento que recebi
        galera[index] = Nova; //coloco em galera
    }
}
//ate agora só fiz pra CR e um protótipo de CA (talvez nem isso), vou tentar os outros comandos depois