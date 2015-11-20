#include <stdio.h>
#include <stdlib.h>

void imprimir_matriz_amplificada(void* aux,size_t cant_filas,size_t cant_columnas, FILE* out);

void inicializar_matriz(void* aux,size_t cant_filas,size_t cant_columnas);

void calcular_prox_fila(void* matriz, unsigned int fila, unsigned char regla, unsigned int n);

void imprimir_fila_matriz(void* aux, size_t fila_a_imprimir, size_t cant_columnas, FILE* out);