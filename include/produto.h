#ifndef _PRODUTO_H_
#define _PRODUTO_H_

// ----- Produto ----- //

typedef struct _Produto {
   char * nome;
   float  valor;
   int quantidade;
   float total;
} Produto;

Produto * criarProduto(char * nome, float valor, int quantidade);
Produto * copiarProduto(Produto * produto);
void deletarProduto(Produto * produto);
void imprimirProduto(Produto * produto);
float totalProduto(Produto * produto);

// ----- Lista de Produtos ----- //

typedef struct _NoProduto {
   Produto * produto;
   struct _NoProduto * proximo;
} NoProduto;

typedef struct _Produtos {
   int tamanho;
   NoProduto * base;
} Produtos;

Produtos * criarProdutos(void);
Produtos * copiarProdutos(Produtos * produtos);
void deletarProdutos(Produtos * produtos);
int adicionarProduto(Produtos * produtos, Produto * produto);
int removerProduto(Produtos * produtos, int indice);
void imprimirProdutos(Produtos * produtos);
float totalProdutos(Produtos * produtos);
Produto * indiceProduto(Produtos * produtos, int indice);

#endif