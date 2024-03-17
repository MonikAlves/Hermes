#include <gtk/gtk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/utilidades.h"
#include "../include/produto.h"
#include "../include/pedido.h"
#include "../include/sessao.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------------- Variáveis do Programa ----------------------------------- //
//////////////////////////////////////////////////////////////////////////////////////////////////

Pedidos * pedidos = NULL;
Sessao * sessao = NULL;

int indice_pedido = 0;

GError* error = NULL;
GtkBuilder* builder = NULL;

GtkWindow* janela_principal = NULL;
GtkWindow* janela_carregar_sessao = NULL;
GtkWindow* janela_nova_sessao = NULL;

GtkStack* pilha_janelas = NULL;

GtkListBox* lista_arquivos = NULL;
GtkListBox* lista_pedidos = NULL;
GtkListBox* lista_produtos = NULL;

GtkHeaderBar* barra_janela_inicio = NULL;
GtkHeaderBar* barra_janela_pedido = NULL;
GtkHeaderBar* barra_janela_pedidos = NULL;
GtkHeaderBar* barra_janela_produtos = NULL;

GtkEntry* entrada_nova_sessao = NULL;
GtkEntry* entrada_nome_pedido = NULL;
GtkEntry* entrada_contato_pedido = NULL;
GtkEntry* entrada_endereco_pedido = NULL;

GtkComboBox* selecao_estado_pedido = NULL;
GtkComboBox* selecao_pagamento_pedido = NULL;

GtkLabel* rotulo_preco_pedido = NULL;

GtkButton* botao_concluir_sessao = NULL;
GtkButton* botao_todos_pedidos = NULL;
GtkButton* botao_fechar_lista_produto = NULL;
GtkButton* botao_adicionar_produto = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------------- Funções do Programa ----------------------------------- //
////////////////////////////////////////////////////////////////////////////////////////////////

char * obter_texto_entrada(GtkEntry*);
void alterar_texto_entrada(GtkEntry*, const char * );

void botao_selecionar_arquivo_clidado(GtkWidget*, GtkWidget*);
void botao_excluir_arquivo_clidado(GtkWidget*, GtkWidget*);
GtkWidget* criar_linha_lista_arquivo(int);
void inicializar_lista_arquivos(void);
void limpar_lista_arquivos(void);

void botao_selecionar_pedido_clidado(GtkWidget*, GtkWidget*);
void botao_excluir_pedido_clidado(GtkWidget*, GtkWidget*);
GtkWidget* criar_linha_lista_pedido(int);
void limpar_lista_pedidos(void);
void inicializar_lista_pedidos(void);

/* @brief Obtem o texto de uma entrada e retorna ele
 *
 * @param entrada Entrada de onde o texto será obtido
 *
 * @return Texto obtido da entrada
 */
char * obter_texto_entrada(GtkEntry* entrada) {
    char * texto = copiarString(gtk_entry_get_text(entrada));

    return texto;
}

/* @brief Altera o texto de uma entrada
 *
 * @param entrada Entrada onde o texto será alterado
 * @param texto Texto a ser colocado na entrada
 */
void alterar_texto_entrada(GtkEntry* entrada, const char * texto) {
    gtk_entry_set_text(entrada, texto);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------------- Interface da Lista de Arquivos ----------------------------------- //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

/* @brief Seleciona o arquivo e o carrega
 *
 * @param botao Botao do arquivo selecionado
 * @param linha Linha do arquivo selecionado
 */
void botao_selecionar_arquivo_clidado(GtkWidget* botao, GtkWidget* linha) {
    int arquivo = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    if (arquivo == -1) {
        return;
    }

    // sessao.carregar(pedidos, arquivo);
    carregarSessao(sessao, &pedidos, arquivo);
    inicializar_lista_pedidos();
    gtk_widget_hide(GTK_WIDGET(janela_carregar_sessao));
    gtk_stack_set_visible_child_name(pilha_janelas, "janela_pedidos");
    g_signal_emit_by_name(botao_todos_pedidos, "clicked");
}

/* @brief Seleciona o arquivo e o exclui
 *
 * @param botao Botao do arquivo selecionado
 * @param linha Linha do arquivo selecionado
 */
void botao_excluir_arquivo_clidado(GtkWidget* botao, GtkWidget* linha) {
    int arquivo =  gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    if (arquivo == -1) {
        return;
    }

    gtk_container_remove(GTK_CONTAINER(lista_arquivos), GTK_WIDGET(linha));
    // sessao.remover(arquivo);
    removerSessao(sessao, arquivo);
}

/* @brief Cria uma nova linha para a lista de arquivos
 *
 * @param arquivo Número do arquivo que será adicionado à lista
 *
 * @return Linha da lista de arquivos
 */
GtkWidget* criar_linha_lista_arquivo(int arquivo) {
    static int borda = 5;

    if (arquivo < 0 || arquivo >= sessao->arquivos->tamanho) {
        return NULL;
    }

    char * nome = nomeSessao(sessao, arquivo);

    // botão de selecionar arquivo
    GtkWidget* botao_selecionar_arquivo = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(botao_selecionar_arquivo), nome);
    gtk_widget_set_visible(GTK_WIDGET(botao_selecionar_arquivo), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(botao_selecionar_arquivo), TRUE);
    gtk_widget_set_receives_default(GTK_WIDGET(botao_selecionar_arquivo), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(botao_selecionar_arquivo), borda);

    // botão de excluir arquivo
    GtkWidget* botao_excluir_arquivo = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(botao_excluir_arquivo), "x");
    gtk_widget_set_visible(GTK_WIDGET(botao_excluir_arquivo), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(botao_excluir_arquivo), TRUE);
    gtk_widget_set_receives_default(GTK_WIDGET(botao_excluir_arquivo), TRUE);
    gtk_container_set_border_width(GTK_CONTAINER(botao_excluir_arquivo), borda);

    // grade dos botões
    GtkWidget* grade = gtk_grid_new();

    gtk_widget_set_visible(grade, TRUE);
    gtk_widget_set_can_focus(grade, FALSE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grade), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grade), TRUE);

    gtk_grid_attach(GTK_GRID(grade), botao_selecionar_arquivo, 0, 0, 4, 1);
    gtk_grid_attach(GTK_GRID(grade), botao_excluir_arquivo, 4, 0, 1, 1);

    // linha da lista
    GtkWidget* linha = gtk_list_box_row_new();

    gtk_widget_set_visible(linha, TRUE);
    gtk_widget_set_can_focus(linha, TRUE);
    gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(linha), FALSE);
    gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(linha), FALSE);

    gtk_container_add(GTK_CONTAINER(linha), grade);

    g_signal_connect(botao_selecionar_arquivo, "clicked", G_CALLBACK(botao_selecionar_arquivo_clidado), linha);
    g_signal_connect(botao_excluir_arquivo, "clicked", G_CALLBACK(botao_excluir_arquivo_clidado), linha);

    return linha;
}

