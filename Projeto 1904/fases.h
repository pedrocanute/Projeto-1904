#ifndef FASES_H
#define FASES_H

#include <stdbool.h>

typedef struct Inimigo Inimigo; //ESTA P0RR4 É UM FORWARD DECLARATION, PERDI QUASE 5HRS TENTANDO ARRUMAR ESTA MERD4 DE DEPENDENCIA CIRCULAR >:/ 
//ISSO BASICAMENTE DECLARA A EXISTENCIA DA TAG INIMIGO

typedef enum { 
	FASE_1 = 1, 
	FASE_2 = 2, 
	FASE_3 = 3
}Fases;

typedef struct SistemaFases {
	int faseAtual;
	int metaEliminacoes;
	int inimigosMortos;
	bool faseCompleta;
}SistemaFases;

void inicializarSistemaFases(SistemaFases* sistema, Inimigo* inimigo);

void configurarFase(SistemaFases* sistema, int numeroFase, Inimigo* inimigo);

bool verificarProgressoDaFase(SistemaFases* sistema);

void avancarFase(SistemaFases* sistema, Inimigo* inimigo);

#endif