#include "projetil.h"
#include "fases.h"
#include <math.h>

void atirar_multiplos_inimigos(ProjetilPosicao* pp, Jogador jogador, Inimigo* inimigos, int numInimigos, Bitmaps *bitmap, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int larguraJogador, int width, float projetilVelocidade, float projetilCadencia, float* posicaoCamera, SistemaFases* sistemaFase, BarraBoss* barraVidaBoss) {

    const float projetilTimer = al_get_time();

    // CRIAR PROJÉTEIS
    if (espaco && projetilTimer >= pp->proxProjetil) {
        for (int i = 0; i < 50; i++) {
            if (!pp->projetilAtivo[i]) {
                pp->tipoProjetil[i] = pp->tipo;
                pp->frameProjetil[i] = 0;
                pp->contadorFrameProjetil[i] = 0;

                int larguraProj, alturaProj;

                switch (pp->tipo) {
                case ARMA_VENENO:
                    larguraProj = 104;
                    alturaProj = 72;
                    break;

                case ARMA_VACINA:
                    larguraProj = 27;
                    alturaProj = 15;
                    break;

                case ARMA_VASSOURA:
                default:
                    larguraProj = 16;
                    alturaProj = 16;
                    break;
                }

                pp->larguraProjetil[i] = larguraProj;
                pp->alturaProjetil[i] = alturaProj;

                if (jogador.paraDireita) {
                    pp->projetilX[i] = jogador.jogadorX + larguraJogador;
                }
                else if (jogador.paraEsquerda) {
                    pp->projetilX[i] = jogador.jogadorX - larguraProj;
                }
                else {
                    pp->projetilX[i] = jogador.jogadorX + (larguraJogador / 2);
                }

                pp->projetilY[i] = jogador.jogadorY + (alturaJogador / 2) - (alturaProj / 2);

                pp->projetilAtivo[i] = true;
                pp->posicaoInicialX[i] = pp->projetilX[i];

                float velocidadeArma;
                float cadenciaArma;
                configuracoes_armas(pp->tipo, &velocidadeArma, &cadenciaArma);

                if (jogador.paraDireita)
                    pp->projetilDirecao[i] = +velocidadeArma;
                else if (jogador.paraEsquerda)
                    pp->projetilDirecao[i] = -velocidadeArma;

                pp->proxProjetil = projetilTimer + cadenciaArma;
                break;
            }
        }
    }

    // MOVER E VERIFICAR PROJÉTEIS
    for (int i = 0; i < 50; i++) {
        if (pp->projetilAtivo[i]) {
            
            pp->projetilX[i] += pp->projetilDirecao[i];

            float distanciaPercorrida = fabsf(pp->projetilX[i] - pp->posicaoInicialX[i]);

            ALLEGRO_BITMAP* spriteProjetil = NULL;
            int espelhar = 0;
            int frameAtual = 0;

            switch (pp->tipoProjetil[i]) {
                case ARMA_VASSOURA:
                    break;

                case ARMA_VENENO:
                    spriteProjetil = bitmap->projetil_veneno;

                    pp->contadorFrameProjetil[i]++;
                    if (pp->contadorFrameProjetil[i] >= 10) { //VELOCIDADE ANIMACAO VENENO

                        if (pp->frameProjetil[i] < 2)
                            pp->frameProjetil[i]++;

                        pp->contadorFrameProjetil[i] = 0;
                    }
                    frameAtual = pp->frameProjetil[i];

                    if (pp->projetilDirecao[i] < 0)
                        espelhar = ALLEGRO_FLIP_HORIZONTAL;
                    break;

                case ARMA_VACINA:
                    if (pp->projetilDirecao[i] > 0) {
                        spriteProjetil = bitmap->projetilDireita;
                    }
                    else {
                        spriteProjetil = bitmap->projetilEsquerda;
                    }
                    espelhar = 0;
                    frameAtual = 0;
                    break;
                }

            // DESENHAR PROJÉTIL
            if (spriteProjetil != NULL)
                if (pp->tipoProjetil[i] == ARMA_VENENO) {

                    int larguraTotal = al_get_bitmap_width(spriteProjetil);
                    int alturaTotal = al_get_bitmap_height(spriteProjetil);
                    int larguraFrame = larguraTotal / 3;
                    int sx = frameAtual * larguraFrame;
                    int sy = 0;

                    al_draw_bitmap_region(spriteProjetil, sx, sy, larguraFrame, alturaTotal, pp->projetilX[i], pp->projetilY[i], espelhar);
                }
                else {
                    al_draw_bitmap(spriteProjetil, pp->projetilX[i], pp->projetilY[i], espelhar);
                }

            //ALCANCE DO PROJETIL (VENENO)
            if ((pp->tipoProjetil[i] == ARMA_VENENO) && (distanciaPercorrida >= 500.0f)) {
                pp->projetilAtivo[i] = false;
                continue;
            }

            // VERIFICAR COLISÃO COM TODOS OS INIMIGOS 
            bool colidiu = false;
            for (int j = 0; j < numInimigos; j++) {
                if (!inimigos[j].ativo) continue;

                if (colisao_projetil_inimigo(pp->projetilX[i], pp->projetilY[i], pp->larguraProjetil[i],pp->alturaProjetil[i], &inimigos[j])) {
                    
                    // SISTEMA DE VIDA E “MORTE” 
                    inimigos[j].vida--;
                    if (inimigos[j].vida <= 0) {
                        
                        inimigos[j].ativo = false;
                        sistemaFase->inimigosMortos++;
                    }

                    // Atualize a barra de vida do boss apenas se o inimigo for o boss
                    if (inimigos[j].tipo == TIPO_BOSS) {
                        barraVidaBoss->barraVida -= 6;
                    }

                    pp->projetilAtivo[i] = false;
                    colidiu = true;
                    break;
                }
            }

            if (colidiu) {
                pp->projetilAtivo[i] = false;
            }
            // VERIFICA LIMITES DA CÂMERA
            else if (pp->projetilX[i] < posicaoCamera[0] - 100 || pp->projetilX[i] > posicaoCamera[0] + width + 100) {
                pp->projetilAtivo[i] = false;
            }
        }
    }
}

