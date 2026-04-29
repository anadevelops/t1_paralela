#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_SIZE_FILA 10
#define NUM_CLIENTES 3
#define NUM_WORKERS 2

// Estrutura do pedido
typedef struct {
    int id_pedido;
    int id_cliente;
} Pedido;

// Estrutura da fila 
typedef struct {
    Pedido buffer[MAX_SIZE_FILA];
    int inicio;
    int fim;
    int quantidade;

    pthread_mutex_t mutex;
    pthread_cond_t cond_nao_vazia;
    pthread_cond_t cond_nao_cheia;
} Fila;

Fila fila;

void inicializar_fila() {
    fila.inicio = 0;
    fila.fim = 0;
    fila.quantidade = 0;
    pthread_mutex_init(&fila.mutex, NULL);
    pthread_cond_init(&fila.cond_nao_vazia, NULL);
    pthread_cond_init(&fila.cond_nao_cheia, NULL);
}

// SUSPENSÃO CONTROLADA: PRODUTOR
void enfileirar_pedido(Pedido p) {
    pthread_mutex_lock(&fila.mutex);

    // Espera quando a fila estiver cheia
    while (fila.quantidade == MAX_SIZE_FILA) {
        pthread_cond_wait(&fila.cond_nao_cheia, &fila.mutex);
    }

    // Adiciona pedido na fila
    fila.buffer[fila.fim] = p;
    fila.fim = (fila.fim + 1) % MAX_SIZE_FILA;
    fila.quantidade++;

    printf("[PRODUTOR] Cliente %d fez o pedido %d. (Fila: %d/%d)\n", p.id_cliente, p.id_pedido, fila.quantidade, MAX_SIZE_FILA);

    pthread_cond_signal(&fila.cond_nao_vazia);

    pthread_mutex_unlock(&fila.mutex);
}

// SUSPENSÃO CONTROLADA: CONSUMIDOR
Pedido desenfileirar_pedido(int id_worker) {
    pthread_mutex_lock(&fila.mutex);

    // Espera quando a fila estiver vazia
    while (fila.quantidade == 0) {
        pthread_cond_wait(&fila.cond_nao_vazia, &fila.mutex);
    }

    // Retira o pedido da fila
    Pedido p = fila.buffer[fila.inicio];
    fila.inicio = (fila.inicio + 1) % MAX_SIZE_FILA;
    fila.quantidade--;

    printf("[CONSUMIDOR] Worker %d pegou o pedido %d, (Fila: %d/%d)\n", id_worker, p.id_pedido, fila.quantidade, MAX_SIZE_FILA);

    pthread_cond_signal(&fila.cond_nao_cheia);

    pthread_mutex_unlock(&fila.mutex);

    return p;
}

// Threads
void* rotina_cliente(void* arg) {
    int id_cliente = *((int*)arg);
    for(int i = 1; i <= 3; i++) { //Cada cliente vai gerar 3 pedidos
        Pedido p = { .id_pedido = (id_cliente * 100) + i, .id_cliente = id_cliente};
        enfileirar_pedido(p);
        sleep(rand() % 3); //Tempo randômico entre pedidos
    }
    return NULL;
}

void* rotina_worker_vendas(void* arg) {
    int id_worker = *((int*)arg);
    while (true) {
        Pedido p = desenfileirar_pedido(id_worker);
        // Inserir etapas de validação:
        //1. Validação de Cadastro do cliente
        //2. Operação 'financeira'
        //3. Logística
        printf(" --> Processando etapas do pedido %d...\n", p.id_pedido);
        sleep(rand() % 4);
    }
    return NULL;
}

int main() {
    inicializar_fila();
    pthread_t clientes[NUM_CLIENTES];
    pthread_t workers[NUM_WORKERS];
    int ids_clientes[NUM_CLIENTES];
    int ids_workers[NUM_WORKERS];

    //Inicia workers
    for (int i = 0; i < NUM_WORKERS; i++) {
        ids_workers[i] = i + 1;
        pthread_create(&workers[i], NULL, rotina_worker_vendas, &ids_workers[i]);
    }

    //Inicia clientes
    for (int i = 0; i < NUM_CLIENTES; i++) {
        ids_clientes[i] = i + 1;
        pthread_create(&clientes[i], NULL, rotina_cliente, &ids_clientes[i]);
    }

    for (int i = 0; i < NUM_CLIENTES; i++) {
        pthread_join(clientes[i], NULL);
    }

    return 0;
}

