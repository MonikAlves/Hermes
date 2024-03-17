#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utilidades.h"
#include "../include/sessao.h"
#include "../include/produto.h"
#include "../include/pedido.h"

char * lerLinha(FILE * arquivo) {
    typedef struct _String {
        char caractere;
        struct _String * proximo;
    } String;

    if (arquivo == NULL) {
        return NULL;
    }

    String * base = (String*) malloc(sizeof(String));
    String * no = base;

    char caractere;
    int tamanho = 0;

    while ("sexo") {
        caractere = getc(arquivo);

        if (caractere == '\n' || caractere == EOF) {
            break;
        }

        no->proximo = (String*) malloc(sizeof(String));

        no->caractere = caractere;
        no = no->proximo;

        tamanho++;
    }

    char * texto = (char*) malloc((tamanho + 1) * sizeof(char));

    no = base;

    for (int i = 0; i < tamanho; i++) {
        String * outro = no;

        texto[i] = no->caractere;

        no = no->proximo;

        free(outro);
    }

    texto[tamanho] = '\0';

    return texto;
}

char * caminhoArquivo(char * nome) {
    char * caminho = (char*) malloc((13 + strlen(nome)) * sizeof(char));
    sprintf(caminho, "arquivos/%s", nome);
    return caminho;
}

FILE * abrirArquivo(char * nome, char * abertura) {
    char * caminho = caminhoArquivo(nome);
    FILE * arquivo = fopen(caminho, abertura);
    free(caminho);
    return arquivo;
}

