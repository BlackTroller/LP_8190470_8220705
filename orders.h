/**
 * @file:   orders.h
 * @author: Diogo Miranda
 * @date January 13, 2022, 12:16 AM
 * @brief Estruturas, constantes e prototipagem de funções
 * 
 * Este ficheiro contêm todas as estruturas e constantes para o funcionamento do registo de encomendas.
 * Contêm também as prototipagens das funções que são acedidas pelo ficheiro main.c 
 */

#ifndef ORDERS_H
#define ORDERS_H

#include "articles.h"
#include "pricetables.h"

// CONSTANTS
#define ITENS_INITIAL_SIZE 2
#define ORDERS_INITIAL_SIZE 2

#define CLIENT_MENU_MIN_OPTION 0
#define CLIENT_MENU_MAX_OPTION 2
#define BUY_MENU_MIN_OPTION 0
#define BUY_MENU_MAX_OPTION 1

#define ORDER_MIN_QUANTITY 1
#define ORDER_MAX_QUANTITY 9999
#define MSG_GET_QUANTITY "Insira a quantidade que deseja: "

#define ORDERS_RESUME_DB_FILE  "orders_resume.txt"

typedef struct {
    int articleCode, size, quantity;
    float price;
} Item;

typedef struct {
    Item itens[99];
    float totalPrice;
    int clientCode, counter;
} Order;

typedef struct {
    int counter, size;
    Order *orders;
    int isRemoved;
} Orders;


void setupOrders(Orders *orders);
void freeOrders(Orders *orders);
void saveOrders(Orders orders, char *file);
int loadOrders(Orders *orders, char *file);
void deleteOrders(Orders *orders);


// FUNCTIONS
Item createItem(int code, int position, Articles articles, PriceTables priceTables);



void createOrder(Item item, Order *order);
void addFinalInfoToOrder(int clientCode, float price, Order *order);
float getTotalPriceOrder(Order order);
void printOrder(Order order);

void createOrders(Order order, Orders *orders);
void printOrdersResume(Orders orders);
void printAllOrders(Orders orders);

// FILES
void saveOrdersResume(Orders orders, char *file);

// MENUS
int printClientMenu();
int printBuyMenu();

#endif /* ORDERS_H */
