#include "projetil.h"
#include "inimigo.h"

void atirar_multiplos_inimigos(ProjetilPosicao* pp, Jogador jogador, Inimigo* inimigos, int numInimigos, ALLEGRO_BITMAP* projetilDireita, ALLEGRO_BITMAP* projetilEsquerda, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int larguraJogador, int width, float projetilVelocidade, float projetilCadencia, float* posicaoCamera) {

    const float projetilTimer = al_get_time();

    // CRIAR PROJÉTEIS
    if (espaco && projetilTimer >= pp->proxProjetil) {
        for (int i = 0; i < 50; i++) {
            if (pp->projetilAtivo[i] == false) {
                pp->projetilX[i] = jogador.jogadorX;
                pp->projetilY[i] = jogador.jogadorY + (alturaJogador / 2) - (projetilAltura / 2);
                pp->projetilAtivo[i] = true;

                if (jogador.paraDireita) {
                    pp->projetilDirecao[i] = +projetilVelocidade;
                }
                else if (jogador.paraEsquerda) {
                    pp->projetilDirecao[i] = -projetilVelocidade;
                }

                pp->proxProjetil = projetilTimer + projetilCadencia;
                break;
            }
        }
    }

    // MOVER E VERIFICAR PROJÉTEIS
    for (int i = 0; i < 50; i++) {
        if (pp->projetilAtivo[i]) {
            pp->projetilX[i] += pp->projetilDirecao[i];

            // DESENHAR PROJÉTIL
            if (pp->projetilDirecao[i] > 0) {
                al_draw_bitmap(projetilDireita, pp->projetilX[i], pp->projetilY[i], 0);
            }
            else {
                al_draw_bitmap(projetilEsquerda, pp->projetilX[i], pp->projetilY[i], 0);
            }

            // VERIFICAR COLISÃO COM TODOS OS INIMIGOS
            bool colidiu = false;
            for (int j = 0; j < numInimigos; j++) {
                if (!inimigos[j].ativo) continue;

                if (pp->projetilX[i] + projetilLargura >= inimigos[j].botX && pp->projetilY[i] + projetilAltura >= inimigos[j].botY && pp->projetilX[i] <= inimigos[j].botX + inimigos[j].larguraBot && pp->projetilY[i] <= inimigos[j].botY + inimigos[j].alturaBot) {

                    // SISTEMA DE VIDA
                    inimigos[j].vida--;

                    // Só mata se a vida chegar a 0
                    if (inimigos[j].vida <= 0) {
                        inimigos[j].ativo = false;
                    }

                    pp->projetilAtivo[i] = false; // Projétil desaparece
                    colidiu = true;
                    break;
                }
            }

            if (colidiu) {
                pp->projetilAtivo[i] = false;
            }
            // VERIFICA LIMITES DA CAMERA
            else if (pp->projetilX[i] < posicaoCamera[0] - 100 || pp->projetilX[i] > posicaoCamera[0] + width + 100) {
                pp->projetilAtivo[i] = false;
            }
        }
    }
}
