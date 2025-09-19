#ifndef INIMIGO_H
#define INIMIGO_H

#include "personagem.h"
#include <stdbool.h>

typedef struct {
	int posicaoX, posicaoY, largura, altura;
}Inimigo;

void perseguir(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador, float velocidadeInimigo);

//Funções Inimigo
#endif