/* @brief Inicializa a lista de arquivos com os arquivos carregados
 */
void inicializar_lista_arquivos(void) {
    limpar_lista_arquivos();

    for(int arquivo = 0; arquivo < sessao->arquivos->tamanho; arquivo++) {
        GtkWidget* linha = criar_linha_lista_arquivo(arquivo);

        gtk_list_box_insert(lista_arquivos, linha, arquivo);
    }
}

/* @brief Limpa a lista de arquivos
 */
void limpar_lista_arquivos(void) {
    while (true) {
        GtkListBoxRow* linha = gtk_list_box_get_row_at_index(lista_arquivos, 0);

        if (linha == NULL) {
            break;
        }

        gtk_container_remove(GTK_CONTAINER(lista_arquivos), GTK_WIDGET(linha));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------------- Interface da Lista de Pedidos ----------------------------------- //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/* @brief Define os valores da janela de pedido
 */
void definir_valores_janela_pedido(void) {
    Pedido * pedido = indicePedido(pedidos, indice_pedido);

    gtk_entry_set_text(entrada_nome_pedido, pedido->nome); //pedidos[pedido].nome().c_str());
    gtk_entry_set_text(entrada_contato_pedido, pedido->contato);
    gtk_entry_set_text(entrada_endereco_pedido, pedido->endereco);

    char preco[64];

    sprintf(preco, "Preço R$%.2f", totalPedido(pedido));

    gtk_label_set_label(rotulo_preco_pedido, preco);

    gtk_combo_box_set_active(selecao_estado_pedido, numeroStatus(pedido->status));
    gtk_combo_box_set_active(selecao_pagamento_pedido, numeroPagamento(pedido->pagamento));
}

/* @brief Seleciona o arquivo e o carrega
 *
 * @param botao Botao do pedido selecionado
 * @param linha Linha do pedido selecionado
 */
void botao_selecionar_pedido_clidado(GtkWidget* botao, GtkWidget* linha) {
    indice_pedido = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    if (indice_pedido == -1) {
        return;
    }

    gtk_stack_set_visible_child_name(pilha_janelas, "janela_pedido");
    definir_valores_janela_pedido();
}

/* @brief Seleciona o pedido e o exclui
 *
 * @param botao Botao do pedido selecionado
 * @param linha Linha do pedido selecionado
 */
void botao_excluir_pedido_clidado(GtkWidget* botao, GtkWidget* linha) {
    int pedido = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    if (pedido == -1) {
        return;
    }

    gtk_container_remove(GTK_CONTAINER(lista_pedidos), GTK_WIDGET(linha));
    removerPedido(pedidos, pedido);
}

/* @brief Cria uma nova linha para a lista de pedidos
 *
 * @param pedido Número do pedido que será adicionado à lista
 *
 * @return Linha da lista de pedidos
 */
GtkWidget* criar_linha_lista_pedido(int indice) {
    if (indice >= pedidos->tamanho) {
        return NULL;
    }

    Pedido * pedido = indicePedido(pedidos, indice);

    char * nome = pedido->nome;
    char * status = pedido->status;

    char numero[16];
    char preco[64];

    sprintf(numero, "#%d", pedido->numero);
    sprintf(preco, "R$%.2f", totalPedido(pedido));

    // botão de selecionar pedido
    GtkWidget* botao_selecionar_pedido = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(botao_selecionar_pedido), ">");
    gtk_widget_set_visible(GTK_WIDGET(botao_selecionar_pedido), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(botao_selecionar_pedido), TRUE);
    gtk_widget_set_receives_default(GTK_WIDGET(botao_selecionar_pedido), TRUE);
    gtk_widget_set_halign(GTK_WIDGET(botao_selecionar_pedido), GTK_ALIGN_START);
    gtk_widget_set_margin_start(GTK_WIDGET(botao_selecionar_pedido), 8);
    gtk_widget_set_margin_top(GTK_WIDGET(botao_selecionar_pedido), 8);
    gtk_widget_set_margin_bottom(GTK_WIDGET(botao_selecionar_pedido), 8);


    // rótulo para o número do pedido
    GtkWidget* rotulo_numero_pedido = gtk_label_new(numero);

    gtk_widget_set_visible(GTK_WIDGET(rotulo_numero_pedido), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(rotulo_numero_pedido), TRUE);
    gtk_widget_set_margin_start(GTK_WIDGET(rotulo_numero_pedido), 8);
    gtk_widget_set_margin_end(GTK_WIDGET(rotulo_numero_pedido), 8);
    gtk_widget_set_margin_top(GTK_WIDGET(rotulo_numero_pedido), 8);
    gtk_widget_set_margin_bottom(GTK_WIDGET(rotulo_numero_pedido), 8);
    gtk_label_set_justify(GTK_LABEL(rotulo_numero_pedido), GTK_JUSTIFY_CENTER);
    gtk_label_set_ellipsize(GTK_LABEL(rotulo_numero_pedido), PANGO_ELLIPSIZE_END);
    gtk_label_set_max_width_chars(GTK_LABEL(rotulo_numero_pedido), 1);

    // rótulo para o nome do pedido
    GtkWidget* rotulo_nome_pedido = gtk_label_new(nome);

    gtk_widget_set_visible(GTK_WIDGET(rotulo_nome_pedido), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(rotulo_nome_pedido), TRUE);
    gtk_widget_set_margin_start(GTK_WIDGET(rotulo_nome_pedido), 8);
    gtk_widget_set_margin_end(GTK_WIDGET(rotulo_nome_pedido), 8);
    gtk_widget_set_margin_top(GTK_WIDGET(rotulo_nome_pedido), 8);
    gtk_widget_set_margin_bottom(GTK_WIDGET(rotulo_nome_pedido), 8);
    gtk_label_set_justify(GTK_LABEL(rotulo_nome_pedido), GTK_JUSTIFY_CENTER);
    gtk_label_set_ellipsize(GTK_LABEL(rotulo_nome_pedido), PANGO_ELLIPSIZE_END);
    gtk_label_set_max_width_chars(GTK_LABEL(rotulo_nome_pedido), 1);

    // rótulo para o nome do pedido
    GtkWidget* rotulo_estado_pedido = gtk_label_new(status);

    gtk_widget_set_visible(GTK_WIDGET(rotulo_estado_pedido), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(rotulo_estado_pedido), TRUE);
    gtk_widget_set_margin_start(GTK_WIDGET(rotulo_estado_pedido), 8);
    gtk_widget_set_margin_end(GTK_WIDGET(rotulo_estado_pedido), 8);
    gtk_widget_set_margin_top(GTK_WIDGET(rotulo_estado_pedido), 8);
    gtk_widget_set_margin_bottom(GTK_WIDGET(rotulo_estado_pedido), 8);
    gtk_label_set_justify(GTK_LABEL(rotulo_estado_pedido), GTK_JUSTIFY_CENTER);
    gtk_label_set_ellipsize(GTK_LABEL(rotulo_estado_pedido), PANGO_ELLIPSIZE_END);
    gtk_label_set_max_width_chars(GTK_LABEL(rotulo_estado_pedido), 1);

    // rótulo para o preco do pedido
    GtkWidget* rotulo_preco_pedido = gtk_label_new(preco);

    gtk_widget_set_visible(GTK_WIDGET(rotulo_preco_pedido), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(rotulo_preco_pedido), TRUE);
    gtk_widget_set_margin_start(GTK_WIDGET(rotulo_preco_pedido), 8);
    gtk_widget_set_margin_end(GTK_WIDGET(rotulo_preco_pedido), 8);
    gtk_widget_set_margin_top(GTK_WIDGET(rotulo_preco_pedido), 8);
    gtk_widget_set_margin_bottom(GTK_WIDGET(rotulo_preco_pedido), 8);
    gtk_label_set_justify(GTK_LABEL(rotulo_preco_pedido), GTK_JUSTIFY_CENTER);
    gtk_label_set_ellipsize(GTK_LABEL(rotulo_preco_pedido), PANGO_ELLIPSIZE_END);
    gtk_label_set_max_width_chars(GTK_LABEL(rotulo_preco_pedido), 1);

    // botão de selecionar pedido
    GtkWidget* botao_excluir_pedido = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(botao_excluir_pedido), "x");
    gtk_widget_set_visible(GTK_WIDGET(botao_excluir_pedido), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(botao_excluir_pedido), TRUE);
    gtk_widget_set_receives_default(GTK_WIDGET(botao_excluir_pedido), TRUE);
    gtk_widget_set_halign(GTK_WIDGET(botao_excluir_pedido), GTK_ALIGN_END);
    gtk_widget_set_margin_end(GTK_WIDGET(botao_excluir_pedido), 8);
    gtk_widget_set_margin_top(GTK_WIDGET(botao_excluir_pedido), 8);
    gtk_widget_set_margin_bottom(GTK_WIDGET(botao_excluir_pedido), 8);

    // grade dos elementos da linha
    GtkWidget* grade = gtk_grid_new();

    gtk_widget_set_visible(grade, TRUE);
    gtk_widget_set_can_focus(grade, FALSE);
    gtk_grid_set_column_spacing(GTK_GRID(grade), 10);
    gtk_grid_set_row_homogeneous(GTK_GRID(grade), TRUE);
    gtk_grid_set_column_homogeneous(GTK_GRID(grade), TRUE);

    gtk_grid_attach(GTK_GRID(grade), botao_selecionar_pedido, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grade), rotulo_numero_pedido, 1, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grade), rotulo_nome_pedido, 3, 0, 4, 1);
    gtk_grid_attach(GTK_GRID(grade), rotulo_estado_pedido, 7, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grade), rotulo_preco_pedido, 9, 0, 2, 1);
    gtk_grid_attach(GTK_GRID(grade), botao_excluir_pedido, 11, 0, 1, 1);

    // linha da lista
    GtkWidget* linha = gtk_list_box_row_new();

    gtk_widget_set_visible(linha, TRUE);
    gtk_widget_set_can_focus(linha, TRUE);
    gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(linha), FALSE);
    gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(linha), FALSE);

    gtk_container_add(GTK_CONTAINER(linha), grade);

    g_signal_connect(botao_selecionar_pedido, "clicked", G_CALLBACK(botao_selecionar_pedido_clidado), linha);
    g_signal_connect(botao_excluir_pedido, "clicked", G_CALLBACK(botao_excluir_pedido_clidado), linha);

    return linha;
}

