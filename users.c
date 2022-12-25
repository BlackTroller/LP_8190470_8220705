/**
 * 
 * @file users.c
 * @author Pedro Ribeiro
 * @date December 15, 2022
 * @brief Funcionalidades da gestão de clientes
 * 
 * Este ficheiro contém todas as estruturas, funções de CRUD, memória dinâmica,
 * persistência de dados e listagens referentes aos clientes. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strings.h"
#include "users.h"
#include "input.h"

/**
 * Esta função inicializa o array (users) com memória dinâmica da estrutura Users
 * @param users ponteiro da variável tipo Users
 */

void setupUsers(Users *users) {
    users->users = (User*) malloc(USERS_INITIAL_SIZE * sizeof (User));
    users->counter = 0;
    users->size = USERS_INITIAL_SIZE;
}

/**
 * Esta função guarda para um ficheiro binário o contador atual
 * e o array de clientes da variável Users.
 * @param users variável do tipo users
 * @param file ponteiro da variável com o nome e tipo do ficheiro a serem 
 * escritos os dados
 */
void saveUsers(Users users, char *file) {
    FILE *fp = fopen(file, "wb");
    if (fp == NULL) {
        exit(EXIT_FAILURE);
    }
    fwrite(&users.counter, sizeof (int), 1, fp);
    for (int i = 0; i < users.counter; i++) {
        fwrite(&users.users[i], sizeof (User), 1, fp);
    }
    fclose(fp);
    puts(MSG_SUCCESSFUL_SAVE);
}

/**
 * Esta função carrega para a memória os dados dos clientes guardados no ficheiro.
 * @param users ponteiro da variável do tipo users para atualizar os valores
 * @param file ponteiro da variável com o nome e tipo do ficheiro a serem 
 * escritos os dados
 * @return 1 - caso conseguiu carregar os dados, 0 - caso falhou ao carregar 
 * os dados
 */
int loadUsers(Users *users, char *file) {
    int successful = 0;

    FILE *fp = fopen(file, "rb");
    if (fp != NULL) {

        fread(&users->counter, sizeof (int), 1, fp);

        if (users->counter > 0) {
            users->users = (User*) malloc(users->counter * sizeof (User));
            fread(users->users, sizeof (User), users->counter, fp);

            users->size = users->counter;

            successful = 1;
            puts(MSG_SUCESSFUL_LOADING);
        }
        fclose(fp);
    }

    if (!successful) {
        fp = fopen(file, "wb");
        if (fp != NULL) {
            setupUsers(users);
            fclose(fp);
            printf(MSG_NEW_FILE_CREATED);
            successful = 1;
        }
    }

    return successful;
}

/**
 * Esta função imprime as informações referentes a um cliente
 * @param user variável do tipo user
 */
void printUser(User user) {
    printf("\nCódigo: %d", user.code);
    printf("\nNome: %s", user.name);
    printf("\nNome: %s", user.address);
    printf("\nNIF: %d", user.nif);
    printf("\nPaís: %s", user.country);
    printf("\nNº total de encomendas: %d", user.totalOrders);
    printf("\nApagado?: %d", user.isRemoved);
    puts("\n");
}

/**
 * Esta função irá imprimir todos os clientes, utilizando a função printUser.
 * Caso a lista não tenha nenhum cliente em memória, é apresentada uma mensagem.
 * @param users variável do tipo users 
 */
void printUsers(Users users) {
    if (users.counter > 0) {
        int i;
        for (i = 0; i < users.counter; i++) {
            puts("");
            printUser(users.users[i]);
        }
    } else {
        puts("\n"ERROR_LIST_EMPTY);
    }
}

/**
 * Esta função irá atualizar o valor do campo isRemoved para 1 (true), 
 * mostrando assim que o cliente foi removido, mas mantendo as informações
 * referentes ao mesmo. 
 * @param user ponteiro da variável do tipo user
 */
void deleteUser(User *user) {
    user->isRemoved = 1;
    printf(DELETE_SUCESSFUL);
    puts("\n");
}

/**
 * 
 * @param users variável do tipo users 
 * @param code valor do código do cliente
 * @return Caso o cliente seja encontrado, retorna o valor da posição do cliente,
 * caso contrário retorna -1
 */
int searchUser(Users users, int code) {
    for (int i = 0; i < users.counter; i++) {
        if (users.users[i].code == code) {
            return i;
        }
    }
    return -1;
}

/**
 * Esta função irá criar um cliente com a sua respectiva informação (cod_user, username, morada, nif, pais)
 * @param users ponteiro da variável do tipo users
 * @return o novo valor do contador da variável do tipo users
 */
int createUser(Users *users) {
    puts("\n");
    int code = getInt(MIN_COD_USER, MAX_COD_USER, MSG_GET_USER_CODE);

    if (searchUser(*users, code) == -1) {
        users->users[users->counter].code = code;

        getString(users->users[users->counter].name, MAX_USER_NAME,
                MSG_GET_USER_NAME);
        
        getString(users->users[users->counter].address, MAX_USER_ADDRESS,
                MSG_GET_USER_ADDRESS);

        users->users[users->counter].nif = getInt(MIN_SIZE_NIF, MAX_SIZE_NIF, MSG_GET_USER_NIF);

        getString(users->users[users->counter].country, MAX_COUNTRY_NAME,
                MSG_GET_USER_COUNTRY);

        users->users[users->counter].isRemoved = 0;
        users->users[users->counter].totalOrders = 0;

        printf(CREATE_SUCESSFUL);
        puts("\n");

        return users->counter++;
    }
    return -1;
}

