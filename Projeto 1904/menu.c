#include "menu.h"
#include "configuracoes.h"

void menu_principal(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao, ALLEGRO_FONT* fonte) {
    ALLEGRO_EVENT event;

    // Limpa fila de eventos
    al_flush_event_queue(menuEvent->fila_eventos);

    while (*menuEstado->telaMenu) {
        al_wait_for_event(menuEvent->fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *menuEstado->telaMenu = false;
            *menuEstado->jogando = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *menuEvent->mouseX = event.mouse.x;
            *menuEvent->mouseY = event.mouse.y;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Botão Jogar
            if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
                *menuEstado->telaMenu = false;
                *menuEstado->jogando = true;
                break;
            }

            // Botão Regras
            if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
                *menuEstado->regrasAberta = true;

                while (*menuEstado->regrasAberta) {
                    al_wait_for_event(menuEvent->fila_eventos, &event);

                    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        *menuEstado->regrasAberta = false;
                        *menuEstado->telaMenu = false;
                        *menuEstado->jogando = false;
                        break;
                    }

                    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        *menuEvent->mouseX = event.mouse.x;
                        *menuEvent->mouseY = event.mouse.y;
                    }

                    // ========== DESENHA TELA DE REGRAS COM FUNDO ROXO ==========
                    
                    // Fundo roxo sólido (mesma cor do menu de pausa)
                    al_clear_to_color(al_map_rgb(45, 29, 46));

                    // Título
                    float centroX = WIDTH / 2.0f;
                    float inicioY = 60.0f;
                    float espacamento = 32.0f;

                    if (fonte) {
                        // Título
                        al_draw_text(fonte, al_map_rgb(255, 215, 0), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "COMANDOS DO JOGO");
                        inicioY += espacamento * 1.8f;

                        // Comandos de movimento
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "WASD - Mover");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "SHIFT - Correr");
                        inicioY += espacamento;

                        // Comandos de combate
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "ESPACO - Atirar");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "1 - Vassoura (Efetivo contra Ratos)");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "2 - Veneno (Efetivo contra Mosquitos)");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "3 - Vacina (Efetivo contra Zumbis)");
                        inicioY += espacamento * 1.5f;

                        // Menu
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "ESC - Pausar");
                        inicioY += espacamento * 1.8f;

                        // Objetivo
                        al_draw_text(fonte, al_map_rgb(255, 215, 0), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "OBJETIVO");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Proteja a caravana e elimine as epidemias!");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Use a arma certa para cada inimigo.");
                    }

                    // Botão Voltar 
                    int botaoVoltarYAjustado = 600;
                    if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= botaoVoltarYAjustado && *menuEvent->mouseY <= botaoVoltarYAjustado + menuBotao->botaoVoltarAltura) {
                        al_draw_bitmap(menuImg->botaoVoltar2, menuBotao->botaoVoltarX, botaoVoltarYAjustado, 0);
                    }
                    else {
                        al_draw_bitmap(menuImg->botaoVoltar, menuBotao->botaoVoltarX, botaoVoltarYAjustado, 0);
                    }

                    al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= botaoVoltarYAjustado && *menuEvent->mouseY <= botaoVoltarYAjustado + menuBotao->botaoVoltarAltura) {
                            *menuEstado->regrasAberta = false;
                        }
                    }
                }
            }

            // Botão Sair
            if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
                *menuEstado->telaMenu = false;
                *menuEstado->jogando = false;
                break;
            }
        }

        // Desenha menu principal
        al_draw_scaled_bitmap(menuImg->fundoMenu, 0, 0, menuBotao->fundoMenuLargura, menuBotao->fundoMenuAltura, 0, 0, WIDTH, HEIGHT, 0);

        // Botão Jogar (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
            al_draw_bitmap(menuImg->botaoJogar2, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoJogar, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }

        // Botão Regras (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
            al_draw_bitmap(menuImg->botaoRegras2, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoRegras, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }

        // Botão Sair (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
            al_draw_bitmap(menuImg->botaoSair2, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoSair, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }

        al_flip_display();
    }
}