/* @brief Inicializa a lista de pedidos com os pedidos armazenados
 */
void inicializar_lista_pedidos(void) {
    limpar_lista_pedidos();

    for (int pedido = 0; pedido < pedidos->tamanho; pedido++) {
        GtkWidget* linha = criar_linha_lista_pedido(pedido);

        gtk_list_box_insert(lista_pedidos, linha, pedido);
    }
}

/* @brief Limpa a lista de pedidos
 */
void limpar_lista_pedidos(void) {
    while (true) {
        GtkListBoxRow* linha = gtk_list_box_get_row_at_index(lista_pedidos, 0);

        if (linha == NULL) {
            break;
        }

        gtk_container_remove(GTK_CONTAINER(lista_pedidos), GTK_WIDGET(linha));
    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ---------------------------------- Interface da Lista de Produtos ----------------------------------- //
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void alterar_nome_produto(GtkWidget*, GtkWidget*);
void alterar_preco_produto(GtkWidget*, GtkWidget*);
void alterar_quantidade_produto(GtkWidget*, GtkWidget*);
void entrada_nome_produto_ativada(GtkWidget*, GtkWidget*);
void entrada_preco_produto_ativada(GtkWidget*, GtkWidget*);
void entrada_quantidade_produto_ativada(GtkWidget*, GtkWidget*);
bool entrada_nome_produto_alterada(GtkWidget*, GdkEventFocus, GtkWidget*);
bool entrada_preco_produto_alterada(GtkWidget*, GdkEventFocus, GtkWidget*);
bool entrada_quantidade_produto_alterada(GtkWidget*, GdkEventFocus, GtkWidget*);
void botao_excluir_produto_clicado(GtkWidget*, GtkWidget*);
GtkWidget* criar_linha_lista_produto(int produto);
void inicializar_lista_produtos(void);
void limpar_lista_produtos(void);

/* @brief Atualiza o nome do produto
 */
void alterar_nome_produto(GtkWidget* entrada, GtkWidget* linha) {
    int pedido = indice_pedido;
    int produto = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    char * nome = copiarString(gtk_entry_get_text(GTK_ENTRY(entrada)));

    indiceProduto(indicePedido(pedidos, pedido)->produtos, produto)->nome = nome;
}

/* @brief Atualiza o preco do produto
 */
void alterar_preco_produto(GtkWidget* entrada, GtkWidget* linha) {
    int pedido = indice_pedido;
    int produto = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    const char * texto = gtk_entry_get_text(GTK_ENTRY(entrada));
    char * invalido = NULL;

    double preco = strtod(texto, &invalido);

    if (*invalido) {
        char total[64];

        sprintf(total, "%.2f", indiceProduto(indicePedido(pedidos, pedido)->produtos, produto)->valor);

        gtk_entry_set_text(GTK_ENTRY(entrada), total);
    }
    else {
        indiceProduto(indicePedido(pedidos, pedido)->produtos, produto)->valor = preco;
    }
}

/* @brief Atualiza a quantidade do produto
 */
void alterar_quantidade_produto(GtkWidget* entrada, GtkWidget* linha) {
    int pedido = indice_pedido;
    int produto = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    const char* texto = gtk_entry_get_text(GTK_ENTRY(entrada));
    char* invalido = NULL;

    int quantidade = strtol(texto, &invalido, 10);

    if (*invalido) {
        char quantidade[64];

        sprintf(quantidade, "%d", indiceProduto(indicePedido(pedidos, pedido)->produtos, produto)->quantidade);

        gtk_entry_set_text(GTK_ENTRY(entrada), quantidade);
    }
    else {
        indiceProduto(indicePedido(pedidos, pedido)->produtos, produto)->quantidade = quantidade;
    }
}

/* @brief Atualiza o nome do produto
 */
void entrada_nome_produto_ativada(GtkWidget* entrada, GtkWidget* linha) {
    alterar_nome_produto(entrada, linha);
}

/* @brief Atualiza o preco do produto
 */
void entrada_preco_produto_ativada(GtkWidget* entrada, GtkWidget* linha) {
    alterar_preco_produto(entrada, linha);
}

/* @brief Atualiza a quantidade do produto
 */
void entrada_quantidade_produto_ativada(GtkWidget* entrada, GtkWidget* linha) {
    alterar_quantidade_produto(entrada, linha);
}

/* @brief Atualiza o nome do produto
 */
bool entrada_nome_produto_perde_foco(GtkWidget* entrada, GdkEventFocus event, GtkWidget* linha) {
    alterar_nome_produto(entrada, linha);

    return TRUE;
}

/* @brief Atualiza o preco do produto
 */
bool entrada_preco_produto_perde_foco(GtkWidget* entrada, GdkEventFocus event, GtkWidget* linha) {
    alterar_preco_produto(entrada, linha);

    return TRUE;
}

/* @brief Atualiza a quantidade do produto
 */
bool entrada_quantidade_produto_perde_foco(GtkWidget* entrada, GdkEventFocus event, GtkWidget* linha) {
    alterar_quantidade_produto(entrada, linha);

    return TRUE;
}

/* @brief Seleciona o produto e o exclui
 *
 * @param botao Botao do produto selecionado
 * @param linha Linha do produto selecionado
 */
void botao_excluir_produto_clicado(GtkWidget* botao, GtkWidget* linha) {
    int produto = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));
    int pedido = indice_pedido;

    if (produto == -1) {
        return;
    }

    gtk_container_remove(GTK_CONTAINER(lista_produtos), GTK_WIDGET(linha));
    removerProduto(indicePedido(pedidos, pedido)->produtos, produto);
}

