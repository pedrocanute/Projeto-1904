#ifndef PROJETIL_H
#define PROJETIL_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "movimentacao.h"

typedef struct {
    float projetilX, projetilY;
    bool projetilAtivo;
} ProjetilPosicao;

void atirar(ProjetilPosicao* pp, Posicao jogador, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int width, float projetilVelocidade);

#endif
