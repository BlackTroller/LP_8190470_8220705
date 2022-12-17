/* 
 * @file:   input.h
 * @author: Pedro Ribeiro
 * @date December 14, 2022, 14:36 PM
 * @brief Biblioteca de inputs
 * 
 * Contêm também as prototipagens das funções que são acedidas pelo ficheiro main.c
 */

#ifndef INPUT_H
#define INPUT_H

int getInt(int minValue, int maxValue, char msg);
float getFloat(float minValue, float maxValue, char msg);
void getString(char string, unsigned int size, char msg);
void changeIntValueArray(int type, int minValue, int maxValue, int arr);
void changeFloatValueArray(int type, float minValue, float maxValue, float arr);

#endif /* INPUT_H */

