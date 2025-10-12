#include "personagem.h"

void mover(Jogador* p, bool w, bool a, bool s, bool d, bool shift, float velocidade, int* frames_por_sprite) {
    float velocidade_caminhada = velocidade;
    float velocidade_corrida = velocidade * 1.5;

    if (w) {
        if (shift) {
            p->jogadorY -= velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorY -= velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }

    if (a) {
        p->paraEsquerda = true;
        p->paraDireita = false;

        if (shift) {
            p->jogadorX -= velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorX -= velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }

    if (s) {
        if (shift) {
            p->jogadorY += velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorY += velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }

    if (d) {
        p->paraDireita = true;
        p->paraEsquerda = false;

        if (shift) {
            p->jogadorX += velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorX += velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }
}

void restringirPosicao(Jogador* p, Caravana* caravana, float WIDTH, float HEIGHT, float larguraJogador, float alturaJogador) {
    if (p->jogadorX < caravana->caravanaX + caravana->caravanaLargura) 
        p->jogadorX = caravana->caravanaX + caravana->caravanaLargura;
    if (p->jogadorY < 0) 
        p->jogadorY = 0;

    if (p->jogadorY < (HEIGHT / 2) - alturaJogador + 52) 
        p->jogadorY = (HEIGHT / 2) - alturaJogador + 52;
    if (p->jogadorY > HEIGHT - alturaJogador) 
        p->jogadorY = HEIGHT - alturaJogador;
}

void desenhar_jogador(Jogador jogador, bool w, bool a, bool s, bool d, bool espaco, ALLEGRO_BITMAP* sprite_direita, ALLEGRO_BITMAP* sprite_esquerda, ALLEGRO_BITMAP* sprite_atirando_direita, ALLEGRO_BITMAP* sprite_atirando_esquerda, int* frame_atual, int* contador_frame, int frames_por_sprite, bool* virado_direita, int* frame_tiro, int* contador_frame_tiro) {

    // Determina direção baseado no movimento
    if (d || (d && s) || (d && w)) {
        *virado_direita = true;
    }
    else if (a || (a && s) || (a && w)) {
        *virado_direita = false;
    }

    // ESCOLHER SPRITE E SISTEMA DE ANIMAÇÃO
    ALLEGRO_BITMAP* sprite_atual;
    bool esta_atirando = espaco;
    bool esta_em_movimento = (a || d || s || w);

    if (esta_atirando) {
        // USANDO SPRITES DE TIRO - 3 COLUNAS do SPRiTESHEET
        if (*virado_direita) {
            sprite_atual = sprite_atirando_direita;
        }
        else {
            sprite_atual = sprite_atirando_esquerda;
        }

        // ANIMAÇÃO DE TIRO - SINCRONIZADA COM CADENCIA
        (*contador_frame_tiro)++;
        if (*contador_frame_tiro >= 4) {  // 4 frames = mantém sincronia com cadência
            *frame_tiro = (*frame_tiro + 1) % 3; 
            *contador_frame_tiro = 0;
        }

        // Calcula coordenadas usando 3 colunas do spritesheet
        int largura_frame = al_get_bitmap_width(sprite_atual) / 3; 
        int altura_frame = al_get_bitmap_height(sprite_atual);
        int sx = *frame_tiro * largura_frame;
        int sy = 0;

        al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame,
            jogador.jogadorX, jogador.jogadorY, 0);
    }
    else {
        // USANDO SPRITES NORMAIS DE CAMINHADA
        if (*virado_direita) {
            sprite_atual = sprite_direita;
        }
        else {
            sprite_atual = sprite_esquerda;
        }

        // ANIMAÇÃO NORMAL DE CAMINHADA (2 colunas)
        if (esta_em_movimento) {
            (*contador_frame)++;
            if (*contador_frame >= frames_por_sprite) {
                *frame_atual = (*frame_atual + 1) % 2; 
                *contador_frame = 0;
            }
        }
        else {
            // Parado - primeiro frame
            *frame_atual = 0;
            *contador_frame = 1;
        }

        // Calcula coordenadas usando 2 colunas para caminhada
        int largura_frame = al_get_bitmap_width(sprite_atual) / 2; // 2 colunas para caminhada
        int altura_frame = al_get_bitmap_height(sprite_atual);
        int sx = *frame_atual * largura_frame;
        int sy = 0;

        al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame,
            jogador.jogadorX, jogador.jogadorY, 0);
    }
}

void camera_jogador(float* posicaoCamera, Jogador jogador, int larguraTela, int larguraJogador, int alturaJogador) {
    posicaoCamera[0] = jogador.jogadorX - (larguraTela / 2) + (larguraJogador / 2);

    if (posicaoCamera[0] < 0)
        posicaoCamera[0] = 0;
}
