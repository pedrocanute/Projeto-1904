#ifndef TRANSICOES_H
#define TRANSICOES_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "menu.h"
#include "introducao.h"

// ============= SISTEMA DE FADE =============

typedef struct {
    float alfa;
    double tempoInicio;
    double duracao;
    bool completo;
    bool fadeIn;  
} SistemaFade;


void inicializarFade(SistemaFade* fade, double duracao, bool fadeIn);

void atualizarFade(SistemaFade* fade);

void desenharFade(SistemaFade* fade, int largura, int altura);

bool fadeCompleto(SistemaFade* fade);

bool executarIntroducao(MenuEstados* menuEstado, MenuEvents* menuEvent);

bool executarFadeTransicao(MenuEstados* menuEstado, MenuEvents* menuEvent, double duracao, bool fadeIn);

bool executarDialogoInicial(Dialogo* dialogo, SistemaFases* sistemaFase, MenuEvents* menuEvent, MenuEstados* menuEstado);

#endif

