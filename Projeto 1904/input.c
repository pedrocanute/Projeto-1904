#include "input.h"

void verificar_Input(ALLEGRO_EVENT event, bool* w, bool* a, bool* s, bool* d, bool* espaco, bool* shift, bool* esc) {
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
            if (*esc == false) {
                *esc = true;
            }
            else if (*esc == true) {
                *esc = false;
            }
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
        }
    }
}