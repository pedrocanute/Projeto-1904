#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "configuracoes.h"


typedef struct {
    bool* telaMenu;
    bool* jogando;
    bool* regrasAberta;
    bool* esc;
    bool* jogoPausado;
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
} MenuImgs;

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

void menu_principal(MenuEstados* mes, MenuEvents* mev, MenuImgs* mi, MenuBotoes* bt);

void menu_pausa(MenuEstados* mes, MenuEvents* mev, MenuImgs* mi, MenuBotoes* bt);

#endif 
