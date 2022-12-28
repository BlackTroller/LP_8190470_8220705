//preparar o crud das orders e guardar os mesmo num fichero com a sua estrutura ligado a um user

/**
 * 
 * @file orders.c
 * @author Diogo Miranda
 * @date December 26, 2022
 * @brief Funcionalidades de registo de uma encomenda
 * 
 * Este ficheiro contém todas as estruturas, funções de CRUD, memória dinâmica,
 * persistência de dados e listagens referentes às encomendas. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orders.h"
#include "strings.h"
#include "input.h"
#include "pricetables.h"

/**
 * Calcula o valor das taxas de mão de obra
 * @param type tipo do calçado
 * @param priceTables tabela de preços
 * @return O valor calculado da taxa de mão de obra 
 */
float calculateLaborTax(int type, PriceTables priceTables) {
    return priceTables.priceTables[type].laborTax * 2;
}

/**
 * Calcula o valor dos custos fixos
 * @param type tipo do calçado
 * @param priceTables tabela de preços
 * @return O valor calculado dos custos fixos
 */
float calculateFixedCosts(int type, PriceTables priceTables) {
    return priceTables.priceTables[type].fixedCosts;
}

/**
 * Calcula a porcentagem do calçado pelo seus respetivo tamanho
 * @param type tipo do calçado
 * @param size tamanho escolhido
 * @param priceTables tabela de preços
 * @return O valor calculado do tamanho
 */
float calculateSize(int type, int size, PriceTables priceTables) {
    int differenceValue = MAX_SIZE_VALUE_TABLE - MIN_SIZE_VALUE_TABLE;
    int sizePosition = size - MAX_SIZE_VALUE_TABLE + differenceValue;
    return priceTables.priceTables[type].sizes[sizePosition];
}

/**
 * Calcula o valor da margem de lucro
 * @param type tipo do caçado
 * @param priceTables tabela de preços
 * @return O valor calculado da margem de lucro
 */
float calculateProfitMargin(int type, PriceTables priceTables) {
    return (1 + priceTables.priceTables[type].profitMargin);
}

/**
 * Calcula o preço final aplicando todas as taxas
 * @param quantity quantidade de calçados que o cliente comprou
 * @param type tipo do calçado comprado pelo cliente
 * @param size tamanho do calçado comprado pelo cliente
 * @param priceTables tabela de preços
 * @return o preço final daquele pedido
 */
float calculatePrice(int quantity, int type, int size, PriceTables priceTables) {
    float price = 0;
    price = calculateLaborTax(type, priceTables);
    price += calculateFixedCosts(type, priceTables);
    price *= calculateSize(type, size, priceTables);
    price *= calculateProfitMargin(type, priceTables);
    return price * quantity;
}

/**
 * Esta função inicializa o array (orders) com memória dinâmica da estrutura Orders
 * @param orders
 */
void setupOrders(Orders *orders) {
    orders->orders = (Order*) malloc(ORDERS_INITIAL_SIZE * sizeof (Order));
    orders->counter = 0;
    orders->size = ORDERS_INITIAL_SIZE;
}

/**
 * Esta função irá aumentar dinâmicamente o tamanho do array (orders)
 * da variável do tipo orders
 * @param orders
 */
void expandOrders(Orders *orders) {
    Order *temp = (Order*) realloc(orders->orders, sizeof (Order) * (orders->size * 2));
    if (temp != NULL) {
        orders->size *= 2;
        orders->orders = temp;
    }
}

/**
 * Cria um item da encomenda faça às escolhas do cliente
 * @param code código do artigo comprado pelo cliente
 * @param position posição do artigo comprado pelo cliente
 * @param articles artigos
 * @param priceTables tabela de preços
 * @return o item criado com os respectivos valores escolhidos pelo cliente
 */
Item createItem(int code, int position, Articles articles, PriceTables priceTables) {
    Item item;
    item.articleCode = code;
    int type = articles.articles[position].type;
    int minSize = getArticleMinSize(articles.articles[position]);
    int maxSize = getArticleMaxSize(articles.articles[position]);
    printf("\nTamanho mínimo: %d", minSize);
    printf("\nTamanho máximo: %d", maxSize);
    item.size = getInt(minSize, maxSize, "Escolha o tamanho que deseja: ");
    item.quantity = getInt(ORDER_MIN_QUANTITY, ORDER_MAX_QUANTITY, MSG_GET_QUANTITY);
    item.price = calculatePrice(item.quantity, type, item.size, priceTables);
    puts("\nEncomendado com sucesso.");
    return item;
}

/**
 * Adiciona à encomenda o pedido
 * @param item Pedido do cliente
 * @param order 
 */
void createOrder(Item item, Order *order) {
    order->itens[order->counter] = item;
    order->counter++;
}

/**
 * Calcula o preço total da encomenda face a todos os itens encomendados
 * @param order Encomenda
 * @return o preço final calculado
 */
float getTotalPriceOrder(Order order) {
    float totalPrice = 0;
    for (int i = 0; i < order.counter; i++) {
        totalPrice += order.itens[i].price;
    }
    return totalPrice;
}

/**
 * Esta função adiciona os dados finais (Data, Código Cliente, Preço Final),
 * na encomenda
 * @param clientCode Código do cliente
 * @param price Preço final
 * @param order ponteiro da variável do tipo orders
 */
