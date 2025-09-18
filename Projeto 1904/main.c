#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "personagens.h"
#include "projetil.h"
#include "inimigo.h"

int main() {
    //Inicializações do Allegro
    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();

    //Declaração de variáveis
    //Janela do jogo
    const int width = 640, height = 480, fps = 60; 
    bool jogando = true;
    //Jogador
    Jogador jogador = { 320, 240};
    jogador.largurap = 32; 
    jogador.alturap = 32;
    const float velocidade = 5.5f;
    bool w = false, a = false, s = false, d = false, espaco = false;
    const int projetilLargura = 16, projetilAltura = 16;
    const float projetilVelocidade = 9.0;
    ProjetilPosicao projetil = { 0, 0, false };
    //Inimigo
    Inimigo bot;
    bot.posicaoX = 630;
    bot.posicaoY = 480 / 2 - 50 / 2;
    bot.largura = 50;
    bot.altura = 50;

    //Declarações do Allegro
    ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);

    ALLEGRO_DISPLAY* janela = al_create_display(width, height);
    al_set_window_title(janela, "Projeto 1904");
    al_set_window_position(janela, 200, 200);

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / fps);

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_start_timer(timer);

    ALLEGRO_EVENT event;

    //Loop do jogo
    while (jogando) {
        while (!al_is_event_queue_empty(fila_eventos)) { //Garante que todos os eventos sejam executados | múltiplos eventos podem ser executado em um único loop.

            ALLEGRO_EVENT event;
            al_wait_for_event(fila_eventos, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                jogando = false;

            //ações jogador
            else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_W)
                    w = true;
                if (event.keyboard.keycode == ALLEGRO_KEY_S)
                    s = true;
                if (event.keyboard.keycode == ALLEGRO_KEY_A)
                    a = true;
                if (event.keyboard.keycode == ALLEGRO_KEY_D)
                    d = true;
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
                    espaco = true;
            }
            else if (event.type == ALLEGRO_EVENT_KEY_UP) {
                if (event.keyboard.keycode == ALLEGRO_KEY_W)
                    w = false;
                if (event.keyboard.keycode == ALLEGRO_KEY_S)
                    s = false;
                if (event.keyboard.keycode == ALLEGRO_KEY_A)
                    a = false;
                if (event.keyboard.keycode == ALLEGRO_KEY_D)
                    d = false;
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
                    espaco = false;
            }
            else if (event.type == ALLEGRO_EVENT_TIMER) {
                mover(&jogador, w, a, s, d, velocidade);
                restringirPosicao(&jogador, width, height, jogador.largurap, jogador.alturap);
            }

            //ações bot
            //Perseguição
            perseguir(&bot, &jogador);

            //Colisão
            if (jogador.posicaoXp <= bot.posicaoX + bot.largura && jogador.posicaoXp + jogador.largurap >= bot.posicaoX && jogador.posicaoYp <= bot.posicaoY + bot.altura && jogador.posicaoYp + jogador.alturap >= bot.posicaoY)
                // Colidiu
                cor = al_map_rgb(255, 0, 0); 
            else
                // Não colidiu
                cor = al_map_rgb(0, 0, 0); 
        }
        //Imagens plotadas na tela
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(jogador.posicaoXp, jogador.posicaoYp, jogador.posicaoXp + jogador.largurap, jogador.posicaoYp + jogador.alturap, cor);
        al_draw_filled_rectangle(bot.posicaoX, bot.posicaoY, bot.posicaoX + bot.largura, bot.posicaoY + bot.altura, cor);
        atirar(&projetil, jogador, espaco, projetilLargura, projetilAltura, jogador.alturap, width, projetilVelocidade);
        al_draw_text(font, al_map_rgb(0, 0, 0), 230, 200, 0, "TESTE");
        al_flip_display();
    }

    //Destroys
    al_destroy_font(font);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}