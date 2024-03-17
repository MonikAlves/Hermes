#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "../include/utilidades.h"
#include "../include/produto.h"
#include "../include/pedido.h"

// ----- Pedido ----- //

int numero_pedido = 0;

void atualizar_numero(int numero) {
    if (numero > 0) {
        numero_pedido = numero;
    }
}

Pedido * criarPedido(
   char * nome,
   char * endereco,
   char * contato,
   char * pagamento,
   char * status,
   Produtos * produtos
) {
    Pedido * pedido = (Pedido*) malloc(sizeof(Pedido));

    pedido->numero = numero_pedido++;
    pedido->nome = copiarString(nome);
    pedido->endereco = copiarString(endereco);
    pedido->contato = copiarString(contato);
    pedido->pagamento = copiarString(pagamento);
    pedido->status = copiarString(status);
    pedido->produtos = copiarProdutos(produtos);
    pedido->total = totalPedido(pedido);
    
    return pedido;
}

Pedido * copiarPedido(Pedido * pedido) {
    if (pedido == NULL) return NULL;

    Pedido * copia = (Pedido*) malloc(sizeof(Pedido));

    copia->numero = pedido->numero;
    copia->nome = copiarString(pedido->nome);
    copia->endereco = copiarString(pedido->endereco);
    copia->contato = copiarString(pedido->contato);
    copia->pagamento = copiarString(pedido->pagamento);
    copia->status = copiarString(pedido->status);
    copia->produtos = copiarProdutos(pedido->produtos);
    copia->total = totalPedido(copia);
    
    return copia;
}

void deletarPedido(Pedido * pedido) {
    if (pedido == NULL) return;
    
    deletarProdutos(pedido->produtos);

    free(pedido->nome);
    free(pedido->endereco);
    free(pedido->contato);
    free(pedido->pagamento);
    free(pedido->status);
    
    free(pedido);
}

void imprimirPedido(Pedido * pedido) {
    if (pedido == NULL) return;

    printf("Pedido: {Numero: %d, Nome: %s, Contato: %s, Endereco: %s, Total: %.2f, Pagamento: %s, Status: %s}\n",
        pedido->numero,
        pedido->nome,
        pedido->contato,
        pedido->endereco,
        pedido->total,
        pedido->pagamento,
        pedido->status
    );

    // imprimirProdutos(pedido->produtos);
}

float totalPedido(Pedido * pedido) {
    if (pedido == NULL) return 0;
    
    pedido->total = totalProdutos(pedido->produtos);

    return pedido->total;
}

int numeroStatus(char * status) {
    if (strcmp(status, "Pendente") == 0) return 0;
    if (strcmp(status, "Pago") == 0) return 1;
    if (strcmp(status, "Preparado") == 0) return 2;
    if (strcmp(status, "Entregue") == 0) return 3;

    return 0;
}

int numeroPagamento(char * pagamento) {
    if (strcmp(pagamento, "Dinheiro") == 0) return 0;
    if (strcmp(pagamento, "Debito") == 0) return 1;
    if (strcmp(pagamento, "Credito") == 0) return 2;
    if (strcmp(pagamento, "Pix") == 0) return 3;

    return 0;
}

char * textoStatus(int status) {
    switch (status) {
        case 0: return copiarString("Pendente");
        case 1: return copiarString("Pago");
        case 2: return copiarString("Preparado");
        case 3: return copiarString("Entregue");
    }

    return copiarString("Pendente");
}

char * textoPagamento(int pagamento) {
    switch (pagamento) {
        case 0: return copiarString("Dinheiro");
        case 1: return copiarString("Debito");
        case 2: return copiarString("Credito");
        case 3: return copiarString("Pix");
    }

    return copiarString("Dinheiro");
}

// ----- Lista de Pedidos ----- //

NoPedido * criarNoPedido(Pedido* pedido) {
    if(pedido == NULL) return NULL;

    NoPedido * no = (NoPedido*) malloc(sizeof(NoPedido));

    no->pedido = pedido;
    no->proximo = NULL;

    return no;
}

NoPedido * copiarNoPedido(NoPedido * no){
    if (no == NULL) return NULL;

    NoPedido * copia = (NoPedido*) malloc(sizeof(NoPedido));

    copia->pedido = copiarPedido(no->pedido);
    copia->proximo = NULL;

    return copia;
}

void deletarNoPedido(NoPedido * no){
    if (no == NULL) return;
    deletarPedido(no->pedido);
    free(no);
}

Pedidos * criarPedidos(void){
    Pedidos * lista = (Pedidos*) malloc(sizeof(Pedidos));
    lista->base = NULL;
    lista->tamanho = 0;

    return lista;
}

