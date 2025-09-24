#ifndef INIMIGO_H
#define INIMIGO_H

#include "personagem.h"
#include <stdbool.h>

typedef struct {
	float botX, botY, larguraBot, alturaBot;
}Inimigo;

void perseguir(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador, float velocidadeInimigo);

//Funções Inimigo
#endif