GtkWidget* criar_linha_lista_produto(int indice) {
    static int borda = 5;

    Pedido * pedido = indicePedido(pedidos, indice_pedido);

    if (indice >= pedido->produtos->tamanho) {
        return NULL;
    }

    Produto * produto = indiceProduto(pedido->produtos, indice);

    char * nome = produto->nome;

    char preco[16];
    char quantidade[64];

    sprintf(preco, "%.2f", produto->valor);
    sprintf(quantidade, "%d", produto->quantidade);

    // rótulo para o nome do produto
    GtkWidget* rotulo_nome_produto = gtk_label_new("Produto");

    gtk_widget_set_visible(GTK_WIDGET(rotulo_nome_produto), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(rotulo_nome_produto), FALSE);
    gtk_widget_set_margin_start(GTK_WIDGET(rotulo_nome_produto), borda);
    gtk_widget_set_margin_end(GTK_WIDGET(rotulo_nome_produto), borda);
    gtk_widget_set_margin_top(GTK_WIDGET(rotulo_nome_produto), borda);
    gtk_widget_set_margin_bottom(GTK_WIDGET(rotulo_nome_produto), borda);

    // entrada para o nome do produto
    GtkWidget* entrada_nome_produto = gtk_entry_new();

    gtk_widget_set_visible(GTK_WIDGET(entrada_nome_produto), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(entrada_nome_produto), TRUE);
    gtk_widget_set_margin_start(GTK_WIDGET(entrada_nome_produto), borda);
    gtk_widget_set_margin_end(GTK_WIDGET(entrada_nome_produto), borda);
    gtk_widget_set_margin_top(GTK_WIDGET(entrada_nome_produto), borda);
    gtk_widget_set_margin_bottom(GTK_WIDGET(entrada_nome_produto), borda);
    gtk_widget_set_hexpand(GTK_WIDGET(entrada_nome_produto), TRUE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entrada_nome_produto), "Digite o Nome...");

    gtk_entry_set_text(GTK_ENTRY(entrada_nome_produto), nome);

    // rótulo para o preco do produto
    GtkWidget* rotulo_preco_produto = gtk_label_new("Preço R$");

    gtk_widget_set_visible(GTK_WIDGET(rotulo_preco_produto), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(rotulo_preco_produto), FALSE);
    gtk_widget_set_margin_start(GTK_WIDGET(rotulo_preco_produto), borda);
    gtk_widget_set_margin_end(GTK_WIDGET(rotulo_preco_produto), borda);
    gtk_widget_set_margin_top(GTK_WIDGET(rotulo_preco_produto), borda);
    gtk_widget_set_margin_bottom(GTK_WIDGET(rotulo_preco_produto), borda);

    // entrada para o preco do produto
    GtkWidget* entrada_preco_produto = gtk_entry_new();

    gtk_widget_set_visible(GTK_WIDGET(entrada_preco_produto), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(entrada_preco_produto), TRUE);
    gtk_widget_set_margin_start(GTK_WIDGET(entrada_preco_produto), borda);
    gtk_widget_set_margin_end(GTK_WIDGET(entrada_preco_produto), borda);
    gtk_widget_set_margin_top(GTK_WIDGET(entrada_preco_produto), borda);
    gtk_widget_set_margin_bottom(GTK_WIDGET(entrada_preco_produto), borda);
    gtk_widget_set_hexpand(GTK_WIDGET(entrada_preco_produto), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entrada_preco_produto), "Digite o Preco...");

    gtk_entry_set_text(GTK_ENTRY(entrada_preco_produto), preco);

    // rótulo para o quantidade do produto
    GtkWidget* rotulo_quantidade_produto = gtk_label_new("Quantidade");

    gtk_widget_set_visible(GTK_WIDGET(rotulo_quantidade_produto), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(rotulo_quantidade_produto), FALSE);
    gtk_widget_set_margin_start(GTK_WIDGET(rotulo_quantidade_produto), borda);
    gtk_widget_set_margin_end(GTK_WIDGET(rotulo_quantidade_produto), borda);
    gtk_widget_set_margin_top(GTK_WIDGET(rotulo_quantidade_produto), borda);
    gtk_widget_set_margin_bottom(GTK_WIDGET(rotulo_quantidade_produto), borda);

    // entrada para o quantidade do produto
    GtkWidget* entrada_quantidade_produto = gtk_entry_new();

    gtk_widget_set_visible(GTK_WIDGET(entrada_quantidade_produto), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(entrada_quantidade_produto), TRUE);
    gtk_widget_set_margin_start(GTK_WIDGET(entrada_quantidade_produto), borda);
    gtk_widget_set_margin_end(GTK_WIDGET(entrada_quantidade_produto), borda);
    gtk_widget_set_margin_top(GTK_WIDGET(entrada_quantidade_produto), borda);
    gtk_widget_set_margin_bottom(GTK_WIDGET(entrada_quantidade_produto), borda);
    gtk_widget_set_hexpand(GTK_WIDGET(entrada_quantidade_produto), FALSE);
    gtk_entry_set_placeholder_text(GTK_ENTRY(entrada_quantidade_produto), "Digite a Quantidade...");

    gtk_entry_set_text(GTK_ENTRY(entrada_quantidade_produto), quantidade);
    
    // botão de selecionar produto
    GtkWidget* botao_excluir_produto = gtk_button_new();

    gtk_button_set_label(GTK_BUTTON(botao_excluir_produto), "x");
    gtk_widget_set_visible(GTK_WIDGET(botao_excluir_produto), TRUE);
    gtk_widget_set_can_focus(GTK_WIDGET(botao_excluir_produto), TRUE);
    gtk_widget_set_receives_default(GTK_WIDGET(botao_excluir_produto), TRUE);
    gtk_widget_set_margin_start(GTK_WIDGET(botao_excluir_produto), borda);
    gtk_widget_set_margin_end(GTK_WIDGET(botao_excluir_produto), borda);
    gtk_widget_set_margin_top(GTK_WIDGET(botao_excluir_produto), borda);
    gtk_widget_set_margin_bottom(GTK_WIDGET(botao_excluir_produto), borda);

    // grade dos elementos da linha
    GtkWidget* grade = gtk_grid_new();

    gtk_widget_set_visible(grade, TRUE);
    gtk_widget_set_can_focus(grade, FALSE);

    gtk_grid_attach(GTK_GRID(grade), rotulo_nome_produto, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grade), entrada_nome_produto, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grade), rotulo_preco_produto, 2, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grade), entrada_preco_produto, 3, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grade), rotulo_quantidade_produto, 4, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grade), entrada_quantidade_produto, 5, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grade), botao_excluir_produto, 6, 0, 1, 1);

    // linha da lista
    GtkWidget* linha = gtk_list_box_row_new();

    gtk_widget_set_visible(linha, TRUE);
    gtk_widget_set_can_focus(linha, TRUE);
    gtk_list_box_row_set_activatable(GTK_LIST_BOX_ROW(linha), FALSE);
    gtk_list_box_row_set_selectable(GTK_LIST_BOX_ROW(linha), FALSE);

    gtk_container_add(GTK_CONTAINER(linha), grade);

    g_signal_connect(entrada_nome_produto, "activate", G_CALLBACK(entrada_nome_produto_ativada), linha);
    g_signal_connect(entrada_preco_produto, "activate", G_CALLBACK(entrada_preco_produto_ativada), linha);
    g_signal_connect(entrada_quantidade_produto, "activate", G_CALLBACK(entrada_quantidade_produto_ativada), linha);
    g_signal_connect(entrada_nome_produto, "focus-out-event", G_CALLBACK(entrada_nome_produto_perde_foco), linha);
    g_signal_connect(entrada_preco_produto, "focus-out-event", G_CALLBACK(entrada_preco_produto_perde_foco), linha);
    g_signal_connect(entrada_quantidade_produto, "focus-out-event", G_CALLBACK(entrada_quantidade_produto_perde_foco), linha);
    g_signal_connect(botao_excluir_produto, "clicked", G_CALLBACK(botao_excluir_produto_clicado), linha);

    return linha;
}

