#include "menu.h"

void menu_principal(MenuEstados* mes, MenuEvents* mev, MenuImgs* mi, MenuBotoes* bt, bool redesenhar) {
    ALLEGRO_EVENT event;

    while (*mes->telaMenu) {
        al_wait_for_event(mev->fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *mes->telaMenu = false;
            *mes->jogando = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *mev->mouseX = event.mouse.x;
            *mev->mouseY = event.mouse.y;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Jogar
            if (*mev->mouseX >= bt->botaoJogarX && *mev->mouseX <= bt->botaoJogarX + bt->botaoJogarLargura && *mev->mouseY >= bt->botaoJogarY && *mev->mouseY <= bt->botaoJogarY + bt->botaoJogarAltura) {
                *mes->telaMenu = false;
                *mes->jogando = true;
                break;
            }

            // Regras
            if (*mev->mouseX >= bt->botaoRegrasX && *mev->mouseX <= bt->botaoRegrasX + bt->botaoRegrasLargura && *mev->mouseY >= bt->botaoRegrasY && *mev->mouseY <= bt->botaoRegrasY + bt->botaoRegrasAltura) {
                *mes->regrasAberta = true;

                while (*mes->regrasAberta) {
                    al_wait_for_event(mev->fila_eventos, &event);

                    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        *mes->regrasAberta = false;
                        *mes->telaMenu = false;
                        *mes->jogando = false;
                        break;
                    }

                    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        *mev->mouseX = event.mouse.x;
                        *mev->mouseY = event.mouse.y;
                    }

                    // Aba de regras
                    al_draw_scaled_bitmap(mi->abaRegras, 0, 0, bt->abaRegrasLargura, bt->abaRegrasAltura, 0, 0, WIDTH, HEIGHT, 0);

                    if (*mev->mouseX >= bt->botaoVoltarX && *mev->mouseX <= bt->botaoVoltarX + bt->botaoVoltarLargura && *mev->mouseY >= bt->botaoVoltarY && *mev->mouseY <= bt->botaoVoltarY + bt->botaoVoltarAltura)
                        al_draw_bitmap(mi->botaoVoltar2, bt->botaoVoltarX, bt->botaoVoltarY, 0);
                    else
                        al_draw_bitmap(mi->botaoVoltar, bt->botaoVoltarX, bt->botaoVoltarY, 0);

                    al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && *mev->mouseX >= bt->botaoVoltarX && *mev->mouseX <= bt->botaoVoltarX + bt->botaoVoltarLargura && *mev->mouseY >= bt->botaoVoltarY && *mev->mouseY <= bt->botaoVoltarY + bt->botaoVoltarAltura) {
                            *mes->regrasAberta = false;
                    }
                }
            }

            // Sair
            if (*mev->mouseX >= bt->botaoSairX && *mev->mouseX <= bt->botaoSairX + bt->botaoSairLargura && *mev->mouseY >= bt->botaoSairY && *mev->mouseY <= bt->botaoSairY + bt->botaoSairAltura) {
                *mes->telaMenu = false;
                *mes->jogando = false;
                break;
            }
        }

        // Desenha menu
        al_draw_scaled_bitmap(mi->fundoMenu, 0, 0, bt->fundoMenuLargura, bt->fundoMenuAltura, 0, 0, WIDTH, HEIGHT, 0);
        if (*mev->mouseX >= bt->botaoJogarX && *mev->mouseX <= (bt->botaoJogarX + bt->botaoJogarLargura) && *mev->mouseY >= bt->botaoJogarY && *mev->mouseY <= (bt->botaoJogarY + bt->botaoJogarAltura)) {
            al_draw_bitmap(mi->botaoJogar2, bt->botaoJogarX, bt->botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(mi->botaoJogar, bt->botaoJogarX, bt->botaoJogarY, 0);
        }
        if (*mev->mouseX >= bt->botaoRegrasX && *mev->mouseX <= (bt->botaoRegrasX + bt->botaoRegrasLargura) && *mev->mouseY >= bt->botaoRegrasY && *mev->mouseY <= (bt->botaoRegrasY + bt->botaoRegrasAltura)) {
            al_draw_bitmap(mi->botaoRegras2, bt->botaoRegrasX, bt->botaoRegrasY, 0);
        }
        else {
            al_draw_bitmap(mi->botaoRegras, bt->botaoRegrasX, bt->botaoRegrasY, 0);
        }
        if (*mev->mouseX >= bt->botaoSairX && *mev->mouseX <= (bt->botaoSairX + bt->botaoSairLargura) && *mev->mouseY >= bt->botaoSairY && *mev->mouseY <= (bt->botaoSairY + bt->botaoSairAltura)) {
            al_draw_bitmap(mi->botaoSair2, bt->botaoSairX, bt->botaoSairY, 0);
        }
        else {
            al_draw_bitmap(mi->botaoSair, bt->botaoSairX, bt->botaoSairY, 0);
        }
        al_flip_display();
    }
}

