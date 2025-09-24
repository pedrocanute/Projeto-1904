#include "configuracoes.h"

int main() {

    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();

    ALLEGRO_DISPLAY* janela = al_create_display(WIDTH, HEIGHT);
    al_set_window_title(janela, "Projeto 1904");
    al_set_window_position(janela, 200, 200);

//--DECLARACAO DE VARIAVEIS--//
    bool jogando = true;
    bool w = false, a = false, s = false, d = false, espaco = false, shift = false;
    ProjetilPosicao projetil = { 0 };

    // INIMIGO
    Inimigo bot;
    bot.larguraBot = 50.0f;
    bot.alturaBot = 50.0f;
    bot.botX = 1350;
    bot.botY = 520;
    ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);

    // JOGADOR
    Jogador jogador = { 120.0, 520.0 };
    ALLEGRO_BITMAP* sprite_andando_direita = al_load_bitmap("AndandoDireita.png");
    ALLEGRO_BITMAP* sprite_andando_esquerda = al_load_bitmap("AndandoEsquerda.png");
    int frame_atual = 0;
    int contador_frame = 0;
    int frames_por_sprite = 11;
    bool virado_direita = true;

    // CENARIO
    ALLEGRO_BITMAP* cenario1 = al_load_bitmap("Mapa01.png");
    ALLEGRO_BITMAP* cenario2 = al_load_bitmap("Mapa02.png");

    // CAMERA
    float posicaoCamera[2] = { 0, 0 };
//-----------------//

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TRANSFORM camera;

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_start_timer(timer);

    ALLEGRO_EVENT event;

    //LOOP PRINCIPAL
    while (jogando) {
        al_wait_for_event(fila_eventos, &event);

        // CONDICAO DE PARADA
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogando = false;
        }

        // VERIFICACAO DE TECLAS
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
            verificar_Input(event, &w, &a, &s, &d, &espaco, &shift);

        // MOVIMENTACAO E RESTRICAO DO PERSONAGEM
        if (event.type == ALLEGRO_EVENT_TIMER) {
            mover(&jogador, w, a, s, d, shift, VELOCIDADE_JOGADOR, &frames_por_sprite);
            restringirPosicao(&jogador, WIDTH, HEIGHT, LARGURA_JOGADOR, ALTURA_JOGADOR);
            perseguir(&bot, &jogador, LARGURA_JOGADOR, ALTURA_JOGADOR, VELOCIDADE_INIMIGO);
            camera_jogador(posicaoCamera, jogador, WIDTH, LARGURA_JOGADOR, ALTURA_JOGADOR);
        }

        // Colisao
        if ((jogador.jogadorX <= bot.botX + bot.larguraBot) &&
            (jogador.jogadorX + LARGURA_JOGADOR >= bot.botX) &&
            (jogador.jogadorY <= bot.botY + bot.alturaBot) &&
            (jogador.jogadorY + ALTURA_JOGADOR >= bot.botY))
            cor = al_map_rgb(255, 0, 0);
        else
            cor = al_map_rgb(0, 0, 0);

        al_identity_transform(&camera);
        al_translate_transform(&camera, -posicaoCamera[0], -posicaoCamera[1]);
        al_use_transform(&camera);

        al_clear_to_color(cor);
        desenhar_cenario(cenario1, cenario2, jogador.jogadorX, posicaoCamera);

        al_draw_filled_rectangle(bot.botX, bot.botY, bot.botX + bot.larguraBot, bot.botY + bot.alturaBot, cor);
        desenhar_jogador(jogador, w, a, s, d, sprite_andando_direita, sprite_andando_esquerda, &frame_atual, &contador_frame, frames_por_sprite, &virado_direita);
        atirar(&projetil, jogador, espaco, LARGURA_PROJETIL, ALTURA_PROJETIL, ALTURA_JOGADOR, WIDTH, VELOCIDADE_PROJETIL, CADENCIA);
        al_draw_text(font, al_map_rgb(0, 0, 0), 230, 200, 0, "TESTE");
        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_bitmap(sprite_andando_direita);
    al_destroy_bitmap(sprite_andando_esquerda);
    al_destroy_bitmap(cenario1);
    al_destroy_bitmap(cenario2);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}
