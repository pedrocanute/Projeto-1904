#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "inimigo.h"
#include "personagem.h"
#include "fases.h"

void inicializar_inimigo(Inimigo* inimigo, TipoInimigo tipo, float x, float y, ALLEGRO_BITMAP* sprite_dir, ALLEGRO_BITMAP* sprite_esq) {

    inimigo->tipo = tipo;
    inimigo->botX = x;
    inimigo->botY = y;
    inimigo->sprite_direita = sprite_dir;
    inimigo->sprite_esquerda = sprite_esq;
    inimigo->frame_atual = 0;
    inimigo->contador_frame = 0;
    inimigo->frames_por_sprite = 11;
    inimigo->virado_direita = false;
    inimigo->em_movimento = true;
    inimigo->ativo = true;
    inimigo->vida = 1;

    // ATRIBUTOS DOS INIMIGOS
    switch (tipo) {
    case TIPO_ZUMBI:
        inimigo->larguraBot = 60.0f;
        inimigo->alturaBot = 80.0f;
        inimigo->velocidade = 2.5f;
        break;
    case TIPO_RATO:
        inimigo->larguraBot = 60.0f;
        inimigo->alturaBot = 50.0f;
        inimigo->velocidade = 2.8f;
        break;
    case TIPO_MOSQUITO:
        inimigo->larguraBot = 80.0f;
        inimigo->alturaBot = 80.0f;
        inimigo->frames_por_sprite = 6;
        inimigo->velocidade = 2.9f;
        break;
    case TIPO_BOSS:
        inimigo->larguraBot = 220.0f;
        inimigo->alturaBot = 220.0f;
        inimigo->frames_por_sprite = 15;
        inimigo->velocidade = 2.0f;
        inimigo->vida = 50;
        break;
    }
}

// SPAWNA INIMIGOS COM BASE NA CAMERA
void respawn_inimigo_na_camera(Inimigo* inimigo, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera) {

    float camera_direita = posicaoCamera[0] + 1280;
    float spawn_x_min = camera_direita + 50;
    float spawn_x_max = camera_direita + 300;

    float spawn_y_min = (720 / 2) + 52;
    float spawn_y_max = 720 - 100;

    float x = spawn_x_min + (float)(rand() % (int)(spawn_x_max - spawn_x_min)); //(float) converte toda a opera??o da funcao rand (que tem tipo 
    float y = spawn_y_min + (float)(rand() % (int)(spawn_y_max - spawn_y_min)); //int) em float

    TipoInimigo tipo = (TipoInimigo)(rand() % 3);

    switch (tipo) {
    case TIPO_ZUMBI:
        inicializar_inimigo(inimigo, TIPO_ZUMBI, x, y, zumbi_dir, zumbi_esq);
        break;
    case TIPO_RATO:
        inicializar_inimigo(inimigo, TIPO_RATO, x, y, rato_dir, rato_esq);
        break;
    case TIPO_MOSQUITO:
        inicializar_inimigo(inimigo, TIPO_MOSQUITO, x, y, mosquito_dir, mosquito_esq);
        break;
    }
}

void inicializar_array_inimigos(Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera) {
    srand(time(NULL));
    for (int i = 0; i < quantidade; i++) {
        respawn_inimigo_na_camera(&inimigos[i], zumbi_dir, zumbi_esq, rato_dir, rato_esq, mosquito_dir, mosquito_esq, posicaoCamera);
    }
}

void atualizar_movimento_inimigos(Caravana* caravana,Inimigo* inimigos, int quantidade) {

    for (int i = 0; i < quantidade; i++) {
        if (inimigos[i].ativo == false)
            continue;
        if (inimigos[i].tipo == TIPO_BOSS)
            continue;

        inimigos[i].botX -= inimigos[i].velocidade;

        if (inimigos[i].botX < caravana->caravanaX) {
            inimigos[i].botX = caravana->caravanaX;
        }
    }
}

