#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "personagem.h"
#include "projetil.h"
#include "input.h"
#include "inimigo.h"
#include "colisao.h"
#include "cenario.h"

// DEFINES
#define WIDTH 1280
#define HEIGHT 720
#define FPS 60
#define VELOCIDADE_JOGADOR 5.5f
#define LARGURA_JOGADOR 80
#define ALTURA_JOGADOR 96
#define VELOCIDADE_PROJETIL 15.5f
#define CADENCIA 0.2f
#define LARGURA_PROJETIL 16
#define ALTURA_PROJETIL 16
#define VELOCIDADE_INIMIGO 4.5f

#endif
