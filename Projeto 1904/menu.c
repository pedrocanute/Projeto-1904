#include "menu.h"


void menu_principal(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao) {
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
            if (*menuEvent->mouseX >= menuBotao->botaoJogarX &&
                *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura &&
                *menuEvent->mouseY >= menuBotao->botaoJogarY &&
                *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
                *menuEstado->telaMenu = false;
                *menuEstado->jogando = true;
                break;
            }

            // Botão Regras
            if (*menuEvent->mouseX >= menuBotao->botaoRegrasX &&
                *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura &&
                *menuEvent->mouseY >= menuBotao->botaoRegrasY &&
                *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
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

                    // Desenha aba de regras
                    al_draw_scaled_bitmap(menuImg->abaRegras, 0, 0,
                        menuBotao->abaRegrasLargura, menuBotao->abaRegrasAltura,
                        0, 0, WIDTH, HEIGHT, 0);

                    // Botão Voltar (hover)
                    if (*menuEvent->mouseX >= menuBotao->botaoVoltarX &&
                        *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura &&
                        *menuEvent->mouseY >= menuBotao->botaoVoltarY &&
                        *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura) {
                        al_draw_bitmap(menuImg->botaoVoltar2, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);
                    }
                    else {
                        al_draw_bitmap(menuImg->botaoVoltar, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);
                    }

                    al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        if (*menuEvent->mouseX >= menuBotao->botaoVoltarX &&
                            *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura &&
                            *menuEvent->mouseY >= menuBotao->botaoVoltarY &&
                            *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura) {
                            *menuEstado->regrasAberta = false;
                        }
                    }
                }
            }

            // Botão Sair
            if (*menuEvent->mouseX >= menuBotao->botaoSairX &&
                *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura &&
                *menuEvent->mouseY >= menuBotao->botaoSairY &&
                *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
                *menuEstado->telaMenu = false;
                *menuEstado->jogando = false;
                break;
            }
        }

        // Desenha menu principal
        al_draw_scaled_bitmap(menuImg->fundoMenu, 0, 0,
            menuBotao->fundoMenuLargura, menuBotao->fundoMenuAltura,
            0, 0, WIDTH, HEIGHT, 0);

        // Botão Jogar (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoJogarX &&
            *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura &&
            *menuEvent->mouseY >= menuBotao->botaoJogarY &&
            *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
            al_draw_bitmap(menuImg->botaoJogar2, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoJogar, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }

        // Botão Regras (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoRegrasX &&
            *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura &&
            *menuEvent->mouseY >= menuBotao->botaoRegrasY &&
            *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
            al_draw_bitmap(menuImg->botaoRegras2, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoRegras, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }

        // Botão Sair (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoSairX &&
            *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura &&
            *menuEvent->mouseY >= menuBotao->botaoSairY &&
            *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
            al_draw_bitmap(menuImg->botaoSair2, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoSair, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }

        al_flip_display();
    }
}

