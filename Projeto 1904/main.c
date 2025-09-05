#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>


int main() {

    al_init();
    al_init_font_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY* janela = al_create_display(640, 480);
    al_set_window_title(janela, "Projeto 1904");
    al_set_window_position(janela, 200, 200);

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_start_timer(timer);

    bool jogando = true;

    while (jogando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(fila_eventos, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            jogando = false;
       
        al_clear_to_color(al_map_rgb(255, 255, 255));
        al_draw_text(font, al_map_rgb(0, 0, 0), 230, 200, 0, "TESTE");
        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}