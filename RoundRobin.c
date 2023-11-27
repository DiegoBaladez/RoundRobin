/*
1) Escrever um algoritmo que simula a atuação do método de escalonamento Round Robin com feedback.

2) Escrever um relatório: premissas + saídas do trabalho

1.1) Como funciona o Round Robin?

Premissas:

*Limite máximo de processos criados: 4

*Fatia de tempo dada para os processos: quanto tempo nosso processos ficaram com a CPU? Definir qtd de tempo.

*Tempo de serviço e I/O: cada processo precisa ter seu tempo de serviço e uma quantidade aleatória de I/O

*Tempo de I/O e suas possibilidades: Disco, Fita e impressora. 


Escalonador com o Round Robin:


Processos:
cara processo precisa ter um PID. Cada processo precisa ter informação do PCB (Contexto de software: prioridade, pid,ppid,status)

Escalona dor com 3 filas baseadas na sua prioridade: uma de alta e outra de baixa, 1 fila de I/O. Ordem de entrada: 
    Processos novos: entram na fila de alta prioridade.






O que precisamos fazer?
1) Escrever uma struct de processos com as características aceima.

2) Implementar uma função para o Round Robin.

3) Criar essas 3 filas de prioridade.

4) Criar uma função que escolhe qual processo aleatoriamente vai chegar primeiro no round robin.


*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
int QUANTUM =  2;
int qtdProcessos = 100;

struct Processos {
    int tempoDeServico;
    int contadorDeTempoAtivo;
    int pid;
    int prioridade;
    char status[20]; // pronto, suspenso, bloqueado
    int quantidadeDeIO;
};

struct Processos* criaProcesso() {
    struct Processos *novoProcesso = (struct Processos*)malloc(sizeof(struct Processos));

    if (novoProcesso == NULL) {
        printf("Erro: Não foi possível alocar memória para o processo.\n");
        exit(EXIT_FAILURE);
    }

    novoProcesso->tempoDeServico = rand() % (90 - 4 + 1) + 1;
    novoProcesso->contadorDeTempoAtivo = 0;
    novoProcesso->pid = rand() % (80 - 0 + 1) + 1;
    novoProcesso->prioridade = rand() % (5 - 1 + 1) + 1;
    strcpy(novoProcesso->status, "pronto");
    novoProcesso->quantidadeDeIO = rand() % (3 - 0);

    return novoProcesso;
};

bool roundRobin(struct Processos *listaDeProcessos[]) {
    struct Processos *processoAtual;
    int contaProcessosTerminados = 0;
    int contaPrint = 0;

    // itera sobre os processos prontos
    /*for (int i = 0; i < qtdProcessos; i++) {
        processoAtual = listaDeProcessos[i];

        if (processoAtual->contadorDeTempoAtivo >= processoAtual->tempoDeServico) continue;

        // adiciona 1 unidade de tempo, até duas, no contador interno do processo
        for (int j = 0; j < QUANTUM; j++) {
            processoAtual->contadorDeTempoAtivo++;

            // verifica se atingiu o tempo de serviço. Se sim, incrementa o contador de processos terminados.
            if (processoAtual->contadorDeTempoAtivo == processoAtual->tempoDeServico) {
                contaProcessosTerminados++;
                printf("O processo com o PID %d, acabou com %d unidades de tempo:, sua posição na fila eh %d \n", processoAtual->pid,processoAtual->contadorDeTempoAtivo,i);
                break;
            }
        }

        // sai do loop se todos os processos terminaram
        if (contaProcessosTerminados == qtdProcessos) break;

        // reseta o contador para repassar pelos programas que ainda não terminaram
        if (i == qtdProcessos - 1) i = -1;
    }
    */

    while(contaProcessosTerminados != qtdProcessos){
        for (int i = 0; i < qtdProcessos; i++) {
        processoAtual = listaDeProcessos[i];
        

        if (processoAtual->contadorDeTempoAtivo >= processoAtual->tempoDeServico) continue;

        // adiciona 1 unidade de tempo, até duas, no contador interno do processo
        for (int j = 0; j < QUANTUM; j++) {
            processoAtual->contadorDeTempoAtivo++;

            // verifica se atingiu o tempo de serviço. Se sim, incrementa o contador de processos terminados.
            if (processoAtual->contadorDeTempoAtivo == processoAtual->tempoDeServico) {
                contaProcessosTerminados++;
                printf("O processo com o PID %d, acabou com %d unidades de tempo:, sua posição na fila eh %d \n", processoAtual->pid,processoAtual->contadorDeTempoAtivo,i);
                contaPrint ++;
                break;
            }
        }       

        // realiza a circularidade do roundrobin para passar pelos programas que ainda não finalizaram
        if (i == qtdProcessos - 1) i = -1;
    }
    }
//saiu [0], saiu[3],saiu[4],saiu[6], saiu[5], saiu[9],saiu[1],saiu[2]
    printf("Contagem dos prints %d\n",contaPrint);
    return true;
}

int main(void) {
    bool terminou;
    srand(time(NULL));
    struct Processos *listaDeAltaPrioridade[qtdProcessos];

    // preenche a lista de alta prioridade com os processos criados
    for (int i = 0; i < qtdProcessos; i++) {
        listaDeAltaPrioridade[i] = criaProcesso();
    }

    // exibe os pids dos processos criados
    for (int i = 0; i < qtdProcessos; i++) {        
        printf("Esse processo foi criado com o PID = %d, está na posição da fila %d e possui o tempo de serviço igual a: %d \n", listaDeAltaPrioridade[i]->pid, i,listaDeAltaPrioridade[i]->tempoDeServico);
    }
    
    printf("\n\n");

    terminou = roundRobin(listaDeAltaPrioridade);

    if (terminou) {
        printf("Terminou com sucesso\n");
    } else {
        printf("Deu ruim\n");
    }

    // libera a memória alocada para cada processo
    for (int i = 0; i < qtdProcessos; i++) {
        free(listaDeAltaPrioridade[i]);
    }

    return 0;
}
/*
Atribui frações de tempo iguais para cada processo, manipula sem prioridade.
Entra um processo, 

Atributo Quantum. Processos armazenados em uma fila circular.


    Recebe um array de processos
    Tempo do processo ativo = TempoDeServiço - QUANTUM. 
    Se tempo de processo ativo = tempo de serviço. pop da lista de 
    As listas vão ficar dentro dele. Aí ele faz o controle das filas suspenso, bloqueado e pronto.
    

*/




/*
Recebo o processo.
Aumento o seu tempo de atividade. Quando o incremento desse tempo for igual a = 2. 
Preciso trocar para o proximo processo. 


*/