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

// FUN플O FUNDO DA BARRA DO PERSONAGEM / CARAVANA
void desenhar_barra(float barraX, float barraY, float barraLargura, float barraAltura);

// FUN플O BARRA DE INFEC플O
void desenhar_barra_infeccao(float infeccaoX, float infeccaoY, float infeccaoLargura, float infeccaoAltura, float* posicaoCamera);

// FUN플O BARRA DE VIDA DO PERSONAGEM
void desenhar_barra_vida_personagem(float vidaX, float vidaY, float vidaLargura, float vidaAltura, float* posicaoCamera);

// FUN플O FUNDO DA BARRA DE VIDA DO BOSS
void desenhar_fundo_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura);

// FUN플O BARRA DE VIDA DO BOSS
void desenhar_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura);

#endif
