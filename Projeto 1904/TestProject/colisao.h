#ifndef COLISAO_H
#define COLISAO_H

#include <stdbool.h>

typedef struct {
	float jogadorX, jogadorY;
}Posicao;

void mover(Posicao* p, bool w, bool a, bool s, bool d, float velocidade);

void restringirPosicao(Posicao* p, float maximoX, float maximoY, float larguraJogador, float alturaJogador);

#endif

