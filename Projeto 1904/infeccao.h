#ifndef INFECCAO_H
#define INFECCAO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "inimigo.h"

typedef struct {
    float barraX, barraY, barraLargura, barraAltura;
} Barra;

typedef struct {
    float barraX, barraY, barraLargura, barraAltura, barraVida;
} BarraBoss;

// FUNÇÃO FUNDO DA BARRA DO PERSONAGEM / CARAVANA
void desenhar_barra(float barraX, float barraY, float barraLargura, float barraAltura);

// FUNÇÃO BARRA DE INFECÇÃO
void desenhar_barra_infeccao(float infeccaoX, float infeccaoY, float infeccaoLargura, float infeccaoAltura, float* posicaoCamera);

// FUNÇÃO FUNDO DA BARRA DE VIDA DO BOSS
void desenhar_fundo_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura);

// FUNÇÃO BARRA DE VIDA DO BOSS
void desenhar_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura);
#endif