void menu_pausa(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao, ALLEGRO_FONT* fonte) {
    if (!*menuEstado->esc) return;

    al_stop_timer(menuEvent->timer);
    al_identity_transform(menuEvent->camera);
    al_use_transform(menuEvent->camera);

    // Limpa eventos antigos
    al_flush_event_queue(menuEvent->fila_eventos);

    ALLEGRO_EVENT event;
    *menuEstado->jogoPausado = true;

    while (*menuEstado->jogoPausado) {
        al_wait_for_event(menuEvent->fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *menuEstado->jogando = false;
            *menuEstado->jogoPausado = false;
            *menuEstado->esc = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *menuEvent->mouseX = event.mouse.x;
            *menuEvent->mouseY = event.mouse.y;
        }

        // ESC para continuar
        if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
            event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            *menuEstado->jogoPausado = false;
            *menuEstado->esc = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Continuar
            if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
                *menuEstado->jogoPausado = false;
                *menuEstado->esc = false;
                break;
            }

            // Regras
            if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {

                // Entra no submenu de regras
                bool regrasAbertaLocal = true;
                al_flush_event_queue(menuEvent->fila_eventos);

                while (regrasAbertaLocal) {
                    al_wait_for_event(menuEvent->fila_eventos, &event);

                    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        *menuEstado->jogando = false;
                        *menuEstado->jogoPausado = false;
                        *menuEstado->esc = false;
                        regrasAbertaLocal = false;
                        break;
                    }

                    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        *menuEvent->mouseX = event.mouse.x;
                        *menuEvent->mouseY = event.mouse.y;
                    }

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        // Verifica clique no botão voltar
                        int botaoVoltarYAjustado = 600;
                        if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= botaoVoltarYAjustado && *menuEvent->mouseY <= botaoVoltarYAjustado + menuBotao->botaoVoltarAltura) {
                            regrasAbertaLocal = false;
                            // Limpa eventos antes de voltar
                            al_flush_event_queue(menuEvent->fila_eventos);
                        }
                    }

                    // ========== DESENHA TELA DE REGRAS COM FUNDO ROXO ==========
                    
                    // Fundo roxo sólido
                    al_clear_to_color(al_map_rgb(45, 29, 46));

                    // Título
                    float centroX = WIDTH / 2.0f;
                    float inicioY = 60.0f;
                    float espacamento = 32.0f;

                    if (fonte) {
                        // Título
                        al_draw_text(fonte, al_map_rgb(255, 215, 0), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "COMANDOS DO JOGO");
                        inicioY += espacamento * 1.8f;

                        // Comandos de movimento
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "WASD - Mover");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "SHIFT - Correr");
                        inicioY += espacamento;

                        // Comandos de combate
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "ESPACO - Atirar");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "1 - Vassoura (Efetivo contra Ratos)");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "2 - Veneno (Efetivo contra Mosquitos)");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "3 - Vacina (Efetivo contra Zumbis)");
                        inicioY += espacamento * 1.5f;

                        // Menu
                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "ESC - Pausar");
                        inicioY += espacamento * 1.8f;

                        // Objetivo
                        al_draw_text(fonte, al_map_rgb(255, 215, 0), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "OBJETIVO");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Proteja a caravana e elimine as epidemias!");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Use a arma certa para cada inimigo.");
                    }

                    // Desenha botão voltar 
                    int botaoVoltarYAjustado = 600;
                    if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= botaoVoltarYAjustado && *menuEvent->mouseY <= botaoVoltarYAjustado + menuBotao->botaoVoltarAltura) {
                        al_draw_bitmap(menuImg->botaoVoltar2, menuBotao->botaoVoltarX, botaoVoltarYAjustado, 0);
                    }
                    else {
                        al_draw_bitmap(menuImg->botaoVoltar, menuBotao->botaoVoltarX, botaoVoltarYAjustado, 0);
                    }

                    al_flip_display();
                }
                continue;
            }

            // Sair
            if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
                *menuEstado->jogando = false;
                *menuEstado->jogoPausado = false;
                *menuEstado->esc = false;
                break;
            }
        }

        // Redesenha UI do pause
        al_clear_to_color(al_map_rgb(45, 29, 46)); // Fundo Roxo do Menu Pause

        // Botão Continuar (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
            al_draw_bitmap(menuImg->botaoJogar2, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoJogar, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }

        // Botão Regras (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
            al_draw_bitmap(menuImg->botaoRegras2, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoRegras, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }

        // Botão Sair (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
            al_draw_bitmap(menuImg->botaoSair2, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoSair, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }

        al_flip_display();
    }

    // Configurar posições dos botões de pausa
    configurarPosicoesBotoesPausa(menuBotao);
}

