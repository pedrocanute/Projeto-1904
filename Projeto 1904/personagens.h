#ifndef PERSONAGENS_H
#define PERSONAGENS_H

#include <stdbool.h>

typedef struct {
	int posicaoXp, posicaoYp, largurap, alturap;
}Jogador;

// Funções jogador
void mover(Jogador *p, bool w, bool a, bool s, bool d, float velocidade);

void restringirPosicao(Jogador* p, float maximoX, float maximoY, float larguraJogador, float alturaJogador);

//Funções Inimigo
#endif
