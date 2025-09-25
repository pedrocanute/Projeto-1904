#ifndef PROJETIL_H
#define PROJETIL_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "personagem.h"
#include "inimigo.h"

typedef struct {
    float projetilX[50], projetilY[50], projetilDirecao[50];
    bool projetilAtivo[50];
    float proxProjetil;
} ProjetilPosicao;

void atirar(ProjetilPosicao* pp, Jogador jogador, Inimigo bot, ALLEGRO_BITMAP* projetilDireita, ALLEGRO_BITMAP* projetilEsquerda, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int larguraJogador, int width, float projetilVelocidade, float projetilCadencia, float novaPosicaoX, float novaPosicaoY, float* posicaoCamera);

#endif