void desenhar_tela_gameOver(GameOver* gameover, Barra* infec, MenuEvents* menuEvent, MenuEstados* menuEstado) {
    ALLEGRO_EVENT event;

    // Limpa fila de eventos
    al_flush_event_queue(menuEvent->fila_eventos);

    while (*menuEstado->fimDeJogo) {
        al_wait_for_event(menuEvent->fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *menuEstado->jogando = false;
            *menuEstado->fimDeJogo = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *menuEvent->mouseX = event.mouse.x;
            *menuEvent->mouseY = event.mouse.y;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Sair do jogo
            if (*menuEvent->mouseX >= gameover->botaoSairDoJogoX && *menuEvent->mouseX <= gameover->botaoSairDoJogoX + gameover->botaoSairDoJogoLargura && *menuEvent->mouseY >= gameover->botaoSairDoJogoY && *menuEvent->mouseY <= gameover->botaoSairDoJogoY + gameover->botaoSairDoJogoAltura) {
                *menuEstado->jogando = false;
                *menuEstado->fimDeJogo = false;
                break;
            }
        }

        // Desenha tela de Game Over
        al_draw_scaled_bitmap(gameover->telaGameOver, 0, 0, gameover->telaGameOverLargura, gameover->telaGameOverAltura, 0, 0, WIDTH, HEIGHT, 0);

        // Botão Sair (hover)
        if (*menuEvent->mouseX >= gameover->botaoSairDoJogoX && *menuEvent->mouseX <= gameover->botaoSairDoJogoX + gameover->botaoSairDoJogoLargura && *menuEvent->mouseY >= gameover->botaoSairDoJogoY && *menuEvent->mouseY <= gameover->botaoSairDoJogoY + gameover->botaoSairDoJogoAltura) {
            al_draw_bitmap(gameover->botaoSairDoJogo2, gameover->botaoSairDoJogoX, gameover->botaoSairDoJogoY, 0);
        }
        else {
            al_draw_bitmap(gameover->botaoSairDoJogo, gameover->botaoSairDoJogoX, gameover->botaoSairDoJogoY, 0);
        }

        al_flip_display();
    }
}

