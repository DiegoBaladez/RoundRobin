#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int QUANTUM = 2;
int qtdProcessos = 3;

struct Processos {
    int tempoDeServico;
    int contadorDeTempoAtivo;
    int pid;
    int prioridade;
    int status; // 0 = pronto, 1 = suspenso, 2 = bloqueado
    int fazIO; //1 faz I/O e 0 nao faz I/O
    int tempoDeServicoDeIO;
    int contadorDeTempoAtivoEmIO;
    int tipoDeIo; // 0 - não tem. 1 Fita, 2 Impressora, 3 Disco
};

// Protótipo de função para evitar conflito de declaração
bool verificaSeEstaVazio(struct Processos *listaDeProcessos[], int tamanho);

bool verificaSeEstaVazio(struct Processos *listaDeProcessos[], int tamanho) {
    bool todasVazias = 1;

    for (int i = 0; i < tamanho; i++) {
        if (listaDeProcessos[i] != NULL) { //se for diferente de nulo, é pq tem algo lá.
            todasVazias = 0; //todas vazias é falso.
            break;
        }
    }

    if (todasVazias) {
        return true; //estao vazias
    } else {
        return false; //nao está vazio
    }
}

void executaTempoDeIo(struct Processos *processoEmIo)
{
    for (int j = 0; j < QUANTUM; j++)
     {
        processoEmIo->contadorDeTempoAtivoEmIO++; 
        if(processoEmIo->contadorDeTempoAtivoEmIO == processoEmIo->tempoDeServicoDeIO) break;               
    }
}

void inserirNaPrimeiraPosicaoVazia(struct Processos *listaDeProcessos[], int tamanho, int elemento) {
    bool posicaoVaziaEncontrada = false;

    for (int i = 0; i < tamanho; i++) {
        if (listaDeProcessos[i] == NULL) { 
            listaDeProcessos[i] = elemento;
            posicaoVaziaEncontrada = true;
            break;
        }
    }

    if (!posicaoVaziaEncontrada) {
        printf("Não foi possível inserir o elemento. O array está cheio.\n");
    }
}

void removerElemento(struct Processos *listaDeProcessos[], int tamanho, int posicao) {
    if (posicao >= tamanho || posicao < 0) {
        printf("Posição inválida!\n");
        return;
    }

    // Move os elementos para a esquerda a partir da posição especificada
    for (int i = posicao; i < tamanho - 1; i++) {
        listaDeProcessos[i] = listaDeProcessos[i + 1];
    }

    // Define como vazio o último elemento do array após a movimentação
    listaDeProcessos[tamanho - 1] = 0; // Aqui, 0 é considerado como "vazio"
}

struct Processos* criaProcesso() {
    struct Processos *novoProcesso = (struct Processos*)malloc(sizeof(struct Processos));

    if (novoProcesso == NULL) {
        printf("Erro: Não foi possível alocar memória para o processo.\n");
        exit(EXIT_FAILURE);
    }

    novoProcesso->tempoDeServico = rand() % (90 - 4 + 1) + 1;
    novoProcesso->contadorDeTempoAtivo = 0;
    novoProcesso->pid = rand() % (80 - 0 + 1) + 1;
    novoProcesso->prioridade = 0; 
    novoProcesso->status = 0; 
    novoProcesso->fazIO = rand() % 2;
    novoProcesso->contadorDeTempoAtivoEmIO = 0;
    novoProcesso->tipoDeIo = 0;

    if (novoProcesso->fazIO == 1) {
        novoProcesso->tempoDeServicoDeIO = rand() % (5 - 1 + 1) + 1;
        novoProcesso->tipoDeIo = rand() % (3 - 0) + 1;
    } else {
        novoProcesso->tempoDeServicoDeIO = 0;
        novoProcesso->tipoDeIo = 0;
    }

    return novoProcesso;
}

