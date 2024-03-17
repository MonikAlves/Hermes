#include <stdbool.h>

#include "produto.h"

#ifndef _PEDIDO_H_
#define _PEDIDO_H_

// ----- Pedido ----- //

void atualizar_numero(int numero);

typedef struct _Pedido {
   char * nome;
   int  numero;
   char * endereco;
   char * contato;
   float total;
   char * pagamento;
   char * status;
   Produtos * produtos;
} Pedido;

Pedido * criarPedido(
   char * nome,
   char * endereco,
   char * contato,
   char * pagamento,
   char * status,
   Produtos * produtos   
);
Pedido * copiarPedido(Pedido * pedido);
void deletarPedido(Pedido * pedido);
void imprimirPedido(Pedido * pedido);
float totalPedido(Pedido * pedido);

int numeroStatus(char * status);
int numeroPagamento(char * pagamento);
char * textoStatus(int status);
char * textoPagamento(int pagamento);

// ----- Lista de Pedidos ----- //

typedef struct _NoPedido {
   Pedido * pedido;
   struct _NoPedido * proximo;
} NoPedido;

typedef struct _Pedidos{
   int tamanho;
   NoPedido * base;
}Pedidos;

Pedidos * criarPedidos(void);
Pedidos * copiarPedidos(Pedidos * lista);
void deletarPedidos(Pedidos * pedido);
int adicionarPedido(Pedidos * lista, Pedido * pedido);
int removerPedido(Pedidos * lista, int indice);
void imprimirPedidos(Pedidos * lista);
Pedido * indicePedido(Pedidos * lista, int indice);
void Ordenar(NoPedido * no, NoPedido * inserir);
bool compararPedido(Pedido * pedido_a, Pedido * pedido_b);
void ordenarPedidos(Pedidos * lista);

#endif