#ifndef COLISAO_H
#define COLISAO_H

#include <stdbool.h>
#include "personagem.h"
#include "inimigo.h"
#include "projetil.h"

// Teste AABB genérico entre dois retângulos
bool colisao_aabb(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2);

// Colisão entre um inimigo (retângulo do sprite) e o jogador (retângulo do jogador)
bool colisao_jogador_inimigo(const Inimigo* bot, const Jogador* jogador, float larguraJogador, float alturaJogador);

// Colisão entre um projétil (retângulo) e um inimigo (retângulo do sprite)
bool colisao_projetil_inimigo(float projX, float projY, int projW, int projH, const Inimigo* inimigo);

// Colisão entre um inimigo e a caravana
bool colisao_inimigo_caravana(const Inimigo* bot, const Jogador* caravana, float caravanaLargura, float caravanaAltura);
#endif

