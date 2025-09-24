#include "personagem.h"

void mover(Jogador* p, bool w, bool a, bool s, bool d,  float velocidade) {
    if (w)
        p-> jogadorY -= velocidade;
    if (a)  
        p-> jogadorX -= velocidade;
    if (s)  
        p-> jogadorY += velocidade;
    if (d) 
        p-> jogadorX += velocidade;
}

void restringirPosicao(Jogador* p, float WIDTH, float HEIGHT, float larguraJogador, float alturaJogador) {
    if (p->jogadorX < 0) p->jogadorX = 0;
    if (p->jogadorY < 0) p->jogadorY = 0;
    if (p->jogadorY < (HEIGHT / 2) - alturaJogador )  p->jogadorY = (HEIGHT / 2) - alturaJogador;
    if (p->jogadorX > WIDTH - larguraJogador)  p->jogadorX = WIDTH - larguraJogador;
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