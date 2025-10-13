#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

// DEFINES
#define WIDTH 1280
#define HEIGHT 720
#define FPS 60
#define VELOCIDADE_JOGADOR 5.5f
#define LARGURA_JOGADOR 80
#define ALTURA_JOGADOR 96
#define VELOCIDADE_PROJETIL 15.5f
#define CADENCIA 0.2f
#define LARGURA_PROJETIL 16
#define ALTURA_PROJETIL 16
#define VELOCIDADE_INIMIGO 4.5f

typedef struct {
    // Inimigos
    ALLEGRO_BITMAP* zumbi_direita;
    ALLEGRO_BITMAP* zumbi_esquerda;
    ALLEGRO_BITMAP* rato_direita;
    ALLEGRO_BITMAP* rato_esquerda;
    ALLEGRO_BITMAP* mosquito_direita;
    ALLEGRO_BITMAP* mosquito_esquerda;
    ALLEGRO_BITMAP* boss_variola_esquerda;
    ALLEGRO_BITMAP* boss_variola_direita;
    //Caravana
    ALLEGRO_BITMAP* soldado;
    // Jogador
    ALLEGRO_BITMAP* sprite_andando_direita;
    ALLEGRO_BITMAP* sprite_andando_esquerda;
    ALLEGRO_BITMAP* sprite_atirando_direita;
    ALLEGRO_BITMAP* sprite_atirando_esquerda;
    // Projetil
    ALLEGRO_BITMAP* projetilDireita;
    ALLEGRO_BITMAP* projetilEsquerda;
    // Cenário
    ALLEGRO_BITMAP* cenario1;
    ALLEGRO_BITMAP* cenario2;
    // Menu
    ALLEGRO_BITMAP* fundoMenu;
    ALLEGRO_BITMAP* botaoJogar;
    ALLEGRO_BITMAP* botaoJogar2;
    ALLEGRO_BITMAP* botaoRegras;
    ALLEGRO_BITMAP* botaoRegras2;
    ALLEGRO_BITMAP* botaoSair;
    ALLEGRO_BITMAP* botaoSair2;
    ALLEGRO_BITMAP* abaRegras;
    ALLEGRO_BITMAP* botaoVoltar;
    ALLEGRO_BITMAP* botaoVoltar2;
} Bitmaps;

bool carregar_bitmaps(Bitmaps* bitmap);

void destruir_bitmaps(Bitmaps* bitmap);

#endif