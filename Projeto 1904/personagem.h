#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>
#include <allegro5/allegro.h>

#include <allegro5/allegro_primitives.h>

#include "caravana.h"

typedef struct {
    float jogadorX, jogadorY;
    bool paraDireita, paraEsquerda;
} Jogador;

// Funções jogador
void mover(Jogador* p, bool w, bool a, bool s, bool d, bool shift, float velocidade, int* frames_por_sprite);

void restringirPosicao(Jogador* p, Caravana* caranava, float maximoX, float maximoY, float larguraJogador, float alturaJogador);

// DESENHA JOGADOR COM BASE NO MOVIMENTO E FUNCAO ATIRAR
void desenhar_jogador(Jogador jogador, bool w, bool a, bool s, bool d, bool espaco, ALLEGRO_BITMAP* sprite_direita, ALLEGRO_BITMAP* sprite_esquerda, ALLEGRO_BITMAP* sprite_atirando_direita, ALLEGRO_BITMAP* sprite_atirando_esquerda, int* frame_atual, int* contador_frame, int frames_por_sprite, bool* virado_direita, int* frame_tiro, int* contador_frame_tiro);  

void camera_jogador(float* posicaoCamera, Jogador jogador, int larguraTela, int larguraJogador, int alturaJogador, float caravanaX, float caravanaVelocidade);

#endif
