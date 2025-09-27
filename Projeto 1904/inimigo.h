#ifndef INIMIGO_H
#define INIMIGO_H

#include "personagem.h"
#include <stdbool.h>
#include <allegro5/allegro.h>

#define MAX_INIMIGOS 8

typedef enum {
    TIPO_ZUMBI,
    TIPO_RATO,
    TIPO_MOSQUITO
} TipoInimigo;

typedef struct {
    float botX, botY, larguraBot, alturaBot;
    TipoInimigo tipo;
    bool ativo;
    int vida;
    // Variáveis de animação
    int frame_atual;
    int contador_frame;
    int frames_por_sprite;
    bool virado_direita;
    bool em_movimento;
    
    ALLEGRO_BITMAP* sprite_direita;
    ALLEGRO_BITMAP* sprite_esquerda;

    float velocidade;
} Inimigo;

void desenhar_inimigo(Inimigo* inimigo, bool em_movimento);

void inicializar_inimigo(Inimigo* inimigo, TipoInimigo tipo, float x, float y,
    ALLEGRO_BITMAP* sprite_dir, ALLEGRO_BITMAP* sprite_esq);

// DE ACORDO COM A POSICAO DA CAMERA
void inicializar_array_inimigos(Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera);

void desenhar_todos_inimigos(Inimigo* inimigos, int quantidade);

// ATAULIZA DE ACORDO COM A CAMERA
void atualizar_movimento_inimigos(Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera);

bool detectarColisao(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador);

#endif
