#ifndef PROJETIL_H
#define PROJETIL_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "personagem.h"

typedef struct {
    float projetilX[50], projetilY[50];
    bool qtdProjetil[50];
    float proxProjetil;
} ProjetilPosicao;

void atirar(ProjetilPosicao* pp, Jogador jogador, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int width, float projetilVelocidade, float projetiCadencia);

#endif