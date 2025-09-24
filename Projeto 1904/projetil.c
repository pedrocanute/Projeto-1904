#include "projetil.h"
#include "inimigo.h"

void atirar(ProjetilPosicao* pp, Jogador jogador, Inimigo bot, ALLEGRO_BITMAP* projetilDireita, ALLEGRO_BITMAP* projetilEsquerda, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int larguraJogador, int width, float projetilVelocidade, float projetilCadencia, float novaPosicaoX, float novaPosicaoY) {

    const float projetilTimer = al_get_time();

    // CRIAR PROJÉTEIS
    if (espaco && projetilTimer >= pp->proxProjetil) {
        for (int i = 0; i < 50; i++) {
            // A PARTIR DO XY DO JOGADOR
            if (pp->projetilAtivo[i] == false) {
                pp->projetilX[i] = jogador.jogadorX;
                pp->projetilY[i] = jogador.jogadorY + (alturaJogador / 2) - (projetilAltura / 2);
                pp->projetilAtivo[i] = true;

                // DIRECIONAR PROJÉTIL
                if (jogador.paraDireita) {
                    pp->projetilDirecao[i] = +projetilVelocidade;
                }
                else if (jogador.paraEsquerda) {
                    pp->projetilDirecao[i] = -projetilVelocidade;
                }

                // INTERVALO 
                pp->proxProjetil = projetilTimer + projetilCadencia;
                break;
            }
        }
    }

    // MOVER PROJETEIS
    for (int i = 0; i < 50; i++) {
        // PARA DIREITA
        if (pp->projetilAtivo[i] && jogador.paraDireita) {
            pp->projetilX[i] += pp->projetilDirecao[i];
            al_draw_bitmap(projetilDireita, pp->projetilX[i], pp->projetilY[i], 0);
        }
        // PARA ESQUERDA
        else if (pp->projetilAtivo[i] && jogador.paraEsquerda) {
             pp->projetilX[i] += pp->projetilDirecao[i];
             al_draw_bitmap(projetilEsquerda, pp->projetilX[i], pp->projetilY[i], 0);
        }
        // COLIDIU INIMIGO
        if (pp->projetilX[i] + projetilLargura >= novaPosicaoX && pp->projetilY[i] + projetilAltura >= novaPosicaoY && pp->projetilX[i] + projetilLargura <= novaPosicaoX + bot.larguraBot && pp->projetilY[i] + projetilAltura <= novaPosicaoY + bot.alturaBot) {
                pp->projetilAtivo[i] = false;
        }
        // SAIU DA TELA
        else if (pp->projetilX[i] > width) {
                pp->projetilAtivo[i] = false;
        }
    }
}