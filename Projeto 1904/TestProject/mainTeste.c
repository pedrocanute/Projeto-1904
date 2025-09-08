//indludes
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_color.h>

//Declarações



int main(void){

	struct objeto
	{
		ALLEGRO_BITMAP* imagem;
		int x;
		int y;
		int largura;
		int altura;
	};

	typedef struct objeto Objeto;

	ALLEGRO_DISPLAY* janela = NULL;
	ALLEGRO_EVENT_QUEUE* filaEventos = NULL;
	Objeto jogador;
	Objeto bot;

	al_init();

	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();

	al_is_primitives_addon_initialized();

	janela = al_create_display(640, 480);
	filaEventos = al_create_event_queue();
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_set_window_title(janela, "PONG");
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);

	al_register_event_source(filaEventos, al_get_keyboard_event_source());
	al_register_event_source(filaEventos, al_get_display_event_source(janela));
	al_register_event_source(filaEventos, al_get_timer_event_source(timer));
	al_start_timer(timer);

	//jogador = (Objeto*)malloc(sizeof(Objeto));
	//reta-> al_draw_filled_rectangle(150, 160, 200, 200, green);
	jogador.x = 8;
	jogador.y = 480 / 2 - 50 / 2;
	jogador.largura = jogador.x + 50;
	jogador.altura = jogador.y + 50;

	bot.x = 630;
	bot.y = 480 / 2 - 50 / 2;
	bot.largura = bot.x + 50;
	bot.altura = bot.y + 50;

	bool jogando = true;
	int pressionadoY = 0;
	int pressionadoX = 0;

	ALLEGRO_COLOR cor = al_map_rgb(255, 255, 255);

	while (jogando)
	{
		while (!al_is_event_queue_empty(filaEventos)) {

			ALLEGRO_EVENT evento;
			al_wait_for_event(filaEventos, &evento);

			if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				jogando = false;
			else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
				switch (evento.keyboard.keycode) {
				case ALLEGRO_KEY_W:
					pressionadoY = -2;
					break;
				case ALLEGRO_KEY_S:
					pressionadoY = 2;
					break;
				case ALLEGRO_KEY_D:
					pressionadoX = 4;
					break;
				case ALLEGRO_KEY_A:
					pressionadoX = -4;
					break;
				}
			}
			else if (evento.type == ALLEGRO_EVENT_KEY_UP) {
				pressionadoY = 0;
				pressionadoX = 0;
			}
			if (evento.type == ALLEGRO_EVENT_TIMER) {
				if (pressionadoX != 0 || pressionadoY != 0)
					if (!(jogador.y + pressionadoY < 0 || jogador.altura + pressionadoY > 480 || jogador.x + pressionadoX < 0 || jogador.largura + pressionadoX > 640)) {
						jogador.y += pressionadoY;
						jogador.x += pressionadoX;
						jogador.altura = jogador.y + 50;
						jogador.largura = jogador.x + 50;
					}
				if (jogador.x < bot.x) {
					bot.x--;
					bot.largura--;
				}
				else if (jogador.x > bot.x) {
					bot.x++;
					bot.largura++;
				}
				if (jogador.y < bot.y) {
					bot.y--;
					bot.altura--;
				}
				else if (jogador.y > bot.y) {
					bot.y++;
					bot.altura++;
				}

				if ((bot.x >= jogador.x && bot.x <= jogador.largura && bot.y >= jogador.y && bot.y <= jogador.altura) || (bot.largura >= jogador.x && bot.largura <= jogador.largura && bot.altura >= jogador.y && bot.altura >= jogador.altura))
					cor = al_map_rgb(255, 0, 0);
				else
					cor = al_map_rgb(255, 255, 255);
			}
		}
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(jogador.x, jogador.y, jogador.largura, jogador.altura, cor);
		al_draw_filled_rectangle(bot.x, bot.y, bot.largura, bot.altura, cor);
		al_flip_display();
	}

	//DESTROY
	al_destroy_display(janela);
	al_destroy_event_queue(filaEventos);
	al_destroy_timer(timer);

return 0;
}