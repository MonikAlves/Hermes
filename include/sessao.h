#include <stdio.h>

#include "pedido.h"

#ifndef _SESSAO_H_
#define _SESSAO_H_

void escreverPedidos(Pedidos * pedidos, char * nome);
Pedidos * lerPedidos(char * nome);

typedef struct _NoArquivos {
    char * nome;
    struct _NoArquivos * proximo;
} NoArquivos;

typedef struct _Arquivos {
    int tamanho;
    NoArquivos * base;
} Arquivos;

Arquivos * criarArquivos(void);
void deletarArquivos(Arquivos * arquivos);
void removerArquivos(Arquivos * arquivos, int indice);
void adicionarArquivos(Arquivos * arquivos, char * nome);
void escreverArquivos(Arquivos * arquivos);
char * indiceArquivos(Arquivos * arquivos, int indice);

typedef struct {
    int arquivo;
    Arquivos * arquivos;
} Sessao;

Sessao * criarSessao(void);
void deletarSessao(Sessao * sessao);
bool novaSessao(Sessao * sessao, Pedidos ** pedidos, char * nome);
bool ultimaSessao(Sessao * sessao, Pedidos ** pedidos);
bool carregarSessao(Sessao * sessao, Pedidos ** pedidos, int arquivo);
void removerSessao(Sessao * sessao, int arquivo);
void salvarSessao(Sessao * sessao, Pedidos * pedidos);
char * nomeSessao(Sessao * sessao, int arquivo);

#endif