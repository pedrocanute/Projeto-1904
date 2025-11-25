#include "input.h"
#include <stdio.h>

void verificar_Input(ALLEGRO_EVENT event, bool* w, bool* a, bool* s, bool* d, bool* espaco, bool* shift, bool* esc, bool* seta_cima, bool* seta_direita, bool* seta_baixo, bool* seta_esquerda, bool* r) {
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_W:
            *w = true;
            break;
        case ALLEGRO_KEY_S:
            *s = true;
            break;
        case ALLEGRO_KEY_A:
            *a = true;
            break;
        case ALLEGRO_KEY_D:
            *d = true;
            break;
        case ALLEGRO_KEY_SPACE:
            *espaco = true;
            break;
        case ALLEGRO_KEY_LSHIFT:
            *shift = true;
            break;
        case ALLEGRO_KEY_ESCAPE:
            *esc = true;
            break;
        case ALLEGRO_KEY_R:
            *r = true;
            break;
        case ALLEGRO_KEY_UP:
            *seta_cima = true;
            break;
        case ALLEGRO_KEY_RIGHT:
            *seta_direita = true;
            break;
        case ALLEGRO_KEY_DOWN:
            *seta_baixo = true;
            break;
        case ALLEGRO_KEY_LEFT:
            *seta_esquerda = true;
            break;
        }
    }
    else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_W:
            *w = false;
            break;
        case ALLEGRO_KEY_S:
            *s = false;
            break;
        case ALLEGRO_KEY_A:
            *a = false;
            break;
        case ALLEGRO_KEY_D:
            *d = false;
            break;
        case ALLEGRO_KEY_SPACE:
            *espaco = false;
            break;
        case ALLEGRO_KEY_LSHIFT:
            *shift = false;
            break;
        case ALLEGRO_KEY_ESCAPE:
            *esc = false;
            break;
        case ALLEGRO_KEY_R:
            *r = false;
            break;
        case ALLEGRO_KEY_UP:
            *seta_cima = false;
            break;
        case ALLEGRO_KEY_RIGHT:
            *seta_direita = false;
            break;
        case ALLEGRO_KEY_DOWN:
            *seta_baixo = false;
            break;
        case ALLEGRO_KEY_LEFT:
            *seta_esquerda = false;
            break;
        }
    }
}