Pedidos * copiarPedidos(Pedidos * pedidos){
    if (pedidos == NULL) return NULL;

    Pedidos * copia = (Pedidos*) malloc(sizeof(Pedidos));
    copia->base = NULL;
    copia->tamanho = pedidos->tamanho;

    if (pedidos->tamanho != 0) {
        copia->base = copiarNoPedido(pedidos->base);

        NoPedido * no_pedido = pedidos->base;
        NoPedido * no_copia = copia->base;

        while (no_pedido->proximo != NULL) {
            no_copia->proximo = copiarNoPedido(no_pedido->proximo);

            no_pedido = no_pedido->proximo;
            no_copia = no_copia->proximo;
        }
    }

    return copia;
}

int ordemStatus(char * status) {
    if (strcmp(status, "Preparado") == 0) return 0;
    if (strcmp(status, "Pago") == 0) return 1;
    if (strcmp(status, "Pendente") == 0) return 2;
    if (strcmp(status, "Entregue") == 0) return 3;

    return 0;
}

bool compararPedido(Pedido * pedido_a, Pedido * pedido_b){
    if (strcmp(pedido_a->status, pedido_b->status) == 0) {
        if (strcmp(pedido_a->status, "Entregue") == 0) {
            return true;
        }
        
        return pedido_a->numero < pedido_b->numero;

    } else {
        return ordemStatus(pedido_a->status) < ordemStatus(pedido_b->status);
    }
}

int adicionarPedido(Pedidos * lista, Pedido * pedido){
    if (lista == NULL) return -1;

    pedido = copiarPedido(pedido);

    int indice = 0;

    if(lista->tamanho == 0){
        lista->base = criarNoPedido(pedido);
        lista->tamanho = 1;
    }else{
        if (compararPedido(pedido, lista->base->pedido)) {
            NoPedido * no = criarNoPedido(pedido);

            no->proximo = lista->base;
            lista->base = no;
        }
        else {
            NoPedido * no = lista->base;

            for (indice = 1; no->proximo != NULL; no = no->proximo, indice++) {
                if (compararPedido(pedido, no->proximo->pedido)) {
                    break;
                }
            }

            NoPedido * novo_no = criarNoPedido(pedido);

            novo_no->proximo = no->proximo;
            no->proximo = novo_no;
        }
        
        lista->tamanho++;
    }

    return indice;
}

int removerPedido(Pedidos * lista,int indice){
    if(lista == NULL) return 0;

    if(indice < 0 || indice >= lista->tamanho) return 0;

    if(indice == 0){
        NoPedido* aux = lista->base;
        lista->base = aux->proximo;
        deletarNoPedido(aux);
    }else{
        NoPedido * anterior = lista->base;
        while(indice > 1){
            anterior = anterior->proximo;
            indice--;
        }
        NoPedido * aux = anterior->proximo;
        anterior->proximo = aux->proximo;
        deletarNoPedido(aux);
    }

    lista->tamanho--;

    return 1;
}

void _deletarPedidos(NoPedido * no){
    if(no == NULL) return;
    _deletarPedidos(no->proximo);
    deletarNoPedido(no);
}

void deletarPedidos(Pedidos * pedidos){
    if(pedidos == NULL) return;
    _deletarPedidos(pedidos->base);
    free(pedidos);
}

void imprimirPedidos(Pedidos * lista){
    if (lista == NULL) return;

    NoPedido * no = lista->base;

    for (; no != NULL; no = no->proximo) {
        imprimirPedido(no->pedido);
    }

    printf("\n");
}

Pedido * indicePedido(Pedidos * lista, int indice){
    if(lista == NULL) return NULL;

    if(indice == 0){
        return lista->base->pedido;
    } else if(indice < lista->tamanho){
        NoPedido * aux = lista->base;
        for(;indice>0;indice--){
            aux = aux->proximo;
        }
        return aux->pedido;
    }
    return NULL;
}

void ordenarPedidos(Pedidos * lista) {
    return;
}

// comecaria com Ordenar(aux,copia);

/* Colocar ordem dos status
preparado
pago
pendente
entregue
*/


void Ordenar(NoPedido * no, NoPedido * inserir){
    if(no->proximo == NULL){
        no->proximo = inserir;
        return;
    }

    if(strcmp(no->pedido->status,inserir->pedido->status) == 0 && strcmp(inserir->pedido->status,"entregue") != 0){

        if(no->pedido->numero < inserir->pedido->numero && no->proximo->pedido->numero > inserir->pedido->numero
            && strcmp(inserir->pedido->status,no->proximo->pedido->status) == 0){

                inserir->proximo = no->proximo;
                no->proximo = inserir;
                return;
        }else if(strcmp(no->pedido->status,inserir->pedido->status) == 0 && strcmp(inserir->pedido->status,no->proximo->pedido->status) != 0){
                inserir->proximo = no->proximo;
                no->proximo = inserir;
                return;
        }

        Ordenar(no->proximo,inserir);
        return;

    }else if(strcmp(no->pedido->status,"entregue") != 0 && strcmp(no->proximo->pedido->status,"entregue") == 0){
                inserir->proximo = no->proximo;
                no->proximo = inserir;
                return;
    }

    Ordenar(no->proximo,inserir);
}