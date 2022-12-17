/**
 * 
 * @file input.c
 * @author Pedro Ribeiro
 * @date December 14 2022
 * @brief Biblioteca de inputs
 * 
 * Biblioteca para leitura de dados do standard input.
 * Tipos suportados: Inteiro, Float, String
 *  
 */
#include <stdio.h>
#include <string.h>

#include "strings.h"

#define INVALID_VALUE "O valor inserido é inválido."

/**
 * Limpa o buffer do teclado
 */
void cleanInputBuffer() {
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

/**
 * Obtem um número inteiro, face aos parametros
 * @param minValue Mínimo valor, que o número inteiro pode ter
 * @param maxValue Máximo valor, que o número inteiro pode ter
 * @param msg Mensagem para pedir o número inteiro
 * @return o valor do inteiro lido
 */
int getInt(int minValue, int maxValue, char *msg) {
    int value;
    printf("\n%s", msg);
    while (scanf("%d", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf("%s", msg);
    }
    cleanInputBuffer();
    return value;
}

/**
 * Obtem um float, face aos parâmetros
 * @param minValue Mínimo valor, que o float pode ter
 * @param maxValue Máximo valor, que o float pode ter
 * @param msg Mensagem para pedir o float
 * @return o valor do float lido
 */
float getFloat(float minValue, float maxValue, char *msg) {
    float value;
    printf("\n%s", msg);
    while (scanf("%f", &value) != 1 || value < minValue || value > maxValue) {
        puts(INVALID_VALUE);
        cleanInputBuffer();
        printf("%s", msg);
    }
    cleanInputBuffer();
    return value;
}

/**
 * Obtem uma string.
 * @param string Ponteiro da string para onde irá armazenar
 * @param size Tamanho máximo da string
 * @param msg Mensagem para pedir a string
 */
void getString(char *string, unsigned int size, char *msg) {
    printf("\n%s", msg);
    if (fgets(string, size, stdin) != NULL) {
        unsigned int len = strlen(string) - 1;
        if (string[len] == '\n') {
            string[len] = '\0';
        } else {
            cleanInputBuffer();
        }
    }
}

/**
 * Trocar o valor de um array de inteiros
 * @param type tipo 
 * @param minValue Mínimo valor, que o número inteiro pode ter
 * @param maxValue Máximo valor, que o número inteiro pode ter
 * @param arr ponteiro do array a mudar o valor
 */
void changeIntValueArray(int type, int minValue, int maxValue, int *arr) {
    printf("\n%s%d€", MSG_CURRENT_VALUE, arr[type]);
    arr[type] = getInt(minValue, maxValue, MSG_GET_NEW_VALUE);
    puts(UPDATE_SUCESSFUL);
}

/**
 * Trocar o valor de um array de floats
 * @param type tipo 
 * @param minValue Mínimo valor, que o float pode ter
 * @param maxValue Máximo valor, que o float pode ter
 * @param arr ponteiro do array a mudar o valor
 */
void changeFloatValueArray(int type, float minValue, float maxValue, float *arr) {
    printf("\n%s%.2f%%", MSG_CURRENT_VALUE, arr[type]);
    arr[type] = getFloat(minValue, maxValue, MSG_GET_NEW_VALUE);
    puts(UPDATE_SUCESSFUL);
}








