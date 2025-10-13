#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "configuracoes.h"


typedef struct {
    bool* telaMenu;
    bool* jogando;
    bool* configAberta;
    bool* esc;
    bool* jogoPausado;
} MenuFlags;

typedef struct {
    ALLEGRO_EVENT_QUEUE* fila_eventos;
    ALLEGRO_TIMER* timer;
    ALLEGRO_TRANSFORM* camera;
    float* mouseX;
    float* mouseY;
} MenuIO;

typedef struct {
    ALLEGRO_BITMAP* fundoMenu;
    ALLEGRO_BITMAP* botaoJogar;   ALLEGRO_BITMAP* botaoJogar2;
    ALLEGRO_BITMAP* botaoConfig;  ALLEGRO_BITMAP* botaoConfig2;
    ALLEGRO_BITMAP* botaoSair;    ALLEGRO_BITMAP* botaoSair2;
    ALLEGRO_BITMAP* abaConfig;
    ALLEGRO_BITMAP* botaoVoltar;  ALLEGRO_BITMAP* botaoVoltar2;
} MenuBitmaps;

typedef struct {
    int botaoJogarX, botaoJogarY;
    int botaoConfigX, botaoConfigY;
    int botaoSairX, botaoSairY;
    int botaoVoltarX, botaoVoltarY;
    int botaoJogarLargura, botaoJogarAltura;
    int botaoConfigLargura, botaoConfigAltura;
    int botaoSairLargura, botaoSairAltura;
    int fundoMenuLargura, fundoMenuAltura;
    int abaConfigLargura, abaConfigAltura;
    int botaoVoltarLargura, botaoVoltarAltura;
} MenuLayout;

void menu_principal(MenuFlags* flags, const MenuIO* io, const MenuBitmaps* bmp, const MenuLayout* lay);

void menu_pausa(MenuFlags* flags, const MenuIO* io, const MenuBitmaps* bmp, const MenuLayout* lay);

#endif 