void escreverPedidos(Pedidos * pedidos, char * nome){
    FILE * arquivo = abrirArquivo(nome, "w");

    if (arquivo == NULL) {
        return;
    }

    for (int i = 0; i < pedidos->tamanho; i++) {
        Pedido * pedido = indicePedido(pedidos, i);

        fprintf(arquivo, "%d\n", pedido->numero);
        fprintf(arquivo, "%s\n", pedido->nome);
        fprintf(arquivo, "%s\n", pedido->contato);
        fprintf(arquivo, "%s\n", pedido->endereco);
        fprintf(arquivo, "%.2f\n", pedido->total);
        fprintf(arquivo, "%s\n", pedido->pagamento);
        fprintf(arquivo, "%s\n", pedido->status);

        for (int j = 0; j < pedido->produtos->tamanho; j++) {
            Produto * produto = indiceProduto(pedido->produtos, j);

            fprintf(arquivo, "%s\n", produto->nome);
            fprintf(arquivo, "%.2f\n", produto->valor);
            fprintf(arquivo, "%d\n", produto->quantidade);
        }

        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
}

Pedidos * lerPedidos(char * nome){
    FILE * fptr = abrirArquivo(nome, "r");

    if (fptr == NULL) {
        return NULL;
    }

    char * linha = NULL;

    Pedidos * pedidos = criarPedidos();

    int numero = 0;

    while ("sexo") {
        Pedido * pedido = (Pedido*) malloc(sizeof(Pedido));
        
        linha = lerLinha(fptr);

        if (strlen(linha) == 0) {
            free(pedido);
            free(linha);
            break;
        }

        pedido->numero = atoi(linha);
        free(linha);
    
        pedido->nome = lerLinha(fptr);
        pedido->contato = lerLinha(fptr);
        pedido->endereco = lerLinha(fptr);

        linha = lerLinha(fptr);
        pedido->total = atof(linha);
        free(linha);

        pedido->pagamento = lerLinha(fptr);
        pedido->status = lerLinha(fptr);

        Produtos * produtos = criarProdutos();

        if (numero < pedido->numero) {
            numero = pedido->numero;
        }

        while ("qualquer coisa"){
            Produto * produto = (Produto*) malloc(sizeof(Produto));
            produto->nome = lerLinha(fptr);

            if (strlen(produto->nome) == 0) {
                free(produto->nome);
                free(produto);
                break;
            }

            linha = lerLinha(fptr);
            produto->valor = atof(linha);
            free(linha);

            linha = lerLinha(fptr);
            produto->quantidade = atoi(linha);
            free(linha);

            adicionarProduto(produtos, produto);
            deletarProduto(produto);
        }

        pedido->produtos = produtos;

        adicionarPedido(pedidos, pedido);
        deletarPedido(pedido);
    }

    atualizar_numero(numero + 1);

    fclose(fptr);

    return pedidos;
}

Arquivos * criarArquivos(void) {
    Arquivos * arquivos = (Arquivos*) malloc(sizeof(Arquivos));

    arquivos->tamanho = 0;
    arquivos->base = NULL;

    FILE * fptr = abrirArquivo(".nomes", "r");

    if (fptr == NULL) {
        return arquivos;
    }

    while(1) {
        char * nome = lerLinha(fptr);

        if (strlen(nome) == 0) {
            free(nome);

            break;
        }

        adicionarArquivos(arquivos, nome);
    }

    fclose(fptr);

    return arquivos;
}

NoArquivos * criarNoArquivos(char * nome){
    NoArquivos * no = (NoArquivos*) malloc(sizeof(NoArquivos));
    no->nome = nome;
    no->proximo = NULL;

    return no;
}

void deletarNoArquivos(NoArquivos * no) {
    if (no == NULL) return;

    char * caminho = caminhoArquivo(no->nome);

    remove(caminho);
    free(caminho);
    
    free(no->nome);
    free(no);
}

void deletarArquivos(Arquivos * arquivos) {
    if (arquivos == NULL) return;

    NoArquivos * no = arquivos->base;

    while (no != NULL) {
        NoArquivos * proximo = no->proximo;

        free(no->nome);
        free(no);

        no = proximo;
    }

    free(arquivos);
}

void escreverArquivos(Arquivos * arquivos){
    if (arquivos == NULL) return;

    FILE * fptr = abrirArquivo(".nomes","w");

    NoArquivos * no = arquivos->base;

    while (no != NULL) {
        fprintf(fptr, "%s\n", no->nome);
        no = no->proximo;
    }

    fclose(fptr);
}

void adicionarArquivos(Arquivos * arquivos, char * nome){
    if (arquivos == NULL) return;
    
    nome = copiarString(nome);

    if (arquivos->tamanho == 0){
        arquivos->base = criarNoArquivos(nome);
    } else {
        NoArquivos * no = arquivos->base;

        while (no->proximo != NULL){
            no = no->proximo;
        }

        no->proximo = criarNoArquivos(nome);
    }

    arquivos->tamanho++;
}

void removerArquivos(Arquivos * arquivos, int indice){
    if(arquivos == NULL) return;

    if (indice < 0 || indice >= arquivos->tamanho) {
        return;
    }
    
    if(indice == 0){
        NoArquivos * aux = arquivos->base;
        arquivos->base = aux->proximo;
        deletarNoArquivos(aux);
    }else{
        NoArquivos * no = arquivos->base;
        while(indice > 1){  
            no = no->proximo;
            indice--;
        }
        NoArquivos * proximo = no->proximo;
        no->proximo = proximo->proximo;
        deletarNoArquivos(proximo);
    }

    arquivos->tamanho--;
}

char * indiceArquivos(Arquivos * arquivos, int indice) {
    if (arquivos == NULL) {
        return NULL;
    }

    if (indice < 0 || indice >= arquivos->tamanho) {
        return NULL;
    }

    NoArquivos * no = arquivos->base;

    for (; indice > 0; indice--) {
        no = no->proximo;
    }

    return no->nome;
}

void salvarUltimo(int indice){
    FILE * fptr = abrirArquivo(".ultimo", "w");
    
    if (indice >= 0) {
        fprintf(fptr, "%d", indice);
    }

    fclose(fptr);
}

int lerUltimo(){
    FILE * fptr = abrirArquivo(".ultimo", "r");

    int indice = -1;

    fscanf(fptr, "%d", &indice);
    fclose(fptr);

    return indice;
}

Sessao * criarSessao(void) {
    Sessao * sessao = (Sessao*) malloc(sizeof(Sessao));

    sessao->arquivo = lerUltimo();
    sessao->arquivos = criarArquivos();

    return sessao;
}

void deletarSessao(Sessao * sessao) {
    if (sessao == NULL) {
        return;
    }

    salvarUltimo(sessao->arquivo);
    escreverArquivos(sessao->arquivos);
    deletarArquivos(sessao->arquivos);
    free(sessao);
}

bool novaSessao(Sessao * sessao, Pedidos ** pedidos, char * nome) {
    if (sessao == NULL) {
        return false;
    }

    if (*pedidos != NULL) {
        deletarPedidos(*pedidos);
    }

    *pedidos = criarPedidos();

    for(int arquivo = 0; arquivo < sessao->arquivos->tamanho; arquivo++) {
        if (strcmp(nome, indiceArquivos(sessao->arquivos, arquivo)) == 0) {
            return false;
        }
    }

    sessao->arquivo = sessao->arquivos->tamanho;
    adicionarArquivos(sessao->arquivos, copiarString(nome));

    atualizar_numero(1);

    return true;
}

bool ultimaSessao(Sessao * sessao, Pedidos ** pedidos) {
    if (sessao == NULL) {
        return false;
    }

    if (sessao->arquivo < 0 || sessao->arquivo >= sessao->arquivos->tamanho) {
        return false;
    }

    if (*pedidos != NULL) {
        deletarPedidos(*pedidos);
    }

    *pedidos = lerPedidos(indiceArquivos(sessao->arquivos, sessao->arquivo));

    return true;
}

bool carregarSessao(Sessao * sessao, Pedidos ** pedidos, int arquivo) {
    if (sessao == NULL) {
        return false;
    }

    if (arquivo < 0 || arquivo >= sessao->arquivos->tamanho) {
        return false;
    }

    if (*pedidos != NULL) {
        deletarPedidos(*pedidos);
    }

    sessao->arquivo = arquivo;
    *pedidos = lerPedidos(indiceArquivos(sessao->arquivos, sessao->arquivo));

    return true;
}

void removerSessao(Sessao * sessao, int arquivo) {
    if (sessao == NULL) {
        return;
    }

    if (arquivo < 0 || arquivo >= sessao->arquivos->tamanho) {
        return;
    }

    removerArquivos(sessao->arquivos, arquivo);

    if (sessao->arquivo == arquivo) {
        sessao->arquivo = -1;
    }
    if (sessao->arquivo > arquivo) {
        sessao->arquivo--;
    }
}

void salvarSessao(Sessao * sessao, Pedidos * pedidos) {
    if (sessao == NULL) {
        return;
    }

    if (sessao->arquivo < 0 || sessao->arquivo >= sessao->arquivos->tamanho) {
        return;
    }

    escreverPedidos(pedidos, indiceArquivos(sessao->arquivos, sessao->arquivo));
}

char * nomeSessao(Sessao * sessao, int arquivo) {
    if (sessao == NULL) {
        return NULL;
    }

    if (arquivo < 0 || arquivo >= sessao->arquivos->tamanho) {
        return NULL;
    }

    return indiceArquivos(sessao->arquivos, arquivo);
}