#include "projetil.h"
#include "fases.h"
#include <math.h>

void atirar_multiplos_inimigos(ProjetilPosicao* pp, Jogador jogador, Inimigo* inimigos, int numInimigos, Bitmaps* bitmap, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int larguraJogador, int width, float projetilVelocidade, float projetilCadencia, float* posicaoCamera, SistemaFases* sistemaFase, BarraBoss* barraVidaBoss, SistemaSom* sons) {

    if (pp->tipo == ARMA_VASSOURA) {
        ataque_corpo_a_corpo(pp, jogador, inimigos, numInimigos, espaco, larguraJogador, alturaJogador, sistemaFase, barraVidaBoss, sons);
    }

    const float projetilTimer = al_get_time();

    // CRIAR PROJÉTEIS
    if (espaco && projetilTimer >= pp->proxProjetil) {
        for (int i = 0; i < 50; i++) {
            if (!pp->projetilAtivo[i]) {
                // Toca o som apropriado para cada arma
                if (pp->tipo == ARMA_VACINA) {
                    tocarSomTiro(sons);
                }

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

            // VERIFICA COLISÃO COM TODOS OS INIMIGOS 
            bool colidiu = false;
            for (int j = 0; j < numInimigos; j++) {
                if (!inimigos[j].ativo) continue;

                // Verificação de tipo de projétil e inimigo
                bool podeAtingir = false;
                switch (pp->tipoProjetil[i]) {
                case ARMA_VACINA:
                    podeAtingir = (inimigos[j].tipo == TIPO_ZUMBI || inimigos[j].tipo == TIPO_BOSS);
                    break;
                case ARMA_VENENO:
                    podeAtingir = (inimigos[j].tipo == TIPO_MOSQUITO || inimigos[j].tipo == TIPO_BOSS_MOSQUITO);
                    break;
                    // Vassoura não dispara projétil, só corpo a corpo
                default:
                    podeAtingir = false;
                    break;
                }

                if (!podeAtingir) continue;

                if (colisao_projetil_inimigo(pp->projetilX[i], pp->projetilY[i], pp->larguraProjetil[i], pp->alturaProjetil[i], &inimigos[j])) {

                    // SISTEMA DE VIDA E "MORTE" 
                    inimigos[j].vida--;
                    if (inimigos[j].vida <= 0) {
                        parar_som_inimigo(&inimigos[j]); // Para o som antes de desativar
                        inimigos[j].ativo = false;
                        sistemaFase->inimigosMortos++;
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

    arma->ataqueCorpoACorpoAtivo = false;
    arma->tempoInicioAtaque = 0.0f;
    arma->duracaoAtaque = 0.3f;

    for (int k = 0; k < 20; k++) {
        arma->inimigosAtingidos[k] = false;
    }

    for (int i = 0; i < 50; i++) {
        arma->projetilAtivo[i] = false;
        arma->projetilX[i] = 0.0f;
        arma->projetilY[i] = 0.0f;
        arma->projetilDirecao[i] = 0.0f;

        arma->tipoProjetil[i] = tipo_arma;
        arma->frameProjetil[i] = 0;
        arma->contadorFrameProjetil[i] = 0;
        arma->posicaoInicialX[i] = 0.0f;
        arma->larguraProjetil[i] = 16;
        arma->alturaProjetil[i] = 16;
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

void ataque_corpo_a_corpo(ProjetilPosicao* pp, Jogador jogador, Inimigo* inimigos, int numInimigos, bool espaco, int larguraJogador, int alturaJogador, SistemaFases* sistemaFase, BarraBoss* barraVidaBoss, SistemaSom* sons) {

    const float tempoAtual = al_get_time();

    if (espaco && tempoAtual >= pp->proxProjetil && !pp->ataqueCorpoACorpoAtivo) {
        float cadencia;
        float velocidade;
        configuracoes_armas(pp->tipo, &velocidade, &cadencia);

        pp->ataqueCorpoACorpoAtivo = true;
        pp->tempoInicioAtaque = tempoAtual;
        pp->duracaoAtaque = 0.3f;
        pp->proxProjetil = tempoAtual + cadencia;

        for (int k = 0; k < 20; k++) {  // MAX_INIMIGOS
            pp->inimigosAtingidos[k] = false;
        }
    }

    if (pp->ataqueCorpoACorpoAtivo) {
        if ((tempoAtual - pp->tempoInicioAtaque) >= pp->duracaoAtaque) {
            pp->ataqueCorpoACorpoAtivo = false;
            return;
        }

        //HITBOX DO ATAQUE
        float ataqueX, ataqueY, ataqueLargura, ataqueAltura;

        if (jogador.paraDireita) {
            ataqueX = jogador.jogadorX + larguraJogador;
            ataqueY = jogador.jogadorY + (alturaJogador * 0.3f);
            ataqueLargura = 80.0f; //ALCANCE
            ataqueAltura = alturaJogador * 0.5f;
        }
        else if (jogador.paraEsquerda) {
            ataqueX = jogador.jogadorX - 60.0f;
            ataqueY = jogador.jogadorY + (alturaJogador * 0.3f);
            ataqueLargura = 80.0f;
            ataqueAltura = alturaJogador * 0.5f;
        }
        else {
            // Parado - ataque para frente da última direção
            ataqueX = jogador.jogadorX + larguraJogador;
            ataqueY = jogador.jogadorY + (alturaJogador * 0.3f);
            ataqueLargura = 80.0f;
            ataqueAltura = alturaJogador * 0.5f;
        }

        //VERIFICAR COLISAO
        for (int j = 0; j < numInimigos; j++) {
            if (!inimigos[j].ativo) continue;

            if (pp->inimigosAtingidos[j]) continue;

            // Só atinge ratos
            if (inimigos[j].tipo != TIPO_RATO && inimigos[j].tipo != TIPO_BOSS_RATO) continue;

            if (colisao_aabb(ataqueX, ataqueY, ataqueLargura, ataqueAltura, inimigos[j].botX, inimigos[j].botY, inimigos[j].larguraBot, inimigos[j].alturaBot)) {

                pp->inimigosAtingidos[j] = true;

                // Boss recebe mais dano que inimigos normais
                int dano = 1;
                if (inimigos[j].tipo == TIPO_BOSS_RATO) {
                    dano = 3;  // Boss Rato recebe 3 de dano (30 vida / 3 = 10 ataques = 5 segundos)
                }

                inimigos[j].vida -= dano;

                if (inimigos[j].vida <= 0) {
                    parar_som_inimigo(&inimigos[j]); // Para o som antes de desativar
                    inimigos[j].ativo = false;
                    sistemaFase->inimigosMortos++;
                }
            }
        }
    }
}