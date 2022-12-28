/**
 * 
 * @file pricetables.c
 * @author Diogo Miranda
 * @date December 26, 2022
 * @brief Funcionalidades da tabela de preços
 * 
 * Este ficheiro contém todas as estruturas, funções de CRUD,
 * persistência de dados e listagens referentes às tabelas de preço. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pricetables.h"
#include "strings.h"
#include "input.h"

/**
 * Cria a tabela de preços com as linhas de valores recebidos
 * @param priceTable linha de valores
 * @param priceTables tabela de preços final
 */
void createPriceTables(PriceTables *priceTable, PriceTables *priceTables) {
    priceTables->priceTables[priceTables->counter] = priceTable;
    priceTables->counter++;
}

/**
 * Carrega o ficheiro .csv da tabela de preços e cada linha é lida.
 * Cada linha vai ser cortada quando for encontrado o delimitador (,) e o valor
 * será armazenada na respetiva variável que irá formar a tabela de preço para
 * o respetivo tipo de calçado.
 * @param priceTables tabela de preços
 * @param file ficheiro de onde irá ler as informações
 */
void loadPriceTables(PriceTables *priceTables, char *file) {

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    char line[1024];
    char *sp;
    int skipFirstLine = 0;
    while (fgets(line, 100, fp) != NULL) {

        PriceTable tempPriceTable;

        if (skipFirstLine == 0) {
            skipFirstLine = 1;
            continue;
        }

        sp = strtok(line, ",");
        strcpy(tempPriceTable.name, sp);

        sp = strtok(NULL, ",");
        tempPriceTable.laborTax = atoi(sp);

        sp = strtok(NULL, ",");
        tempPriceTable.fixedCosts = atoi(sp);

        for (int i = 0; i <= SIZES_SIZE; i++) {
            sp = strtok(NULL, ",");
            tempPriceTable.sizes[i] = atof(sp);
        }

        sp = strtok(NULL, ",");
        tempPriceTable.profitMargin = atof(sp);

        createPriceTables(tempPriceTable, priceTables);
    }
    fclose(fp);
    puts(MSG_SUCESSFUL_LOADING);
}

/**
 * Guarda a tabela de preços para o respetivo ficheiro
 * @param priceTables
 * @param file
 */
void savePriceTables(PriceTables priceTables, char *file) {

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }

    char line[1024];
    fgets(line, 100, fp);

    fclose(fp);

    FILE *fp2 = fopen(file, "w");
    fprintf(fp2, "%s", line);


    for (int i = 0; i < MAX_SIZE_TYPES; i++) {
        fprintf(fp2, "%s,%d,%d,", priceTables.priceTables[i].name,
                priceTables.priceTables[i].laborTax,
                priceTables.priceTables[i].fixedCosts);

        for (int j = 0; j <= SIZES_SIZE; j++) {
            fprintf(fp2, "%.2f,", priceTables.priceTables[i].sizes[j]);
        }

        fprintf(fp2, "%.2f", priceTables.priceTables[i].profitMargin);
        fprintf(fp2, "\n");

    }
    fclose(fp2);
    puts(MSG_SUCCESSFUL_SAVE);
}

/**
 * Atualiza o valor da taxa de mão de obra para o respectivo tipo de calçado
 * @param priceTables tabela de preços
 * @param type tipo de calçado
 */
void updateLaborTax(PriceTables *priceTables, int type) {
    priceTables->priceTables[type].laborTax =
            getInt(MIN_VALUE_TAXES, MAX_VALUE_TAXES, MSG_GET_LABOR_TAX);
    puts(UPDATE_SUCESSFUL);
}

/**
 * Atualiza o valor dos custos fixos para o respectivo tipo de calçado
 * @param priceTables tabela de preços
 * @param type tipo de calçado
 */
void updateFixedCosts(PriceTables *priceTables, int type) {
    priceTables->priceTables[type].fixedCosts =
            getInt(MIN_VALUE_TAXES, MAX_VALUE_TAXES, MSG_GET_FIXED_COSTS);
    puts(UPDATE_SUCESSFUL);
}

/**
 * Atualiza o valor da margem de lucro para o respectivo tipo de calçado
 * @param priceTables tabela de preços
 * @param type tipo de calçado
 */
void updateProfitMargin(PriceTables *priceTables, int type) {
    priceTables->priceTables[type].profitMargin =
            getFloat(MIN_VALUE_PROFIT_MARGIN, MAX_VALUE_PROFIT_MARGIN,
            MSG_GET_PROFIT_MARGIN);
    puts(UPDATE_SUCESSFUL);
}

/**
 * Atualiza o valor do tamanho para o respectivo tipo de calçado
 * @param priceTables tabela de preços
 * @param type tipo de calçado
 * @param sizePosition posição no array do tamanho escolhido
 */
void updateSize(PriceTables *priceTables, int type, int sizePosition) {
    priceTables->priceTables[type].sizes[sizePosition] =
            getFloat(MIN_SIZE_PERCENTAGE_VALUE, MAX_SIZE_PERCENTAGE_VALUE,
            MSG_GET_SIZE_PERCENTAGE_VALUE);
    puts(UPDATE_SUCESSFUL);
}

/**
 * Imprime a tabela de preços
 * @param priceTables tabela de preços
 */
void printPriceTables(PriceTables priceTables) {
    // Headers
    for (int i = 0; i < MAX_SIZE_TYPES; i++) {
        printf("\n%s %d€ %d€ %.2f%% ",
                priceTables.priceTables[i].name,
                priceTables.priceTables[i].laborTax,
                priceTables.priceTables[i].fixedCosts,
                priceTables.priceTables[i].profitMargin);
        
        for (int j = 0; j <= SIZES_SIZE; j++) {
            printf("%.2f%% ", priceTables.priceTables[i].sizes[j]);
        }
    }
}

/**
 * Imprime o menu para a gestão da tabela de preços
 * @return a opção escolhida pelo utilizador
 */
int printPriceTablesMenu() {
    printf("\n--- Gestão da Tabela de Preços ---");
    printf("\n1. - Atualizar");
    printf("\n2. - Listar");
    printf("\n3. - Carregar");
    printf("\n4. - Guardar");
    printf("\n0 - Sair");
    return getInt(MIN_MAIN_MENU, MAX_MAIN_MENU, MSG_GET_OPTION);
}



/**
 * Imprime o menu do tipo de calçados
 * @param priceTables tabela de preços
 * @return o valor escolhido pelo utilizador
 */
int printTypeMenu(PriceTables priceTables) {
    for (int i = 0; i < MAX_SIZE_TYPES; i++) {
        printf("\n%d. %s", i + 1, priceTables.priceTables[i].name);
    }

    return getInt(MIN_TYPE_MENU, MAX_TYPE_MENU, MSG_GET_OPTION) - 1;
}

/**
 * Imprime o menu dos valores que o utilizador pode alterar da tabela
 * @return o valor escolhido pelo utilizador
 */
int printChangeValuesMenu() {
    printf("\n1. Taxas de mão de obra (peça).");
    printf("\n2. Taxas de custos fixos (par).");
    printf("\n3. Tamanhos");
    printf("\n4. Margens de lucro.");
    printf("\n0. Sair.");
    return getInt(MIN_MENU_OPTIONS, MAX_MENU_OPTIONS, MSG_GET_OPTION);
}


