#ifndef INIMIGO_H
#define INIMIGO_H

#include "personagens.h"
#include <stdbool.h>

typedef struct {
	int posicaoX, posicaoY, largura, altura;
}Inimigo;

void perseguir(Inimigo* bot, Jogador* jogador);

//Funções Inimigo
#endif
#pragma once
