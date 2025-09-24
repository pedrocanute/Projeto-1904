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


void restringirPosicao(Jogador* p, float WIDTH, float HEIGHT, float larguraJogador, float alturaJogador) {
    if (p->jogadorX < 0) p->jogadorX = 0;
    if (p->jogadorY < 0) p->jogadorY = 0;
    if (p->jogadorY < (HEIGHT / 2) - alturaJogador + 52)  p->jogadorY = (HEIGHT / 2) - alturaJogador + 52;
    if (p->jogadorY > HEIGHT - alturaJogador)   p->jogadorY = HEIGHT - alturaJogador;
}

void desenhar_jogador(Jogador jogador, bool w, bool a, bool s, bool d, ALLEGRO_BITMAP* sprite_direita,
    ALLEGRO_BITMAP* sprite_esquerda, int* frame_atual, int* contador_frame,
    int frames_por_sprite, bool* virado_direita) {

    // Determina qual sprite usar baseado na tecla pressionada
    if (d || (d && s) || (d && w)) {
        *virado_direita = true;
    }
    else if (a || (a && s) || (a && w)) {
        *virado_direita = false;
    }

    ALLEGRO_BITMAP* sprite_atual = sprite_direita;
    if (*virado_direita) {
        sprite_atual = sprite_direita;
    }
    else {
        sprite_atual = sprite_esquerda;
    }

    // Atualiza a animação apenas se estiver se movendo
    if (a || d || (sprite_atual == sprite_direita && s) || (sprite_atual == sprite_direita && w) || (sprite_atual == sprite_esquerda && s) || sprite_atual == sprite_esquerda && w) {
        (*contador_frame)++;
        if (*contador_frame >= frames_por_sprite) {
            *frame_atual = (*frame_atual + 1) % 2; // 2 colunas no spritesheet
            *contador_frame = 0;
        }
    }
    else {
        // Parado - usa o primeiro frame
        *frame_atual = 0;
        *contador_frame = 1;
    }

    // Calcula as coordenadas do frame no spritesheet
    int largura_frame = al_get_bitmap_width(sprite_atual) / 2; // 2 colunas
    int altura_frame = al_get_bitmap_height(sprite_atual);
    int sx = *frame_atual * largura_frame;
    int sy = 0;

    // Desenha o sprite
    al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame,
        jogador.jogadorX, jogador.jogadorY, 0);

}

void camera_jogador(float* posicaoCamera, Jogador jogador, int larguraTela, int larguraJogador, int alturaJogador) {

    posicaoCamera[0] = jogador.jogadorX - (larguraTela / 2) + (larguraJogador / 2);

    if (posicaoCamera[0] < 0)
        posicaoCamera[0] = 0;
}