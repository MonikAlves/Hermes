#include <gtk/gtk.h>

#include "produto.hpp"
#include "pedido.hpp"
#include "edicao.hpp"
#include "sessao.hpp"

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
GtkWidget* criar_linha_lista_produto(unsigned produto);
void inicializar_lista_produtos(void);
void limpar_lista_produtos(void);

/* @brief Atualiza o nome do produto
 */
void alterar_nome_produto(GtkWidget* entrada, GtkWidget* linha) {
    int pedido = indice_pedido;
    int produto = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    std::string nome = gtk_entry_get_text(GTK_ENTRY(entrada));

    pedidos[pedido].produtos[produto].nome(nome);
}

/* @brief Atualiza o preco do produto
 */
void alterar_preco_produto(GtkWidget* entrada, GtkWidget* linha) {
    int pedido = indice_pedido;
    int produto = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    const char* texto = gtk_entry_get_text(GTK_ENTRY(entrada));
    char* invalido = NULL;

    double preco = strtod(texto, &invalido);

    if (*invalido) {
        gtk_entry_set_text(GTK_ENTRY(entrada), std::to_string(pedidos[pedido].produtos[produto].preco()).c_str());
    }
    else {
        pedidos[pedido].produtos[produto].preco(preco);
    }
}

/* @brief Atualiza a quantidade do produto
 */