void menu_pausa(MenuEstados* mes, MenuEvents* mev, MenuImgs* mi, MenuBotoes* bt) {
    
    if (!*mes->esc) return;

    al_stop_timer(mev->timer);
    al_identity_transform(mev->camera);
    al_use_transform(mev->camera);

    ALLEGRO_EVENT event;
    *mes->jogoPausado = true;

    while (*mes->jogoPausado) {
        al_wait_for_event(mev->fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *mes->jogando = false;
            *mes->jogoPausado = false;
            *mes->esc = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *mev->mouseX = event.mouse.x;
            *mev->mouseY = event.mouse.y;
        }

        // ESC para continuar
        if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
            event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            *mes->jogoPausado = false;
            *mes->esc = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Continuar
            if (*mev->mouseX >= bt->botaoJogarX && *mev->mouseX <= bt->botaoJogarX + bt->botaoJogarLargura && *mev->mouseY >= bt->botaoJogarY && *mev->mouseY <= bt->botaoJogarY + bt->botaoJogarAltura) {
                *mes->jogoPausado = false;
                *mes->esc = false;
                break;
            }

            // Regras (sub-loop)
            if (*mev->mouseX >= bt->botaoRegrasX && *mev->mouseX <= bt->botaoRegrasX + bt->botaoRegrasLargura && *mev->mouseY >= bt->botaoRegrasY && *mev->mouseY <= bt->botaoRegrasY + bt->botaoRegrasAltura) {
                bool regrasAbertaLocal = true;
                while (regrasAbertaLocal) {
                    al_wait_for_event(mev->fila_eventos, &event);

                    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        *mes->jogando = false;
                        *mes->jogoPausado = false;
                        *mes->esc = false;
                        regrasAbertaLocal = false;
                        break;
                    }

                    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        *mev->mouseX = event.mouse.x;
                        *mev->mouseY = event.mouse.y;
                    }

                    al_draw_scaled_bitmap(mi->abaRegras, 0, 0, bt->abaRegrasLargura, bt->abaRegrasAltura, 0, 0, WIDTH, HEIGHT, 0);

                    if (*mev->mouseX >= bt->botaoVoltarX && *mev->mouseX <= bt->botaoVoltarX + bt->botaoVoltarLargura && *mev->mouseY >= bt->botaoVoltarY && *mev->mouseY <= bt->botaoVoltarY + bt->botaoVoltarAltura)
                        al_draw_bitmap(mi->botaoVoltar2, bt->botaoVoltarX, bt->botaoVoltarY, 0);
                    else
                        al_draw_bitmap(mi->botaoVoltar, bt->botaoVoltarX, bt->botaoVoltarY, 0);

                        al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && *mev->mouseX >= bt->botaoVoltarX && *mev->mouseX <= bt->botaoVoltarX + bt->botaoVoltarLargura && *mev->mouseY >= bt->botaoVoltarY && *mev->mouseY <= bt->botaoVoltarY + bt->botaoVoltarAltura) {
                            regrasAbertaLocal = false;
                    }
                }
            }

            // Sair
            if (*mev->mouseX >= bt->botaoSairX && *mev->mouseX <= bt->botaoSairX + bt->botaoSairLargura && *mev->mouseY >= bt->botaoSairY && *mev->mouseY <= bt->botaoSairY + bt->botaoSairAltura) {
                *mes->jogando = false;
                *mes->jogoPausado = false;
                *mes->esc = false;
                break;
            }
        }

        // UI do pause com hover
        if (*mev->mouseX >= bt->botaoJogarX && *mev->mouseX <= (bt->botaoJogarX + bt->botaoJogarLargura) && *mev->mouseY >= bt->botaoJogarY && *mev->mouseY <= (bt->botaoJogarY + bt->botaoJogarAltura)) {
            al_draw_bitmap(mi->botaoJogar2, bt->botaoJogarX, bt->botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(mi->botaoJogar, bt->botaoJogarX, bt->botaoJogarY, 0);
        }
        if (*mev->mouseX >= bt->botaoRegrasX && *mev->mouseX <= (bt->botaoRegrasX + bt->botaoRegrasLargura) && *mev->mouseY >= bt->botaoRegrasY && *mev->mouseY <= (bt->botaoRegrasY + bt->botaoRegrasAltura)) {
            al_draw_bitmap(mi->botaoRegras2, bt->botaoRegrasX, bt->botaoRegrasY, 0);
        }
        else {
            al_draw_bitmap(mi->botaoRegras, bt->botaoRegrasX, bt->botaoRegrasY, 0);
        }
        if (*mev->mouseX >= bt->botaoSairX && *mev->mouseX <= (bt->botaoSairX + bt->botaoSairLargura) && *mev->mouseY >= bt->botaoSairY && *mev->mouseY <= (bt->botaoSairY + bt->botaoSairAltura)) {
            al_draw_bitmap(mi->botaoSair2, bt->botaoSairX, bt->botaoSairY, 0);
        }
        else {
            al_draw_bitmap(mi->botaoSair, bt->botaoSairX, bt->botaoSairY, 0);
        }
        al_flip_display();
    }

    if (*mes->jogando && !*mes->jogoPausado) {
        al_start_timer(mev->timer);
    }
}
