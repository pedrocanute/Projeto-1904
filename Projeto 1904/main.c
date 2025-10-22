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
    bool regrasAberta = false;
    bool jogando = false;            
    bool jogoPausado = false;
    bool fimDeJogo = false;
    bool dialogo1 = false, dialogo2 = false, dialogo3 = false;
    bool redesenhar = false;

    bool w = false, a = false, s = false, d = false, espaco = false, shift = false, esc = false, num1 = false, num2 = false, num3 = false;
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
    inicializar_array_inimigos(inimigos, MAX_INIMIGOS, bitmap.zumbi_direita, bitmap.zumbi_esquerda, bitmap.rato_direita, bitmap.rato_esquerda, bitmap.mosquito_direita, bitmap.mosquito_esquerda, posicaoCamera);

    ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);
    ALLEGRO_COLOR corCaravana = al_map_rgb(0, 0, 0);

    // JOGADOR
    Jogador jogador = { 120.0f, 520.0f, true, false };
    SpritesJogador spritesJogador = {
    .sprite_andando_direita = bitmap.sprite_andando_direita,
    .sprite_andando_esquerda = bitmap.sprite_andando_esquerda,
    .sprite_atirando_direita = bitmap.sprite_atirando_direita,
    .sprite_atirando_esquerda = bitmap.sprite_atirando_esquerda,
    .sprite_andando_direita_vassoura = bitmap.sprite_andando_direita_vassoura,
    .sprite_andando_esquerda_vassoura = bitmap.sprite_andando_esquerda_vassoura,
    .atacando_vassoura_direita = bitmap.atacando_vassoura_direita,
    .atacando_vassoura_esquerda = bitmap.atacando_vassoura_esquerda,
    .sprite_andando_direita_veneno = bitmap.sprite_andando_direita_veneno,
    .sprite_andando_esquerda_veneno = bitmap.sprite_andando_esquerda_veneno,
    .atacando_veneno_direita = bitmap.atacando_veneno_direita,
    .atacando_veneno_esquerda = bitmap.atacando_veneno_esquerda
    };
    projetil.tipo = ARMA_VACINA;
    // CARAVANA
    Caravana caravana = { 0.0f, 305.0f, 80.0f, 732.0f, 1.0f };

    // BARRA INFECÇÃO
    Barra barraFundo = { 75.0f, 50.0f, 400.0f, 100.0f };
    Barra barraInfeccao = { 75.0f, 50.0f, 75.0f, 100.0f };

    // BARRA VIDA BOSS
    BarraBoss barraVidaBossFundo = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    BarraBoss barraVidaBoss = {0.0f, 0.0f, 0.0f, 0.0f};

    //TIMER CARAVANA / BARRA INFECÇÃO
    float timer_regen_infeccao = 0.0f;
    const float TEMPO_REGEN_INFECCAO = 5.0f;

    //INTANGIBILIDADE
    const float TEMPO_INTANGIBILIDADE = 0.7f;

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
    int botaoSairDoJogoLargura = al_get_bitmap_width(bitmap.botaoSairDoJogo);
    int botaoSairDoJogoAltura = al_get_bitmap_height(bitmap.botaoSairDoJogo);
    int oswaldoLargura = al_get_bitmap_width(bitmap.oswaldo);
    int oswaldoAltura = al_get_bitmap_height(bitmap.oswaldo);
    int caixaDialogoLargura = al_get_bitmap_width(bitmap.caixaDialogo);
    int caixaDialogoAltura = al_get_bitmap_height(bitmap.caixaDialogo);

    int botaoJogarX = 200, botaoJogarY = 620;
    int botaoRegrasX = 500, botaoRegrasY = 620;
    int botaoSairX = 800, botaoSairY = 620;
    int botaoVoltarX = 520, botaoVoltarY = 500;
    int botaoSairDoJogoX = 445, botaoSairDoJogoY = 560;
    int caixaDialogoX = 10, caixaDialogoY = 540;

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
    MenuEstados menuEstado = {
        .telaMenu = &telaMenu,
        .jogando = &jogando,
        .regrasAberta = &regrasAberta,
        .esc = &esc,
        .jogoPausado = &jogoPausado,
        .fimDeJogo = &fimDeJogo,
    };

    MenuEvents menuEvent = {
        .fila_eventos = fila_eventos,
        .timer = timer,
        .camera = &camera,
        .mouseX = &mouseX,
        .mouseY = &mouseY
    };

    MenuImagens menuImg = {
        .fundoMenu = bitmap.fundoMenu,
        .botaoJogar = bitmap.botaoJogar, .botaoJogar2 = bitmap.botaoJogar2,
        .botaoRegras = bitmap.botaoRegras, .botaoRegras2 = bitmap.botaoRegras2,
        .botaoSair = bitmap.botaoSair, .botaoSair2 = bitmap.botaoSair2,
        .abaRegras = bitmap.abaRegras,
        .botaoVoltar = bitmap.botaoVoltar, .botaoVoltar2 = bitmap.botaoVoltar2
    };

    MenuBotoes menuBotao = {
        .botaoJogarX = botaoJogarX, .botaoJogarY = botaoJogarY,
        .botaoRegrasX = botaoRegrasX, .botaoRegrasY = botaoRegrasY,
        .botaoSairX = botaoSairX, .botaoSairY = botaoSairY,
        .botaoVoltarX = botaoVoltarX, .botaoVoltarY = botaoVoltarY,

        .botaoJogarLargura = botaoJogarLargura, .botaoJogarAltura = botaoJogarAltura,
        .botaoRegrasLargura = botaoRegrasLargura, .botaoRegrasAltura = botaoRegrasAltura,
        .botaoSairLargura = botaoSairLargura, .botaoSairAltura = botaoSairAltura,
        .fundoMenuLargura = fundoMenuLargura, .fundoMenuAltura = fundoMenuAltura,
        .abaRegrasLargura = abaRegrasLargura, .abaRegrasAltura = abaRegrasAltura,
        .botaoVoltarLargura = botaoVoltarLargura, .botaoVoltarAltura = botaoVoltarAltura
    };

    GameOver gameOver = {
        .telaGameOver = bitmap.telaGameOver,
        .botaoSairDoJogo = bitmap.botaoSairDoJogo,
        .botaoSairDoJogo2 = bitmap.botaoSairDoJogo2,

        .telaGameOverLargura = telaGameOverLargura,
        .telaGameOverAltura = telaGameOverAltura,
        .botaoSairDoJogoX = botaoSairDoJogoX, .botaoSairDoJogoY = botaoSairDoJogoY,        
        .botaoSairDoJogoLargura = botaoSairDoJogoLargura, .botaoSairDoJogoAltura = botaoSairDoJogoAltura
    };

    Dialogo dialogo = {
    .oswaldo = bitmap.oswaldo,
    .caixaDialogo = bitmap.caixaDialogo,

    .oswaldoLargura = oswaldoLargura,
    .oswaldoAltura = oswaldoAltura,
    .caixaDialogoLargura = caixaDialogoLargura, .caixaDialogoAltura = caixaDialogoAltura,
    .caixaDialogoX = caixaDialogoX, .caixaDialogoY = caixaDialogoY,

    .dialogo1 = &dialogo1,
    .dialogo2 = &dialogo2,
    .dialogo3 = &dialogo3
    };

    // MENU PRINCIPAL
    menu_principal(&menuEstado, &menuEvent, &menuImg, &menuBotao);
    if (!jogando) {
        // Saiu pelo menu
        al_destroy_font(font);
        destruir_bitmaps(&bitmap);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        return 0;
    }

    desenhar_tela_dialogo(&dialogo, &sistemaFase, &menuEvent, &menuEstado);
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
            verificar_Input(event, &w, &a, &s, &d, &espaco, &shift, &esc, &num1, &num2, &num3);
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                esc = true;
            }

            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_1) {
                    trocar_arma(&projetil, ARMA_VASSOURA);
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_2) {
                    trocar_arma(&projetil, ARMA_VENENO);
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_3) {
                    trocar_arma(&projetil, ARMA_VACINA);
                }
            }
        }

        // PAUSE
        if (esc) {
            menuBotao.botaoJogarX = 525; menuBotao.botaoJogarY = 260;
            menuBotao.botaoRegrasX = 525; menuBotao.botaoRegrasY = 340;
            menuBotao.botaoSairX = 525; menuBotao.botaoSairY = 420;

            menu_pausa(&menuEstado, &menuEvent, &menuImg, &menuBotao);
            if (!jogando)
                break;  // saiu pelo "Sair" no pause
            // se voltou, esc foi limpo dentro de menu_pausa
        }

        // ATUALIZAÇÃO DE JOGO (quando não pausado)
        if (event.type == ALLEGRO_EVENT_TIMER && esc == false) {
            mover(&jogador, w, a, s, d, shift, VELOCIDADE_JOGADOR, &frames_por_sprite);
            restringirPosicao(&jogador, &caravana, WIDTH, HEIGHT, LARGURA_JOGADOR, ALTURA_JOGADOR);

            atualizar_movimento_inimigos(&caravana, inimigos, MAX_INIMIGOS);
            
            for (int i = 0; i < MAX_INIMIGOS; i++) {
                
                if (inimigos[i].ativo &&
                    (inimigos[i].tipo == TIPO_BOSS || inimigos[i].tipo == TIPO_BOSS_RATO)) {

                    atualizar_boss_perseguindo(&inimigos[i], &jogador, 120.0f);

                    // Posiciona barra de vida do boss
                    barraVidaBoss.barraX = inimigos[i].botX + (inimigos[i].larguraBot - 300) / 2;
                    barraVidaBoss.barraY = inimigos[i].botY - 30;
                    barraVidaBoss.barraLargura = barraVidaBoss.barraX + barraVidaBoss.barraVida;
                    barraVidaBoss.barraAltura = barraVidaBoss.barraY + 25;

                    // Fundo da barra
                    barraVidaBossFundo.barraX = inimigos[i].botX + (inimigos[i].larguraBot - 300) / 2;
                    barraVidaBossFundo.barraY = inimigos[i].botY - 30;
                    barraVidaBossFundo.barraLargura = barraVidaBossFundo.barraX + 300;
                    barraVidaBossFundo.barraAltura = barraVidaBossFundo.barraY + 25;

                    break;
                }
            }

            camera_jogador(posicaoCamera, jogador, WIDTH, LARGURA_JOGADOR, ALTURA_JOGADOR, caravana.caravanaX, caravana.caravanaVelocidade);
            redesenhar = true;
            if (!fase_boss_ativa) {
                atualizar_movimento_caravana(&caravana);
            }
        }

        //Colisão de inimigo com caravana / Intangibilidade / colisão boss com personagem
        bool colisaoCaravana = false;
        float dano_na_caravana = 0.0f;
        ALLEGRO_COLOR corCaravana = al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f); // branco;

        if (fase_boss_ativa) {
            for (int i = 0; i < MAX_INIMIGOS; i++) {
                if (inimigos[i].ativo && (inimigos[i].tipo == TIPO_BOSS || inimigos[i].tipo == TIPO_BOSS_RATO)) {
                    if (colisao_jogador_inimigo(&inimigos[0], &jogador, LARGURA_JOGADOR, ALTURA_JOGADOR)) {
                        float tempoAtual = al_get_time();
                        if (tempoAtual - inimigos[i].timer_intangibilidade >= TEMPO_INTANGIBILIDADE) {
                            colisaoCaravana = true;
                            barraInfeccao.barraLargura += inimigos[i].dano;
                            timer_regen_infeccao = tempoAtual;
                            inimigos[i].timer_intangibilidade = tempoAtual;

                            dano_na_caravana = tempoAtual + 0.12f; //aplica um temporizador de dano

                            if (al_get_time() < dano_na_caravana) { //verifica o timer de dano e muda a cor da sprite para representar o dano
                                corCaravana = al_map_rgba_f(1.0f, 0.3f, 0.3f, 1.0f); // vermelho claro
                            }
                            else {
                                corCaravana = al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f); // branco no Allegro o branco nao altera a cor da sprite original
                            }
                        }
                    }
                }
            }
        }
        else{
            for (int i = 0; i < MAX_INIMIGOS; i++) {
                if (colisao_inimigo_caravana(&inimigos[i], &caravana, caravana.caravanaLargura, caravana.caravanaAltura) && barraInfeccao.barraLargura < 400) {
                    float tempoAtual = al_get_time();
                    if (tempoAtual - inimigos[i].timer_intangibilidade >= TEMPO_INTANGIBILIDADE) {
                        colisaoCaravana = true;
                        barraInfeccao.barraLargura += inimigos[i].dano;
                        timer_regen_infeccao = tempoAtual;
                        inimigos[i].timer_intangibilidade = tempoAtual;

                        dano_na_caravana = tempoAtual + 0.12f; //aplica um temporizador de dano

                        if (al_get_time() < dano_na_caravana) { //verifica o timer de dano e muda a cor da sprite para representar o dano
                            corCaravana = al_map_rgba_f(1.0f, 0.3f, 0.3f, 1.0f); // vermelho claro
                        }
                        else {
                            corCaravana = al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f); // branco no Allegro nao altera a cor da sprite original
                        }
                    }
                }
            }
        }
       

        //Regeneração de Vida
        // !!!!!!!!!Compara se não há colisão e se a caravana levou dano!!!!!!!!!
        if (!colisaoCaravana && al_get_time() - timer_regen_infeccao >= TEMPO_REGEN_INFECCAO && barraInfeccao.barraLargura > barraInfeccao.barraX) {
            barraInfeccao.barraLargura--;
        }

        // VERIFICAÇÃO DE GAME OVER 
        // Verifica se a barra de infecção atingiu o máximo
        if (barraInfeccao.barraLargura >= 400.0f) {
            fimDeJogo = true;

            // Para o timer do jogo
            al_stop_timer(timer);

            // Reseta a transformação da câmera para a tela de game over
            al_identity_transform(&camera);
            al_use_transform(&camera);

            // Chama a tela de game over
            desenhar_tela_gameOver(&gameOver, &barraInfeccao, &menuEvent, &menuEstado);

            if (!jogando) {
                break;
            }

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
                        if (!inimigos[i].ativo) { 
                            idx_boss = i; 
                            break; 
                        }
                    }
                    if (idx_boss >= 0) {

                        if (sistemaFase.faseAtual == 1) {
                            spawnar_boss_rato(&inimigos[idx_boss], bitmap.rato_direita, bitmap.rato_esquerda, posicaoCamera);
                        }
                        else if (sistemaFase.faseAtual == 2 || sistemaFase.faseAtual == 3) {
                            spawnar_boss(&inimigos[idx_boss], bitmap.boss_variola_direita, bitmap.boss_variola_esquerda, posicaoCamera);
                        }
                        boss_spawnado = true;
                        fase_boss_ativa = true;
                        spawn_ativo = false;
                    barraVidaBoss.barraVida = 300.0f;
                    }
                }
            }
        }
        else {
            // Durante o boss: pausar novos spawns comuns e aguardar morte do boss
            bool bossVivo = false;
            for (int i = 0; i < MAX_INIMIGOS; ++i) {
                if (inimigos[i].ativo && (inimigos[i].tipo == TIPO_BOSS || inimigos[i].tipo == TIPO_BOSS_RATO)) {
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
            desenhar_barra(barraFundo.barraX + posicaoCamera[0], barraFundo.barraY, barraFundo.barraLargura + posicaoCamera[0], barraFundo.barraAltura);
            desenhar_barra_infeccao(barraInfeccao.barraX, barraInfeccao.barraY, barraInfeccao.barraLargura, barraInfeccao.barraAltura, posicaoCamera);

            // BARRA DE VIDA DO BOSS
            if (fase_boss_ativa) {
                desenhar_fundo_barra_vida_boss(barraVidaBossFundo.barraX, barraVidaBossFundo.barraY, barraVidaBossFundo.barraLargura, barraVidaBossFundo.barraAltura);
                desenhar_barra_vida_boss(barraVidaBoss.barraX, barraVidaBoss.barraY, barraVidaBoss.barraLargura, barraVidaBoss.barraAltura);
            };
            // caravana
            desenhar_caravana(bitmap.soldado, caravana.caravanaX, caravana.caravanaY , caravana.caravanaLargura, caravana.caravanaAltura, corCaravana);

            // jogador e inimigos
            desenhar_jogador(&jogador, w, a, s, d, espaco,&spritesJogador, projetil.tipo,&frame_atual, &contador_frame, frames_por_sprite,&virado_direita, &frame_tiro, &contador_frame_tiro);
            desenhar_todos_inimigos(inimigos, MAX_INIMIGOS);
            desenhar_todos_inimigos(inimigos, MAX_INIMIGOS);

            // tiros
            atirar_multiplos_inimigos(&projetil, jogador, inimigos, MAX_INIMIGOS, &bitmap, espaco,LARGURA_PROJETIL, ALTURA_PROJETIL,ALTURA_JOGADOR, LARGURA_JOGADOR,WIDTH, VELOCIDADE_PROJETIL, CADENCIA, posicaoCamera, &sistemaFase, &barraVidaBoss);

            // Diálogo
            desenhar_tela_dialogo(&dialogo, &sistemaFase, &menuEvent, &menuEstado);

            // Game Over
            desenhar_tela_gameOver(&gameOver, &barraInfeccao, &menuEvent, &menuEstado);

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
