#include "caravana.h"

void desenhar_caravana(float caravanaX, float caravanaY, float caravanaLargura, float caravanaAltura, ALLEGRO_COLOR corCaravana) {
    al_draw_filled_rectangle(caravanaX, caravanaY, caravanaX + caravanaLargura, caravanaAltura, corCaravana);
};

void atualizar_movimento_caravana(Caravana* caravana) {
    caravana->caravanaX += caravana->caravanaVelocidade;
};