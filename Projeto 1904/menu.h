#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "configuracoes.h"
#include "infeccao.h"


typedef struct {
    bool* telaMenu;
    bool* jogando;
    bool* regrasAberta;
    bool* esc;
    bool* jogoPausado;
    bool* fimDeJogo;
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

void menu_principal(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao);

void menu_pausa(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao);

void desenhar_tela_gameOver(GameOver* gameover, InfeccaoEstagio* infec, MenuEvents* menuEvent, MenuEstados* menuEstado);

#endif 
