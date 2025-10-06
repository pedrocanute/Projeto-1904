#ifndef CARAVANA_H
#define CARAVANA_H

#include <stdbool.h>
#include <allegro5/allegro.h>

#include <allegro5/allegro_primitives.h>

typedef struct {
    float caravanaX;
    float caravanaY;
    float caravaLargura;
    float caravaAltura;
} Caravana;

void desenhar_caravana(float caravanaX, float caravanaY, float caravanaLargura, float caravanaAltura, ALLEGRO_COLOR corCaravana);

#endif

