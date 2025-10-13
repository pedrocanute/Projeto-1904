#include "caravana.h"

void desenhar_caravana(ALLEGRO_BITMAP* sprite_soldado, float caravanaX, float caravanaY, float caravanaLargura, float caravanaAltura, ALLEGRO_COLOR corCaravana) {
    static int frame_atual = 0;
    static int contador = 0;
    const int velocidade_animacao = 20;
    int largura = al_get_bitmap_width(sprite_soldado);
    int altura = al_get_bitmap_height(sprite_soldado);
    int largura_frame = (largura / 2) + 12;
    int altura_frame = altura;

        contador++;
        if (contador >= velocidade_animacao) {
            frame_atual = (frame_atual + 1) % 2;
            contador = 0;
        }
        int sx = frame_atual * largura_frame;
        int sy = 0;
        al_draw_tinted_bitmap_region(sprite_soldado, corCaravana, (float)sx, sy, (float)largura_frame, altura_frame, caravanaX, caravanaY, 0);
        al_draw_tinted_bitmap_region(sprite_soldado, corCaravana, (float)sx, sy, (float)largura_frame, altura_frame, caravanaX, caravanaY + 104, 0);
        al_draw_tinted_bitmap_region(sprite_soldado, corCaravana, (float)sx, sy, (float)largura_frame, altura_frame, caravanaX, caravanaY + 208, 0);
        al_draw_tinted_bitmap_region(sprite_soldado, corCaravana, (float)sx, sy, (float)largura_frame, altura_frame, caravanaX, caravanaY + 312, 0);
}

void atualizar_movimento_caravana(Caravana* caravana) {
    caravana->caravanaX += caravana->caravanaVelocidade;
};