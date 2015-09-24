#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#include "parametros.h"

#define ERROR_PARAMETRO_DESCONOCIDO 1
#define ERROR_PARAMETRO_NO_ENTERO 2
#define ERROR_ARCHIVO_ENTRADA_NO_EXISTE 3
#define ERROR_ARCHIVO_SALIDA_NO_ENCONTRADO 4
#define ERROR_CANTIDAD_DE_PARAMETROS 5
#define ERROR_REGLA_INVALIDA 6
#define ERROR_CELDA_INVALIDA 7
#define ERROR_FALTA_PARAMETRO_REGLA 8
#define ERROR_FALTA_PARAMETRO_CELDAS 9
#define ERROR_FALTA_PARAMETRO_ENTRADA 10
#define ERROR_ARCHIVO_ENTRADA_FORMATO_INVALIDO 11
#define ERROR_CELDAS_Y_ENTRADA_NO_COINCIDEN 12


void informar_error(int error) {
	switch (error) {
		case (ERROR_PARAMETRO_DESCONOCIDO):
			fputs("Parametro ingresado desconocido\n", stderr);
			break;
		case (ERROR_PARAMETRO_NO_ENTERO):
			fputs("El parametro no es entero\n", stderr);
			break;
		case (ERROR_ARCHIVO_ENTRADA_NO_EXISTE):
			fputs("El archivo de entrada no se encuentra.\n", stderr);
			break;
		case (ERROR_ARCHIVO_SALIDA_NO_ENCONTRADO):
			fputs("Falta archivo de salida\n", stderr);
			break;
		case (ERROR_CANTIDAD_DE_PARAMETROS):
			fputs("Cantidad de parametros invalida\n", stderr);
			break;
		case (ERROR_REGLA_INVALIDA):
			fputs("Numero de regla invalido\n", stderr);
			break;
		case (ERROR_CELDA_INVALIDA):
			fputs("Cantidad de celdas invalida\n", stderr);
			break;
		case (ERROR_FALTA_PARAMETRO_REGLA):
			fputs("No se ha ingresado el numero de regla.\n", stderr);
			break;
		case (ERROR_FALTA_PARAMETRO_CELDAS):
			fputs("No se ha ingresado la cantidad de celdas.\n", stderr);
			break;
		case (ERROR_FALTA_PARAMETRO_ENTRADA):
			fputs("No se ha ingresado el nombre del archivo de entrada.\n", stderr);
			break;
		case (ERROR_ARCHIVO_ENTRADA_FORMATO_INVALIDO):
			fputs("El archivo de entrada no cumple con el formato esperado.\n", stderr);
			break;	
		case (ERROR_CELDAS_Y_ENTRADA_NO_COINCIDEN):
			fputs("La cantidad de caracteres del archivo no coincide con las celdas ingresadas\n", stderr);
			break;
		default:
			fputs("Error\n", stderr);
			break;
	}
}

void imprimir_help() {
	printf("Uso:\n\tautcel -h\n\tautcel -V\n\tautcel R N inputfile [-o outputprefix]\n");
}

void imprimir_version() {
	printf("Version 1.0\n");
}


bool es_archivo_entrada_valido(char* nombre_archivo) {
	//Se verifica que el archivo exista.
	FILE* archivo = fopen(nombre_archivo, "rb");
	if (archivo == NULL) {
		informar_error(ERROR_ARCHIVO_ENTRADA_NO_EXISTE);
		return false;
	}
	
	char caracter = fgetc(archivo);
	
	//Si el archivo esta vacio, reportar error.
	if (caracter == -1) {
		informar_error(ERROR_ARCHIVO_ENTRADA_FORMATO_INVALIDO);
		fclose(archivo);
		return false;
	}

	while (!feof(archivo)) {					
		//Los unicos caracteres validos son 0, 1 y /n.
		if (!((caracter == '0') || (caracter == '1') || (caracter == '\n'))) {
			informar_error(ERROR_ARCHIVO_ENTRADA_FORMATO_INVALIDO);
			return false;
		}
		
		caracter = fgetc(archivo);
		if (caracter == '\n') break;
	}
	
	fclose(archivo);
	return true;
}

bool isDigit(char caracter) {
	return (caracter >= '0' && caracter <= '9');
}

bool es_numerico(char* string) {
	int indice;
	for (indice = 0; indice < strlen(string); indice++) {
		if (!isDigit(string[indice])) {
			return false;
		}
	}
	return true;
}

