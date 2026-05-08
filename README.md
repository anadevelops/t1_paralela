# Trabalho 1: Simulador de Vendas

Este projeto é um simulador de fluxo de vendas desenvolvido em **C** utilizando a biblioteca **Pthreads**. O objetivo é demonstrar a aplicação de concorrência e sincronização de threads através do processamento paralelo de pedidos, validações (cadastro, pagamento, validade) e logística.

A arquitetura foi desenhada aplicando três padrões de projeto voltados para programação multithread:
1. **Produtor/Consumidor:** Desacoplamento da geração de pedidos (clientes) do processamento (workers) através de um buffer circular.
2. **Suspensão Controlada:** Utilização de variáveis de condição (`pthread_cond_t`) para adormecer threads quando a fila está cheia/vazia ou para criar barreiras de sincronização durante as validações paralelas (pagamento e validade).
3. **Pool de Threads:** Reutilização de um número fixo de workers criados na inicialização do sistema, evitando overhead de criação de novas threads para cada pedido e protegendo o sistema contra sobrecarga.

## Ambiente

O sistema foi desenvolvido e testado em ambiente **Linux (Ubuntu)**

## Instruções para compilação
Utilizar o comando:
```gcc -pthread -o main main.c```

## Autores
- Ana Clara Stupp de Souza
- Gabriel Pessi Buchweitz
- Reni Rogete Rosa Ferreira Junior