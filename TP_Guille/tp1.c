#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "automata.h"
#include "parametros.h"


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
	unsigned char matriz [cant_celdas][cant_celdas];
	inicializar_matriz(matriz,cant_celdas,cant_celdas);

	//Escribo el estado inicial en la primera fila de la matriz
	unsigned int i = 0;
	archivo_entrada = fopen(nombre_arch_entrada, "r");
	while (!feof(archivo_entrada)) {
		if(i >= cant_celdas){
			fprintf(stderr,"ERROR: la cantidad de celdas del archivo no coincide con el parametro pasado. \n");
			return 0;
		}
		int valor;
		fscanf(archivo_entrada, "%1d", &valor);
		matriz[0][i] = (unsigned char) valor;
		i++;
	}
	fclose(archivo_entrada);

	int version;
	while(1){
		printf("Ingrese un 0 para la version normal con archivos de salida, o 1 para la version por terminal: \n");
		scanf("%d",&version);
		if (version == 1 || version == 0){
			break;
		}
		fprintf(stderr,"Entrada no valida. Intentelo nuevamente. \n");
	}

	if (version == 1){ // Version de terminal: permite determinar un paso variable para completar la matriz.
		int iteraciones = 0;
		int sumatoria = 0;
		while(sumatoria < (cant_celdas - 1)){
			printf("Iteraciones realizadas hasta el momento: %d \n", sumatoria);
			printf("Ingrese la cantidad_celdas de iteraciones a realizar: \n");
			scanf("%d",&iteraciones);
			if (iteraciones < 1){
				fprintf(stderr, "Numero invalido, se realizara una iteracion. \n");
				iteraciones = 1;
			}
			if(iteraciones > (cant_celdas - 1 - sumatoria)){
				fprintf(stderr, "Se realizaran todas las iteraciones restantes. \n");
				iteraciones = cant_celdas - 1 - sumatoria;
			}
			unsigned int fila;
			for (fila = 0; fila < iteraciones; fila++){
				calcular_prox_fila(matriz, fila + sumatoria, regla, cant_celdas);
				imprimir_fila_matriz(matriz, fila + sumatoria, cant_celdas, stdout);
			}
			sumatoria += iteraciones;
		}
		return 0;
	}

	if (version == 0){ // Version normal con archivos de salida.
		//Se popula la matriz segun la regla dada.
		unsigned int fila;
		for (fila = 0; fila < (cant_celdas - 1); fila++){
			calcular_prox_fila(matriz, fila, regla, cant_celdas);
		}

		char* arch_salida_pbm = strcat(nombre_arch_salida, ".pbm");

		archivo_salida = fopen(arch_salida_pbm, "wb");
		fprintf(archivo_salida, "P1\n");
		fprintf(archivo_salida, "# Esto es una matriz completa\n");
		fprintf(archivo_salida, "%d %d\n",cant_celdas*4,cant_celdas*4);
		imprimir_matriz_amplificada(matriz,cant_celdas,cant_celdas,archivo_salida);
		fclose(archivo_salida);
		return 0;
	}

	fprintf(stderr,"Error: no se ejecuto ninguna version. \n");
	return 0;
}