void desenhar_tela_dialogo(Dialogo* dialogo, SistemaFases* fase, MenuEvents* menuEvent, MenuEstados* menuEstado) {
    // Valida se diálogo foi inicializado corretamente
    if (!dialogo || !dialogo->falando || !dialogo->balao || !dialogo->fonteDialogo) {
        return; // Não desenha se recursos essenciais não foram carregados
    }

    // Reseta transformação da câmera
    al_identity_transform(menuEvent->camera);
    al_use_transform(menuEvent->camera);

    // Verifica se deve iniciar diálogo baseado na fase
    bool iniciarDialogo = false;
    if (fase->faseAtual == 1 && !dialogo->dialogo1) {
        iniciarDialogo = true;
        configurarTextosDialogo(dialogo, 1);
    }
    else if (fase->faseAtual == 2 && !dialogo->dialogo2) {
        iniciarDialogo = true;
        configurarTextosDialogo(dialogo, 2);
    }
    else if (fase->faseAtual == 3 && !dialogo->dialogo3) {
        iniciarDialogo = true;
        configurarTextosDialogo(dialogo, 3);
    }

    if (!iniciarDialogo) return;

    // Para o timer do jogo
    al_stop_timer(menuEvent->timer);

    // Limpa eventos antigos
    al_flush_event_queue(menuEvent->fila_eventos);

    ALLEGRO_EVENT event;
    bool dialogoAtivo = true;

    // Reinicia no primeiro texto
    dialogo->textoAtual = 0;

    // Timer manual para animação
    double ultimoTempoAnimacao = al_get_time();
    const double TEMPO_POR_FRAME = 0.15;

    bool precisaRedesenhar = true;

    // FADE IN (clarear) - 1 segundo
    float alfaFadeIn = 255.0f;
    double tempoInicioFadeIn = al_get_time();
    const double DURACAO_FADE_IN = 1.0;
    bool fadeInCompleto = false;

    while (dialogoAtivo && *menuEstado->jogando) {

        bool temEvento = al_get_next_event(menuEvent->fila_eventos, &event);

        if (temEvento) {

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                *menuEstado->jogando = false;
                dialogoAtivo = false;
                // Limpa fila para garantir que o loop principal detecta o fechamento
                al_flush_event_queue(menuEvent->fila_eventos);
                return; 
            }

            // SPACE avança texto (só após fade in)
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && fadeInCompleto) {
                // ENTER pula todo o diálogo
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    dialogoAtivo = false;
                    break;
                }
                // SPACE avança para o próximo texto
                else if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    dialogo->textoAtual++;

                    if (dialogo->textoAtual >= dialogo->numeroTextos) {
                        dialogoAtivo = false;
                    }
                    precisaRedesenhar = true;
                }
            }
        }

        // Verifica se ainda está jogando
        if (!*menuEstado->jogando) {
            return; // Sai imediatamente
        }

        // Atualiza fade in
        if (!fadeInCompleto) {
            double tempoAtual = al_get_time();
            double progresso = (tempoAtual - tempoInicioFadeIn) / DURACAO_FADE_IN;

            if (progresso >= 1.0) {
                alfaFadeIn = 0.0f;
                fadeInCompleto = true;
            }
            else {
                alfaFadeIn = 255.0f * (1.0f - progresso);
            }
            precisaRedesenhar = true;
        }

        // Atualiza animação
        double tempoAtual = al_get_time();
        if (tempoAtual - ultimoTempoAnimacao >= TEMPO_POR_FRAME) {
            dialogo->frameAtual = (dialogo->frameAtual + 1) % 2;
            ultimoTempoAnimacao = tempoAtual;
            precisaRedesenhar = true;
        }

        // Desenha sempre
        if (precisaRedesenhar) {
            // 1. FUNDO RGB(45, 29, 46)
            al_clear_to_color(al_map_rgb(45, 29, 46));

            // 2. SPRITESHEET FALANDO
            int larguraFrame = 576;
            int alturaFrame = 576;
            int falandoX = (WIDTH / 2) - (larguraFrame / 2);
            int falandoY = -20;
            int sx = dialogo->frameAtual * larguraFrame;
            int sy = 0;

            al_draw_bitmap_region(dialogo->falando, sx, sy, larguraFrame, alturaFrame, falandoX, falandoY, 0);

            // 3. BALÃO
            int balaoX = (WIDTH / 2) - (1000 / 2);
            int balaoY = HEIGHT - 280 - 20;
            al_draw_bitmap(dialogo->balao, balaoX, balaoY, 0);

            // 4. TEXTO
            if (dialogo->textoAtual < dialogo->numeroTextos) {
                char* textoAtual = dialogo->textos[dialogo->textoAtual];

                float textoX = balaoX + 40;
                float textoY = balaoY + 30;
                float textoLarguraMax = 920.0f;
                float alturaLinha = 28.0f;

                al_draw_multiline_text(dialogo->fonteDialogo, al_map_rgb(255, 255, 255), textoX, textoY, textoLarguraMax, alturaLinha, ALLEGRO_ALIGN_LEFT, textoAtual);

                // Indicador
                float indicadorX = balaoX + 560;
                float indicadorY = balaoY + 220;

                if (dialogo->textoAtual < dialogo->numeroTextos - 1) {
                    al_draw_text(dialogo->fonteDialogo, al_map_rgb(200, 200, 200), indicadorX, indicadorY, ALLEGRO_ALIGN_RIGHT, "[SPACE]");
                }
            }

            // 5. FADE IN
            if (!fadeInCompleto) {
                al_draw_filled_rectangle(0, 0, WIDTH, HEIGHT,
                    al_map_rgba_f(0, 0, 0, alfaFadeIn / 255.0f));
            }

            al_flip_display();
            precisaRedesenhar = false;
        }

        al_rest(0.001);
    }

    // Verifica se foi fechado antes de continuar
    if (!*menuEstado->jogando) {
        return;
    }

    // Marca diálogo completo
    if (fase->faseAtual == 1) dialogo->dialogo1 = true;
    if (fase->faseAtual == 2) dialogo->dialogo2 = true;
    if (fase->faseAtual == 3) dialogo->dialogo3 = true;

    // Limpa eventos antes de retomar
    al_flush_event_queue(menuEvent->fila_eventos);

    // Retoma o jogo
    if (*menuEstado->jogando) {
        al_start_timer(menuEvent->timer);
    }
}