bool roundRobin(struct Processos *listaDeProcessos[]) {
    struct Processos *processoAtual;
    struct Processos *processoEmIo;
    int contaProcessosTerminados = 0;
    int contaProcessosEmIo = 0;
    int contaProcessosEmBaixaPrioridade = 0;
    struct Processos *listaDeIO[qtdProcessos];
    struct Processos *listaDeBaixaPrioridade[qtdProcessos];
    
    for (int i = 0; i < qtdProcessos; i++) {
        listaDeIO[i] = NULL;
    }

    int contaPrint = 0;

    while (contaProcessosTerminados != qtdProcessos && contaProcessosEmIo == 0 && contaProcessosEmBaixaPrioridade == 0) {
        for (int i = 0; i < qtdProcessos; i++) {
            
            if (verificaSeEstaVazio(listaDeIO, qtdProcessos)) {
                processoAtual = listaDeProcessos[i];
            } else {
                processoAtual = listaDeIO[0];
            }            
            
            if (processoAtual->tempoDeServicoDeIO != 0 && processoAtual->contadorDeTempoAtivoEmIO == 0)
            {
              processoAtual->status = 1;
              removerElemento(listaDeProcessos,qtdProcessos,i);
              contaProcessosEmIo++;
              inserirNaPrimeiraPosicaoVazia(listaDeIO,qtdProcessos,processoAtual);                    
            }

            if(!verificaSeEstaVazio(listaDeIO,qtdProcessos))
            {
                printf("executando IO do processo\n");
                processoEmIo = listaDeIO[0];

                executaTempoDeIo(processoEmIo);
                
                if(processoEmIo->contadorDeTempoAtivoEmIO == processoEmIo->tempoDeServicoDeIO)
                {
                    switch (processoEmIo->tipoDeIo)
                    {
                    case 1:
                        printf("processo de IO do Processo %d terminou.\n", processoEmIo->pid);
                        processoEmIo->fazIO = 0;
                        removerElemento(listaDeIO,qtdProcessos,0);
                        contaProcessosEmIo--;
                        inserirNaPrimeiraPosicaoVazia(listaDeProcessos,qtdProcessos,processoEmIo);                                                
                        break;
                    
                    case 2:
                        printf("processo de IO do Processo %d terminou.\n", processoEmIo->pid);
                        processoEmIo->fazIO = 0;
                        removerElemento(listaDeIO,qtdProcessos,0);
                        contaProcessosEmIo--;
                        inserirNaPrimeiraPosicaoVazia(listaDeProcessos,qtdProcessos,processoEmIo);                                                
                        break;

                    case 3: //esse aqui precisa ir para uma fila  de baixa prioridade que deve rodar depois de todos os da fila de alta... mudar esse codigo
                        printf("processo de IO do Processo %d terminou.\n", processoEmIo->pid);
                        processoEmIo->fazIO = 0;
                        removerElemento(listaDeIO,qtdProcessos,0);
                        contaProcessosEmIo--;
                        inserirNaPrimeiraPosicaoVazia(listaDeProcessos,qtdProcessos,processoEmIo);                                                
                        break;

                    default:
                        break;
                    }
                }
            }
            
            if (processoAtual->fazIO == 1) continue;

            if (processoAtual->contadorDeTempoAtivo >= processoAtual->tempoDeServico) continue;

            for (int j = 0; j < QUANTUM; j++) {
                processoAtual->contadorDeTempoAtivo++;

                if (processoAtual->contadorDeTempoAtivo == processoAtual->tempoDeServico) {
                    contaProcessosTerminados++;
                    printf("O processo com o PID %d, acabou com %d unidades de tempo:, sua posição na fila eh %d \n", processoAtual->pid, processoAtual->contadorDeTempoAtivo, i);
                    contaPrint++;
                    break;
                }
            }

            if (i == qtdProcessos - 1) i = -1;
        }
    }

    printf("Contagem dos prints %d\n", contaPrint);
    return true;
}

int main(void) {
    bool terminou;
    srand(time(NULL));
    struct Processos *listaDeAltaPrioridade[qtdProcessos];

    for (int i = 0; i < qtdProcessos; i++) {
        listaDeAltaPrioridade[i] = criaProcesso();
    }

    for (int i = 0; i < qtdProcessos; i++) {
        printf("Esse processo foi criado com o PID = %d, está na posição da fila %d e possui o tempo de serviço igual a: %d \n", listaDeAltaPrioridade[i]->pid, i, listaDeAltaPrioridade[i]->tempoDeServico);
    }

    printf("\n\n");
    terminou = roundRobin(listaDeAltaPrioridade);

    if (terminou) {
        printf("Terminou com sucesso\n");
    } else {
        printf("Deu ruim\n");
    }

    for (int i = 0; i < qtdProcessos; i++) {
        free(listaDeAltaPrioridade[i]);
    }

    return 0;
}