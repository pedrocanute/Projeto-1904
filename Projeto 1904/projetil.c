#include "projetil.h"
#include "inimigo.h"

void atirar(ProjetilPosicao* pp, Jogador jogador, Inimigo bot, ALLEGRO_BITMAP* projetilDireita, ALLEGRO_BITMAP* projetilEsquerda, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int larguraJogador, int width, float projetilVelocidade, float projetilCadencia, float novaPosicaoX, float novaPosicaoY, float* posicaoCamera) {

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

    // CÁLCULO DOS LIMITES DA CÂMERA
    float cameraLeft = posicaoCamera[0];                    // Limite esquerdo
    float cameraRight = posicaoCamera[0] + width;          // Limite direito  
    float cameraTop = posicaoCamera[1];                     // Limite superior
    float cameraBottom = posicaoCamera[1] + 720;           // Limite inferior (altura da tela)

    // Margem extra para não cortar projéteis muito cedo
    float margem = 100.0f;

    // MOVER E VERIFICAR PROJÉTEIS
    for (int i = 0; i < 50; i++) {
        if (pp->projetilAtivo[i]) {
            // MOVER PROJÉTIL
            pp->projetilX[i] += pp->projetilDirecao[i];

            // DESENHAR PROJÉTIL (baseado na direção original, não atual)
            if (pp->projetilDirecao[i] > 0) {
                // Indo para direita
                al_draw_bitmap(projetilDireita, pp->projetilX[i], pp->projetilY[i], 0);
            }
            else {
                // Indo para esquerda  
                al_draw_bitmap(projetilEsquerda, pp->projetilX[i], pp->projetilY[i], 0);
            }

            // VERIFICAR COLISÃO COM INIMIGO
            if (pp->projetilX[i] + projetilLargura >= novaPosicaoX &&
                pp->projetilY[i] + projetilAltura >= novaPosicaoY &&
                pp->projetilX[i] <= novaPosicaoX + bot.larguraBot &&
                pp->projetilY[i] <= novaPosicaoY + bot.alturaBot) {
                pp->projetilAtivo[i] = false;
            }
            // VERIFICAR SE SAIU DOS LIMITES DA CÂMERA (com margem)
            else if (pp->projetilX[i] + projetilLargura < cameraLeft - margem ||     // Saiu pela esquerda
                pp->projetilX[i] > cameraRight + margem ||                      // Saiu pela direita
                pp->projetilY[i] + projetilAltura < cameraTop - margem ||       // Saiu por cima
                pp->projetilY[i] > cameraBottom + margem) {                     // Saiu por baixo
                pp->projetilAtivo[i] = false;
            }
        }
    }
}
