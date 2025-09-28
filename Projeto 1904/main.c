#include "configuracoes.h"
#include "personagem.h"
#include "fases.h"
#include "inimigo.h"
#include "projetil.h"
#include "input.h"
#include "colisao.h"
#include "cenario.h"

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
    SistemaFases sistemaFase;

    // TIMER PARA SPAWN DE INIMIGOS
    float timer_spawn_inimigos = 0.0;
    bool spawn_ativo = false;
    const float TEMPO_SPAWN = 3.0; 

    // CARREGAMENTO DOS SPRITES DOS INIMIGOS
    ALLEGRO_BITMAP* zumbi_direita = al_load_bitmap("ZumbiAndandoDireita.png");
    ALLEGRO_BITMAP* zumbi_esquerda = al_load_bitmap("ZumbiAndandoEsquerda.png");
    ALLEGRO_BITMAP* rato_direita = al_load_bitmap("RatoAndandoDireita.png");
    ALLEGRO_BITMAP* rato_esquerda = al_load_bitmap("RatoAndandoEsquerda.png");
    ALLEGRO_BITMAP* mosquito_direita = al_load_bitmap("MosquitoDireita.png");
    ALLEGRO_BITMAP* mosquito_esquerda = al_load_bitmap("MosquitoEsquerda.png");

    // CAMERA
    float posicaoCamera[2] = { 0, 0 };

    // ARRAY DE 20 INIMIGOS
    Inimigo inimigos[MAX_INIMIGOS];
    inicializarSistemaFases(&sistemaFase, &inimigos[0]);

    inicializar_array_inimigos(inimigos, MAX_INIMIGOS, zumbi_direita, zumbi_esquerda, rato_direita, rato_esquerda, mosquito_direita, mosquito_esquerda, posicaoCamera);

    ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);

    // JOGADOR
    Jogador jogador = { 120.0, 520.0, true, false };
    ALLEGRO_BITMAP* sprite_andando_direita = al_load_bitmap("AndandoDireita.png");
    ALLEGRO_BITMAP* sprite_andando_esquerda = al_load_bitmap("AndandoEsquerda.png");
    ALLEGRO_BITMAP* sprite_atirando_direita = al_load_bitmap("AtirandoDireita.png");
    ALLEGRO_BITMAP* sprite_atirando_esquerda = al_load_bitmap("AtirandoEsquerda.png");

    // Variáveis de animação normal
    int frame_atual = 0;
    int contador_frame = 0;
    int frames_por_sprite = 11;
    bool virado_direita = true;
    int frame_tiro = 0;
    int contador_frame_tiro = 0;

    //PROJETIL
    ALLEGRO_BITMAP* projetilDireita = al_load_bitmap("imagens/VacinaProjetilDireita.png");
    ALLEGRO_BITMAP* projetilEsquerda = al_load_bitmap("imagens/VacinaProjetilEsquerda.png");

    // CENARIO
    ALLEGRO_BITMAP* cenario1 = al_load_bitmap("Mapa01.png");
    ALLEGRO_BITMAP* cenario2 = al_load_bitmap("Mapa02.png");

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

            // ATUALIZA MOVIMENTO DOS INIMIGOS (SEM RESPAWN AUTOMATICO)
            atualizar_movimento_inimigos(inimigos, MAX_INIMIGOS, zumbi_direita, zumbi_esquerda, rato_direita, rato_esquerda, mosquito_direita, mosquito_esquerda, posicaoCamera);

            camera_jogador(posicaoCamera, jogador, WIDTH, LARGURA_JOGADOR, ALTURA_JOGADOR);
        }

        // VERIFICA COLISAO
        bool colidiu = false;
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            if (colisao_jogador_inimigo(&inimigos[i], &jogador, LARGURA_JOGADOR, ALTURA_JOGADOR)) {
                colidiu = true;
                break;
            }
        }

        // SISTEMA DE RESPAWN COM TIMER DE 5 SEGUNDOS
        if (!verificarProgressoDaFase(&sistemaFase)) {
            if (contarInimigosAtivos(inimigos, MAX_INIMIGOS) == 0) { // Não há inimigos ativos
                if (!spawn_ativo) {
                    // Inicia o timer
                    timer_spawn_inimigos = al_get_time();
                    spawn_ativo = true;
                }
                else if (al_get_time() - timer_spawn_inimigos >= TEMPO_SPAWN) {
                    // 5 segundos passaram, respawn todos os inimigos
                    inicializar_array_inimigos(inimigos, MAX_INIMIGOS, zumbi_direita, zumbi_esquerda, rato_direita, rato_esquerda, mosquito_direita, mosquito_esquerda, posicaoCamera);
                    aplicar_buffs_por_fase(inimigos, MAX_INIMIGOS, sistemaFase.faseAtual);
                    spawn_ativo = false;
                }
            }
            else {
                // Há inimigos ativos, desativa o timer
                spawn_ativo = false;
            }
        }
        else {
            avancarFase(&sistemaFase, inimigos);
            spawn_ativo = false;
        }
    
        //CAMERA

        al_identity_transform(&camera);
        al_translate_transform(&camera, -posicaoCamera[0], -posicaoCamera[1]);
        al_use_transform(&camera);

        al_clear_to_color(cor);
        desenhar_cenario(cenario1, cenario2, jogador.jogadorX, posicaoCamera);

        // DESENHAR TODOS OS INIMIGOS ATIVOS
        desenhar_jogador(jogador, w, a, s, d, espaco, sprite_andando_direita, sprite_andando_esquerda, sprite_atirando_direita, sprite_atirando_esquerda, &frame_atual, &contador_frame, frames_por_sprite, &virado_direita, &frame_tiro, &contador_frame_tiro);
        desenhar_todos_inimigos(inimigos, MAX_INIMIGOS);

        // ATIRAR
        atirar_multiplos_inimigos(&projetil, jogador, inimigos, MAX_INIMIGOS, projetilDireita, projetilEsquerda, espaco, LARGURA_PROJETIL, ALTURA_PROJETIL, ALTURA_JOGADOR, LARGURA_JOGADOR, WIDTH, VELOCIDADE_PROJETIL, CADENCIA, posicaoCamera, &sistemaFase);

        // TEXTO INFORMATIVO COM TIMER
        char texto[100];
        if (spawn_ativo) {
            float tempo_restante = TEMPO_SPAWN - (al_get_time() - timer_spawn_inimigos);
            if (tempo_restante > 0)
                sprintf_s(texto, sizeof(texto), "PROJETO 1904 - NOVOS INIMIGOS EM: %.1f s", tempo_restante);
            else
                sprintf_s(texto, sizeof(texto), "PROJETO 1904 - SPAWNING...");
        }
        else {
            int inimigos_restantes = sistemaFase.metaEliminacoes - sistemaFase.inimigosMortos;
            if (inimigos_restantes < 0) inimigos_restantes = 0;
            sprintf_s(texto, sizeof(texto), "PROJETO 1904 - INIMIGOS RESTANTES: %d", inimigos_restantes);
        }

        // HUD 
        ALLEGRO_TRANSFORM world_backup;
        al_copy_transform(&world_backup, al_get_current_transform()); // salva a câmera

        ALLEGRO_TRANSFORM hud;
        al_identity_transform(&hud);    // zera  coords de tela
        al_use_transform(&hud);

        // desenha fixo no canto superior esquerdo da tela
        al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, texto);

        // restaura a câmera 
        al_use_transform(&world_backup);

        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_bitmap(sprite_andando_direita);
    al_destroy_bitmap(sprite_andando_esquerda);
    al_destroy_bitmap(cenario1);
    al_destroy_bitmap(cenario2);
    al_destroy_bitmap(projetilDireita);
    al_destroy_bitmap(projetilEsquerda);
    al_destroy_bitmap(zumbi_direita);
    al_destroy_bitmap(zumbi_esquerda);
    al_destroy_bitmap(sprite_atirando_direita);
    al_destroy_bitmap(sprite_atirando_esquerda);
    al_destroy_bitmap(rato_direita);
    al_destroy_bitmap(rato_esquerda);
    al_destroy_bitmap(mosquito_direita);
    al_destroy_bitmap(mosquito_esquerda);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}
