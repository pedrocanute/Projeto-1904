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

// Desenha a tela de Game Over ao atingir o limite de infec��o
void desenhar_tela_gameOver(GameOver* gameover, InfeccaoEstagio* infec, MenuEvents* menuEvent, MenuEstados* menuEstado) {

    ALLEGRO_EVENT event;

    // Para o jogo e chama a tela de game over
    if (*infec->infeccaoAtual >= infec->infeccaoMaxima) {
        *menuEstado->fimDeJogo = true;
    }

    // Tela de Game Over
    while (*menuEstado->fimDeJogo) {
        al_wait_for_event(menuEvent->fila_eventos, &event);

        // Clicar no X pra sair
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *menuEstado->fimDeJogo = false;
            *menuEstado->jogando = false;
            break;
        }

        // Fun��o do mouse
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *menuEvent->mouseX = event.mouse.x;
            *menuEvent->mouseY = event.mouse.y;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Clicou em Jogar Novamente, volta o jogo
            if (*menuEvent->mouseX >= gameover->botaoJogarNovamenteX && *menuEvent->mouseX <= gameover->botaoJogarNovamenteX + gameover->botaoJogarNovamenteLargura && *menuEvent->mouseY >= gameover->botaoJogarNovamenteY && *menuEvent->mouseY <= gameover->botaoJogarNovamenteY + gameover->botaoJogarNovamenteAltura) {
                main();
                *menuEstado->fimDeJogo = false;
                break;
            }

            // Clicou em Sair do Jogo, sai do jogo
            if (*menuEvent->mouseX >= gameover->botaoSairDoJogoX && *menuEvent->mouseX <= gameover->botaoSairDoJogoX + gameover->botaoSairDoJogoLargura && *menuEvent->mouseY >= gameover->botaoSairDoJogoY && *menuEvent->mouseY <= gameover->botaoSairDoJogoY + gameover->botaoSairDoJogoAltura) {
                *menuEstado->jogando = false;
                break;
            }
        }

        // Desenha a tela de Game Over e os bot�es
        al_draw_scaled_bitmap(gameover->telaGameOver, 0, 0, gameover->telaGameOverLargura, gameover->telaGameOverAltura, 0, 0, WIDTH, HEIGHT, 0);
        if (*menuEvent->mouseX >= gameover->botaoJogarNovamenteX && *menuEvent->mouseX <= (gameover->botaoJogarNovamenteX + gameover->botaoJogarNovamenteLargura) && *menuEvent->mouseY >= gameover->botaoJogarNovamenteY && *menuEvent->mouseY <= (gameover->botaoJogarNovamenteY + gameover->botaoJogarNovamenteAltura)) {
            al_draw_bitmap(gameover->botaoJogarNovamente2, gameover->botaoJogarNovamenteX, gameover->botaoJogarNovamenteY, 0);
        }
        else {
            al_draw_bitmap(gameover->botaoJogarNovamente, gameover->botaoJogarNovamenteX, gameover->botaoJogarNovamenteY, 0);
        }
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
