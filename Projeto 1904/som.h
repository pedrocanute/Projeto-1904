#ifndef SOM_H
#define SOM_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>

// Estrutura para gerenciar todos os sons do jogo
typedef struct {
    ALLEGRO_SAMPLE* somTiro;
    ALLEGRO_SAMPLE* somClick;
    ALLEGRO_SAMPLE* somAndando;
    ALLEGRO_SAMPLE* somMosquito;
    ALLEGRO_SAMPLE* somVassoura;
    ALLEGRO_SAMPLE* somZumbi;
    ALLEGRO_SAMPLE* somVeneno;
    ALLEGRO_SAMPLE* somRato;
    ALLEGRO_AUDIO_STREAM* musicaMenu;
    
} SistemaSom;

// Função para inicializar o sistema de áudio do Allegro
bool inicializarSistemaAudio();

// Função para carregar todos os sons do jogo
bool carregarSons(SistemaSom* sons);

void tocarSomTiro(SistemaSom* sons);

void tocarSomClick(SistemaSom* sons);

void tocarSomAndando(SistemaSom* sons);

void tocarSomMosquito(SistemaSom* sons);

void tocarSomVassoura(SistemaSom* sons);

void tocarSomZumbi(SistemaSom* sons);

void tocarSomVeneno(SistemaSom* sons);

void tocarSomRato(SistemaSom* sons);

void tocarMusicaMenu(SistemaSom* sons);

void pararMusicaMenu(SistemaSom* sons);

// Função para destruir e liberar memória dos sons
void destruirSons(SistemaSom* sons);

#endif
