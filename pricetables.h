/**
 * @file:   pricetables.h
 * @author: Diogo Miranda.
 * @date January 13, 2022, 12:16 AM
 * @brief Estruturas, constantes e prototipagem de funções
 * 
 * Este ficheiro contêm todas as estruturas e constantes para o funcionamento da gestão da tabela de preços.
 * Contêm também as prototipagens das funções que são acedidas pelo ficheiro main.c 
 */
#ifndef PRICETABLES_H
#define PRICETABLES_H


// MENUS
#define MIN_MAIN_MENU 0
#define MAX_MAIN_MENU 4

#define MIN_TYPE_MENU 0
#define MAX_TYPE_MENU 3

#define MIN_MENU_OPTIONS 0
#define MAX_MENU_OPTIONS 3

#define MAX_SIZE_TYPES 3

#define MAX_VALUE_TAXES 99
#define MIN_VALUE_TAXES 0

// LABOR TAXES €
#define MSG_GET_LABOR_TAX "Insira o novo preço de mão de obra por peça (MAX 99€): "
#define LABOR_TAXES_SIZE 3

// FIXED COSTS €
#define FIXED_COSTS_SIZE 3
#define MSG_GET_FIXED_COSTS "Insira o novo preço de custos fixos por par (MAX: 99€): "

// PROFIT MARGINS %
#define PROFIT_MARGIN_SIZE 3
#define MIN_VALUE_PROFIT_MARGIN 0
#define MAX_VALUE_PROFIT_MARGIN 1
#define MSG_GET_PROFIT_MARGIN "Insira a nova porcentagem para a margem de lucro (MAX: 1): "

#define PRICE_TABLE_DB_FILE "prices.csv"

#define MAX_NAME_SIZE 31
#define SIZES_SIZE 14
#define MIN_SIZE_PERCENTAGE_VALUE 0.50
#define MAX_SIZE_PERCENTAGE_VALUE 1.99
#define MSG_GET_SIZE_PERCENTAGE_VALUE "Insira a nova porcentagem para o tamanho (MIN: 0.5, MAX: 1.99)"


#define MIN_SIZE_VALUE_TABLE 34
#define MAX_SIZE_VALUE_TABLE 48

#define MSG_GET_SIZE "Escolha um tamanho entre 34-48 (1. - 34 & 14. - 48)"

typedef struct {
    char name[MAX_NAME_SIZE];
    int laborTax, fixedCosts; // piece; pair
    float sizes[SIZES_SIZE], profitMargin;
} PriceTable;

typedef struct {
    int counter, size;
    PriceTable priceTables[MAX_SIZE_TYPES];
} PriceTables;


// FUNCTIONS DECLARATION

void loadPriceTables(PriceTables *priceTables, char *file);
void savePriceTables(PriceTables priceTables, char *file);

// Updates like java sets
void updateLaborTax(PriceTables *priceTables, int type);
void updateFixedCosts(PriceTables *priceTables, int type);
void updateProfitMargin(PriceTables *priceTables, int type);
void updateSize(PriceTables *priceTables, int type, int sizePosition);
void printPriceTables(PriceTables priceTables);

// MENUS
int printPriceTablesMenu();
int printChangeValuesMenu();
int printTypeMenu(PriceTables priceTables);

#endif /* PRICETABLES_H */