void menu_pausa(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao) {
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
            if (*menuEvent->mouseX >= menuBotao->botaoJogarX &&
                *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura &&
                *menuEvent->mouseY >= menuBotao->botaoJogarY &&
                *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
                *menuEstado->jogoPausado = false;
                *menuEstado->esc = false;
                break;
            }

            // Regras
            if (*menuEvent->mouseX >= menuBotao->botaoRegrasX &&
                *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura &&
                *menuEvent->mouseY >= menuBotao->botaoRegrasY &&
                *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {

                //  ENTRA NO SUBMENU DE REGRAS
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
                        //  VERIFICA CLIQUE NO BOTÃO VOLTAR
                        if (*menuEvent->mouseX >= menuBotao->botaoVoltarX &&
                            *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura &&
                            *menuEvent->mouseY >= menuBotao->botaoVoltarY &&
                            *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura) {
                            regrasAbertaLocal = false;
                            //  LIMPA EVENTOS ANTES DE VOLTAR
                            al_flush_event_queue(menuEvent->fila_eventos);
                        }
                    }

                    //  DESENHA ABA DE REGRAS
                    al_draw_scaled_bitmap(menuImg->abaRegras, 0, 0,
                        menuBotao->abaRegrasLargura, menuBotao->abaRegrasAltura,
                        0, 0, WIDTH, HEIGHT, 0);

                    //DESENHA BOTÃO VOLTAR (com hover)
                    if (*menuEvent->mouseX >= menuBotao->botaoVoltarX &&
                        *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura &&
                        *menuEvent->mouseY >= menuBotao->botaoVoltarY &&
                        *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura) {
                        al_draw_bitmap(menuImg->botaoVoltar2, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);
                    }
                    else {
                        al_draw_bitmap(menuImg->botaoVoltar, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);
                    }

                    al_flip_display();
                }
                continue;
            }

            // Sair
            if (*menuEvent->mouseX >= menuBotao->botaoSairX &&
                *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura &&
                *menuEvent->mouseY >= menuBotao->botaoSairY &&
                *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
                *menuEstado->jogando = false;
                *menuEstado->jogoPausado = false;
                *menuEstado->esc = false;
                break;
            }
        }

        //  REDESENHA UI DO PAUSE  
        al_clear_to_color(al_map_rgb(45, 29, 46)); //Fundo Roxo do Menu Pause

        // Botão Continuar (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoJogarX &&
            *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura &&
            *menuEvent->mouseY >= menuBotao->botaoJogarY &&
            *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
            al_draw_bitmap(menuImg->botaoJogar2, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoJogar, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }

        // Botão Regras (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoRegrasX &&
            *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura &&
            *menuEvent->mouseY >= menuBotao->botaoRegrasY &&
            *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
            al_draw_bitmap(menuImg->botaoRegras2, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoRegras, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }

        // Botão Sair (hover)
        if (*menuEvent->mouseX >= menuBotao->botaoSairX &&
            *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura &&
            *menuEvent->mouseY >= menuBotao->botaoSairY &&
            *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
            al_draw_bitmap(menuImg->botaoSair2, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoSair, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }

        al_flip_display();
    }

    // Retoma o jogo se ainda estiver jogando
    if (*menuEstado->jogando && !*menuEstado->jogoPausado) {
        al_flush_event_queue(menuEvent->fila_eventos);
        al_start_timer(menuEvent->timer);
    }
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
            if (*menuEvent->mouseX >= gameover->botaoSairDoJogoX &&
                *menuEvent->mouseX <= gameover->botaoSairDoJogoX + gameover->botaoSairDoJogoLargura &&
                *menuEvent->mouseY >= gameover->botaoSairDoJogoY &&
                *menuEvent->mouseY <= gameover->botaoSairDoJogoY + gameover->botaoSairDoJogoAltura) {
                *menuEstado->jogando = false;
                *menuEstado->fimDeJogo = false;
                break;
            }
        }

        // Desenha tela de Game Over
        al_draw_scaled_bitmap(gameover->telaGameOver, 0, 0,
            gameover->telaGameOverLargura, gameover->telaGameOverAltura,
            0, 0, WIDTH, HEIGHT, 0);

        // Botão Sair (hover)
        if (*menuEvent->mouseX >= gameover->botaoSairDoJogoX &&
            *menuEvent->mouseX <= gameover->botaoSairDoJogoX + gameover->botaoSairDoJogoLargura &&
            *menuEvent->mouseY >= gameover->botaoSairDoJogoY &&
            *menuEvent->mouseY <= gameover->botaoSairDoJogoY + gameover->botaoSairDoJogoAltura) {
            al_draw_bitmap(gameover->botaoSairDoJogo2, gameover->botaoSairDoJogoX, gameover->botaoSairDoJogoY, 0);
        }
        else {
            al_draw_bitmap(gameover->botaoSairDoJogo, gameover->botaoSairDoJogoX, gameover->botaoSairDoJogoY, 0);
        }

        al_flip_display();
    }
}

void desenhar_tela_dialogo(Dialogo* dialogo, SistemaFases* fase, MenuEvents* menuEvent, MenuEstados* menuEstado) {
    // Reseta transformação da câmera
    al_identity_transform(menuEvent->camera);
    al_use_transform(menuEvent->camera);

    // Reseta diálogos quando volta para fase 1
    if (fase->faseAtual == 1 && dialogo->dialogo2) {
        dialogo->dialogo1 = false;
        dialogo->dialogo2 = false;
        dialogo->dialogo3 = false;
    }

    // Verifica se deve iniciar diálogo
    bool iniciarDialogo = false;
    if (fase->faseAtual == 1 && !dialogo->dialogo1) iniciarDialogo = true;
    if (fase->faseAtual == 2 && !dialogo->dialogo2) iniciarDialogo = true;
    if (fase->faseAtual == 3 && !dialogo->dialogo3) iniciarDialogo = true;

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
                break;  //  SAI IMEDIATAMENTE
            }

            // SPACE avança texto (só após fade in)
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && fadeInCompleto) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE || event.keyboard.keycode == ALLEGRO_KEY_ENTER) {

                    dialogo->textoAtual++;

                    if (dialogo->textoAtual >= dialogo->numeroTextos) {
                        dialogoAtivo = false;
                    }

                    precisaRedesenhar = true;
                }
            }
        }

        //  Verifica se ainda está jogando
        if (!*menuEstado->jogando) {
            break;
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

            al_draw_bitmap_region(dialogo->falando,sx, sy,larguraFrame, alturaFrame,falandoX, falandoY, 0);

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

                al_draw_multiline_text(dialogo->fonteDialogo,al_map_rgb(255, 255, 255),textoX, textoY,textoLarguraMax,alturaLinha,ALLEGRO_ALIGN_LEFT,textoAtual);

                // Indicador
                float indicadorX = balaoX + 560;
                float indicadorY = balaoY + 220;

                if (dialogo->textoAtual < dialogo->numeroTextos - 1) {
                    al_draw_text(dialogo->fonteDialogo,al_map_rgb(200, 200, 200),indicadorX, indicadorY,ALLEGRO_ALIGN_RIGHT,"[SPACE]");
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

    //  Verifica se foi fechado antes de continuar
    if (!*menuEstado->jogando) {
        return;
    }

    // Marca diálogo completo
    if (fase->faseAtual == 1) dialogo->dialogo1 = true;
    if (fase->faseAtual == 2) dialogo->dialogo2 = true;
    if (fase->faseAtual == 3) dialogo->dialogo3 = true;

    // Limpa eventos
    al_flush_event_queue(menuEvent->fila_eventos);

    // Retoma o jogo
    if (*menuEstado->jogando) {
        al_start_timer(menuEvent->timer);
    }
}

