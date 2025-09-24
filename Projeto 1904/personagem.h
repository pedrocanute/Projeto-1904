#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>
#include <allegro5/allegro.h>

typedef struct {
	float jogadorX, jogadorY;
}Jogador;

// Funções jogador
void mover(Jogador *p, bool w, bool a, bool s, bool d, float velocidade);

void restringirPosicao(Jogador* p, float maximoX, float maximoY, float larguraJogador, float alturaJogador);

void desenhar_jogador(Jogador jogador, bool w, bool a, bool s, bool d, ALLEGRO_BITMAP* sprite_direita,
    ALLEGRO_BITMAP* sprite_esquerda, int* frame_atual, int* contador_frame,
    int frames_por_sprite, bool* virado_direita);

//Funções Inimigo
#endif
