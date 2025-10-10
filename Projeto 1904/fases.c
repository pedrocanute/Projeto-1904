#include "fases.h"
#include "inimigo.h"

void inicializarSistemaFases(SistemaFases* sistema, Inimigo* inimigo) {
	sistema->faseAtual = 1;
	sistema->inimigosMortos = 0;
	sistema->metaEliminacoes = 200;
	sistema->faseCompleta = false;
	configurarFase(sistema, sistema->faseAtual, inimigo);
}

void configurarFase(SistemaFases* sistema, int numeroFase, Inimigo* inimigo) {
	sistema->faseAtual = numeroFase;
	sistema->inimigosMortos = 0;
	sistema->faseCompleta = false;

	switch (numeroFase) {
	case FASE_1:
		sistema->metaEliminacoes = 20;
		break;
	case FASE_2:
		sistema->metaEliminacoes = 20;
		break;
	case FASE_3:
		sistema->metaEliminacoes = 20;
		
		break;
	default:
		break;
	}
}

bool verificarProgressoDaFase(SistemaFases* sistema) {
	return sistema->inimigosMortos >= sistema->metaEliminacoes;
}

void avancarFase(SistemaFases* sistema, Inimigo* inimigo) {
	if (verificarProgressoDaFase(sistema)) {
		int proxima = sistema->faseAtual + 1;
		if (proxima > FASE_3) 
			proxima = FASE_3;
		configurarFase(sistema, proxima, inimigo);
	}
	return;
}
