#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/produto.h"
#include "../include/utilidades.h"

// ----- Produto ----- //

Produto * criarProduto(char * nome, float  valor, int quantidade){
   Produto * produto = (Produto*) malloc(sizeof(Produto));

   produto->nome = copiarString(nome);
   produto->quantidade = quantidade;
   produto->valor = valor;
   produto->total = valor * quantidade;

   return produto;
}

Produto * copiarProduto(Produto * produto) {
    if (produto == NULL) return NULL;

    Produto * copia = (Produto*) malloc(sizeof(Produto));

    copia->nome = copiarString(produto->nome);
    copia->quantidade = produto->quantidade;
    copia->valor = produto->valor;
    copia->total = produto->valor * produto->quantidade;

    return copia;
}

void deletarProduto(Produto * produto) {
    if (produto == NULL) return;

    free(produto->nome);
    free(produto);
}

void imprimirProduto(Produto * produto) {
    if (produto == NULL) return;

    printf("Produto: {Nome: %s, Preço: R$%.2f, Quantidade: %d}\n",
        produto->nome,
        produto->valor,
        produto->quantidade
    );
}

float totalProduto(Produto * produto) {
    if (produto == NULL) return 0;

    produto->total = produto->valor * produto->quantidade;

    return produto->total;
}

// ----- Lista de Produtos ----- //

NoProduto * criarNoProduto( Produto * produto){
   if (produto == NULL) return NULL;

   NoProduto * p = (NoProduto*) malloc(sizeof(NoProduto));
   p->produto = produto;
   p->proximo = NULL;

    return p;
}

NoProduto * copiarNoProduto(NoProduto * no){
    if (no == NULL) return NULL;

    NoProduto * copia = (NoProduto*) malloc(sizeof(NoProduto));

    copia->produto = copiarProduto(no->produto);
    copia->proximo = NULL;

    return copia;
}

void deletarNoProduto(NoProduto * no) {
    if (no == NULL) return;
    deletarProduto(no->produto);
    free(no);
}

Produtos * criarProdutos(void){
   Produtos * produtos = (Produtos*) malloc(sizeof(Produtos));
   produtos->base = NULL;
   produtos->tamanho = 0;

   return produtos;
}

Produtos * copiarProdutos(Produtos * produtos){
    if (produtos == NULL) return NULL;

    Produtos * copia = (Produtos*) malloc(sizeof(Produtos));
    copia->base = NULL;
    copia->tamanho = produtos->tamanho;

    if (produtos->tamanho != 0) {
        copia->base = copiarNoProduto(produtos->base);

        NoProduto * no_produto = produtos->base;
        NoProduto * no_copia = copia->base;

        while (no_produto->proximo != NULL) {
            no_copia->proximo = copiarNoProduto(no_produto->proximo);

            no_produto = no_produto->proximo;
            no_copia = no_copia->proximo;
        }
    }

    return copia;
}

int adicionarProduto(Produtos * produtos, Produto * produto){
   if(produtos == NULL) return 0;

   Produto * copia = copiarProduto(produto);

   if(produtos->tamanho == 0){
      produtos->base = criarNoProduto(copia);
      produtos->tamanho = 1;
   }else{
    NoProduto * aux = produtos->base;
      while(aux->proximo != NULL){
          aux= aux->proximo;
      }
      aux->proximo = criarNoProduto(copia);
      produtos->tamanho++;
   }
   return 1;
}

int removerProduto(Produtos * produtos, int indice) {
    if (produtos == NULL) {
        return 0;
    }
    if (indice < 0 || indice >= produtos->tamanho) {
        return 0;
    }

    if (indice == 0) {
        NoProduto * no = produtos->base;
        produtos->base = produtos->base->proximo;

        deletarNoProduto(no);
    }
    else {
        NoProduto * no = produtos->base;
        
        for (; indice > 1; indice--) {
            no = no->proximo;
        }

        NoProduto * proximo = no->proximo;
        no->proximo = proximo->proximo;

        deletarNoProduto(proximo);
    }

    produtos->tamanho--;

    return 1;
}

void _deletarProdutos(NoProduto * no) {
   if(no == NULL) return;
   
    _deletarProdutos(no->proximo);

   deletarNoProduto(no);
}

void deletarProdutos(Produtos * produtos) {
   if(produtos == NULL) return;
   _deletarProdutos(produtos->base);
   free(produtos);
}

void imprimirProdutos(Produtos * produtos) {
    if (produtos == NULL) return;

    NoProduto * no = produtos->base;

    for (; no != NULL; no = no->proximo) {
        imprimirProduto(no->produto);
    }

    printf("\n");
}

float totalProdutos(Produtos * produtos) {
    if (produtos == NULL) return 0;

    float total = 0;

    NoProduto * no = produtos->base;

    for (; no != NULL; no = no->proximo) {
        total += totalProduto(no->produto);
    }

    return total;
}

Produto * indiceProduto(Produtos * produtos, int indice) {
    if (produtos == NULL) {
        return NULL;
    }

    if (indice < 0 || indice >= produtos->tamanho) {
        return NULL;
    }

    NoProduto * no = produtos->base;

    for (; indice > 0; indice--) {
        no = no->proximo;
    }

    return no->produto;
}