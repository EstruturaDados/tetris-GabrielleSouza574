#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // Capacidade da fila
#define MAX_PILHA 3  // Capacidade da pilha

// Estrutura que representa uma peça de Tetris
typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Estrutura da pilha linear
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ---------------- Funções da Fila ----------------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->frente = 0;
    f->tras = -1;
    f->quantidade = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->quantidade == MAX_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// Insere peça no final da fila (enqueue)
void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return; // nunca deve encher além do limite
    f->tras = (f->tras + 1) % MAX_FILA;
    f->itens[f->tras] = p;
    f->quantidade++;
}

// Remove peça da frente da fila (dequeue)
Peca dequeue(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->frente];
    f->frente = (f->frente + 1) % MAX_FILA;
    f->quantidade--;
    return removida;
}

// Exibe o estado atual da fila
void exibirFila(Fila *f) {
    printf("\nFila de peças:\n");
    if (filaVazia(f)) {
        printf("[ Vazia ]\n");
        return;
    }
    for (int i = 0; i < f->quantidade; i++) {
        int pos = (f->frente + i) % MAX_FILA;
        printf("[ %c %d ] ", f->itens[pos].nome, f->itens[pos].id);
    }
    printf("\n");
}

// ---------------- Funções da Pilha ----------------

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Empilha uma peça (push)
void push(Pilha *p, Peca x) {
    if (pilhaCheia(p)) {
        printf("A pilha está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->itens[++p->topo] = x;
}

// Desempilha uma peça (pop)
Peca pop(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) {
        printf("A pilha está vazia! Nenhuma peça reservada.\n");
        return removida;
    }
    return p->itens[p->topo--];
}

// Exibe o estado da pilha
void exibirPilha(Pilha *p) {
    printf("\nPilha de reserva:\n");
    if (pilhaVazia(p)) {
        printf("[ Vazia ]\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        printf("[ %c %d ]\n", p->itens[i].nome, p->itens[i].id);
    }
}

// ---------------- Geração de peças ----------------

// Função que gera automaticamente uma peça
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'I', 'O', 'T', 'L'};
    nova.nome = tipos[rand() % 4]; // Sorteia um tipo
    nova.id = id;                  // ID único
    return nova;
}

// ---------------- Programa Principal ----------------
int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorID = 0; // IDs únicos

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enqueue(&fila, gerarPeca(contadorID++));
    }

    do {
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = dequeue(&fila);
                if (jogada.id != -1) {
                    printf("Você jogou a peça [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca(contadorID++)); // mantém fila cheia
                }
                break;
            }
            case 2: {
                Peca reservada = dequeue(&fila);
                if (reservada.id != -1) {
                    push(&pilha, reservada);
                    printf("Peça [%c %d] reservada!\n", reservada.nome, reservada.id);
                    enqueue(&fila, gerarPeca(contadorID++)); // mantém fila cheia
                }
                break;
            }
            case 3: {
                Peca usada = pop(&pilha);
                if (usada.id != -1)
                    printf("Você usou a peça reservada [%c %d]\n", usada.nome, usada.id);
                break;
            }
            case 0:
                printf("Saindo do jogo...\n");
                break;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
