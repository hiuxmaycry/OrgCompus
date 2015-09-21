#ifndef __PARAMETROS_H_INCLUDED__
#define __PARAMETROS_H_INCLUDED__
//Estructura que contiene los parametros del programa.
struct parametros_t {
	//Flag que indica si se realizo una consulta del tipo Help o Version.
	int informacion;
	
	//Numero de regla.
	char* regla;

	//Cantidad de celdas del mundo unidimensional. 	
	char* celdas;

	//Archivo de entrada para el estado inicial.
	char* arch_entrada;

	//Nombre del archivo de salida.
	char* arch_salida;
};

int validar_parametros(struct parametros_t* parametros);

int cargar_parametros(struct parametros_t* parametros, int argc, char** argv);
#endif
