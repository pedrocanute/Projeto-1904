#include "configuracoes.h"
#include "personagem.h"
#include "fases.h"
#include "inimigo.h"
#include "projetil.h"
#include "input.h"
#include "colisao.h"
#include "cenario.h"
#include "menu.h"
#include "infeccao.h"
#include "caravana.h"

int main() {

    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();
    al_install_mouse();

    ALLEGRO_DISPLAY* janela = al_create_display(WIDTH, HEIGHT);
    al_set_window_title(janela, "Projeto 1904");
    al_set_window_position(janela, 200, 200);

    //--DECLARACAO DE VARIAVEIS--//
#pragma region VARIAVEIS

    bool telaMenu = true;
    bool configAberta = false;
    bool jogando = false;            // vira true ao clicar "Jogar"
    bool jogoPausado = false;
    bool redesenhar = false;

    bool w = false, a = false, s = false, d = false, espaco = false, shift = false, esc = false;
    float mouseX = 0.0f, mouseY = 0.0f;

    ProjetilPosicao projetil = { 0 };
    SistemaFases sistemaFase;

    // TIMER PARA SPAWN DE INIMIGOS
    float timer_spawn_inimigos = 0.0f;
    bool spawn_ativo = false;
    const float TEMPO_SPAWN = 3.0f;

    // CARREGAMENTO DOS SPRITES DOS INIMIGOS
    Bitmaps bitmap;
    carregar_bitmaps(&bitmap);

    // CAMERA
    float posicaoCamera[2] = { 0, 0 };

    // ARRAY DE INIMIGOS
    Inimigo inimigos[MAX_INIMIGOS];
    inicializarSistemaFases(&sistemaFase, &inimigos[0]);
    inicializar_array_inimigos(inimigos, MAX_INIMIGOS, bitmap.zumbi_direita, bitmap.zumbi_esquerda, bitmap.rato_direita, bitmap.rato_esquerda, bitmap.mosquito_direita, bitmap.mosquito_esquerda,posicaoCamera);

    ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR corCaravana = al_map_rgb(0, 0, 0);

    // JOGADOR
    Jogador jogador = { 120.0f, 520.0f, true, false };

    // CARAVANA
    Caravana caravana = { 0.0f, 412.0f, caravana.caravanaX + 80, caravana.caravanaY + 320};

    // BARRA INFECÇÃO
    Infeccao barraFundo = { 75.0f, 50.0f, 400.0f, 100.0f };
    Infeccao barraInfeccao = { 75.0f, 50.0f, 75.0f, 100.0f };
    float *infeccaoAtual = &barraInfeccao.infeccaoLargura;
    float infeccaoMaxima = barraFundo.infeccaoLargura;

    //TIMER CARAVANA / BARRA INFECÇÃO
    float timer_regen_infeccao = 0.0f;
    const float TEMPO_REGEN_INFECCAO = 5.0f;

    //INTANGIBILIDADE
    float timer_intangibilidade = 0.0f;
    const float TEMPO_INTANGIBILIDADE = 1.0f;

    // ANIMAÇÃO
    int  frame_atual = 0;
    int  contador_frame = 0;
    int  frames_por_sprite = 11;
    bool virado_direita = true;
    int  frame_tiro = 0;
    int  contador_frame_tiro = 0;

    int botaoJogarLargura = al_get_bitmap_width(bitmap.botaoJogar);
    int botaoJogarAltura = al_get_bitmap_height(bitmap.botaoJogar);
    int botaoRegrasLargura = al_get_bitmap_width(bitmap.botaoRegras);
    int botaoRegrasAltura = al_get_bitmap_height(bitmap.botaoRegras);
    int botaoSairLargura = al_get_bitmap_width(bitmap.botaoSair);
    int botaoSairAltura = al_get_bitmap_height(bitmap.botaoSair);
    int fundoMenuLargura = al_get_bitmap_width(bitmap.fundoMenu);
    int fundoMenuAltura = al_get_bitmap_height(bitmap.fundoMenu);
    int abaRegrasLargura = al_get_bitmap_width(bitmap.abaRegras);
    int abaRegrasAltura = al_get_bitmap_height(bitmap.abaRegras);
    int botaoVoltarLargura = al_get_bitmap_width(bitmap.botaoVoltar);
    int botaoVoltarAltura = al_get_bitmap_height(bitmap.botaoVoltar);
    int telaGameOverLargura = al_get_bitmap_width(bitmap.telaGameOver);
    int telaGameOverAltura = al_get_bitmap_height(bitmap.telaGameOver);

    int botaoJogarX = 200, botaoJogarY = 620;
    int botaoConfigX = 500, botaoConfigY = 620;
    int botaoSairX = 800, botaoSairY = 620;
    int botaoVoltarX = 520, botaoVoltarY = 500;

    bool boss_spawnado = false;
    bool fase_boss_ativa = false;

#pragma endregion

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TRANSFORM camera;

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_start_timer(timer);

    // SISTEMA MENUS
    MenuFlags flags = {
        .telaMenu = &telaMenu,
        .jogando = &jogando,
        .configAberta = &configAberta,
        .esc = &esc,
        .jogoPausado = &jogoPausado
    };

    MenuIO io = {
        .fila_eventos = fila_eventos,
        .timer = timer,
        .camera = &camera,
        .mouseX = &mouseX,
        .mouseY = &mouseY
    };

    MenuBitmaps bmp = {
        .fundoMenu = bitmap.fundoMenu,
        .botaoJogar = bitmap.botaoJogar, .botaoJogar2 = bitmap.botaoJogar2,
        .botaoConfig = bitmap.botaoRegras, .botaoConfig2 = bitmap.botaoRegras2,
        .botaoSair = bitmap.botaoSair, .botaoSair2 = bitmap.botaoSair2,
        .abaConfig = bitmap.abaRegras,
        .botaoVoltar = bitmap.botaoVoltar, .botaoVoltar2 = bitmap.botaoVoltar2
    };

    MenuLayout lay = {
        .botaoJogarX = botaoJogarX, .botaoJogarY = botaoJogarY,
        .botaoConfigX = botaoConfigX, .botaoConfigY = botaoConfigY,
        .botaoSairX = botaoSairX, .botaoSairY = botaoSairY,
        .botaoVoltarX = botaoVoltarX, .botaoVoltarY = botaoVoltarY,

        .botaoJogarLargura = botaoJogarLargura, .botaoJogarAltura = botaoJogarAltura,
        .botaoConfigLargura = botaoRegrasLargura, .botaoConfigAltura = botaoRegrasAltura,
        .botaoSairLargura = botaoSairLargura, .botaoSairAltura = botaoSairAltura,
        .fundoMenuLargura = fundoMenuLargura, .fundoMenuAltura = fundoMenuAltura,
        .abaConfigLargura = abaRegrasLargura, .abaConfigAltura = abaRegrasAltura,
        .botaoVoltarLargura = botaoVoltarLargura, .botaoVoltarAltura = botaoVoltarAltura
    };

    GameOver GameOver = {
        .telaGameOver = bitmap.telaGameOver,
        .telaGameOverLargura = telaGameOverLargura,
        .telaGameOverAltura = telaGameOverAltura
    };

    // MENU PRINCIPAL
    menu_principal(&flags, &io, &bmp, &lay);
    if (!jogando) {
        // Saiu pelo menu
        al_destroy_font(font);
        destruir_bitmaps(&bitmap);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        return 0;
    }

    ALLEGRO_EVENT event;

    // LOOP PRINCIPAL
    while (jogando) {
        al_wait_for_event(fila_eventos, &event);

        // CONDIÇÃO DE PARADA
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogando = false;
        }

        // TECLADO
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
            verificar_Input(event, &w, &a, &s, &d, &espaco, &shift, &esc);
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                esc = true;
            }
        }

        // PAUSE
        if (esc) {
            lay.botaoJogarX = 525; lay.botaoJogarY = 260;
            lay.botaoConfigX = 525; lay.botaoConfigY = 340;
            lay.botaoSairX = 525; lay.botaoSairY = 420;

            menu_pausa(&flags, &io, &bmp, &lay);
            if (!jogando)
                break;  // saiu pelo "Sair" no pause
            // se voltou, esc foi limpo dentro de menu_pausa
        }

        // ATUALIZAÇÃO DE JOGO (quando não pausado)
        if (event.type == ALLEGRO_EVENT_TIMER && esc == false) {
            mover(&jogador, w, a, s, d, shift, VELOCIDADE_JOGADOR, &frames_por_sprite);
            restringirPosicao(&jogador, WIDTH, HEIGHT, LARGURA_JOGADOR, ALTURA_JOGADOR);

            atualizar_movimento_inimigos(inimigos, MAX_INIMIGOS);
            if (inimigos[0].ativo && inimigos[0].tipo == TIPO_BOSS) {
                atualizar_boss_perseguindo(&inimigos[0], &jogador, 12.0f); // 8–20 px funciona bem
            }

            camera_jogador(posicaoCamera, jogador, WIDTH, LARGURA_JOGADOR, ALTURA_JOGADOR);
            redesenhar = true;
        }

        // COLISÃO
        // Colisão de jogador com inimigo
        bool colidiu = false;
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            if (colisao_jogador_inimigo(&inimigos[i], &jogador, LARGURA_JOGADOR, ALTURA_JOGADOR)) {
                colidiu = true;
                break;
            }
        }
        cor = colidiu ? al_map_rgb(255, 0, 0) : al_map_rgb(0, 0, 0);

        //Colisão de inimigo com caravana / Intangibilidade
        bool colisaoCaravana = false;
        bool intangibilidadeAtiva = false;
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            if(!intangibilidadeAtiva && al_get_time() - timer_intangibilidade >= TEMPO_INTANGIBILIDADE) {
                if (colisao_inimigo_caravana(&inimigos[i], &caravana, caravana.caravaLargura, caravana.caravaAltura) && barraInfeccao.infeccaoLargura < 400) {
                    colisaoCaravana = true;
                    barraInfeccao.infeccaoLargura += 10;
                    timer_regen_infeccao = al_get_time();
                    timer_intangibilidade = al_get_time();
                    intangibilidadeAtiva = true;
                    break;
                }
            }
        }
        corCaravana = colisaoCaravana ? al_map_rgb(255, 0, 0) : al_map_rgb(0, 0, 0);

        //Regeneração de Vida
        // !!!!!!!!!Compara se não há colisão e se a caravana levou dano!!!!!!!!!
        if (!colisaoCaravana && al_get_time() - timer_regen_infeccao >= TEMPO_REGEN_INFECCAO && barraInfeccao.infeccaoLargura > barraInfeccao.infeccaoX) {
            barraInfeccao.infeccaoLargura--;
        }

        // RESPAWN POR FASE
        if (!fase_boss_ativa) {
            // Ciclo normal de hordas
            if (!verificarProgressoDaFase(&sistemaFase)) {
                if (contarInimigosAtivos(inimigos, MAX_INIMIGOS) == 0) {
                    if (!spawn_ativo) {
                        timer_spawn_inimigos = al_get_time(); // usa relógio do Allegro
                        spawn_ativo = true;
                    }
                    else if (al_get_time() - timer_spawn_inimigos >= TEMPO_SPAWN) {
                        inicializar_array_inimigos(inimigos, MAX_INIMIGOS, bitmap.zumbi_direita, bitmap.zumbi_esquerda, bitmap.rato_direita, bitmap.rato_esquerda, bitmap.mosquito_direita, bitmap.mosquito_esquerda, posicaoCamera);
                        aplicar_buffs_por_fase(inimigos, MAX_INIMIGOS, sistemaFase.faseAtual);
                        spawn_ativo = false;
                    }
                }
                else {
                    spawn_ativo = false;
                }
            }
            else {
                // Meta da fase atingida: spawna boss uma única vez e entra em fase_boss_ativa
                if (!boss_spawnado) {
                    int idx_boss = -1;
                    for (int i = 0; i < MAX_INIMIGOS; ++i) {
                        if (!inimigos[i].ativo) { idx_boss = i; break; }
                    }
                    if (idx_boss < 0) idx_boss = 0;

                    spawnar_boss(&inimigos[idx_boss], bitmap.boss_variola_direita, bitmap.boss_variola_esquerda, posicaoCamera);
                    boss_spawnado = true;
                    fase_boss_ativa = true;
                    spawn_ativo = false; // pausa spawns comuns
                }
            }
        }
        else {
            // Durante o boss: pausar novos spawns comuns e aguardar morte do boss
            bool bossVivo = false;
            for (int i = 0; i < MAX_INIMIGOS; ++i) {
                if (inimigos[i].ativo && inimigos[i].tipo == TIPO_BOSS) {
                    bossVivo = true; break;
                }
            }
            if (!bossVivo) {
                // Boss derrotado: avança fase e reseta estado para próximo ciclo de hordas
                avancarFase(&sistemaFase, inimigos);
                fase_boss_ativa = false;
                boss_spawnado = false;
                spawn_ativo = false;
            }
        }

        // CAMERA
        al_identity_transform(&camera);
        al_translate_transform(&camera, -posicaoCamera[0], -posicaoCamera[1]);
        al_use_transform(&camera);

        // DESENHO
        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            al_clear_to_color(cor);

            // cenário primeiro
            desenhar_cenario(bitmap.cenario1, bitmap.cenario2, jogador.jogadorX, posicaoCamera);

            // Infecção
            desenhar_barra(barraFundo.infeccaoX, barraFundo.infeccaoY, barraFundo.infeccaoLargura, barraFundo.infeccaoAltura);
            desenhar_barra_infeccao(barraInfeccao.infeccaoX, barraInfeccao.infeccaoY, barraInfeccao.infeccaoLargura, barraInfeccao.infeccaoAltura);

            // caravana
            desenhar_caravana(caravana.caravanaX, caravana.caravanaY , caravana.caravaLargura, caravana.caravaAltura, corCaravana);

            // jogador e inimigos
            desenhar_jogador(jogador, w, a, s, d, espaco, bitmap.sprite_andando_direita, bitmap.sprite_andando_esquerda, bitmap.sprite_atirando_direita, bitmap.sprite_atirando_esquerda,&frame_atual, &contador_frame, frames_por_sprite,&virado_direita, &frame_tiro, &contador_frame_tiro);
            desenhar_todos_inimigos(inimigos, MAX_INIMIGOS);

            // tiros
            atirar_multiplos_inimigos(&projetil, jogador, inimigos, MAX_INIMIGOS, bitmap.projetilDireita, bitmap.projetilEsquerda, espaco,LARGURA_PROJETIL, ALTURA_PROJETIL,ALTURA_JOGADOR, LARGURA_JOGADOR,WIDTH, VELOCIDADE_PROJETIL, CADENCIA,posicaoCamera, &sistemaFase);

            // Game Over
            desenhar_tela_gameOver(&GameOver, infeccaoAtual, infeccaoMaxima);

            // HUD (fixo na tela)
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

            ALLEGRO_TRANSFORM world_backup;
            al_copy_transform(&world_backup, al_get_current_transform());
            ALLEGRO_TRANSFORM hud;
            al_identity_transform(&hud);
            al_use_transform(&hud);
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, texto);
            al_use_transform(&world_backup);

            al_flip_display();
            redesenhar = false;
        }
    } // fim while(jogando)

    // LIMPEZA
    al_destroy_font(font);

    destruir_bitmaps(&bitmap);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}
