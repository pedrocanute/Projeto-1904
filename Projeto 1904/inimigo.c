#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "inimigo.h"
#include "personagem.h"

bool detectarColisao(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador) {
    if (!bot->ativo) //SE N:AO COLIDIU, RETORNA FALSO
        return false;

    float botEsquerda = bot->botX;
    float botDireita = bot->botX + bot->larguraBot;
    float botTopo = bot->botY;
    float botBaixo = bot->botY + bot->alturaBot;

    float jogadorEsquerda = jogador->jogadorX;
    float jogadorDireita = jogador->jogadorX + larguraJogador;
    float jogadorTopo = jogador->jogadorY;
    float jogadorBaixo = jogador->jogadorY + alturaJogador;

    if (botEsquerda < jogadorDireita && botDireita > jogadorEsquerda && botTopo < jogadorBaixo && botBaixo > jogadorTopo) {           

        return true;  // TODAS as condições são verdadeiras = COLIDIU!
    }
    else {
        return false; // ALGUMA condição é falsa = NÃO COLIDIU!
    }
}

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
    inimigo->vida = 2;
    
    //ATRIBUTOS DOS INIMIGOS
    switch (tipo) {
    case TIPO_ZUMBI:
        inimigo->larguraBot = 60.0f;
        inimigo->alturaBot = 80.0f;
        inimigo->velocidade = 2.5f;
        break;
    case TIPO_RATO:
        inimigo->larguraBot = 60.0f;
        inimigo->alturaBot = 50.0f;
        inimigo->velocidade = 3.5f;
        break;
    case TIPO_MOSQUITO:
        inimigo->larguraBot = 80.0f;
        inimigo->alturaBot = 80.0f;   
        inimigo->frames_por_sprite = 6;
        inimigo->velocidade = 4.5f;
        break;
    }
}

// SPAWNA INIMIGOS COM BASE NA CAMERA
void respawn_inimigo_na_camera(Inimigo* inimigo,
    ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq,
    ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq,
    ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq,
    float* posicaoCamera) {

    // SEMPRE PELA DIREITA
    float camera_direita = posicaoCamera[0] + 1280; 
    float spawn_x_min = camera_direita + 50;       
    float spawn_x_max = camera_direita + 300;       

    // SPAWNA ABAIXO DA CALÇADA
    float spawn_y_min = (720 / 2) + 52;  
    float spawn_y_max = 720 - 100;       

    float x = spawn_x_min + (float)(rand() % (int)(spawn_x_max - spawn_x_min));
    float y = spawn_y_min + (float)(rand() % (int)(spawn_y_max - spawn_y_min));

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

void inicializar_array_inimigos(Inimigo* inimigos, int quantidade,
    ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq,
    ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq,
    ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq,
    float* posicaoCamera) {

    srand((unsigned int)time(NULL));

    // Inicializar todos os inimigos usando spawn baseado na câmera
    for (int i = 0; i < quantidade; i++) {
        respawn_inimigo_na_camera(&inimigos[i],
            zumbi_dir, zumbi_esq,
            rato_dir, rato_esq,
            mosquito_dir, mosquito_esq,
            posicaoCamera);
    }
}

void atualizar_movimento_inimigos(Inimigo* inimigos, int quantidade,
    ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq,
    ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq,
    ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq,
    float* posicaoCamera) {

    float camera_esquerda = posicaoCamera[0]; // Limite esquerdo da câmera

    for (int i = 0; i < quantidade; i++) {
        if (!inimigos[i].ativo) {
            // Se inimigo está inativo (morreu), respawn na borda direita da câmera
            respawn_inimigo_na_camera(&inimigos[i], zumbi_dir, zumbi_esq, rato_dir, rato_esq, mosquito_dir, mosquito_esq, posicaoCamera);
            continue;
        }

        // INIMIGOS VAO PRA ESQURDA
        inimigos[i].botX -= inimigos[i].velocidade;

        // PASSOU DA ESQUERDA? RESPAWNA
        if (inimigos[i].botX + inimigos[i].larguraBot < camera_esquerda - 100) {
            respawn_inimigo_na_camera(&inimigos[i],
                zumbi_dir, zumbi_esq,
                rato_dir, rato_esq,
                mosquito_dir, mosquito_esq,
                posicaoCamera);
        }
    }
}

void desenhar_inimigo(Inimigo* inimigo, bool em_movimento) {
    if (!inimigo->ativo) //SE ESTA MORTO NAO DESRNHA
        return;

    ALLEGRO_BITMAP* sprite_atual = inimigo->sprite_esquerda;

    //LOOP DE FRAMES DA SPRITE
    if (em_movimento) {
        inimigo->contador_frame++;
        if (inimigo->contador_frame >= inimigo->frames_por_sprite) {
            inimigo->frame_atual = (inimigo->frame_atual + 1) % 2;
            inimigo->contador_frame = 0;
        }
    }

    int largura_frame = al_get_bitmap_width(sprite_atual) / 2;
    int altura_frame = al_get_bitmap_height(sprite_atual);
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
