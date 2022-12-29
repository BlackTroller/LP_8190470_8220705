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
void createPriceTables(PriceTable priceTable, PriceTables *priceTables) {
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
        fprintf(fp2, "%s", priceTables.priceTables[i].name);
    
        fprintf(fp2, "\n");
    }
    
    fclose(fp2);
    puts(MSG_SUCCESSFUL_SAVE);
}

/**
 * Imprime a tabela de preços
 * @param priceTables tabela de preços
 */
void printPriceTables(PriceTables priceTables) {
    // Headers
    for (int i = 0; i < MAX_SIZE_TYPES; i++) {
        printf("\n%s %d€ %d€ %.2f%% ");
        priceTables.priceTables[i].name;
        
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
 * Imprime o menu do tipo 
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
    printf("\n2. Taxas de custos fixos (par).");
    printf("\n0. Sair.");
    return getInt(MIN_MENU_OPTIONS, MAX_MENU_OPTIONS, MSG_GET_OPTION);
}


