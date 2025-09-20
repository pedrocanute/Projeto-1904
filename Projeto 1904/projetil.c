#include "projetil.h"

void atirar(ProjetilPosicao* pp, Jogador jogador, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int width, float projetilVelocidade, float projetilCadencia) {

    const float projetilTimer = al_get_time();

    if (espaco && projetilTimer >= pp->proxProjetil) {
        for (int i = 0; i < 50; i++) {
            if (!pp->qtdProjetil[i]) {
                pp->projetilX[i] = jogador.jogadorX;
                pp->projetilY[i] = jogador.jogadorY + (alturaJogador / 2) - (projetilAltura / 2);
                pp->qtdProjetil[i] = true;
                pp->proxProjetil = projetilTimer + projetilCadencia;
                break;
            }
        }
    }

    for (int i = 0; i < 50; i++) {
        if (pp->qtdProjetil[i]) {
            pp->projetilX[i] += projetilVelocidade;
            al_draw_filled_rectangle(pp->projetilX[i], pp->projetilY[i], pp->projetilX[i] + projetilLargura, pp->projetilY[i] + projetilAltura, al_map_rgb(0, 0, 0));
            if (pp->projetilX[i] > width) {
                pp->qtdProjetil[i] = false;
            }
        }
    }
}