#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>
#include <stdbool.h>

void verificar_Input(ALLEGRO_EVENT event, bool* w, bool* a, bool* s, bool* d, bool* espaco, bool* shift);

#endif
