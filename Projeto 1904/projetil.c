#include "projetil.h"

void atirar(ProjetilPosicao* pp, Posicao jogador, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int width, float projetilVelocidade) {
    if (espaco && !pp->projetilAtivo) {
        pp-> projetilX = jogador.jogadorX;
        pp-> projetilY = jogador.jogadorY + (alturaJogador/2) - (projetilAltura/2);
        pp-> projetilAtivo = true;
    }

    if (pp-> projetilAtivo) {
        pp-> projetilX += projetilVelocidade;

        al_draw_filled_rectangle(pp-> projetilX, pp-> projetilY, pp-> projetilX + projetilLargura, pp-> projetilY + projetilAltura, al_map_rgb(0, 0, 0));

        if (pp-> projetilX > width)
            pp-> projetilAtivo = false;
    }
}
