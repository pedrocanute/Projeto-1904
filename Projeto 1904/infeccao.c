#include "infeccao.h"

void desenhar_barra(float infeccaoX, float infeccaoY, float infeccaoLargura, float infeccaoAltura) {
    al_draw_filled_rectangle(infeccaoX, infeccaoY, infeccaoLargura, infeccaoAltura, al_map_rgb(0, 0, 0));
}

void desenhar_barra_infeccao(float infeccaoX, float infeccaoY, float infeccaoLargura, float infeccaoAltura, float* posicaoCamera){
    al_draw_filled_rectangle(infeccaoX + posicaoCamera[0], infeccaoY, infeccaoLargura + posicaoCamera[0], infeccaoAltura, al_map_rgb(0, 255, 0));
}

void desenhar_fundo_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura) {
    al_draw_filled_rectangle(vidaX, vidaY, vidaLargura, vidaAltura, al_map_rgb(0, 0, 0));
}

void desenhar_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura) {
    al_draw_filled_rectangle(vidaX, vidaY, vidaLargura, vidaAltura, al_map_rgb(255, 0, 0));
}