void desenhar_inimigo(Inimigo* inimigo, bool em_movimento) {
    if (inimigo->ativo == false) return;

    // Escolhe sprite pela orienta??o
    ALLEGRO_BITMAP* sprite_atual = inimigo->virado_direita ? inimigo->sprite_direita : inimigo->sprite_esquerda;

    int num_colunas;
    if (inimigo->tipo == TIPO_BOSS) { //Verifica se o inimigo eh o Boss ou inimigo normal (Boss tem 3 colunas na spritesheet ao inves de 2)
        num_colunas = 3;
    }
    else {
        num_colunas = 2;
    }

    if (em_movimento) {
        inimigo->contador_frame++;
        if (inimigo->contador_frame >= inimigo->frames_por_sprite) {
            inimigo->frame_atual = (inimigo->frame_atual + 1) % num_colunas;
            inimigo->contador_frame = 0;
        }
    }

    int largura_total = al_get_bitmap_width(sprite_atual);
    int altura_frame = al_get_bitmap_height(sprite_atual);
    int largura_frame = largura_total / num_colunas;

    int sx = inimigo->frame_atual * largura_frame;
    int sy = 0;

    al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame,
        inimigo->botX, inimigo->botY, 0);
}


void desenhar_todos_inimigos(Inimigo* inimigos, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        desenhar_inimigo(&inimigos[i], inimigos[i].em_movimento);
    }
}

int contarInimigosAtivos(Inimigo* inimigos, int maxInimigos) {
    int inimigosAtivos = 0;
    for (int i = 0; i < maxInimigos; i++) {
        if (inimigos[i].ativo)
            inimigosAtivos++;
    }
    return inimigosAtivos;
}

void aplicar_buffs_por_fase(Inimigo* inimigos, int quantidade, int faseAtual) {
    int vida = 0;
    float velocidade = 1.0f;

    switch (faseAtual) {
    case FASE_2:
        vida = 1;
        velocidade = 1.50f;
        break;
    case FASE_3:
        vida = 2;
        velocidade = 2.00f;
        break;
    default:
        break;
    }

    for (int i = 0; i < quantidade; ++i) {

        inimigos[i].vida = inimigos[i].vida + vida;
        inimigos[i].velocidade *= velocidade;
    }
}

void spawnar_boss(Inimigo* inimigo, ALLEGRO_BITMAP* boss_dir, ALLEGRO_BITMAP* boss_esq, float* posicaoCamera) {

    // Boss spawna mais a frente da camera
    float camera_direita = posicaoCamera[0] + 1280;
    float spawn_x = camera_direita + 500;  // Mais distante que inimigos normais

    // Boss spawna no centro vertical da tela
    float spawn_y = (720 / 2) + 52;

    inicializar_inimigo(inimigo, TIPO_BOSS, spawn_x, spawn_y, boss_dir, boss_esq);
}

void atualizar_boss_perseguindo(Inimigo* boss, const Jogador* jogador, float distanciaParada) {
    if (!boss || !boss->ativo || boss->tipo != TIPO_BOSS) return;

    if (boss->botY + boss->alturaBot >= 720)
        boss->botY = 720 - boss->alturaBot;

    float distanciaX = jogador->jogadorX - boss->botX;
    float distanciaY = jogador->jogadorY - boss->botY;

    // Vetor de distancia
    float distanciaTotal = sqrtf(distanciaX * distanciaX + distanciaY * distanciaY);

    // Se ja esta muito perto, para de avancar (evita tremedeira)
    if (distanciaTotal <= distanciaParada) {
        boss->em_movimento = false;
        return;
    }

    // Normaliza direcao (evita divisao por zero) - RECOMENDACAO DO GPT
    float direcaoX = 0.0f;
    float direcaoY = 0.0f;
    if (distanciaTotal > 0.0001f) {
        direcaoX = distanciaX / distanciaTotal;
        direcaoY = distanciaY / distanciaTotal;
    }

    // Move em direcao ao jogador
    float velocidadeAtual = boss->velocidade;
    boss->botX += direcaoX * velocidadeAtual;
    boss->botY += direcaoY * velocidadeAtual;

    // Atualiza estado para animacao e orientacao do sprite
    boss->em_movimento = true;
    boss->virado_direita = (direcaoX >= 0.0f);

}