/* @brief Inicializa a lista de produtos com os produtos armazenados
 */
void inicializar_lista_produtos(void) {
    limpar_lista_produtos();

    int pedido = indice_pedido;
    int tamanho = indicePedido(pedidos, pedido)->produtos->tamanho;

    for (int produto = 0; produto < tamanho; produto++) {
        GtkWidget* linha = criar_linha_lista_produto(produto);

        gtk_list_box_insert(lista_produtos, linha, produto);
    }
}

/* @brief Limpa a lista de produto
 */
void limpar_lista_produtos(void) {
    while (true) {
        GtkListBoxRow* linha = gtk_list_box_get_row_at_index(lista_produtos, 0);

        if (linha == NULL) {
            break;
        }

        gtk_container_remove(GTK_CONTAINER(lista_produtos), GTK_WIDGET(linha));
    }
}

///////////////////////////////////////////////////////////////
// ------------------- Janela de Início -------------------- //
///////////////////////////////////////////////////////////////

/* @brief Abre janela de criação de nova sessão
 */
void botao_nova_sessao_clicado(void) {
    gtk_widget_show_all(GTK_WIDGET(janela_nova_sessao));
}

/* @brief Abre a janela da última sessão aberta
 */
void botao_continuar_sessao_clicado(void) {
    if (ultimaSessao(sessao, &pedidos)) {
        inicializar_lista_pedidos();

        gtk_stack_set_visible_child_name(pilha_janelas, "janela_pedidos");
        g_signal_emit_by_name(botao_todos_pedidos, "clicked");
    }
}

