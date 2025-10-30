#include "caravana.h"

void desenhar_caravana(ALLEGRO_BITMAP* sprite_soldado, float caravanaX, float caravanaY, float caravanaLargura, float caravanaAltura, ALLEGRO_COLOR corCaravana) {
    static int frame_atual = 0;
    static int contador = 0;
    const int velocidade_animacao = 20;
    int largura = al_get_bitmap_width(sprite_soldado);
    int altura = al_get_bitmap_height(sprite_soldado);
    float largura_frame = (largura / 2) + 12;
    float altura_frame = altura;

        contador++;
        if (contador >= velocidade_animacao) {
            frame_atual = (frame_atual + 1) % 2;
            contador = 0;
        }
        float sx = frame_atual * largura_frame;
        float sy = 0;
        al_draw_tinted_bitmap_region(sprite_soldado, corCaravana, sx, sy, largura_frame, altura_frame, caravanaX, caravanaY, 0);
        al_draw_tinted_bitmap_region(sprite_soldado, corCaravana, sx, sy, largura_frame, altura_frame, caravanaX, caravanaY + 104, 0);
        al_draw_tinted_bitmap_region(sprite_soldado, corCaravana, sx, sy, largura_frame, altura_frame, caravanaX, caravanaY + 208, 0);
        al_draw_tinted_bitmap_region(sprite_soldado, corCaravana, sx, sy, largura_frame, altura_frame, caravanaX, caravanaY + 312, 0);
}

void atualizar_movimento_caravana(Caravana* caravana) {
    caravana->caravanaX += caravana->caravanaVelocidade;
};