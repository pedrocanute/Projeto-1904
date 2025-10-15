#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include "configuracoes.h"
#include "infeccao.h"
#include "fases.h"
#include "input.h"
#include "cenario.h"


typedef struct {
    bool* telaMenu;
    bool* jogando;
    bool* regrasAberta;
    bool* esc;
    bool* jogoPausado;
    bool* fimDeJogo;
    bool* telaDialogo;
} MenuEstados;

typedef struct {
    ALLEGRO_EVENT_QUEUE* fila_eventos;
    ALLEGRO_TIMER* timer;
    ALLEGRO_TRANSFORM* camera;
    float* mouseX;
    float* mouseY;
} MenuEvents;

typedef struct {
    ALLEGRO_BITMAP* fundoMenu;
    ALLEGRO_BITMAP* botaoJogar;   ALLEGRO_BITMAP* botaoJogar2;
    ALLEGRO_BITMAP* botaoRegras;  ALLEGRO_BITMAP* botaoRegras2;
    ALLEGRO_BITMAP* botaoSair;    ALLEGRO_BITMAP* botaoSair2;
    ALLEGRO_BITMAP* abaRegras;
    ALLEGRO_BITMAP* botaoVoltar;  ALLEGRO_BITMAP* botaoVoltar2;
} MenuImagens;

typedef struct {
    int botaoJogarX, botaoJogarY;
    int botaoRegrasX, botaoRegrasY;
    int botaoSairX, botaoSairY;
    int botaoVoltarX, botaoVoltarY;
    int botaoJogarLargura, botaoJogarAltura;
    int botaoRegrasLargura, botaoRegrasAltura;
    int botaoSairLargura, botaoSairAltura;
    int fundoMenuLargura, fundoMenuAltura;
    int abaRegrasLargura, abaRegrasAltura;
    int botaoVoltarLargura, botaoVoltarAltura;
} MenuBotoes;

// Tela game over
typedef struct {
    ALLEGRO_BITMAP* telaGameOver;
    ALLEGRO_BITMAP* botaoJogarNovamente;  ALLEGRO_BITMAP* botaoJogarNovamente2;
    ALLEGRO_BITMAP* botaoSairDoJogo;  ALLEGRO_BITMAP* botaoSairDoJogo2;
    int botaoJogarNovamenteX, botaoJogarNovamenteY;
    int botaoSairDoJogoX, botaoSairDoJogoY;
    int botaoJogarNovamenteLargura, botaoJogarNovamenteAltura;
    int botaoSairDoJogoLargura, botaoSairDoJogoAltura;
    int telaGameOverLargura, telaGameOverAltura;
} GameOver;

// Em menu.h

typedef struct {
    ALLEGRO_BITMAP* oswaldo;
    ALLEGRO_BITMAP* caixaDialogo;
    int caixaDialogoX, caixaDialogoY;
    int caixaDialogoLargura, caixaDialogoAltura;
    int oswaldoLargura, oswaldoAltura;
    bool* dialogo1;
    bool* dialogo2;
    bool* dialogo3;
} Dialogo;

void menu_principal(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao);

void menu_pausa(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao);

void desenhar_tela_gameOver(GameOver* gameover, Barra* infec, MenuEvents* menuEvent, MenuEstados* menuEstado);

void desenhar_tela_dialogo(Dialogo* dialogo, SistemaFases* fase, MenuEvents* menuEvent, MenuEstados* menuEstado);

#endif 
