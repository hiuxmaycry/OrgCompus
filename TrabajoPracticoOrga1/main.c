#include <stdio.h>
#include <stdlib.h>

#define MAX_BINARIO 8
#define REGLA 30

void decimal_binario(unsigned int* buffer, int regla, unsigned int largo){
	unsigned int x,i = 1;
	for (x = 0; x < largo; x++){
		buffer[x] = 0;
	}
	while (regla != 0){
		buffer[largo - i] = regla % 2;
		regla = regla / 2;
		i++;
	}
}

void imprimir_matriz(unsigned char* aux,size_t cant_filas,size_t cant_columnas, FILE* out){
	unsigned int i;
        for(i=0;i<(cant_filas*cant_columnas);i++){
                aux[i]=(unsigned char)0;
	}
}

void imprimir_matriz_amplificada(void* aux,size_t cant_filas,size_t cant_columnas, FILE* out){
	unsigned char* x = aux;
	unsigned int i,j;
	for (i = 0; i < cant_filas; i++){
		for (aux = 0; aux < 4; aux++){
			for (j = 0; j < cant_columnas; j++){
				fprintf(out, "%d ",(x + i*cant_columnas)[j]);
				fprintf(out, "%d ",(x + i*cant_columnas)[j]);
				fprintf(out, "%d ",(x + i*cant_columnas)[j]);
				fprintf(out, "%d ",(x + i*cant_columnas)[j]);
			}
			fprintf(out, "\n");
		}
	}
}

void inicializar_matriz(void* aux,size_t cant_filas,size_t cant_columnas){
	unsigned char* x = aux;
	unsigned int i,j;
	for (i = 0; i < cant_filas; i++){
		for (j = 0; j < cant_columnas; j++){
			(x + i*cant_columnas)[j] = 0;
		}
	}
}


unsigned char proximo(unsigned char *a, unsigned int i, unsigned int j, unsigned char regla, unsigned int n){
	unsigned char prox;
	unsigned char actual = *(a + n * i + j);
	//Determino el estado de los vecinos
	unsigned char vecino_izq;
	unsigned char vecino_der;
	if(j == 0){
		vecino_izq = *(a + n * i + n - 1);
		vecino_der = *(a + n * i + 1);
	}
	else if (j == (n - 1)){
		vecino_izq = *(a + n * i + j - 1);
		vecino_der = *(a + n * i);
	}
	else{
		vecino_izq = *(a + n * i + j - 1);
		vecino_der = *(a + n * i + j + 1);
	}

	unsigned int buffer[MAX_BINARIO];
	decimal_binario(buffer,regla,MAX_BINARIO);
	//Caso 111
	if ((vecino_izq == 1) && (actual == 1) && (vecino_der == 1)){
		prox = buffer[0];
	}
	//Caso 110
	else if ((vecino_izq == 1) && (actual == 1) && (vecino_der == 0)){
		prox = buffer[1];
	}
	//Caso 101
	else if ((vecino_izq == 1) && (actual == 0) && (vecino_der == 1)){
		prox = buffer[2];
	}
	//Caso 100
	else if ((vecino_izq == 1) && (actual == 0) && (vecino_der == 0)){
		prox = buffer[3];
	}
	//Caso 011
	else if ((vecino_izq == 0) && (actual == 1) && (vecino_der == 1)){
		prox = buffer[4];
	}
	//Caso 010
	else if ((vecino_izq == 0) && (actual == 1) && (vecino_der == 0)){
		prox = buffer[5];
	}
	//Caso 001
	else if ((vecino_izq == 0) && (actual == 0) && (vecino_der == 1)){
		prox = buffer[6];
	}
	//Caso 000
	else if ((vecino_izq == 0) && (actual == 0) && (vecino_der == 0)){
		prox = buffer[7];
	}
	return prox;
}

void calcular_prox_fila(void* matriz, unsigned int fila, unsigned char regla, unsigned int n){
	unsigned char* a = matriz;
	unsigned int columna;
	for(columna = 0; columna < n; columna++){
		*(a + n * (fila + 1) + columna) = proximo(a, fila, columna, regla, n);
	}
}

int main(){

	// Obtengo la cantidad de celdas
	int c;
	unsigned int cantidad = 0;
	FILE* archivo;
	archivo = fopen("inicial.txt", "r");
	while ((c = fgetc(archivo)) != EOF){
		cantidad++;
	}
	if (cantidad == 0){
                printf("El archivo esta vacio\n");
                return 2;
	} else {
                cantidad--;
	}
	fclose(archivo);
	// Creo e inicializo la matriz
	//unsigned char matriz [cantidad][cantidad];
	unsigned char *matriz = (unsigned char *)malloc(cantidad*cantidad*sizeof(unsigned char));
	if (matriz == NULL){
                printf("No hay suficiente memoria para la matriz\n");
                return 3;
	}
	inicializar_matriz(matriz,cantidad,cantidad);
	//Escribo el estado inicial en la primera fila de la matriz
	unsigned int i,fila;
	archivo = fopen("inicial.txt", "r");
	for(i=0;i<cantidad;i++){
                fscanf(archivo, "%c", &(matriz[i]));
                matriz[i]-=(unsigned char)48;
	}
	fclose(archivo);
	//Escribo el estado de las demas filas de la matriz
	for (fila = 0; fila < (cantidad - 1); fila++){
		calcular_prox_fila(matriz, fila, REGLA, cantidad);
	}
	archivo = fopen("salida.pbm", "wb");
	fprintf(archivo, "P1\n");
	fprintf(archivo, "# Esto es una matriz completa\n");
	fprintf(archivo, "%d %d\n",cantidad*4,cantidad*4);
	imprimir_matriz_amplificada(matriz,cantidad,cantidad,archivo);
	fclose(archivo);
        free(matriz);
	return 0;
}