void inicializar_armas(ProjetilPosicao* arma, Arma tipo_arma, float x, float y, ALLEGRO_BITMAP* sprite_esq, ALLEGRO_BITMAP* sprite_dir) {
    
    arma->tipo = tipo_arma;
    arma->proxProjetil = 0.0f;

    for (int i = 0; i < 50; i++) {
        arma->projetilAtivo[i] = false;
        arma->projetilX[i] = 0.0f;
        arma->projetilY[i] = 0.0f;
        arma->projetilDirecao[i] = 0.0f;

        arma->tipoProjetil[i] = tipo_arma;
        arma->frameProjetil[i] = 0;
        arma->contadorFrameProjetil[i] = 0;
        arma->posicaoInicialX[i] = 0.0f;
    }
}

void trocar_arma(ProjetilPosicao* pp, Arma nova_arma) {
    pp->tipo = nova_arma;
}

void configuracoes_armas(Arma tipo, float* velocidade, float* cadencia) {

    //A VELOCIDADE DA ANIMACAO DEVE SER AJUSTADA EM desenhar_jogador EM PERSONAGEM.C
    switch (tipo) {
    case ARMA_VASSOURA:
        *velocidade = 0.0f;
        *cadencia = 0.5f;
        break;
    case ARMA_VENENO:
        *velocidade = 4.0f;
        *cadencia = 0.5f;
        break;
    case ARMA_VACINA:
        *velocidade = 15.5f;
        *cadencia = 0.2f;
        break;
    default:
        *velocidade = 8.0f;
        *cadencia = 0.3f;
        break;
    }
}
