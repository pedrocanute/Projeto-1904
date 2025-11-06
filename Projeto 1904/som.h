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
    ALLEGRO_AUDIO_STREAM* musicaMenu;
    
} SistemaSom;

// Função para inicializar o sistema de áudio do Allegro
bool inicializarSistemaAudio();

// Função para carregar todos os sons do jogo
bool carregarSons(SistemaSom* sons);

// Função para tocar o som de tiro
void tocarSomTiro(SistemaSom* sons);

// Função para tocar o som de clique
void tocarSomClick(SistemaSom* sons);

// Função para tocar a música do menu
void tocarMusicaMenu(SistemaSom* sons);

// Função para parar a música do menu
void pararMusicaMenu(SistemaSom* sons);

// Função para destruir e liberar memória dos sons
void destruirSons(SistemaSom* sons);

#endif // SOM_H