/* @brief Abre a janela da para selecionar uma das sessões salvas
 */
void botao_carregar_sessao_clicado(void) {
    gtk_widget_show_all(GTK_WIDGET(janela_carregar_sessao));
}

/* @brief Fecha a sessão do programa e fecha o programa
 */
void botao_fechar_sessao_clicado(void) {
    if (strcmp(gtk_stack_get_visible_child_name(pilha_janelas), "janela_inicio") == 0) {
        // sessao.salvar(pedidos);
        salvarSessao(sessao, pedidos);
    }

    deletarPedidos(pedidos);
    deletarSessao(sessao);

    gtk_main_quit();
}

////////////////////////////////////////////////////////////////////
// ------------------- Janela de Nova Sessão -------------------- //
////////////////////////////////////////////////////////////////////

/* @brief Cancela a criação da nova sessão
 */
void botao_cancelar_sessao_clicado(void) {
    gtk_widget_hide(GTK_WIDGET(janela_nova_sessao));
}

/* @brief Conclui a criação da nova sessão
 */
void botao_concluir_sessao_clicado(void) {
    char * nome = obter_texto_entrada(entrada_nova_sessao);

    if (strlen(nome) != 0) {
        if (novaSessao(sessao, &pedidos, nome)) {
            inicializar_lista_pedidos();

            gtk_entry_set_text(entrada_nova_sessao, ""); 
            gtk_widget_hide(GTK_WIDGET(janela_nova_sessao));
            gtk_stack_set_visible_child_name(pilha_janelas, "janela_pedidos");
            g_signal_emit_by_name(botao_todos_pedidos, "clicked");
        }
    }
}

/* @brief Emite o sinal para clicar no botão de concluir a criação da nova sessão
 */
void entrada_nova_sessao_ativada(void) {
    g_signal_emit_by_name(botao_concluir_sessao, "clicked");
}

////////////////////////////////////////////////////////////////
// ------------------- Janela de Pedidos -------------------- //
////////////////////////////////////////////////////////////////

/* @brief Filtra os pedidos a serem mostrados na lista de pedidos
 *
 * @param linha Linha do pedido na lista de pedidos
 * @param filtro Ponteiro para o filtro
 *
 * @return Se a linha deve ser filtrada ou não da lista de pedidos
 */
gboolean filtrar_lista_pedido(GtkListBoxRow* linha, gpointer filtro) {
    int pedido = gtk_list_box_row_get_index(linha);

    return strcmp(indicePedido(pedidos, pedido)->status, *(char**)(filtro)) == 0;
}

/* @brief Filtro que permite todos os pedidos
 *
 * @param linha Linha do pedido na lista de pedidos
 * @param filtro Ponteiro para o filtro
 *
 * @return Verdadeiro, para todos os pedidos serem mostrados
 */
gboolean nao_filtrar_lista_pedido(GtkListBoxRow* linha, gpointer filtro) {
    return TRUE;
}

/* @brief Troca para a janela com todos os pedidos
 */
