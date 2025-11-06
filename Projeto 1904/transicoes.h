#ifndef TRANSICOES_H
#define TRANSICOES_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "introducao.h"

// ============= SISTEMA DE FADE =============

typedef struct {
    float alfa;
    double tempoInicio;
    double duracao;
    bool completo;
    bool fadeIn;  
} SistemaFade;

// ============= SISTEMA DE CUTSCENE =============

typedef enum {
    CUTSCENE_JOGADOR_ENTRADA,
    CUTSCENE_CARAVANA_ENTRADA,
    CUTSCENE_COMPLETA
} EstadoCutscene;

typedef struct {
    EstadoCutscene estado;
    float progresso;
    bool completa;
} SistemaCutscene;

// Forward declarations para evitar dependência circular
struct JogoEntidades;
struct JogoCamera;
struct JogoAnimacao;
struct JogoControle;
struct Bitmaps;
struct Dialogo;
struct SistemaFases;
struct MenuEvents;
struct MenuEstados;

void inicializarFade(SistemaFade* fade, double duracao, bool fadeIn);

void atualizarFade(SistemaFade* fade);

void desenharFade(SistemaFade* fade, int largura, int altura);

bool fadeCompleto(SistemaFade* fade);

bool executarIntroducao(struct MenuEstados* menuEstado, struct MenuEvents* menuEvent);

bool executarFadeTransicao(struct MenuEstados* menuEstado, struct MenuEvents* menuEvent, double duracao, bool fadeIn);

bool executarDialogoInicial(struct Dialogo* dialogo, struct SistemaFases* sistemaFase, struct MenuEvents* menuEvent, struct MenuEstados* menuEstado);

// ============= FUNÇÕES DE CUTSCENE =============

void inicializarCutscene(SistemaCutscene* cutscene);

bool executarCutsceneInicial(struct JogoEntidades* entidades, struct JogoCamera* jogoCamera, struct JogoAnimacao* animacao,
    struct MenuEvents* menuEvent, struct MenuEstados* menuEstado, struct Bitmaps* bitmap, struct JogoControle* controle, ALLEGRO_FONT* fonte);

#endif

