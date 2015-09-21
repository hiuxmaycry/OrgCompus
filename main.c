#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BINARIO 8
#define MAX_LARGO 150

void decimal_binario(unsigned int* buffer, int regla, unsigned int largo){
	for (int x = 0; x < largo; x++){
		buffer[x] = 0;
	}
	unsigned int i = 1;
	while (regla != 0){
		buffer[largo - i] = regla % 2;
		regla = regla / 2;
		i++;
	}
}

void imprimir_fila_matriz(void* aux, size_t fila_a_imprimir, size_t cant_columnas, FILE* out){
	unsigned char* x = aux;
	for (int j = 0; j < cant_columnas; j++){
		fprintf(out, "%d", (x + fila_a_imprimir * cant_columnas)[j]);
	}
	fprintf(out, "\n");
}

void imprimir_matriz(void* aux,size_t cant_filas,size_t cant_columnas, FILE* out){
	unsigned char* x = aux;
	for (int i = 0; i < cant_filas; i++){
		for (int j = 0; j < cant_columnas; j++){
			fprintf(out, "%d ",(x + i*cant_columnas)[j]);
		}
		fprintf(out, "\n");
	}
}

void imprimir_matriz_amplificada(void* aux,size_t cant_filas,size_t cant_columnas, FILE* out){
	unsigned char* x = aux;
	for (int i = 0; i < cant_filas; i++){
		for (int aux = 0; aux < 4; aux++){
			for (int j = 0; j < cant_columnas; j++){
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
	for (int i = 0; i < cant_filas; i++){
		for (int j = 0; j < cant_columnas; j++){
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
	for(unsigned int columna = 0; columna < n; columna++){
		*(a + n * (fila + 1) + columna) = proximo(a, fila, columna, regla, n);
	}
}

int main(int argc, char** argv){
	printf("\n");
	if (argc == 2 && (strcmp(argv[1],"-h") == 0)){
		printf("Uso: \n autcel -h \n autcel -V \n autcel R N inputfile [-o outputprefix].\n \
Opciones: \n -h, --help Imprime este mensaje. \n -V, --version Da la version del programa. \n -o Prefijo de los archivos de salida. \n \
Ejemplos: \n autcel 30 80 inicial -o evolucion Calcula la evolucion del automata Regla 30, en un mundo unidimensional de 80 celdas, por 80 iteraciones. \
El archivo de salida se llamara evolucion.pbm. Si no se da un prefijo para los archivos de salida,el prefijo sera el nombre del archivo de entrada.\n");
		return 0;
	}
	else if (argc == 2 && (strcmp(argv[1],"-v") == 0)){
		printf("Version actual: 1.0v\n");
		return 0;
	}
	else if (argc >= 3){
		printf("Leyendo estado inicial.\n");
	}

	/*
	// Obtengo la cantidad_celdas de celdas
	char in[MAX_LARGO];
	strcpy(in,argv[3]);
	strcat(in,".txt");
	
	FILE* archivo;
	int c;
	unsigned int cantidad_celdas = 0;
	archivo = fopen(in, "r");
	while ((c = fgetc(archivo)) != EOF){
		cantidad_celdas++;
	}
	fclose(archivo);
	char *end;
	long int x = strtol(argv[2], &end, 10);
	if (cantidad_celdas != x){
		printf("%d %d", cantidad_celdas, x);
		fprintf(stderr,"Error: cantidad_celdas de numeros en el archivo no coincide con el parametro pasado.\n");
		return 0;
	}*/

	// Creo e inicializo la matriz
	char *end;
	long int x = strtol(argv[2], &end, 10);
	unsigned int cantidad_celdas = x;
	unsigned char matriz [cantidad_celdas][cantidad_celdas];
	inicializar_matriz(matriz,cantidad_celdas,cantidad_celdas);

	//Escribo el estado inicial en la primera fila de la matriz
	char input[MAX_LARGO];
	strcpy(input,argv[3]);
	strcat(input,".txt");
	FILE* archivo;
	unsigned int i = 0;
	archivo = fopen(input, "r");
	while (!feof(archivo)){
		if(i >= x){
			fprintf(stderr,"ERROR: la cantidad_celdas de numeros del archivo no coincide con el parametro pasado. \n");
			return 0;
		}
		int valor;
		fscanf(archivo, "%1d", &valor);
		matriz[0][i] = (unsigned char) valor; 
		i++;
	}
	fclose(archivo);

	int version;
	while(1){
		printf("Ingrese un 0 para la version normal con archivos de salida, o 1 para la version por terminal: \n");
		scanf("%d",&version);
		if (version == 1 || version == 0){
			break;
		}
		fprintf(stderr,"Entrada no valida. Intentelo nuevamente. \n");
	}

	if (version == 1){ //version de terminal: permite decidir un paso variable
		int iteraciones = 0;
		int sumatoria = 0;
		while(sumatoria < (cantidad_celdas - 1)){
			printf("Iteraciones realizadas hasta el momento: %d \n", sumatoria);
			printf("Ingrese la cantidad_celdas de iteraciones a realizar: \n");
			scanf("%d",&iteraciones);
			if (iteraciones < 1 || iteraciones > (cantidad_celdas - 1 - sumatoria)){
				fprintf(stderr, "Numero invalido, se realizara una iteracion. \n");
				iteraciones = 1;
			}
			for (unsigned int fila = 0; fila < iteraciones; fila++){
				char *endptr;
				long int regla = strtol(argv[1], &endptr, 10);
				calcular_prox_fila(matriz, fila + sumatoria, regla, cantidad_celdas);
				imprimir_fila_matriz(matriz, fila + sumatoria, cantidad_celdas, stdout);
			}
			sumatoria += iteraciones;
		}
		return 0;
	}

	if (version == 0){ //version de archivos
		//Calculo la evolucion de toda la matriz
		for (unsigned int fila = 0; fila < (cantidad_celdas - 1); fila++){
			char *endptr;
			long int regla = strtol(argv[1], &endptr, 10);
			calcular_prox_fila(matriz, fila, regla, cantidad_celdas);
		}

		// Escribo el archivo de salida
		char output[MAX_LARGO];
		strcpy(output,argv[3]); // Default es el nombre de la entrada
		if (argc == 6){
			strcpy(output,argv[5]); // Cambio el nombre si fue pasado por parametro
		}
		strcat(output,".pbm");
		archivo = fopen(output, "wb");
		fprintf(archivo, "P1\n");
		fprintf(archivo, "# Esto es una matriz completa\n");
		fprintf(archivo, "%d %d\n",cantidad_celdas*4,cantidad_celdas*4);
		printf("Grabando %s\n", output);
		imprimir_matriz_amplificada(matriz,cantidad_celdas,cantidad_celdas,archivo);
		fclose(archivo);
		printf("Listo.\n");

		return 0;
	}

	fprintf(stderr,"ERROR: no se ejecuto ninguna version.\n");
}