void inicializarMenuEstados(MenuEstados* estado, bool* telaMenu, bool* jogando,bool* regrasAberta, bool* esc, bool* jogoPausado,bool* fimDeJogo) {
    if (!estado) return;

    estado->telaMenu = telaMenu;
    estado->jogando = jogando;
    estado->regrasAberta = regrasAberta;
    estado->esc = esc;
    estado->jogoPausado = jogoPausado;
    estado->fimDeJogo = fimDeJogo;
}

void inicializarMenuEvents(MenuEvents* events, ALLEGRO_EVENT_QUEUE* fila,ALLEGRO_TIMER* timer, ALLEGRO_TRANSFORM* camera,float* mouseX, float* mouseY) {
    if (!events) return;

    events->fila_eventos = fila;
    events->timer = timer;
    events->camera = camera;
    events->mouseX = mouseX;
    events->mouseY = mouseY;
}

void inicializarMenuImagens(MenuImagens* img, Bitmaps* bitmap) {
    if (!img || !bitmap) return;

    img->fundoMenu = bitmap->fundoMenu;
    img->botaoJogar = bitmap->botaoJogar;
    img->botaoJogar2 = bitmap->botaoJogar2;
    img->botaoRegras = bitmap->botaoRegras;
    img->botaoRegras2 = bitmap->botaoRegras2;
    img->botaoSair = bitmap->botaoSair;
    img->botaoSair2 = bitmap->botaoSair2;
    img->abaRegras = bitmap->abaRegras;
    img->botaoVoltar = bitmap->botaoVoltar;
    img->botaoVoltar2 = bitmap->botaoVoltar2;
}

