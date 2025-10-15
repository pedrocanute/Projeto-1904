#include "menu.h"

void menu_principal(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao) {
    ALLEGRO_EVENT event;

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
            // Jogar
            if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
                *menuEstado->telaMenu = false;
                *menuEstado->jogando = true;
                break;
            }

            // Regras
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

                    // Aba de Regras
                    al_draw_scaled_bitmap(menuImg->abaRegras, 0, 0, menuBotao->abaRegrasLargura, menuBotao->abaRegrasAltura, 0, 0, WIDTH, HEIGHT, 0);

                    if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= menuBotao->botaoVoltarY && *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura)
                        al_draw_bitmap(menuImg->botaoVoltar2, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);
                    else
                        al_draw_bitmap(menuImg->botaoVoltar, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);

                    al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= menuBotao->botaoVoltarY && *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura) {
                            *menuEstado->regrasAberta = false;
                        }
                    }
                }
            }

            // Sair
            if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
                *menuEstado->telaMenu = false;
                *menuEstado->jogando = false;
                break;
            }
        }

        // Desenha menu
        al_draw_scaled_bitmap(menuImg->fundoMenu, 0, 0, menuBotao->fundoMenuLargura, menuBotao->fundoMenuAltura, 0, 0, WIDTH, HEIGHT, 0);
        if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= (menuBotao->botaoJogarX + menuBotao->botaoJogarLargura) && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= (menuBotao->botaoJogarY + menuBotao->botaoJogarAltura)) {
            al_draw_bitmap(menuImg->botaoJogar2, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoJogar, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }
        if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= (menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura) && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= (menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura)) {
            al_draw_bitmap(menuImg->botaoRegras2, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoRegras, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }
        if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= (menuBotao->botaoSairX + menuBotao->botaoSairLargura) && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= (menuBotao->botaoSairY + menuBotao->botaoSairAltura)) {
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

            // Regras (sub-loop)
            if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
                bool regrasAbertaLocal = true;
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

                    al_draw_scaled_bitmap(menuImg->abaRegras, 0, 0, menuBotao->abaRegrasLargura, menuBotao->abaRegrasAltura, 0, 0, WIDTH, HEIGHT, 0);

                    if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= menuBotao->botaoVoltarY && *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura)
                        al_draw_bitmap(menuImg->botaoVoltar2, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);
                    else
                        al_draw_bitmap(menuImg->botaoVoltar, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);

                    al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= menuBotao->botaoVoltarY && *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura) {
                            regrasAbertaLocal = false;
                        }
                    }
                }
            }

            // Sair
            if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
                *menuEstado->jogando = false;
                *menuEstado->jogoPausado = false;
                *menuEstado->esc = false;
                break;
            }
        }

        // UI do pause com hover
        if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= (menuBotao->botaoJogarX + menuBotao->botaoJogarLargura) && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= (menuBotao->botaoJogarY + menuBotao->botaoJogarAltura)) {
            al_draw_bitmap(menuImg->botaoJogar2, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoJogar, menuBotao->botaoJogarX, menuBotao->botaoJogarY, 0);
        }
        if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= (menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura) && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= (menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura)) {
            al_draw_bitmap(menuImg->botaoRegras2, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoRegras, menuBotao->botaoRegrasX, menuBotao->botaoRegrasY, 0);
        }
        if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= (menuBotao->botaoSairX + menuBotao->botaoSairLargura) && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= (menuBotao->botaoSairY + menuBotao->botaoSairAltura)) {
            al_draw_bitmap(menuImg->botaoSair2, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }
        else {
            al_draw_bitmap(menuImg->botaoSair, menuBotao->botaoSairX, menuBotao->botaoSairY, 0);
        }
        al_flip_display();
    }

    if (*menuEstado->jogando && !*menuEstado->jogoPausado) {
        al_start_timer(menuEvent->timer);
    }
}

