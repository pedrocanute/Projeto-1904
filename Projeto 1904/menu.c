#include "menu.h"

void menu_principal(MenuFlags* flags, const MenuIO* io, const MenuBitmaps* bmp, const MenuLayout* lay) {
    ALLEGRO_EVENT event;

    while (*flags->telaMenu) {
        al_wait_for_event(io->fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *flags->telaMenu = false;
            *flags->jogando = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *io->mouseX = event.mouse.x;
            *io->mouseY = event.mouse.y;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Jogar
            if (*io->mouseX >= lay->botaoJogarX && *io->mouseX <= lay->botaoJogarX + lay->botaoJogarLargura && *io->mouseY >= lay->botaoJogarY && *io->mouseY <= lay->botaoJogarY + lay->botaoJogarAltura) {
                *flags->telaMenu = false;
                *flags->jogando = true;
                break;
            }

            // Configurações
            if (*io->mouseX >= lay->botaoConfigX && *io->mouseX <= lay->botaoConfigX + lay->botaoConfigLargura && *io->mouseY >= lay->botaoConfigY && *io->mouseY <= lay->botaoConfigY + lay->botaoConfigAltura) {
                *flags->configAberta = true;

                while (*flags->configAberta) {
                    al_wait_for_event(io->fila_eventos, &event);

                    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        *flags->configAberta = false;
                        *flags->telaMenu = false;
                        *flags->jogando = false;
                        break;
                    }

                    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        *io->mouseX = event.mouse.x;
                        *io->mouseY = event.mouse.y;
                    }

                    // Aba de config
                    al_draw_scaled_bitmap(bmp->abaConfig, 0, 0, lay->abaConfigLargura, lay->abaConfigAltura, 0, 0, WIDTH, HEIGHT, 0);

                    if (*io->mouseX >= lay->botaoVoltarX && *io->mouseX <= lay->botaoVoltarX + lay->botaoVoltarLargura && *io->mouseY >= lay->botaoVoltarY && *io->mouseY <= lay->botaoVoltarY + lay->botaoVoltarAltura)
                        al_draw_bitmap(bmp->botaoVoltar2, lay->botaoVoltarX, lay->botaoVoltarY, 0);
                    else
                        al_draw_bitmap(bmp->botaoVoltar, lay->botaoVoltarX, lay->botaoVoltarY, 0);

                    al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        if (*io->mouseX >= lay->botaoVoltarX && *io->mouseX <= lay->botaoVoltarX + lay->botaoVoltarLargura && *io->mouseY >= lay->botaoVoltarY && *io->mouseY <= lay->botaoVoltarY + lay->botaoVoltarAltura) {
                            *flags->configAberta = false;
                        }
                    }
                }
            }

            // Sair
            if (*io->mouseX >= lay->botaoSairX && *io->mouseX <= lay->botaoSairX + lay->botaoSairLargura && *io->mouseY >= lay->botaoSairY && *io->mouseY <= lay->botaoSairY + lay->botaoSairAltura) {
                *flags->telaMenu = false;
                *flags->jogando = false;
                break;
            }
        }

        // Desenha menu com hover
        al_draw_scaled_bitmap(bmp->fundoMenu, 0, 0, lay->fundoMenuLargura, lay->fundoMenuAltura, 0, 0, WIDTH, HEIGHT, 0);

        al_draw_bitmap((*io->mouseX >= lay->botaoJogarX && *io->mouseX <= lay->botaoJogarX + lay->botaoJogarLargura && *io->mouseY >= lay->botaoJogarY && *io->mouseY <= lay->botaoJogarY + lay->botaoJogarAltura) ? bmp->botaoJogar2 : bmp->botaoJogar,lay->botaoJogarX, lay->botaoJogarY, 0);

        al_draw_bitmap((*io->mouseX >= lay->botaoConfigX && *io->mouseX <= lay->botaoConfigX + lay->botaoConfigLargura && *io->mouseY >= lay->botaoConfigY && *io->mouseY <= lay->botaoConfigY + lay->botaoConfigAltura) ? bmp->botaoConfig2 : bmp->botaoConfig,lay->botaoConfigX, lay->botaoConfigY, 0);

        al_draw_bitmap((*io->mouseX >= lay->botaoSairX && *io->mouseX <= lay->botaoSairX + lay->botaoSairLargura && *io->mouseY >= lay->botaoSairY && *io->mouseY <= lay->botaoSairY + lay->botaoSairAltura) ? bmp->botaoSair2 : bmp->botaoSair, lay->botaoSairX, lay->botaoSairY, 0);

        al_flip_display();
    }
}