void inicializarMenuBotoes(MenuBotoes* botao, Bitmaps* bitmap) {
    if (!botao || !bitmap) return;

    // Obtém dimensões dos bitmaps
    botao->botaoJogarLargura = al_get_bitmap_width(bitmap->botaoJogar);
    botao->botaoJogarAltura = al_get_bitmap_height(bitmap->botaoJogar);
    botao->botaoRegrasLargura = al_get_bitmap_width(bitmap->botaoRegras);
    botao->botaoRegrasAltura = al_get_bitmap_height(bitmap->botaoRegras);
    botao->botaoSairLargura = al_get_bitmap_width(bitmap->botaoSair);
    botao->botaoSairAltura = al_get_bitmap_height(bitmap->botaoSair);
    botao->fundoMenuLargura = al_get_bitmap_width(bitmap->fundoMenu);
    botao->fundoMenuAltura = al_get_bitmap_height(bitmap->fundoMenu);
    botao->abaRegrasLargura = al_get_bitmap_width(bitmap->abaRegras);
    botao->abaRegrasAltura = al_get_bitmap_height(bitmap->abaRegras);
    botao->botaoVoltarLargura = al_get_bitmap_width(bitmap->botaoVoltar);
    botao->botaoVoltarAltura = al_get_bitmap_height(bitmap->botaoVoltar);

    // Define posições dos botões - MENU PRINCIPAL
    botao->botaoJogarX = 200;
    botao->botaoJogarY = 520;
    botao->botaoRegrasX = 500;
    botao->botaoRegrasY = 520;
    botao->botaoSairX = 800;
    botao->botaoSairY = 520;
    botao->botaoVoltarX = 520;
    botao->botaoVoltarY = 500;
}

void inicializarGameOver(GameOver* gameOver, Bitmaps* bitmap) {
    if (!gameOver || !bitmap) return;

    gameOver->telaGameOver = bitmap->telaGameOver;
    gameOver->botaoSairDoJogo = bitmap->botaoSairDoJogo;
    gameOver->botaoSairDoJogo2 = bitmap->botaoSairDoJogo2;

    gameOver->telaGameOverLargura = al_get_bitmap_width(bitmap->telaGameOver);
    gameOver->telaGameOverAltura = al_get_bitmap_height(bitmap->telaGameOver);
    gameOver->botaoSairDoJogoLargura = al_get_bitmap_width(bitmap->botaoSairDoJogo);
    gameOver->botaoSairDoJogoAltura = al_get_bitmap_height(bitmap->botaoSairDoJogo);

    // Posições dos botões
    gameOver->botaoSairDoJogoX = 445;
    gameOver->botaoSairDoJogoY = 560;
}

void inicializarDialogo(Dialogo* dialogo, Bitmaps* bitmap, ALLEGRO_FONT* fonte) {
    if (!dialogo || !bitmap) return;

    // Bitmaps - inicializa mesmo se fonte for NULL
    dialogo->falando = bitmap->falando;
    dialogo->balao = bitmap->balao;
    dialogo->fonteDialogo = fonte;

    // Dimensões dos bitmaps - verifica se bitmaps existem
    if (bitmap->falando) {
        dialogo->falandoLargura = al_get_bitmap_width(bitmap->falando);
        dialogo->falandoAltura = al_get_bitmap_height(bitmap->falando);
    }
    else {
        dialogo->falandoLargura = 0;
        dialogo->falandoAltura = 0;
    }

    if (bitmap->balao) {
        dialogo->balaoLargura = al_get_bitmap_width(bitmap->balao);
        dialogo->balaoAltura = al_get_bitmap_height(bitmap->balao);
    }
    else {
        dialogo->balaoLargura = 0;
        dialogo->balaoAltura = 0;
    }

    // Configurações de animação
    dialogo->frameAtual = 0;
    dialogo->contadorFrame = 0;
    dialogo->velocidadeAnimacao = 20;

    // Estados iniciais
    dialogo->numeroTextos = 0;
    dialogo->textoAtual = 0;
    dialogo->dialogo1 = false;
    dialogo->dialogo2 = false;
    dialogo->dialogo3 = false;

    // Configura textos da fase 1 (padrão)
    configurarTextosDialogo(dialogo, 1);
}

