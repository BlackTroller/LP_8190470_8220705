/* 
 * @File:   input.h
 * @Author: pedro
 * @date December 14, 2022, 14:36 PM
 * @brief Biblioteca de inputs
 * 
 * Contêm também as prototipagens das funções que são acedidas pelo ficheiro main.c
 */

#ifndef INPUT_H
#define INPUT_H

int obterInt(int minValor, int maxValor, char *msg);

float obterFloat(float minValor, float maxValor, char *msg);

double obterDouble(double minValor, double maxValor, char *msg);

char obterChar(char *msg);

void lerString(char *string, unsigned int tamanho, char *msg);

#endif /* INPUT_H */

