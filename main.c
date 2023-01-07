/* 
 * @file:   main.c
 * @author: Pedro Miguel de Andrade Ribeiro 8190470 Diogo Henrique Ramos Miranda 8220705
 * @date December 9, 2022, 7:29 PM
 * @brief Menus do programa
 * 
 * É neste ficheiro onde serão chamas todas as funcinalidades criadas nos outros ficheiros.
 * Essas funcionalidades são acessadas através de menus, face às decisões e permissões dos utilizadores
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings.h"
#include "input.h"
#include "articles.h"
#include "pricetables.h"
#include "orders.h"
#include "users.h"

#define MIN_PROFILE_OPTION 0
#define MAX_PROFILE_OPTION 2
#define ADMIN_PASSWORD "NDadm123pw,."
#define MAX_ADM_PASSWORD 16

/*
 * 
 */
int main(int argc, char** argv) {
    
    Articles articles = {.counter = 0};
    Users users = {.counter = 0};
    PriceTables priceTables = {.counter = 0};
    Orders orders = {.counter = 0};
    
    int chooseProfileOption, listingsOption,
            adminOption, articlesOption, usersOption,
            tablePriceOption, typeOption, loadToMemoryPriceTables = 0,
            changeTableValueOption,clientOption, buyOption;
    char askAdminPW[MAX_ADM_PASSWORD];
    int checkLoadingUsers = 0;
    
    do {
        printf("\n--- Escolha de perfil ---");
        printf("\n1. Cliente");
        printf("\n2. Administrador");
        printf("\n0. Sair");
        chooseProfileOption = getInt(CLIENT_MENU_MIN_OPTION, MAX_PROFILE_OPTION,
                MSG_GET_OPTION);
        switch (chooseProfileOption) {
            case(0):
                // Exit
                break;
            case(1):
                // Client Menu
                Users users = {.counter = 0};
                Articles articles = {.counter = 0};
                PriceTables priceTables = {.counter = 0};
                loadUsers(&users, Users_DB_FILE);
                int clientCode = getInt(MIN_COD_USER, MAX_COD_USER, MSG_GET_USER_CODE);
                int clientPosition = searchUser(users, clientCode);
                int isClienteRemoved = users.users[clientPosition].isRemoved;
                if (isClienteRemoved == 0) {
                    loadArticles(&articles, ARTICLES_DB_FILE);
                    loadPriceTables(&priceTables, PRICE_TABLE_DB_FILE);
                    setupOrders(&orders);
                    do {
                        clientOption = printClientMenu();
                        switch (clientOption) {
                            case(0):
                                //Exit
                                break;
                            case(1):
                                // Orders
                                printf("\nOrders: %d/%d", orders.counter, orders.size);
                                Order order = {.counter = 0};
                                do {
                                    int code = getInt(MIN_CODE_ARTICLE, MAX_CODE_ARTICLE, MSG_GET_ARTICLE_CODE);
                                    int position = searchArticle(articles, code);
                                    if (position != -1) {
                                        Item item = createItem(code, position, articles, priceTables);
                                        setArticleSold(item.quantity, &articles.articles[position]);
                                        createOrder(item, &order);
                                        saveArticles(articles, ARTICLES_DB_FILE);
                                    } else {
                                        puts("Artigo não existe.");
                                    }
                                    buyOption = printBuyMenu();
                                } while (buyOption != 0);
                                // Orders
                                if (order.counter != 0) {
                                    incrementUserTotalOrders(&users.users[clientPosition]);
                                    float totalPrice = getTotalPriceOrder(order);
                                    addFinalInfoToOrder(clientCode, totalPrice, &order);
                                    createOrders(order, &orders);
                                    printOrdersResume(orders);
                                    saveUsers(users, Users_DB_FILE);
                                    saveOrdersResume(orders, ORDERS_RESUME_DB_FILE);
                                } else {
                                    printf("Encomenda cancelada.");
                                }
                                break;
                            case(2):
                                printArticles(articles);
                                break;
                            default:
                                puts(INVALID_OPTION);
                        }
                    } while (clientOption != 0);
                } else {
                    puts("\nCliente não encontrado ou removido.");
                }
                freeOrders(&orders);
                break;
                case(2):
                // Admin Menu

                getString(askAdminPW, MAX_ADM_PASSWORD, MSG_GET_PW);

                if (strcmp(ADMIN_PASSWORD, askAdminPW) == 0) {
                    do {
                        puts("\n1. - Artigos");
                        puts("2. - Utilizadores");
                        puts("3. - Tabela de preços");
                        puts("4. - Listagens");
                        puts("0. - Sair");
                        adminOption = getInt(ADMIN_MENU_MIN_OPTION,
                                ADMIN_MENU_MAX_OPTION,
                                MSG_GET_OPTION);
                        switch (adminOption) {
                            case 0:
                                // Exit
                                break;
                            case 1:
                                // Articles Menu Admin
                                setupArticles(&articles);
                                do {
                                    printArticlesMenu();
                                    printf("\nArtigos: %d/%d", articles.counter, articles.size);
                                    printf("\nOpção: ");
                                    scanf("%d", &articlesOption);

                                    switch (articlesOption) {
                                        case 0:
                                            // Exit
                                            break;
                                        case 1:
                                            createArticles(&articles);
                                            break;
                                        case 2:
                                            searchArticles(articles);
                                            break;
                                        case 3:
                                            updateArticles(&articles);
                                            break;
                                        case 4:
                                            deleteArticles(&articles);
                                            break;
                                        case 5:
                                            printArticles(articles);
                                            break;
                                        case 6:
                                            saveArticles(articles, ARTICLES_DB_FILE);
                                            break;
                                        case 7:
                                            loadArticles(&articles, ARTICLES_DB_FILE);
                                            break;
                                        default:
                                            puts(INVALID_OPTION);
                                    }
                                } while (articlesOption != 0);
                                freeArticles(&articles);
                                break;
                            case 2:
                                // Users Menu Admin
                                setupUsers(&users);
                                do {
                                    printUsersMenu();
                                    printf("\nUtilizadores: %d/%d", users.counter, users.size);
                                    printf("\nOpção: ");
                                    scanf("%d", &usersOption);
                                    switch (usersOption) {
                                        case 0:
                                            // Exit
                                            break;
                                        case 1:
                                            createUsers(&users);
                                            break;
                                        case 2:
                                            searchUsers(users);
                                            break;
                                        case 3:
                                            updateUsers(&users);
                                            break;
                                        case 4:
                                            deleteUsers(&users);
                                            break;
                                        case 5:
                                            printUsers(users);
                                            break;
                                        case 6:
                                            saveUsers(users, Users_DB_FILE);
                                            break;
                                        case 7:
                                            loadUsers(&users, Users_DB_FILE);
                                            break;
                                        default:
                                            puts(INVALID_OPTION);
                                    }
                                } while (usersOption != 0);
                                freeUsers(&users);
                                break;
                            case 3:
                                // Table Prices
                                if (loadToMemoryPriceTables == 0) {
                                    loadPriceTables(&priceTables, PRICE_TABLE_DB_FILE);
                                    loadToMemoryPriceTables = 1;
                                }
                                do {
                                    tablePriceOption = printPriceTablesMenu();
                                    switch (tablePriceOption) {
                                        case 0:
                                            // Exit
                                            break;
                                        case 1:
                                            // Update
                                            typeOption = printTypeMenu(priceTables);
                                            changeTableValueOption = printChangeValuesMenu();
                                            switch (changeTableValueOption) {
                                                case(1):
                                                    // LABOR TAX
                                                    updateLaborTax(&priceTables, typeOption);
                                                    break;
                                                case(2):
                                                    // Fixed Costs
                                                    updateFixedCosts(&priceTables, typeOption);
                                                    break;
                                                case(3):
                                                    // Sizes Menu
                                                    sizeOption = getInt(MIN_MAIN_MENU,
                                                            SIZES_SIZE, MSG_GET_SIZE);
                                                    updateSize(&priceTables, typeOption,
                                                            sizeOption);
                                                    break;
                                                case(4):
                                                    // Profit Margin
                                                    updateProfitMargin(&priceTables, typeOption);
                                                    break;
                                                default:
                                                    puts(INVALID_OPTION);
                                            }
                                            break;
                                        case 2:
                                            printPriceTables(priceTables);
                                            break;
                                        case 3:
                                            loadPriceTables(&priceTables, PRICE_TABLE_DB_FILE);
                                            break;
                                        case 4:
                                            savePriceTables(priceTables, PRICE_TABLE_DB_FILE);
                                            break;
                                    }
                                } while (tablePriceOption != 0);
                                break;
                            case 4:
                                // Listagens
                                loadArticles(&articles, ARTICLES_DB_FILE);
                                loadUsers(&users, Users_DB_FILE);
                                do {
                                    printf("\n---Listagens Propostas --- \n");
                                    puts("1. - Cliente com mais e menos encomendas");
                                    puts("2. - Artigos mais e menos vendido");
                                    puts("3. - Número de clientes ativos e removidos");
                                    puts("4. - Número de vendas de cada tipo");
                                    puts("0. - Sair");
                                    printf("\nOpção: ");
                                    scanf("%d", &listingsOption);

                                    switch (listingsOption) {
                                        case (0):
                                            // Exit
                                            break;
                                        case (1):
                                            // 5 clientes com mais
                                            printf("\nNº de cliente com mais encomendas: %d", userWithMostOrders(users));
                                            printf("\nNº de cliente com menos encomendas: %d", userWithFewerOrders(users));
                                            printf("\n");
                                            break;
                                        case (2):
                                            // Artigo mais e menos vendido
                                            printf("\nArtigo com mais vendas: %d", getMostSoldArticle(articles));
                                            printf("\nArtigo com menos vendas: %d", getFewestSoldArticle(articles));
                                            printf("\n");
                                            break;
                                        case (3):
                                            // Número de clientes ativos e removidos
                                            printf("\nNº de clientes ativos: %d", numberActiveUsers(users));
                                            printf("\nNº de clientes removidos: %d", numberDeletedUsers(users));
                                            printf("\n");
                                            break;
                                        case (4):
                                            // Número de vendas de cada tipo
                                            for (int i = 0; i <= MAX_TYPE; i++) {
                                                switch (i) {
                                                    case(0):
                                                        printf("\nNº de vendas sapatos: %d", getSoldQuantityType(i, articles));
                                                        break;
                                                    case(1):
                                                        printf("\nNº de vendas sandálias: %d", getSoldQuantityType(i, articles));
                                                        break;
                                                    case(2):
                                                        printf("\nNº de vendas botas: %d", getSoldQuantityType(i, articles));
                                                        break;
                                                }
                                            }
                                            printf("\n");
                                            break;
                                        default:
                                            puts(INVALID_OPTION);
                                    }
                                } while (listingsOption != 0);

                                break;
                            default:
                                puts(INVALID_OPTION);
                        }

                    } while (adminOption != 0);
                } else {
                    puts("Palavra-passe inválida!");
                }
                break;
            default:
                puts(INVALID_OPTION);
        }
    } while (chooseProfileOption != 0);
    return (EXIT_SUCCESS);
}

