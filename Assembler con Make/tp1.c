#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automata.h"
#include "parametros.h"
#include "proximo.h"

int main(int argc, char** argv) {
	struct parametros_t parametros;
	unsigned int cant_celdas;
	unsigned char regla;
	char* nombre_arch_entrada;
	char* nombre_arch_salida;
	FILE* archivo_entrada;
	FILE* archivo_salida;

	if (cargar_parametros(&parametros, argc, argv)) return 1;
	if (validar_parametros(&parametros)) return 1;
	if (parametros.informacion) return 0;

	// Obtengo los parametros de entrada.
	regla = atoi(parametros.regla);
	cant_celdas = atoi(parametros.celdas);
	nombre_arch_entrada = parametros.arch_entrada;
	nombre_arch_salida = parametros.arch_salida;

	// Creo e inicializo la matriz
	//unsigned char matriz [cant_celdas][cant_celdas];
	unsigned char *matriz ;
	matriz = (unsigned char *)malloc(cant_celdas*cant_celdas*sizeof(unsigned char));
	//inicializar_matriz(matriz,cant_celdas,cant_celdas);

	//Escribo el estado inicial en la primera fila de la matriz
	unsigned int i = 0;
	archivo_entrada = fopen(nombre_arch_entrada, "r");
	//while (!feof(archivo_entrada)) {
	//	fscanf(archivo_entrada, "%1d", &(matriz[0][i]));
	//	i++;
	//}
        for(i=0;i<(cant_celdas*cant_celdas);i++){
                matriz[i]=(unsigned char)0;
	}
        for(i=0;i<cant_celdas;i++){
                fscanf(archivo_entrada, "%c", &(matriz[i]));
                matriz[i]-=(unsigned char)48;
	}
	fclose(archivo_entrada);

	//Se popula la matriz segun la regla dada.
	unsigned int fila;
	for (fila = 0; fila < (cant_celdas - 1); fila++){
		calcular_prox_fila(matriz, fila, regla, cant_celdas);
	}

	char* arch_salida_pbm;
	arch_salida_pbm = strcat(nombre_arch_salida, ".pbm");

	archivo_salida = fopen(arch_salida_pbm, "wb");
	fprintf(archivo_salida, "P1\n");
	fprintf(archivo_salida, "# Esto es una matriz completa\n");
	fprintf(archivo_salida, "%d %d\n",cant_celdas*4,cant_celdas*4);
	imprimir_matriz_amplificada(matriz,cant_celdas,cant_celdas,archivo_salida);
	fclose(archivo_salida);
        free(matriz);
	return 0;
}
