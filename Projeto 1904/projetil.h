#ifndef PROJETIL_H
#define PROJETIL_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "personagem.h"
#include "fases.h"
#include "inimigo.h"
#include "configuracoes.h"
#include "colisao.h"
#include "infeccao.h"

typedef struct SistemaFases SistemaFases; //ESTA P0RR4 � UM FORWARD DECLARATION, PERDI QUASE 5HRS TENTANDO ARRUMAR ESTA MERD4 DE DEPENDENCIA CIRCULAR >:/

typedef struct {
    float projetilX[50], projetilY[50], projetilDirecao[50];
    bool projetilAtivo[50];
    float proxProjetil;
} ProjetilPosicao;

void atirar_multiplos_inimigos(ProjetilPosicao* pp, Jogador jogador, Inimigo* inimigos, int numInimigos, ALLEGRO_BITMAP* projetilDireita, ALLEGRO_BITMAP* projetilEsquerda, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int larguraJogador, int width, float projetilVelocidade, float projetilCadencia, float* posicaoCamera, SistemaFases* sistemaFase, BarraBoss* barraVidaBoss);

#endif
