#ifndef DIALOGOS_H
#define DIALOGOS_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include "configuracoes.h"
#include "fases.h"

#define MAX_DIALOGOS 15

// Forward declarations - APENAS declarações
struct MenuEvents;
struct MenuEstados;

// Estrutura para gerenciar diálogos
typedef struct {
    ALLEGRO_BITMAP* falando;
    ALLEGRO_BITMAP* balao;
    ALLEGRO_FONT* fonteDialogo;
    int falandoLargura, falandoAltura;
    int balaoLargura, balaoAltura;

    // Animação da spritesheet
    int frameAtual;
    int contadorFrame;
    int velocidadeAnimacao;

    char* textos[MAX_DIALOGOS];  // Array de textos
    int numeroTextos;             // Quantidade total de textos
    int textoAtual;               // Texto atual sendo exibido

    // Estados de diálogo
    bool dialogo1;
    bool dialogo2;
    bool dialogo3;
} Dialogo;

// Funções de inicialização
void inicializarDialogo(Dialogo* dialogo, Bitmaps* bitmap, ALLEGRO_FONT* fonte);

// Funções de configuração de texto
void configurarTextosDialogo(Dialogo* dialogo, int fase);

// Função principal de desenho de diálogo - usando ponteiros para structs
void desenhar_tela_dialogo(Dialogo* dialogo, SistemaFases* fase, struct MenuEvents* menuEvent, struct MenuEstados* menuEstado);

#endif