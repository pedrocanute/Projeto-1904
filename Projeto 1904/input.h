#ifndef INPUT_H
#define INPUT_H

#include <allegro5/allegro.h>
#include <stdbool.h>

void verificar_Input(ALLEGRO_EVENT event, bool* w, bool* a, bool* s, bool* d, bool* espaco, bool* shift, bool* esc, bool* num1, bool* num2, bool* num3, bool* seta_cima, bool* seta_direita, bool* seta_baixo, bool* seta_esquerda);

#endif