void alterar_quantidade_produto(GtkWidget* entrada, GtkWidget* linha) {
    int pedido = indice_pedido;
    int produto = gtk_list_box_row_get_index(GTK_LIST_BOX_ROW(linha));

    const char* texto = gtk_entry_get_text(GTK_ENTRY(entrada));
    char* invalido = NULL;

    unsigned quantidade = strtol(texto, &invalido, 10);

    if (*invalido) {
        std::string quantidade = std::to_string(pedidos[pedido].produtos[produto].quantidade());

        quantidade = quantidade.substr(0, quantidade.size() - 4);

        gtk_entry_set_text(GTK_ENTRY(entrada), quantidade.c_str());
    }
    else {
        pedidos[pedido].produtos[produto].quantidade(quantidade);
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
    pedidos[pedido].produtos.remover(produto);
}

/* @brief Cria uma nova linha para a lista de produtos
 *
 * @param produto Número do produto que será adicionado à lista
 *
 * @return Linha da lista de produtos
 */
GtkWidget* criar_linha_lista_produto(unsigned produto) {
    static unsigned borda = 5;

    int pedido = indice_pedido;

    if (produto >= pedidos[pedido].produtos.tamanho()) {
        return NULL;
    }

    std::string nome = pedidos[pedido].produtos[produto].nome();
    std::string preco = std::to_string(pedidos[pedido].produtos[produto].preco());
    std::string quantidade = std::to_string(pedidos[pedido].produtos[produto].quantidade());

    preco = preco.substr(0, preco.size() - 4);

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

    gtk_entry_set_text(GTK_ENTRY(entrada_nome_produto), nome.c_str());

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

    gtk_entry_set_text(GTK_ENTRY(entrada_preco_produto), preco.c_str());

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

    gtk_entry_set_text(GTK_ENTRY(entrada_quantidade_produto), quantidade.c_str());
    
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

    for (unsigned produto = 0; produto < pedidos[pedido].produtos.tamanho(); produto++) {
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
    if (sessao.anterior(pedidos)) {
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
    std::string nome = obter_texto_entrada(entrada_nova_sessao);

    if (not nome.empty()) {
        if (sessao.nova(nome)) {
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
    unsigned pedido = gtk_list_box_row_get_index(linha);

    return pedidos[pedido].estado() == *(Estado*)(filtro);
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
    Estado estado = PENDENTE;

    gtk_list_box_set_filter_func(lista_pedidos, filtrar_lista_pedido, &estado, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Pendentes");
}

/* @brief Troca para a janela com os pedidos pagos
 */
void botao_pedidos_pagos_clicado(void) {
    Estado estado = PAGO;

    gtk_list_box_set_filter_func(lista_pedidos, filtrar_lista_pedido, &estado, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Pagos");
}

/* @brief Troca para a janela com os pedidos preparados
 */
void botao_pedidos_preparados_clicado(void) {
    Estado estado = PREPARADO;

    gtk_list_box_set_filter_func(lista_pedidos, filtrar_lista_pedido, &estado, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Preparados");
}

/* @brief Troca para a janela com os pedidos entregues
 */
void botao_pedidos_entregues_clicado(void) {
    Estado estado = ENTREGUE;

    gtk_list_box_set_filter_func(lista_pedidos, filtrar_lista_pedido, &estado, NULL);

    gtk_header_bar_set_title(barra_janela_pedidos, "Entregues");
}

/* @brief Adiciona um novo pedido a lista de pedidos
 */
void botao_adicionar_pedido_clicado(void) {
    Pedido pedido("", "", "", 0, DINHEIRO, PENDENTE, Produtos());
    unsigned indice = pedidos.tamanho();
    pedidos.inserir(pedido);
    GtkWidget* linha = criar_linha_lista_pedido(indice);
    gtk_list_box_insert(lista_pedidos, linha, indice);
}

/* @brief Volta para a janela de início
 */
void botao_voltar_clicado(void) {
    sessao.salvar(pedidos);

    pedidos = Pedidos();
    edicao = Edicao();

    inicializar_lista_arquivos();

    gtk_stack_set_visible_child_name(pilha_janelas, "janela_inicio");
}

///////////////////////////////////////////////////////////////
// ------------------- Janela de Pedido -------------------- //
///////////////////////////////////////////////////////////////

/* @brief Atualiza o nome do pedido
 */
void entrada_nome_pedido_alterada(void) {
    int pedido = indice_pedido;

    pedidos[pedido].nome(gtk_entry_get_text(GTK_ENTRY(entrada_nome_pedido)));
}

/* @brief Atualiza o contato do pedido
 */
void entrada_contato_pedido_alterada(void) {
    int pedido = indice_pedido;

    pedidos[pedido].contato(gtk_entry_get_text(GTK_ENTRY(entrada_contato_pedido)));
}

/* @brief Atualiza o endereco do pedido
 */
void entrada_endereco_pedido_alterada(void) {
    int pedido = indice_pedido;

    pedidos[pedido].endereco(gtk_entry_get_text(GTK_ENTRY(entrada_endereco_pedido)));
}

/* @brief Atualiza o distancia do pedido
 */
void entrada_distancia_pedido_alterada(void) {
    int pedido = indice_pedido;

    const char* texto = gtk_entry_get_text(GTK_ENTRY(entrada_distancia_pedido));
    char* invalido = NULL;

    unsigned distancia = strtol(texto, &invalido, 10);

    if (*invalido) {
        std::string distancia = std::to_string(pedidos[pedido].distancia());

        distancia = distancia.substr(0, distancia.size() - 4);

        gtk_entry_set_text(GTK_ENTRY(entrada_distancia_pedido), distancia.c_str());
    }
    else {
        pedidos[pedido].distancia(distancia);
    }
}

/* @brief Atualiza o estado do pedido
 */
void selecao_estado_pedido_alterada(void) {
    int pedido = indice_pedido;

    pedidos[pedido].estado(Estado(gtk_combo_box_get_active(GTK_COMBO_BOX(selecao_estado_pedido))));
}

/* @brief Atualiza o pagamento do pedido
 */
void selecao_pagamento_pedido_alterada(void) {
    int pedido = indice_pedido;

    pedidos[pedido].pagamento(Pagamento(gtk_combo_box_get_active(GTK_COMBO_BOX(selecao_pagamento_pedido))));
}

/* @brief Abre a janela com a lista de produtos
 */
void botao_lista_produtos_clicado(void) {
    int pedido = indice_pedido;

    if (pedido >= pedidos.tamanho()) {
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
    int pedido = indice_pedido;

    std::string nome = pedidos[pedido].nome();
    std::string estado = estado_texto(pedidos[pedido].estado());
    std::string preco = "R$" + std::to_string(pedidos[pedido].valor());

    preco = preco.substr(0, preco.size() - 4);

    if (nome.empty()) {
        nome = "[Sem Nome]";
    }

    GtkListBoxRow* linha = gtk_list_box_get_row_at_index(GTK_LIST_BOX(lista_pedidos), pedido);
    GtkWidget* grade = gtk_bin_get_child(GTK_BIN(linha));

    GtkWidget* rotulo_nome_pedido = gtk_grid_get_child_at(GTK_GRID(grade), 3, 0);
    GtkWidget* rotulo_estado_pedido = gtk_grid_get_child_at(GTK_GRID(grade), 7, 0);
    GtkWidget* rotulo_preco_pedido = gtk_grid_get_child_at(GTK_GRID(grade), 9, 0);

    gtk_label_set_label(GTK_LABEL(rotulo_nome_pedido), nome.c_str());
    gtk_label_set_label(GTK_LABEL(rotulo_estado_pedido), estado.c_str());
    gtk_label_set_label(GTK_LABEL(rotulo_preco_pedido), preco.c_str());

    gtk_stack_set_visible_child_name(pilha_janelas, "janela_pedidos");
}

/////////////////////////////////////////////////////////////////
// ------------------- Janela de Produtos -------------------- //
/////////////////////////////////////////////////////////////////

/* @brief Adiciona um novo produto a lista de produtos
 */
void botao_adicionar_produto_clicado(void) {
    int pedido = indice_pedido;
    Produto produto("", 0.0, 0);
    unsigned indice = pedidos[pedido].produtos.tamanho();
    pedidos[pedido].produtos.inserir(produto, indice);
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
    if (gtk_stack_get_visible_child_name(pilha_janelas) != std::string("janela_inicio")) {
        sessao.salvar(pedidos);
    }

    gtk_main_quit();
}

void iniciar(int argc, char** argv) {
    // ----- inicialiando e carregando os arquivos de interface ----- //
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();

    if (gtk_builder_add_from_file(builder, "interface/estrutura/hermes.glade", &error) == 0) {
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
        "entrada_distancia_pedido_ativada", G_CALLBACK(entrada_distancia_pedido_alterada),
        "entrada_nome_pedido_perde_foco", G_CALLBACK(entrada_nome_pedido_alterada),
        "entrada_contato_pedido_perde_foco", G_CALLBACK(entrada_contato_pedido_alterada),
        "entrada_endereco_pedido_perde_foco", G_CALLBACK(entrada_endereco_pedido_alterada),
        "entrada_distancia_pedido_perde_foco", G_CALLBACK(entrada_distancia_pedido_alterada),
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
    entrada_distancia_pedido = GTK_ENTRY(gtk_builder_get_object(builder, "entrada_distancia_pedido"));

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

    inicializar_lista_arquivos();
}

/* @brief Executa o programa
 */
void executar(void) {
    gtk_widget_show_all(GTK_WIDGET(janela_principal));

    gtk_main();
}

int main(int argc, char** argv) {
    iniciar(argc, argv);

    executar();
}