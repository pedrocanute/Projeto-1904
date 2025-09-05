#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include "movimentacao.h"

int main() {

    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();

    const int width = 640, height = 480, fps = 60;
    const int larguraJogador = 32, alturaJogador = 32;
    const float velocidade = 5.5f;
    bool jogando = true;
    bool w = false, a = false, s = false, d = false;
    Posicao jogador = { 320.0, 240.0 };

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


    while (jogando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogando = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_W)
                w = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_S)
                s = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_A)
                a = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_D)
                d = true;
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
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            mover(&jogador, w, a, s, d, velocidade);
            restringirPosicao(&jogador, width, height, larguraJogador, alturaJogador);
        }

        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_filled_rectangle(jogador.jogadorX, jogador.jogadorY, jogador.jogadorX + larguraJogador, jogador.jogadorY + alturaJogador, al_map_rgb(0, 0, 0));
        al_draw_text(font, al_map_rgb(0, 0, 0), 230, 200, 0, "TESTE");
        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}