// Desenha a tela de Game Over ao atingir o limite de infecção
void desenhar_tela_gameOver(GameOver* gameover, InfeccaoEstagio* infec, MenuEvents* menuEvent, MenuEstados* menuEstado) {

    ALLEGRO_EVENT event;

    // Tela de Game Over
    while (*menuEstado->fimDeJogo) {
        al_wait_for_event(menuEvent->fila_eventos, &event);

        // Clicar no X pra sair
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *menuEstado->jogando = false;
            *menuEstado->fimDeJogo = false;
            break;
        }

        // Função do mouse
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *menuEvent->mouseX = event.mouse.x;
            *menuEvent->mouseY = event.mouse.y;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
           

            // Clicou em Sair do Jogo, sai do jogo
            if (*menuEvent->mouseX >= gameover->botaoSairDoJogoX && *menuEvent->mouseX <= gameover->botaoSairDoJogoX + gameover->botaoSairDoJogoLargura && *menuEvent->mouseY >= gameover->botaoSairDoJogoY && *menuEvent->mouseY <= gameover->botaoSairDoJogoY + gameover->botaoSairDoJogoAltura) {
                *menuEstado->jogando = false;
                *menuEstado->fimDeJogo = false;
                break;
            }
        }

        // Desenha a tela de Game Over e os botões
        al_draw_scaled_bitmap(gameover->telaGameOver, 0, 0, gameover->telaGameOverLargura, gameover->telaGameOverAltura, 0, 0, WIDTH, HEIGHT, 0);
        
        if (*menuEvent->mouseX >= gameover->botaoSairDoJogoX && *menuEvent->mouseX <= (gameover->botaoSairDoJogoX + gameover->botaoSairDoJogoLargura) && *menuEvent->mouseY >= gameover->botaoSairDoJogoY && *menuEvent->mouseY <= (gameover->botaoSairDoJogoY + gameover->botaoSairDoJogoAltura)) {
            al_draw_bitmap(gameover->botaoSairDoJogo2, gameover->botaoSairDoJogoX, gameover->botaoSairDoJogoY, 0);
        }
        else {
            al_draw_bitmap(gameover->botaoSairDoJogo, gameover->botaoSairDoJogoX, gameover->botaoSairDoJogoY, 0);
        }
        al_flip_display();

        al_stop_timer(menuEvent->timer);
    }
}

void desenhar_tela_dialogo(Dialogo* dialogo, SistemaFases* fase, MenuEvents* menuEvent, MenuEstados* menuEstado) {

    // Diálogo acompanha a tela
    al_identity_transform(menuEvent->camera);
    al_use_transform(menuEvent->camera);

    // Se o jogo foi reiniciado (estamos na fase 1 mas o diálogo 2 já foi visto),
    // reseta os flags para que os diálogos apareçam novamente.
    if (fase->faseAtual == 1 && *dialogo->dialogo2) {
        *dialogo->dialogo1 = false;
        *dialogo->dialogo2 = false;
        *dialogo->dialogo3 = false;
    }

    bool iniciarDialogo = false;

    if (fase->faseAtual == 1 && !*dialogo->dialogo1) iniciarDialogo = true;
    if (fase->faseAtual == 2 && !*dialogo->dialogo2) iniciarDialogo = true;
    if (fase->faseAtual == 3 && !*dialogo->dialogo3) iniciarDialogo = true;

    // Se não há diálogo para mostrar, a função termina imediatamente e o jogo continua.
    if (!iniciarDialogo) {
        return;
    }

    // Se um diálogo precisa ser exibido, a função assume o controle.
    al_stop_timer(menuEvent->timer);

    ALLEGRO_EVENT event;

    bool dialogoAtivo = true;

    while (dialogoAtivo) {
        al_wait_for_event(menuEvent->fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
            dialogoAtivo = false;
        }

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *menuEstado->jogando = false;
            dialogoAtivo = false;
        }

        // Desenha a tela de diálogo
        al_draw_scaled_bitmap(dialogo->oswaldo, 0, 0, dialogo->oswaldoLargura, dialogo->oswaldoAltura, 0, 0, WIDTH, HEIGHT, 0);
        al_draw_bitmap(dialogo->caixaDialogo, dialogo->caixaDialogoX, dialogo->caixaDialogoY, 0);
        al_flip_display();
    }

    // Marca o diálogo como concluído usando o ponteiro, para não aparecer de novo.
    if (fase->faseAtual == 1) *dialogo->dialogo1 = true;
    if (fase->faseAtual == 2) *dialogo->dialogo2 = true;
    if (fase->faseAtual == 3) *dialogo->dialogo3 = true;

    // Devolve o controle para o jogo, se ele não foi fechado.
    if (*menuEstado->jogando) {
        al_start_timer(menuEvent->timer);
    }
}