void menu_pausa(MenuFlags* flags, const MenuIO* io, const MenuBitmaps* bmp, const MenuLayout* lay) {
    
    if (!*flags->esc) return;

    al_stop_timer(io->timer);
    al_identity_transform(io->camera);
    al_use_transform(io->camera);

    ALLEGRO_EVENT event;
    *flags->jogoPausado = true;

    while (*flags->jogoPausado) {
        al_wait_for_event(io->fila_eventos, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *flags->jogando = false;
            *flags->jogoPausado = false;
            *flags->esc = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            *io->mouseX = event.mouse.x;
            *io->mouseY = event.mouse.y;
        }

        // ESC para continuar
        if (event.type == ALLEGRO_EVENT_KEY_DOWN &&
            event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            *flags->jogoPausado = false;
            *flags->esc = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Continuar
            if (*io->mouseX >= lay->botaoJogarX && *io->mouseX <= lay->botaoJogarX + lay->botaoJogarLargura && *io->mouseY >= lay->botaoJogarY && *io->mouseY <= lay->botaoJogarY + lay->botaoJogarAltura) {
                *flags->jogoPausado = false;
                *flags->esc = false;
                break;
            }

            // Configurações (sub-loop)
            if (*io->mouseX >= lay->botaoConfigX && *io->mouseX <= lay->botaoConfigX + lay->botaoConfigLargura && *io->mouseY >= lay->botaoConfigY && *io->mouseY <= lay->botaoConfigY + lay->botaoConfigAltura) {
                bool configAbertaLocal = true;
                while (configAbertaLocal) {
                    al_wait_for_event(io->fila_eventos, &event);

                    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        *flags->jogando = false;
                        *flags->jogoPausado = false;
                        *flags->esc = false;
                        configAbertaLocal = false;
                        break;
                    }

                    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        *io->mouseX = event.mouse.x;
                        *io->mouseY = event.mouse.y;
                    }

                    al_draw_scaled_bitmap(bmp->abaConfig, 0, 0, lay->abaConfigLargura, lay->abaConfigAltura, 0, 0, WIDTH, HEIGHT, 0);

                    if (*io->mouseX >= lay->botaoVoltarX && *io->mouseX <= lay->botaoVoltarX + lay->botaoVoltarLargura && *io->mouseY >= lay->botaoVoltarY && *io->mouseY <= lay->botaoVoltarY + lay->botaoVoltarAltura)
                        al_draw_bitmap(bmp->botaoVoltar2, lay->botaoVoltarX, lay->botaoVoltarY, 0);
                    else
                        al_draw_bitmap(bmp->botaoVoltar, lay->botaoVoltarX, lay->botaoVoltarY, 0);

                    al_flip_display();

                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        if (*io->mouseX >= lay->botaoVoltarX && *io->mouseX <= lay->botaoVoltarX + lay->botaoVoltarLargura && *io->mouseY >= lay->botaoVoltarY && *io->mouseY <= lay->botaoVoltarY + lay->botaoVoltarAltura) {
                            configAbertaLocal = false;
                        }
                    }
                }
            }

            // Sair
            if (*io->mouseX >= lay->botaoSairX && *io->mouseX <= lay->botaoSairX + lay->botaoSairLargura && *io->mouseY >= lay->botaoSairY && *io->mouseY <= lay->botaoSairY + lay->botaoSairAltura) {
                *flags->jogando = false;
                *flags->jogoPausado = false;
                *flags->esc = false;
                break;
            }
        }

        // UI do pause com hover
        al_draw_bitmap(
            (*io->mouseX >= lay->botaoJogarX && *io->mouseX <= lay->botaoJogarX + lay->botaoJogarLargura && *io->mouseY >= lay->botaoJogarY && *io->mouseY <= lay->botaoJogarY + lay->botaoJogarAltura) ? bmp->botaoJogar2 : bmp->botaoJogar, lay->botaoJogarX, lay->botaoJogarY, 0);

        al_draw_bitmap((*io->mouseX >= lay->botaoConfigX && *io->mouseX <= lay->botaoConfigX + lay->botaoConfigLargura && *io->mouseY >= lay->botaoConfigY && *io->mouseY <= lay->botaoConfigY + lay->botaoConfigAltura) ? bmp->botaoConfig2 : bmp->botaoConfig, lay->botaoConfigX, lay->botaoConfigY, 0);

        al_draw_bitmap((*io->mouseX >= lay->botaoSairX && *io->mouseX <= lay->botaoSairX + lay->botaoSairLargura && *io->mouseY >= lay->botaoSairY && *io->mouseY <= lay->botaoSairY + lay->botaoSairAltura) ? bmp->botaoSair2 : bmp->botaoSair, lay->botaoSairX, lay->botaoSairY, 0);

        al_flip_display();
    }

    if (*flags->jogando && !*flags->jogoPausado) {
        al_start_timer(io->timer);
    }
}
