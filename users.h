/* 
 * @file:   users.h
 * @author: Pedro Ribeiro
 * @date December 15, 2022, 3:27 PM
 * @brief Estruturas, constantes e prototipagem de funções
 * 
 * Este ficheiro contêm todas as estruturas e constantes para o funcionamento do registo de clientes.
 * Contêm também as prototipagens das funções que sao acedidas pelo ficheiro main.c
 */

#ifndef USERS_H
#define USERS_H

#define Users_DB_FILE "users.bin"

// STRINGS
#define ERROR_USER_DOESNT_EXISTS "Esse Utilizador não existe."
#define ERROR_USER_ALREADY_EXISTS "Esse Utilizador já existe."
#define ERROR_USER_HAS_BEEN_DELETED "O utilizador já foi apagado anteriormente"

//CONSTANTS
#define USERS_INITIAL_SIZE 5

#define MIN_COD_USER 0
#define MAX_COD_USER 999999
#define MSG_GET_USER_CODE "Insira o código de utilizador [0 - 999999]: "

#define MAX_USER_NAME 31
#define MSG_GET_USER_NAME "Insira o nome do utilizador: "

#define MAX_USER_ADDRESS 50
#define MSG_GET_USER_ADDRESS "Insira a morada do utilizador: "

#define MSG_GET_USER_NIF "Insira o NIF do utilizador: "
#define MAX_SIZE_NIF 2147483647
#define MIN_SIZE_NIF 1

#define MAX_COUNTRY_NAME 46
#define MSG_GET_USER_COUNTRY "Insira o país do utilizador: "

#define ADMIN_MENU_MIN_OPTION 0
#define ADMIN_MENU_MAX_OPTION 4

// TYPEDEF Estrutura para o modelo do cliente

typedef struct{
    int code;
    char name[MAX_USER_NAME];
    char address[MAX_USER_ADDRESS];
    int nif;
    char country[MAX_COUNTRY_NAME];
    int isRemoved; //0 - False, 1 - TRUE
    int totalOrders;
} User;

typedef struct{
    int counter, size;
    User *users;
} Users;


//FUNCTIONS
//CRUD
void createUsers(Users *users);
void searchUsers(Users users);
int searchUser(Users users, int code);
void printUsers(Users *users);
void updateUsers(Users *users);
void deleteUsers(Users *users);

int numberActiveUsers(Users users);
int numberDeletedUsers(Users users);
void incrementUserTotalOrders(User *user);

//FILE
void setupUsers(Users *users);
void saveUsers(Users users, char *file);
int loadUsers(Users *users, char *file);

//DYNAMIC MEMORY
void freeUsers(Users *users);

//MENU
void printUsersMenu();

#endif /* USERS_H */

