#ifndef INIMIGO_H
#define INIMIGO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "personagem.h"

#define MAX_INIMIGOS 20

typedef enum {
    TIPO_ZUMBI,
    TIPO_RATO,
    TIPO_MOSQUITO,
    TIPO_BOSS
} TipoInimigo;

typedef struct Inimigo { //Aqui cria uma tag que indica pra outro include que essa struct existe
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

void inicializar_inimigo(Inimigo* inimigo, TipoInimigo tipo, float x, float y, ALLEGRO_BITMAP* sprite_dir, ALLEGRO_BITMAP* sprite_esq);

// DE ACORDO COM A POSICAO DA CAMERA
void inicializar_array_inimigos(Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera);

void desenhar_todos_inimigos(Inimigo* inimigos, int quantidade);

// ATUALIZA DE ACORDO COM A CAMERA
void atualizar_movimento_inimigos(Caravana* caravana, Inimigo* inimigos, int quantidade);

int contarInimigosAtivos(Inimigo* inimigos, int maxInimigos);

void aplicar_buffs_por_fase(Inimigo* inimigos, int quantidade, int faseAtual);

void spawnar_boss(Inimigo* inimigo, ALLEGRO_BITMAP* boss_dir, ALLEGRO_BITMAP* boss_esq, float* posicaoCamera);

void atualizar_boss_perseguindo(Inimigo* boss, const Jogador* jogador, float distanciaParada);

#endif