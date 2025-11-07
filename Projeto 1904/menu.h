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
#include "som.h"

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
    int botaoVoltarLargura, botaoVoltarAltura;
    int fundoMenuLargura, fundoMenuAltura;
    int abaRegrasLargura, abaRegrasAltura;
} MenuBotoes;

typedef struct {
    ALLEGRO_BITMAP* botaoSairDoJogo;
    ALLEGRO_BITMAP* botaoSairDoJogo2;

    int botaoSairDoJogoLargura, botaoSairDoJogoAltura;
    int botaoSairDoJogoX, botaoSairDoJogoY;
} GameOver;

// Funções
void menu_principal(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao, ALLEGRO_FONT* fonte, Bitmaps* bitmap, SistemaSom* sons);

void menu_pausa(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao, ALLEGRO_FONT* fonte, SistemaSom* sons);

void desenhar_tela_gameOver(GameOver* gameover, Barra* infec, MenuEvents* menuEvent, MenuEstados* menuEstado, ALLEGRO_FONT* fonte);

void inicializarMenuEstados(MenuEstados* estado, bool* telaMenu, bool* jogando, bool* regrasAberta, bool* esc, bool* jogoPausado, bool* fimDeJogo);

void inicializarMenuEvents(MenuEvents* events, ALLEGRO_EVENT_QUEUE* fila, ALLEGRO_TIMER* timer, ALLEGRO_TRANSFORM* camera, float* mouseX, float* mouseY);

void inicializarMenuImagens(MenuImagens* img, Bitmaps* bitmap);

void inicializarMenuBotoes(MenuBotoes* botao, Bitmaps* bitmap);

void inicializarGameOver(GameOver* gameOver, Bitmaps* bitmap);

void configurarPosicoesBotoesPausa(MenuBotoes* menuBotao);

#endif