void addFinalInfoToOrder(int clientCode, float price, Order *order) {
    order->clientCode = clientCode;
    order->totalPrice = price;
}

/**
 * Esta função começa por verificar se há espaço para inserir uma nova encomenda.
 * Caso não tenha, chama a função para expandir o tamanho do array,
 * para possibilitar a criação da encomenda.
 * Em seguida, é criada a respetiva encomenda.s
 * @param order variável do tipo order
 * @param orders ponteiro da variável do tipo orders
 */
void createOrders(Order order, Orders *orders) {
    if (orders->counter == orders->size) {
        expandOrders(orders);
    }

    if (orders->counter < orders->size) {
        orders->orders[orders->counter] = order;
        orders->counter++;
    } else {
        puts(ERROR_LIST_FULL);
    }
}

/**
 * Imprime o resumo de uma encomenda
 * @param orders variável do tipo orders
 */
void printOrdersResume(Orders orders) {
    printf("\n--- Resumo da Encomenda ---");
    printf("\nCódigo do cliente: %d", orders.orders[orders.counter - 1].clientCode);
    printf("\nPreço total: %.2f€", orders.orders[orders.counter - 1].totalPrice);
    printf("\n-- Pedidos --");
    for (int i = 0; i < orders.orders[orders.counter - 1].counter; i++) {
        printf("\nCódigo do artigo: %d", orders.orders[orders.counter - 1].itens[i].articleCode);
        printf("\nQuantidade: %d", orders.orders[orders.counter - 1].itens[i].quantity);
        printf("\nTamanho: %d", orders.orders[orders.counter - 1].itens[i].size);
        printf("\nPreço: %.2f€", orders.orders[orders.counter - 1].itens[i].price);
        printf("\n");
    }
    printf("------------\n");
    printf("---------------------------\n");
}

/**
 * Imprime as informações de todas as encomendas em memória
 * @param orders variável do tipo orders
 */
void printAllOrders(Orders orders) {
    for (int j = 0; j < orders.counter; j++) {
        printf("\n--- Orders ---");
        printf("\nUser: %d", orders.orders[j].clientCode);
        printf("\nCódigo do cliente: %d", orders.orders[j].clientCode);
        printf("\nPreço total: %.2f€", orders.orders[j].totalPrice);
        printf("\n-- Pedidos --");
        for (int i = 0; i < orders.orders[j].counter; i++) {
            printf("\nCódigo do artigo: %d", orders.orders[j].itens[i].articleCode);
            printf("\nQuantidade: %d", orders.orders[j].itens[i].quantity);
            printf("\nTamanho: %d", orders.orders[j].itens[i].size);
            printf("\nPreço: %.2f€", orders.orders[j].itens[i].price);
            printf("\n");
        }
        printf("------------\n");
        printf("---------------------------\n");
    }

}

/**
 * Guarda para o ficheiro o resumo da encomenda
 * @param orders variável do tipo orders
 * @param file file ponteiro da variável com o nome e tipo do ficheiro a serem 
 * escritos os dados 
 */
void saveOrdersResume(Orders orders, char *file) {
    FILE *fp = fopen(ORDERS_RESUME_DB_FILE, "a");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "\nCódigo do cliente: %d", orders.orders[orders.counter - 1].clientCode);
    fprintf(fp, "\nPreço total: %.2f€", orders.orders[orders.counter - 1].totalPrice);
    fprintf(fp, "\n-- Pedidos --");
    for (int i = 0; i < orders.orders[orders.counter - 1].counter; i++) {
        fprintf(fp, "\nCódigo do artigo: %d", orders.orders[orders.counter - 1].itens[i].articleCode);
        fprintf(fp, "\nQuantidade: %d", orders.orders[orders.counter - 1].itens[i].quantity);
        fprintf(fp, "\nTamanho: %d", orders.orders[orders.counter - 1].itens[i].size);
        fprintf(fp, "\nPreço: %.2f€", orders.orders[orders.counter - 1].itens[i].price);
        fprintf(fp, "\n");
    }
    fprintf(fp, "------------\n");
    fprintf(fp, "\n");
    fclose(fp);
}

// MENUS

/**
 * Imprime o menu das opções do cliente e pede ao cliente a opção que ele 
 * deseja escolher
 * @return o valor da opção escolhida
 */
int printClientMenu() {
    printf("\n1. - Encomendar");
    printf("\n2. - Listar todos os artigos");
    printf("\n0. - Sair");
    return getInt(CLIENT_MENU_MIN_OPTION, CLIENT_MENU_MAX_OPTION,
            MSG_GET_OPTION);
}

/**
 * Imprime o menu de compra da encomenda e pede ao cliente a opção que ele
 * deseja escolher
 * @return o valor da opção escolhida
 */
int printBuyMenu() {
    printf("\n1. - Continuar encomenda");
    printf("\n0. - Finalizar encomenda");
    return getInt(BUY_MENU_MIN_OPTION, BUY_MENU_MAX_OPTION, MSG_GET_OPTION);
}

/**
 * /**
 * Liberta o bloco de memória apontado por orders
 * @param orders ponteiro da variável do tipo orders
 */
void freeOrders(Orders *orders) {
    free(orders->orders);
}