void botao_todos_pedidos_clicado(void) {
    gtk_list_box_set_filter_func(lista_pedidos, nao_filtrar_lista_pedido, NULL, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Todos");
}

/* @brief Troca para a janela com os pedidos pendentes
 */
void botao_pedidos_pendentes_clicado(void) {
    char * estado = "Pendente";

    gtk_list_box_set_filter_func(lista_pedidos, filtrar_lista_pedido, &estado, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Pendentes");
}

/* @brief Troca para a janela com os pedidos pagos
 */
void botao_pedidos_pagos_clicado(void) {
    char * estado = "Pago";

    gtk_list_box_set_filter_func(lista_pedidos, filtrar_lista_pedido, &estado, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Pagos");
}

/* @brief Troca para a janela com os pedidos preparados
 */
void botao_pedidos_preparados_clicado(void) {
    char * estado = "Preparado";

    gtk_list_box_set_filter_func(lista_pedidos, filtrar_lista_pedido, &estado, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Preparados");
}

/* @brief Troca para a janela com os pedidos entregues
 */
void botao_pedidos_entregues_clicado(void) {
    char * estado = "Entregue";

    gtk_list_box_set_filter_func(lista_pedidos, filtrar_lista_pedido, &estado, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Entregues");
}

/* @brief Adiciona um novo pedido a lista de pedidos
 */
void botao_adicionar_pedido_clicado(void) {
    Produtos * produtos = criarProdutos();
    Pedido * pedido = criarPedido("", "", "", "Dinheiro", "Pendente", produtos);
    deletarProdutos(produtos);

    int indice = adicionarPedido(pedidos, pedido);

    GtkWidget* linha = criar_linha_lista_pedido(indice);
    gtk_list_box_insert(lista_pedidos, linha, indice);
}

/* @brief Volta para a janela de início
 */
void botao_voltar_clicado(void) {
    // sessao.salvar(pedidos);
    salvarSessao(sessao, pedidos);

    inicializar_lista_arquivos();

    gtk_stack_set_visible_child_name(pilha_janelas, "janela_inicio");
}

///////////////////////////////////////////////////////////////
// ------------------- Janela de Pedido -------------------- //
///////////////////////////////////////////////////////////////

/* @brief Atualiza o nome do pedido
 */
void entrada_nome_pedido_alterada(void) {
    Pedido * pedido = indicePedido(pedidos, indice_pedido);

    free(pedido->nome);

    pedido->nome = copiarString(gtk_entry_get_text(GTK_ENTRY(entrada_nome_pedido)));
}

/* @brief Atualiza o contato do pedido
 */
void entrada_contato_pedido_alterada(void) {
    Pedido * pedido = indicePedido(pedidos, indice_pedido);

    free(pedido->contato);

    pedido->contato = copiarString(gtk_entry_get_text(GTK_ENTRY(entrada_contato_pedido)));
}

/* @brief Atualiza o endereco do pedido
 */
void entrada_endereco_pedido_alterada(void) {
    Pedido * pedido = indicePedido(pedidos, indice_pedido);

    free(pedido->endereco);

    pedido->endereco = copiarString(gtk_entry_get_text(GTK_ENTRY(entrada_endereco_pedido)));
}

/* @brief Atualiza o distancia do pedido
 */
void entrada_distancia_pedido_alterada(void) {
    /*
    int pedido = indice_pedido;

    const char* texto = gtk_entry_get_text(GTK_ENTRY(entrada_distancia_pedido));
    char* invalido = NULL;

    int distancia = strtol(texto, &invalido, 10);

    if (*invalido) {
        char * distancia = std::to_string(pedidos[pedido].distancia());

        distancia = distancia.substr(0, distancia.size() - 4);

        gtk_entry_set_text(GTK_ENTRY(entrada_distancia_pedido), distancia.c_str());
    }
    else {
        pedidos[pedido].distancia(distancia);
    }
    */
}

/* @brief Atualiza o estado do pedido
 */
void selecao_estado_pedido_alterada(void) {
    Pedido * pedido = indicePedido(pedidos, indice_pedido);

    free(pedido->status);

    pedido->status = textoStatus(gtk_combo_box_get_active(GTK_COMBO_BOX(selecao_estado_pedido)));
}

/* @brief Atualiza o pagamento do pedido
 */
void selecao_pagamento_pedido_alterada(void) {
    Pedido * pedido = indicePedido(pedidos, indice_pedido);

    free(pedido->pagamento);

    pedido->pagamento = textoPagamento(gtk_combo_box_get_active(GTK_COMBO_BOX(selecao_pagamento_pedido)));
}

/* @brief Abre a janela com a lista de produtos
 */
void botao_lista_produtos_clicado(void) {
    int pedido = indice_pedido;

    if (pedido >= pedidos->tamanho) {
        return;
    }

    inicializar_lista_produtos();
    gtk_stack_set_visible_child_name(pilha_janelas, "janela_produtos");
}

/* @brief Fecha a janela de pedido
 */
void botao_fechar_pedido_clicado(void) {
    // gtk_stack_set_visible_child_name(pilha_janelas, "janela_pedidos");
}

/* @brief Salva as alterações no pedido
 */
void botao_concluir_pedido_clicado(void) {
    Pedido * pedido = indicePedido(pedidos, indice_pedido);
    GtkWidget* linha = GTK_WIDGET(gtk_list_box_get_row_at_index(lista_pedidos, indice_pedido));

    pedido = copiarPedido(pedido);

    removerPedido(pedidos, indice_pedido);
    gtk_container_remove(GTK_CONTAINER(lista_pedidos), GTK_WIDGET(linha));

    indice_pedido = adicionarPedido(pedidos, pedido);
    linha = criar_linha_lista_pedido(indice_pedido);
    gtk_list_box_insert(lista_pedidos, linha, indice_pedido);

    deletarPedido(pedido);

    gtk_stack_set_visible_child_name(pilha_janelas, "janela_pedidos");
}

/////////////////////////////////////////////////////////////////
// ------------------- Janela de Produtos -------------------- //
/////////////////////////////////////////////////////////////////

/* @brief Adiciona um novo produto a lista de produtos
 */
void botao_adicionar_produto_clicado(void) {
    Produto * produto = criarProduto("", 0.0, 0);
    Pedido * pedido = indicePedido(pedidos, indice_pedido);

    int indice = pedido->produtos->tamanho;
    adicionarProduto(pedido->produtos, produto);

    GtkWidget* linha = criar_linha_lista_produto(indice);
    gtk_list_box_insert(lista_produtos, linha, indice);
}

/* @brief Fecha a janela da lista de produtos
 */
void botao_fechar_lista_produto_clicado(void) {
    definir_valores_janela_pedido();
    gtk_stack_set_visible_child_name(pilha_janelas, "janela_pedido");
}

/////////////////////////////////////////////////////////////////
// ------------------- Funções Principais -------------------- //
/////////////////////////////////////////////////////////////////

/* @brief Fecha o programa
 */
void fechar_programa(void) {
    if (strcmp(gtk_stack_get_visible_child_name(pilha_janelas), "janela_inicio") == 0) {
        // sessao.salvar(pedidos);
        salvarSessao(sessao, pedidos);
    }

    deletarPedidos(pedidos);
    deletarSessao(sessao);

    gtk_main_quit();
}

void iniciar(int argc, char** argv) {
    // ----- inicialiando e carregando os arquivos de interface ----- //
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    if (gtk_builder_add_from_file(builder, "interface/hermes.glade", &error) == 0) {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);

        return;
    }

    // ----- adicionando as funções de callback aos simbolos ----- //
    gtk_builder_add_callback_symbols(builder,
        // sinais dos widgets da janela de início
        "botao_nova_sessao_clicado", G_CALLBACK(botao_nova_sessao_clicado),
        "botao_continuar_sessao_clicado", G_CALLBACK(botao_continuar_sessao_clicado),
        "botao_carregar_sessao_clicado", G_CALLBACK(botao_carregar_sessao_clicado),
        "botao_fechar_sessao_clicado", G_CALLBACK(botao_fechar_sessao_clicado),

        // sinais dos widgets da janela de nova sessao 
        "botao_cancelar_sessao_clicado", G_CALLBACK(botao_cancelar_sessao_clicado),
        "botao_concluir_sessao_clicado", G_CALLBACK(botao_concluir_sessao_clicado),
        "entrada_nova_sessao_ativada", G_CALLBACK(entrada_nova_sessao_ativada),

        // sinais dos widgets da janela de pedidos
        "botao_todos_pedidos_clicado", G_CALLBACK(botao_todos_pedidos_clicado),
        "botao_pedidos_pendentes_clicado", G_CALLBACK(botao_pedidos_pendentes_clicado),
        "botao_pedidos_pagos_clicado", G_CALLBACK(botao_pedidos_pagos_clicado),
        "botao_pedidos_preparados_clicado", G_CALLBACK(botao_pedidos_preparados_clicado),
        "botao_pedidos_entregues_clicado", G_CALLBACK(botao_pedidos_entregues_clicado),
        "botao_adicionar_pedido_clicado", G_CALLBACK(botao_adicionar_pedido_clicado),
        "botao_voltar_clicado", G_CALLBACK(botao_voltar_clicado),

        // sinais dos widgets da janela do pedido
        "entrada_nome_pedido_ativada", G_CALLBACK(entrada_nome_pedido_alterada),
        "entrada_contato_pedido_ativada", G_CALLBACK(entrada_contato_pedido_alterada),
        "entrada_endereco_pedido_ativada", G_CALLBACK(entrada_endereco_pedido_alterada),
        "entrada_nome_pedido_perde_foco", G_CALLBACK(entrada_nome_pedido_alterada),
        "entrada_contato_pedido_perde_foco", G_CALLBACK(entrada_contato_pedido_alterada),
        "entrada_endereco_pedido_perde_foco", G_CALLBACK(entrada_endereco_pedido_alterada),
        "selecao_estado_pedido_alterada", G_CALLBACK(selecao_estado_pedido_alterada),
        "selecao_pagamento_pedido_alterada", G_CALLBACK(selecao_pagamento_pedido_alterada),
        "botao_lista_produtos_clicado", G_CALLBACK(botao_lista_produtos_clicado),
        "botao_fechar_pedido_clicado", G_CALLBACK(botao_fechar_pedido_clicado),
        "botao_concluir_pedido_clicado", G_CALLBACK(botao_concluir_pedido_clicado),

        // sinais dos widgets da janela de produtos
        "botao_adicionar_produto_clicado", G_CALLBACK(botao_adicionar_produto_clicado),
        "botao_fechar_lista_produto_clicado", G_CALLBACK(botao_fechar_lista_produto_clicado),
        NULL
    );

    gtk_builder_connect_signals(builder, NULL);

    // ----- atribuindo os valores dos membros do programa ----- //
    janela_principal = GTK_WINDOW(gtk_builder_get_object(builder, "janela_principal"));
    janela_carregar_sessao = GTK_WINDOW(gtk_builder_get_object(builder, "janela_carregar_sessao"));
    janela_nova_sessao = GTK_WINDOW(gtk_builder_get_object(builder, "janela_nova_sessao"));

    pilha_janelas = GTK_STACK(gtk_builder_get_object(builder, "pilha_janelas"));

    barra_janela_inicio = GTK_HEADER_BAR(gtk_builder_get_object(builder, "barra_janela_inicio"));
    barra_janela_pedido = GTK_HEADER_BAR(gtk_builder_get_object(builder, "barra_janela_pedido"));
    barra_janela_pedidos = GTK_HEADER_BAR(gtk_builder_get_object(builder, "barra_janela_pedidos"));
    barra_janela_produtos = GTK_HEADER_BAR(gtk_builder_get_object(builder, "barra_janela_produtos"));

    entrada_nova_sessao = GTK_ENTRY(gtk_builder_get_object(builder, "entrada_nova_sessao"));
    entrada_nome_pedido = GTK_ENTRY(gtk_builder_get_object(builder, "entrada_nome_pedido"));
    entrada_contato_pedido = GTK_ENTRY(gtk_builder_get_object(builder, "entrada_contato_pedido"));
    entrada_endereco_pedido = GTK_ENTRY(gtk_builder_get_object(builder, "entrada_endereco_pedido"));

    selecao_estado_pedido = GTK_COMBO_BOX(gtk_builder_get_object(builder, "selecao_estado_pedido"));
    selecao_pagamento_pedido = GTK_COMBO_BOX(gtk_builder_get_object(builder, "selecao_pagamento_pedido"));

    lista_arquivos = GTK_LIST_BOX(gtk_builder_get_object(builder, "lista_arquivos"));
    lista_pedidos = GTK_LIST_BOX(gtk_builder_get_object(builder, "lista_pedidos"));
    lista_produtos = GTK_LIST_BOX(gtk_builder_get_object(builder, "lista_produtos"));

    rotulo_preco_pedido = GTK_LABEL(gtk_builder_get_object(builder, "rotulo_preco_pedido"));

    botao_concluir_sessao = GTK_BUTTON(gtk_builder_get_object(builder, "botao_concluir_sessao"));

    // ----- conectando so sinais às funções ----- //
    g_signal_connect(janela_principal, "destroy", G_CALLBACK(fechar_programa), NULL);
    g_signal_connect(janela_carregar_sessao, "delete_event", G_CALLBACK(gtk_widget_hide_on_delete), janela_carregar_sessao);
    g_signal_connect(janela_nova_sessao, "delete-event", G_CALLBACK(gtk_widget_hide_on_delete), GTK_WIDGET(janela_nova_sessao));
    g_signal_connect(entrada_nova_sessao, "activate", G_CALLBACK(entrada_nova_sessao_ativada), NULL);

    sessao = criarSessao();

    inicializar_lista_arquivos();
}

void executar(void) {
    gtk_widget_show_all(GTK_WIDGET(janela_principal));

    gtk_main();
}

int main(int argc, char** argv) {
    iniciar(argc, argv);

    executar();
}