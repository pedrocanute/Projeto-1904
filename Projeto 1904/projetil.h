#ifndef PROJETIL_H
#define PROJETIL_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include "personagem.h"  
#include "fases.h"
#include "inimigo.h"
#include "configuracoes.h"
#include "colisao.h"
#include "infeccao.h"
#include "som.h"


typedef struct SistemaFases SistemaFases;

typedef struct {
    float projetilX[50], projetilY[50], projetilDirecao[50];
    bool projetilAtivo[50];
    float proxProjetil;
    Arma tipo;  

    Arma tipoProjetil[50];
    int frameProjetil[50];
    int contadorFrameProjetil[50];
    float posicaoInicialX[50];

    int larguraProjetil[50];
    int alturaProjetil[50];

    bool ataqueCorpoACorpoAtivo;
    float tempoInicioAtaque;
    float duracaoAtaque;
    bool inimigosAtingidos[20];

} ProjetilPosicao;

void atirar_multiplos_inimigos(ProjetilPosicao* pp, Jogador jogador, Inimigo* inimigos, int numInimigos, Bitmaps* bitmap, bool espaco, int projetilLargura, int projetilAltura, int alturaJogador, int larguraJogador, int width, float projetilVelocidade, float projetilCadencia, float* posicaoCamera, SistemaFases* sistemaFase, BarraBoss* barraVidaBoss, SistemaSom* sons);

void inicializar_armas(ProjetilPosicao* arma, Arma tipo_arma, float x, float y, ALLEGRO_BITMAP* sprite_esq, ALLEGRO_BITMAP* sprite_dir);

void trocar_arma(ProjetilPosicao* pp, Arma nova_arma);

void configuracoes_armas(Arma tipo, float* velocidade, float* cadencia);

void ataque_corpo_a_corpo(ProjetilPosicao* pp, Jogador jogador, Inimigo* inimigos, int numInimigos, bool espaco, int larguraJogador, int alturaJogador, SistemaFases* sistemaFase, BarraBoss* barraVidaBoss, SistemaSom* sons);

#endif
