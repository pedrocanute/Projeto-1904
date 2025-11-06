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
#include "introducao.h"
#include "transicoes.h"
#include "jogo.h"

int main() {
    // ========== INICIALIZAÇÃO DO ALLEGRO ===========
    al_init();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();
    al_install_mouse();

    ALLEGRO_DISPLAY* janela = al_create_display(WIDTH, HEIGHT);
    al_set_window_title(janela, "Projeto 1904");
    al_set_window_position(janela, 200, 200);

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();

    // ========== DECLARAÇÃO DE VARIÁVEIS DE ESTADO ==========
    bool telaMenu = true;
    bool regrasAberta = false;
    bool jogando = false;
    bool jogoPausado = false;
    bool fimDeJogo = false;
    bool redesenhar = false;
    bool w = false, a = false, s = false, d = false;
    bool espaco = false, shift = false, esc = false;
    bool num1 = false, num2 = false, num3 = false;
    float mouseX = 0.0f, mouseY = 0.0f;

    // ========== CARREGAMENTO DE RECURSOS ==========
    Bitmaps bitmap = { 0 };
    carregar_bitmaps(&bitmap);

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_FONT* fonteDialogo = al_load_ttf_font("joystix monospace.otf", 20, 0);
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);

    ALLEGRO_TRANSFORM camera;

    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
   
    al_start_timer(timer);

    // ========== INICIALIZAÇÃO DAS ESTRUTURAS DO JOGO  ==========

    JogoCamera jogoCamera;
    inicializarJogoCamera(&jogoCamera, &camera);

    JogoEntidades entidades;
    inicializarJogoEntidades(&entidades, &bitmap, jogoCamera.posicaoCamera);

    JogoBarras barras;
    inicializarJogoBarras(&barras);

    JogoControle controle;
    inicializarJogoControle(&controle);

    JogoAnimacao animacao;
    inicializarJogoAnimacao(&animacao);

    // ========== INICIALIZAÇÃO DO SISTEMA DE MENUS  ==========

    MenuEstados menuEstado;
    inicializarMenuEstados(&menuEstado, &telaMenu, &jogando, &regrasAberta, &esc, &jogoPausado, &fimDeJogo);

    MenuEvents menuEvent;
    inicializarMenuEvents(&menuEvent, fila_eventos, timer, &camera, &mouseX, &mouseY);

    MenuImagens menuImg;
    inicializarMenuImagens(&menuImg, &bitmap);

    MenuBotoes menuBotao;
    inicializarMenuBotoes(&menuBotao, &bitmap);

    GameOver gameOver;
    inicializarGameOver(&gameOver, &bitmap);

    Dialogo dialogo;
    inicializarDialogo(&dialogo, &bitmap, fonteDialogo);

    // ========== MENU PRINCIPAL ==========
    menu_principal(&menuEstado, &menuEvent, &menuImg, &menuBotao, fonteDialogo);

    // Se saiu do menu sem jogar, encerra
    if (!jogando) {
        // Limpeza e encerramento
        if (fonteDialogo) al_destroy_font(fonteDialogo);
        destruir_bitmaps(&bitmap);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        return 0;
    }

    // ========== TRANSIÇÕES DE INTRODUÇÃO ==========
    if (menuEstado.jogando) {
        if (!executarIntroducao(&menuEstado, &menuEvent)) {
            menuEstado.jogando = false;
        }
    }

    if (menuEstado.jogando) {
        if (!executarFadeTransicao(&menuEstado, &menuEvent, 1.0, false)) {
            menuEstado.jogando = false;
        }
    }

    if (menuEstado.jogando) {
        al_rest(0.3);
        if (!executarDialogoInicial(&dialogo, &entidades.sistemaFase, &menuEvent, &menuEstado)) {
            menuEstado.jogando = false;
        }
    }

    if (menuEstado.jogando) {
        if (!executarCutsceneInicial(&entidades, &jogoCamera, &animacao, &menuEvent, &menuEstado, &bitmap, &controle, fonteDialogo)) {
            menuEstado.jogando = false;
        }
    }

    // =============LOOP PRINCIPAL==============
    ALLEGRO_EVENT event;
    while (menuEstado.jogando) {
        al_wait_for_event(fila_eventos, &event);

        // CONDIÇÃO DE PARADA
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            menuEstado.jogando = false;
        }

        // TECLADO
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
            verificar_Input(event, &w, &a, &s, &d, &espaco, &shift, &esc, &num1, &num2, &num3);

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_1) {
                    trocar_arma(&entidades.projetil, ARMA_VASSOURA);
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                    trocar_arma(&entidades.projetil, ARMA_VENENO);
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_3) {
                    trocar_arma(&entidades.projetil, ARMA_VACINA);
                }
            }
        }

        // PAUSE
        if (esc) {
            // Para o timer durante o pause
            al_stop_timer(timer);
            
            // Limpa a fila de eventos acumulados
            ALLEGRO_EVENT ev_temp;
            while (al_get_next_event(fila_eventos, &ev_temp)) {
                // Descarta eventos acumulados durante o jogo
            }
            
            configurarPosicoesBotoesPausa(&menuBotao);

            menu_pausa(&menuEstado, &menuEvent, &menuImg, &menuBotao, fonteDialogo);
            
            // LIMPA TODOS OS ESTADOS DE INPUT APÓS SAIR DO PAUSE
            w = false;
            a = false;
            s = false;
            d = false;
            espaco = false;
            shift = false;
            esc = false;  
            num1 = false;
            num2 = false;
            num3 = false;
            
            // Reinicia o timer se voltar ao jogo
            if (*menuEstado.jogando) {
                al_start_timer(timer);
            }
            else {
                break;  // saiu pelo "Sair" no pause
            }
        }

        // ATUALIZAÇÃO DE JOGO (quando não pausado)
        if (event.type == ALLEGRO_EVENT_TIMER && esc == false) {
            mover(&entidades.jogador, w, a, s, d, shift, VELOCIDADE_JOGADOR, &animacao.frames_por_sprite);
            restringirPosicao(&entidades.jogador, &entidades.caravana, WIDTH, HEIGHT, LARGURA_JOGADOR, ALTURA_JOGADOR);

            atualizar_movimento_inimigos(&entidades.caravana, entidades.inimigos, MAX_INIMIGOS);

            // Atualiza boss se estiver ativo
            atualizarBossAtivo(&entidades, &barras);

            camera_jogador(jogoCamera.posicaoCamera, entidades.jogador, WIDTH, LARGURA_JOGADOR, ALTURA_JOGADOR, entidades.caravana.caravanaX, entidades.caravana.caravanaVelocidade);
            redesenhar = true;
            
            // Atualiza movimento da caravana apenas se não houver boss ativo
            if (!controle.fase_boss_ativa) {
                atualizar_movimento_caravana(&entidades.caravana);
            }
        }

        // Processa colisões e dano 
        ResultadoColisao resultadoColisao = processarColisoes(&entidades, &barras, &controle, &jogoCamera);

        // Regeneração de vida
        processarRegeneracaoVida(&barras, &controle, resultadoColisao.ocorreuColisao);

        // VERIFICAÇÃO DE GAME OVER 
        if (barras.barraInfeccao.barraLargura >= 400.0f) {
            fimDeJogo = true;
            al_stop_timer(timer);
            al_identity_transform(&camera);
            al_use_transform(&camera);
            desenhar_tela_gameOver(&gameOver, &barras.barraInfeccao, &menuEvent, &menuEstado);
            if (!jogando) {
                break;
            }
        }

        // SISTEMA DE SPAWN E FASES 
        processarSpawnInimigos(&entidades, &controle, &bitmap, jogoCamera.posicaoCamera);

        // Verifica se boss foi derrotado
        verificarMorteBoss(&entidades, &controle);

        // Verifica se precisa mostrar diálogo de transição de fase
        if (controle.mostrar_dialogo_transicao) {
            controle.mostrar_dialogo_transicao = false;
                
            // Para o jogo e mostra diálogo
            al_stop_timer(timer);
            al_identity_transform(&camera);
            al_use_transform(&camera);
            
            // Mostra diálogo da fase atual (pode ser fase 2, 3 ou 4 - vitória)
            if (!executarDialogoInicial(&dialogo, &entidades.sistemaFase, &menuEvent, &menuEstado)) {
                menuEstado.jogando = false;
            }
            
            w = false;
            a = false;
            s = false;
            d = false;
            espaco = false;  // IMPORTANTE: Impede tiro automático
            shift = false;
            esc = false;
            num1 = false;
            num2 = false;
            num3 = false;

            // Limpa a fila de eventos que possam ter se acumulado durante o diálogo
            ALLEGRO_EVENT evento_temp;
            while (al_get_next_event(fila_eventos, &evento_temp)) {
                // Descarta eventos acumulados durante o diálogo
            }

            // Verifica se completou todas as fases APÓS exibir o diálogo
            if (entidades.sistemaFase.faseAtual > 3) {
                // Jogo concluído! Encerra após mostrar diálogo de vitória (fase 4)
                menuEstado.jogando = false;
                // Não precisa restaurar o timer, pois o jogo vai encerrar
            }
            else {
                // Restaura timer para continuar jogando
                if (menuEstado.jogando) {
                    al_start_timer(timer);
                }
            }
        }

        // CAMERA
        al_identity_transform(&camera);
        al_translate_transform(&camera, -jogoCamera.posicaoCamera[0], -jogoCamera.posicaoCamera[1]);
        al_use_transform(&camera);

        // DESENHO
        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            al_clear_to_color(jogoCamera.cor);

            // cenário primeiro
            desenhar_cenario(bitmap.cenario1, bitmap.cenario2, entidades.jogador.jogadorX, jogoCamera.posicaoCamera);

            // Infecção
            desenhar_barra(barras.barraFundo.barraX + jogoCamera.posicaoCamera[0], barras.barraFundo.barraY, barras.barraFundo.barraLargura + jogoCamera.posicaoCamera[0], barras.barraFundo.barraAltura);
            desenhar_barra_infeccao(barras.barraInfeccao.barraX, barras.barraInfeccao.barraY, barras.barraInfeccao.barraLargura, barras.barraInfeccao.barraAltura, jogoCamera.posicaoCamera);

            // BARRA DE VIDA DO BOSS
            if (controle.fase_boss_ativa) {
                desenhar_fundo_barra_vida_boss(barras.barraVidaBossFundo.barraX, barras.barraVidaBossFundo.barraY, barras.barraVidaBossFundo.barraLargura, barras.barraVidaBossFundo.barraAltura);
                desenhar_barra_vida_boss(barras.barraVidaBoss.barraX, barras.barraVidaBoss.barraY, barras.barraVidaBoss.barraLargura, barras.barraVidaBoss.barraAltura);
            }

            // caravana (usa cor retornada pela colisão)
            desenhar_caravana(bitmap.soldado, entidades.caravana.caravanaX, entidades.caravana.caravanaY, entidades.caravana.caravanaLargura, entidades.caravana.caravanaAltura, resultadoColisao.corCaravana);

            // jogador e inimigos
            desenhar_jogador(&entidades.jogador, w, a, s, d, espaco, &entidades.spritesJogador, entidades.projetil.tipo, &animacao.frame_atual, &animacao.contador_frame, animacao.frames_por_sprite, &animacao.virado_direita, &animacao.frame_tiro, &animacao.contador_frame_tiro);
            desenhar_todos_inimigos(entidades.inimigos, MAX_INIMIGOS);

            // tiros
            atirar_multiplos_inimigos(&entidades.projetil, entidades.jogador, entidades.inimigos, MAX_INIMIGOS, &bitmap, espaco, LARGURA_PROJETIL, ALTURA_PROJETIL, ALTURA_JOGADOR, LARGURA_JOGADOR, WIDTH, VELOCIDADE_PROJETIL, CADENCIA, jogoCamera.posicaoCamera, &entidades.sistemaFase, &barras.barraVidaBoss);

            // HUD (fixo na tela)
            char texto[100];
            if (controle.spawn_ativo) {
                float tempo_restante = controle.TEMPO_SPAWN - (al_get_time() - controle.timer_spawn_inimigos);
                if (tempo_restante > 0)
                    sprintf_s(texto, sizeof(texto), "PROJETO 1904 - NOVOS INIMIGOS EM: %.1f s", tempo_restante);
                else
                    sprintf_s(texto, sizeof(texto), "PROJETO 1904 - SPAWNING...");
            }
            else {
                int inimigos_restantes = entidades.sistemaFase.metaEliminacoes - entidades.sistemaFase.inimigosMortos;
                if (inimigos_restantes < 0) inimigos_restantes = 0;
                sprintf_s(texto, sizeof(texto), "PROJETO 1904 - INIMIGOS RESTANTES: %d | FASE %d", inimigos_restantes, entidades.sistemaFase.faseAtual);
            }

            ALLEGRO_TRANSFORM world_backup;
            al_copy_transform(&world_backup, al_get_current_transform());
            ALLEGRO_TRANSFORM hud;
            al_identity_transform(&hud);
            al_use_transform(&hud);
            al_draw_text(fonteDialogo, al_map_rgb(255, 255, 255), 10, 10, 0, texto);
            al_use_transform(&world_backup);

            al_flip_display();
            redesenhar = false;
        }
    } // fim while(jogando)

    // LIMPEZA
    if (fonteDialogo) {
        al_destroy_font(fonteDialogo);
    }

    destruir_bitmaps(&bitmap);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}