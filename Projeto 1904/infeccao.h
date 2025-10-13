#ifndef INFECCAO_H
#define INFECCAO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

typedef struct {
    float barraX, barraY, barraLargura, barraAltura;
} Barra;

typedef struct {
    float barraX, barraY, barraLargura, barraAltura, barraVida;
} BarraBoss;

// FUN��O FUNDO DA BARRA DO PERSONAGEM / CARAVANA
void desenhar_barra(float barraX, float barraY, float barraLargura, float barraAltura);

// FUN��O BARRA DE INFEC��O
void desenhar_barra_infeccao(float infeccaoX, float infeccaoY, float infeccaoLargura, float infeccaoAltura, float* posicaoCamera);

// FUN��O BARRA DE VIDA DO PERSONAGEM
void desenhar_barra_vida_personagem(float vidaX, float vidaY, float vidaLargura, float vidaAltura, float* posicaoCamera);

// FUN��O FUNDO DA BARRA DE VIDA DO BOSS
void desenhar_fundo_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura);

// FUN��O BARRA DE VIDA DO BOSS
void desenhar_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura);

#endif