/**
 * Esta função irá atualizar os valores atuais de um respetivo cliente face
 * aos novos valores inseridos.
 * @param user ponteiro da variável do tipo user
 */
void updateUser(User *user) {
    puts("\n");
    getString(user->name, MAX_USER_NAME, MSG_GET_USER_NAME);
    getString(user->name, MAX_USER_ADDRESS, MSG_GET_USER_ADDRESS);
    user->nif = getInt(MIN_SIZE_NIF, MAX_SIZE_NIF, MSG_GET_USER_NIF);
    getString(user->country, MAX_COUNTRY_NAME, MSG_GET_USER_COUNTRY);
    printf(UPDATE_SUCESSFUL);
    puts("\n");
}


/**
 * Esta função irá aumentar dinâmicamente o tamanho do array (users)
 * da variável do tipo users
 * @param users ponteiro da variável do tipo users
 */
void expandUsers(Users *users) {
    User *temp = (User*) realloc(users->users, sizeof (User) * (users->size * 2));
    if (temp != NULL) {

        users->size *= 2;
        users->users = temp;
    }
}

/**
 * Esta função começa por verificar se há espaço para inserir um novo cliente.
 * Caso não tenha, chama a função para expandir o tamanho do array,
 * para possibilitar a criação do cliente.
 * Em seguida, é chamada a função de criar um cliente,
 * que irá retornar ou o valor da novo posição do cliente ou
 * -1 caso o cliente já exista.
 * @param users ponteiro da variável do tipo users
 */
void createUsers(Users *users) {

    if (users->counter == users->size) {
        expandUsers(users);
    }

    if (users->counter < users->size) {
        if (createUser(users) == -1) {
            puts(ERROR_USER_ALREADY_EXISTS);
        }
    } else {
        puts(ERROR_LIST_FULL);
    }
}


/**
 * Esta função verifica face ao código do cliente inserido pelo utilizador, 
 * se o utilizador existe ou não.
 * Caso exista, é chamada a função de imprimir informações sobre o utilizador.
 * @param users variável do tipo users
 */
void searchUsers(Users users) {
    int tempCode, code;
    tempCode = getInt(MIN_COD_USER, MAX_COD_USER, MSG_GET_USER_CODE);
    code = searchUser(users, tempCode);
    if (code != -1) {
        printUser(users.users[code]);
    } else {

        puts(ERROR_USER_ALREADY_EXISTS);
    }
}

/**
 * Esta função verifica face ao código do cliente inserido pelo utilizador, 
 * se o utilizador existe ou não.
 * Caso exista, é chamada a função de atualizar cliente.
 * @param users ponteiro da variável do tipo users
 */
void updateUsers(Users *users) {
    int tempCode, code;
    tempCode = getInt(MIN_COD_USER, MAX_COD_USER, MSG_GET_USER_CODE);
    ;
    code = searchUser(*users, tempCode);
    if (code != -1) {
        updateUser(&users->users[code]);
    } else {

        puts(ERROR_USER_DOESNT_EXISTS);
    }
}

/**
 * Esta função verifica face ao código do cliente inserido pelo utilizador, 
 * se o utilizador existe ou não.
 * Caso exista, é chamada a função de apagar o cliente.
 * @param users ponteiro da variável do tipo users
 */
void deleteUsers(Users *users) {
    int tempCode, tempCounter;
    tempCode = getInt(MIN_COD_USER, MAX_COD_USER, MSG_GET_USER_CODE);
    tempCounter = searchUser(*users, tempCode);
    if (tempCounter != -1) {
        if (users->users[tempCounter].isRemoved == 0) {
            deleteUser(&users->users[tempCounter]);
        } else {
            puts(ERROR_USER_HAS_BEEN_DELETED);
        }
    } else {

        puts(ERROR_USER_DOESNT_EXISTS);
    }
}

/**
 * Incrementa +1 ao valor de encomendas do respetivo cliente
 * @param user ponteiro da variável do tipo user
 */
void incrementUserTotalOrders(User *user) {

    user->totalOrders += 1;
};

/**
 * Procura os clientes que estão ativos
 * @param users
 * @return a quantidade de clientes ativos
 */
int numberActiveUsers(Users users) {
    int activeUsers = 0;
    for (int i = 0; i < users.counter; i++) {
        if (users.users[i].isRemoved == 0) {

            activeUsers++;
        }
    }
    return activeUsers;
}


/**
 * Procura os clientes que foram removidos
 * @param users variável do tipo users
 * @return a quantidade de clientes removidos
 */
int numberDeletedUsers(Users users) {
    int deletedUsers = 0;
    for (int i = 0; i < users.counter; i++) {
        if (users.users[i].isRemoved == 1) {

            deletedUsers++;
        }
    }
    return deletedUsers;
}



/**
 * Imprime o menu da gestão de utilizadores
 */
void printUsersMenu() {

    printf("\n--- Gestão de Utilizadores ---");
    printf("\n1. - Inserir");
    printf("\n2. - Procurar");
    printf("\n3. - Atualizar");
    printf("\n4. - Remover");
    printf("\n5. - Listar");
    printf("\n6. - Guardar");
    printf("\n7. - Carregar");
    printf("\n0 - Sair");
}

/**
 * Liberta o bloco de memória apontado por users
 * @param users ponteiro da variável do tipo users
 */
void freeUsers(Users *users) {
    free(users->users);
}