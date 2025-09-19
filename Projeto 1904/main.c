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

#pragma region Defines
//PROGRAMA
#define WIDTH 1280
#define HEIGHT 720
#define FPS 60

//PERSONAGEM
#define VELOCIDADE_JOGADOR 5.5f
#define LARGURA_JOGADOR 32
#define ALTURA_JOGADOR 32

//PROJETIL
#define VELOCIDADE_PROJETIL 15.0f
#define CADENCIA 0.2f
#define LARGURA_PROJETIL 16
#define ALTURA_PROJETIL 16

//INIMIGO
#define VELOCIDADE_INIMIGO 3.0f
#pragma endregion

int main() {

    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();

//DECLARAÇÃO DE VARIÁVEIS
    bool jogando = true;
    bool w = false, a = false, s = false, d = false, espaco = false;
    Jogador jogador = { 320.0, 240.0 };
    ProjetilPosicao projetil = { 0 };

    //INIMIGO
    Inimigo bot;
    bot.botX = 1500;
    bot.botY = 480 / 2.0f - 50 / 2.0f;
    bot.larguraBot = 50.0f;
    bot.alturaBot = 50.0f;
    ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);   

//--------------------------//    

    ALLEGRO_DISPLAY* janela = al_create_display(WIDTH, HEIGHT);
    al_set_window_title(janela, "Projeto 1904");
    al_set_window_position(janela, 200, 200);

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_start_timer(timer);

    ALLEGRO_EVENT event;

    while (jogando) {
        al_wait_for_event(fila_eventos, &event);

        //CONDICAO DE PARADA
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogando = false;
        }
        //VERIFICACAO DE TECLAS
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
            verificar_Input(event, &w, &a, &s, &d, &espaco);

        //MOVIMENTACAO E RESTRICAO DO PERSONAGEM
        if (event.type == ALLEGRO_EVENT_TIMER) {
            mover(&jogador, w, a, s, d, VELOCIDADE_JOGADOR);
            restringirPosicao(&jogador, WIDTH, HEIGHT, LARGURA_JOGADOR, ALTURA_JOGADOR);
            perseguir(&bot, &jogador, LARGURA_JOGADOR, ALTURA_JOGADOR, VELOCIDADE_INIMIGO);
        }


        //Colisão
        if ((jogador.jogadorX <= bot.botX + bot.larguraBot) && 
            (jogador.jogadorX + LARGURA_JOGADOR >= bot.botX) && 
            (jogador.jogadorY <= bot.botY + bot.alturaBot) && 
            (jogador.jogadorY + ALTURA_JOGADOR >= bot.botY))
            // Colidiu
            cor = al_map_rgb(255, 0, 0);
        else
            // Não colidiu
            cor = al_map_rgb(0, 0, 0);

        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(bot.botX, bot.botY, bot.botX + bot.larguraBot, bot.botY + bot.alturaBot, cor);
        al_draw_filled_rectangle(jogador.jogadorX, jogador.jogadorY, jogador.jogadorX + LARGURA_JOGADOR, jogador.jogadorY + ALTURA_JOGADOR, al_map_rgb(0, 0, 0));
        atirar(&projetil, jogador, espaco, LARGURA_PROJETIL, ALTURA_PROJETIL, ALTURA_JOGADOR, WIDTH, VELOCIDADE_PROJETIL, CADENCIA);
        al_draw_text(font, al_map_rgb(0, 0, 0), 230, 200, 0, "TESTE");
        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}