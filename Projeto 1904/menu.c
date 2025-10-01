#include "menu.h"

void menu_principal(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImgs* menuImg, MenuBotoes* menuBotao) {
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
            if ((*menuEvent->mouseX >= menuBotao->botaoRegrasX) && (*menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura) && (*menuEvent->mouseY >= menuBotao->botaoRegrasY) && (*menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura)) {
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

                    // Aba de regras
                    al_draw_scaled_bitmap(menuImg->abaRegras, 0, 0, menuBotao->abaRegrasLargura, menuBotao->abaRegrasAltura, 0, 0, WIDTH, HEIGHT, 0);

                    if (*menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= menuBotao->botaoVoltarY && *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura)
                        al_draw_bitmap(menuImg->botaoVoltar2, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);
                    else
                        al_draw_bitmap(menuImg->botaoVoltar, menuBotao->botaoVoltarX, menuBotao->botaoVoltarY, 0);

                    al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && *menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= menuBotao->botaoVoltarY && *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura) {
                            *menuEstado->regrasAberta = false;
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

void menu_pausa(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImgs* menuImg, MenuBotoes* menuBotao) {
    
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

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && *menuEvent->mouseX >= menuBotao->botaoVoltarX && *menuEvent->mouseX <= menuBotao->botaoVoltarX + menuBotao->botaoVoltarLargura && *menuEvent->mouseY >= menuBotao->botaoVoltarY && *menuEvent->mouseY <= menuBotao->botaoVoltarY + menuBotao->botaoVoltarAltura) {
                            regrasAbertaLocal = false;
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
