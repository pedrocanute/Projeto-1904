#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>

typedef struct {
	float jogadorX, jogadorY;
}Jogador;

// Fun��es jogador
void mover(Jogador *p, bool w, bool a, bool s, bool d, float velocidade);

void restringirPosicao(Jogador* p, float maximoX, float maximoY, float larguraJogador, float alturaJogador);

//Fun��es Inimigo
#endif