bool es_regla_valida(char* regla) {
	//Si la regla es NULL, esta no ha sido ingresada, informar error.
	if (regla == NULL) {
		informar_error(ERROR_REGLA_INVALIDA);
		return false;
	}

	//Se verifica que la regla sea numerica.
	if (!es_numerico(regla)) {
		informar_error(ERROR_REGLA_INVALIDA);
		return false;
	}
	
	//Se convierte la regla a numero entero.
	int regla_int = atoi(regla);
	
	//Si la regla es numerica, se verifica que su valor se encuentre
	//entre 0 y 255.
	if (0 <= regla_int && regla_int <= 255) return true;
	informar_error(ERROR_REGLA_INVALIDA);
	return false;
}


bool es_celda_valida(char* celda) {
	//Si la celda es NULL, esta no ha sido ingresada, informar error.
	if (celda == NULL) {
		informar_error(ERROR_CELDA_INVALIDA);
		return false;
	}
	
	//Se verifica que la celda sea numerica.
	if (!es_numerico(celda)) {
		informar_error(ERROR_CELDA_INVALIDA);
		return false;
	}
		
	return true;
}


int cantidad_columnas(char* nombre_archivo) {
	FILE* archivo = fopen(nombre_archivo, "rb");
	int cant_col =0;
	
	char caracter = fgetc(archivo);
	while (!feof(archivo) && caracter != '\n') {					
		cant_col += 1;
		caracter = fgetc(archivo);
	}
	fclose(archivo);
	
	return cant_col;
}

 
bool coinciden_celdas_con_col_archivo(char* celdas, char* arch_entrada) {
	int columnas = cantidad_columnas(arch_entrada);
	int cant_celdas = atoi(celdas);
	printf("celdas %i\n", columnas);
	
	if (columnas != cant_celdas) {
		informar_error(ERROR_CELDAS_Y_ENTRADA_NO_COINCIDEN);
		return false;
	}
	return true;
}
	

		
int cargar_parametros(struct parametros_t* parametros, int argc, char** argv) {
	int opcion;
	
	//Se inicializan los parametros en NULL.
	parametros->informacion = 0;
	parametros->regla = NULL;
	parametros->celdas = NULL;
	parametros->arch_entrada = NULL;
	parametros->arch_salida = NULL;
	
	//Estructura para las opciones 'long'.
	const struct option opciones[] = {
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'},
		{0, 0, 0, 0}
		};
	
	//Se cargan los parametros.
	while(true) {
		opcion = getopt_long(argc, argv,"hvo:", opciones, NULL);
		
		if (opcion == -1) break;
		switch (opcion) {
			//Si se ingreso la opcion de Ayuda o Version
			//se imprime el mensaje y se retorna.
			case 'h':
				imprimir_help();
				parametros->informacion = 1;
				return 0;
			case 'v':
				imprimir_version();
				parametros->informacion = 1;
				return 0;
			case 'o':
				parametros->arch_salida = optarg;
				break;
			case '?':
				return 1;
			default:
				return 1;
		}
	}
	
	if (optind < argc) parametros->regla = argv[optind++];
	if (optind < argc) parametros->celdas = argv[optind++];
	if (optind < argc) parametros->arch_entrada = argv[optind++];
	
	//Si no se cargo el archivo de salida, este se llamara como el 
	//archivo de entrada.
	if (parametros->arch_salida == NULL) parametros->arch_salida = parametros->arch_entrada;

	return 0;
}


int validar_parametros(struct parametros_t* parametros) {
	if (parametros == NULL) return 1;
	
	//Si todos el flag de Informacion se encuentra en 1, se realizo una
	//consulta, no es error.
	if (parametros->informacion) return 0;
	
	//Se verifica que existan los parametros obligatorios (regla, celdas y entrada).
	bool faltan_parametros = false;
	if (parametros->regla == NULL) {
		informar_error(ERROR_FALTA_PARAMETRO_REGLA);
		faltan_parametros = true; 
	}
	if (parametros->celdas == NULL) {
		informar_error(ERROR_FALTA_PARAMETRO_CELDAS);
		faltan_parametros = true;
	}
	if (parametros->arch_entrada == NULL) {
		informar_error(ERROR_FALTA_PARAMETRO_ENTRADA);
		faltan_parametros = true;
	}
	
	if (faltan_parametros) return 1;
	
	//Si todos los parametros han sido ingresados, verificar que sean validos.
	if (!es_regla_valida(parametros->regla) ||
		!es_celda_valida(parametros->celdas)||
		!es_archivo_entrada_valido(parametros->arch_entrada) ||
		!coinciden_celdas_con_col_archivo(parametros->celdas, parametros->arch_entrada)) return 1;
	
	return 0;
}

			