void configurarTextosDialogo(Dialogo* dialogo, int fase) {
    if (!dialogo) return;

    // Limpa textos anteriores
    dialogo->numeroTextos = 0;
    dialogo->textoAtual = 0;

    // Define textos baseado na fase
    switch (fase) {
    case 1:
        dialogo->textos[0] = "Sou Oswaldo Cruz, diretor de Saúde Pública desde 1903.";
        dialogo->textos[1] = "Fui chamado pelo presidente Rodrigues Alves para sanear a capital.";
        dialogo->textos[2] = "A cidade está doente: varíola, febre amarela e peste.";
        dialogo->textos[3] = "A lei tornou a vacina da varíola obrigatória em todo o Brasil.";
        dialogo->textos[4] = "A medida gerou medo, boatos e protestos nas ruas.";
        dialogo->textos[5] = "Não recuarei: ciência, limpeza urbana e vacinação salvam vidas.";
        dialogo->textos[6] = "Preciso de você no campo: vacinar, desinfetar e exterminar ratos.";
        dialogo->textos[7] = "Cada ferramenta é eficaz à apenas um tipo de inimigo.";
        dialogo->textos[8] = "A vassoura extermina o rato, o veneno neutraliza o mosquito e a vacina cura o povo.";
        dialogo->textos[9] = "Aperte 1 para usar a vassoura, 2 para selecionar o veneno e 3 para a vacina.";
        dialogo->textos[10] = "A ordem é proteger bairros e reduzir contágios rapidamente.";
        dialogo->textos[11] = "Quando a cidade entender, venceremos as epidemias juntos.";
        dialogo->numeroTextos = 12;
        break;

    case 2:
        // Textos para a fase 2 (pode personalizar depois)
        dialogo->textos[0] = "A situação se agravou. Novos focos de infecção surgem.";
        dialogo->textos[1] = "Os protestos aumentaram, mas não podemos recuar.";
        dialogo->textos[2] = "Continue vacinando e controlando os vetores.";
        dialogo->textos[3] = "A ciência prevalecerá sobre a ignorância.";
        dialogo->numeroTextos = 4;
        break;

    case 3:
        // Textos para a fase 3 (pode personalizar depois)
        dialogo->textos[0] = "Esta é a última etapa da campanha.";
        dialogo->textos[1] = "O povo começa a entender a importância da vacinação.";
        dialogo->textos[2] = "Juntos, erradicaremos as epidemias do Rio de Janeiro.";
        dialogo->numeroTextos = 3;
        break;
    case 4:  // === DIÁLOGO DE VITÓRIA ===
        dialogo->textos[0] = "VITÓRIA!";
        dialogo->textos[1] = "As epidemias foram controladas no Rio de Janeiro.";
        dialogo->textos[2] = "A vacinação obrigatória, apesar da resistência inicial,";
        dialogo->textos[3] = "salvou milhares de vidas e transformou a saúde pública.";
        dialogo->textos[4] = "A ciência e a determinação venceram o medo e a ignorância.";
        dialogo->textos[5] = "O legado de Oswaldo Cruz vive até hoje.";
        dialogo->textos[6] = "Parabéns por completar sua missão!";
        dialogo->numeroTextos = 7;
        break;
    default:
        dialogo->numeroTextos = 0;
        break;
    }
}

void configurarPosicoesBotoesPausa(MenuBotoes* menuBotao) {
    if (!menuBotao) return;

    // Posições centralizadas para o menu de pausa
    const int CENTRO_X = 525;
    const int ESPACO_VERTICAL = 80;
    const int INICIO_Y = 260;

    menuBotao->botaoJogarX = CENTRO_X;
    menuBotao->botaoJogarY = INICIO_Y;

    menuBotao->botaoRegrasX = CENTRO_X;
    menuBotao->botaoRegrasY = INICIO_Y + ESPACO_VERTICAL;

    menuBotao->botaoSairX = CENTRO_X;
    menuBotao->botaoSairY = INICIO_Y + (ESPACO_VERTICAL * 2);
}