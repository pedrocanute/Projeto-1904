#include "caravana.h"

void desenhar_caravana(float caravanaX, float caravanaY, float caravanaLargura, float caravanaAltura, ALLEGRO_COLOR corCaravana) {
    al_draw_filled_rectangle(caravanaX, caravanaY, caravanaLargura, caravanaAltura, corCaravana);
};