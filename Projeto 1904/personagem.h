#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "caravana.h"

//FORWARD DECLARATION DE ARMA
typedef enum {
    ARMA_VASSOURA,
    ARMA_VENENO,
    ARMA_VACINA
} Arma;

typedef struct {
    float jogadorX, jogadorY;
    bool paraDireita, paraEsquerda;

    float tempoUltimoDisparo;
    float cadenciaAtual;
} Jogador;

// Estrutura para armazenar todos os sprites do jogador
typedef struct {
    // Sprites Vacina (padrão)
    ALLEGRO_BITMAP* sprite_andando_direita;
    ALLEGRO_BITMAP* sprite_andando_esquerda;
    ALLEGRO_BITMAP* sprite_atirando_direita;
    ALLEGRO_BITMAP* sprite_atirando_esquerda;

    // Sprites Vassoura
    ALLEGRO_BITMAP* sprite_andando_direita_vassoura;
    ALLEGRO_BITMAP* sprite_andando_esquerda_vassoura;
    ALLEGRO_BITMAP* atacando_vassoura_direita;
    ALLEGRO_BITMAP* atacando_vassoura_esquerda;

    // Sprites Veneno
    ALLEGRO_BITMAP* sprite_andando_direita_veneno;
    ALLEGRO_BITMAP* sprite_andando_esquerda_veneno;
    ALLEGRO_BITMAP* atacando_veneno_direita;
    ALLEGRO_BITMAP* atacando_veneno_esquerda;
} SpritesJogador;

// Funções jogador
void mover(Jogador* p, bool w, bool a, bool s, bool d, bool shift, float velocidade, int* frames_por_sprite);

void restringirPosicao(Jogador* p, Caravana* caravana, float maximoX, float maximoY, float larguraJogador, float alturaJogador);

// DESENHA JOGADOR COM BASE NO MOVIMENTO E ARMA EQUIPADA
void desenhar_jogador(Jogador* jogador, bool w, bool a, bool s, bool d, bool espaco,SpritesJogador* sprites, int arma_equipada,int* frame_atual, int* contador_frame, int frames_por_sprite,bool* virado_direita, int* frame_tiro, int* contador_frame_tiro);

void camera_jogador(float* posicaoCamera, Jogador jogador, int larguraTela, int larguraJogador, int alturaJogador, float caravanaX, float caravanaVelocidade);

#endif
