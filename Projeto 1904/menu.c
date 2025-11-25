#include "menu.h"
#include "configuracoes.h"
#include "dialogos.h"
#include <math.h>

// Estrutura para controlar a animação do menu
typedef struct {
    float jogadorX;
    float jogadorY;
    float zumbiX;
    float zumbiY;
    float ratoX;
    float ratoY;
    float mosquitoX;
    float mosquitoY;
    int frameJogador;
    int contadorFrameJogador;
    int frameZumbi;
    int contadorFrameZumbi;
    int frameRato;
    int contadorFrameRato;
    int frameMosquito;
    int contadorFrameMosquito;
    bool jogadorAtivo;
    bool zumbiAtivo;
    bool ratoAtivo;
    bool mosquitoAtivo;
    double tempoUltimaAnimacao;
    // Scrolling do mapa de fundo
    float offsetMapaX;
    float velocidadeScrolling;
} AnimacaoMenu;


void inicializarAnimacaoMenu(AnimacaoMenu* anim) {
    anim->jogadorX = -150.0f;  // Fora da tela à esquerda
    anim->jogadorY = 400.0f;
    anim->zumbiX = WIDTH + 150.0f;  // Fora da tela à direita
    anim->zumbiY = 400.0f;
    anim->ratoX = -150.0f;
    anim->ratoY = 450.0f;
    anim->mosquitoX = WIDTH + 150.0f;
    anim->mosquitoY = 380.0f;

    anim->frameJogador = 0;
    anim->contadorFrameJogador = 0;
    anim->frameZumbi = 0;
    anim->contadorFrameZumbi = 0;
    anim->frameRato = 0;
    anim->contadorFrameRato = 0;
    anim->frameMosquito = 0;
    anim->contadorFrameMosquito = 0;

    anim->jogadorAtivo = true;
    anim->zumbiAtivo = false;
    anim->ratoAtivo = false;
    anim->mosquitoAtivo = false;
    anim->tempoUltimaAnimacao = al_get_time();

    // Inicializa scrolling do mapa
    anim->offsetMapaX = 0.0f;
    anim->velocidadeScrolling = 1.5f;  // Velocidade do scrolling para a direita
}

void atualizarAnimacaoMenu(AnimacaoMenu* anim) {
    const float VELOCIDADE_DOUTOR = 3.0f;
    const float VELOCIDADE_ZUMBI = 2.5f;
    const float VELOCIDADE_RATO = 4.0f;
    const float VELOCIDADE_MOSQUITO = 3.5f;
    const int FRAMES_POR_SPRITE = 8;

    // Atualiza scrolling do mapa de fundo
    anim->offsetMapaX += anim->velocidadeScrolling;

    // Quando o offset atingir a largura do mapa, reinicia (loop infinito)
    // Assumindo que o mapa tem largura de 1280 (WIDTH)
    if (anim->offsetMapaX >= WIDTH) {
        anim->offsetMapaX = 0.0f;
    }

    // Atualiza posição do jogador
    if (anim->jogadorAtivo) {
        anim->jogadorX += VELOCIDADE_DOUTOR;

        // Animação do jogador
        anim->contadorFrameJogador++;
        if (anim->contadorFrameJogador >= FRAMES_POR_SPRITE) {
            anim->contadorFrameJogador = 0;
            anim->frameJogador = (anim->frameJogador + 1) % 2;  // 2 frames de caminhada
        }

        // Se saiu da tela pela direita, ativa o zumbi
        if (anim->jogadorX > WIDTH + 150.0f) {
            anim->jogadorAtivo = false;
            anim->zumbiAtivo = true;
            anim->zumbiX = WIDTH + 150.0f;
        }
    }

    // Atualiza posição do zumbi
    if (anim->zumbiAtivo) {
        anim->zumbiX -= VELOCIDADE_ZUMBI;

        // Animação do zumbi
        anim->contadorFrameZumbi++;
        if (anim->contadorFrameZumbi >= FRAMES_POR_SPRITE) {
            anim->contadorFrameZumbi = 0;
            anim->frameZumbi = (anim->frameZumbi + 1) % 2;  // 2 frames do zumbi
        }

        // Se saiu da tela pela esquerda, ativa o rato
        if (anim->zumbiX < -150.0f) {
            anim->zumbiAtivo = false;
            anim->ratoAtivo = true;
            anim->ratoX = -150.0f;
        }
    }

    // Atualiza posição do rato
    if (anim->ratoAtivo) {
        anim->ratoX += VELOCIDADE_RATO;

        // Animação do rato
        anim->contadorFrameRato++;
        if (anim->contadorFrameRato >= FRAMES_POR_SPRITE) {
            anim->contadorFrameRato = 0;
            anim->frameRato = (anim->frameRato + 1) % 2;  // 2 frames do rato
        }

        // Se saiu da tela pela direita, ativa o mosquito
        if (anim->ratoX > WIDTH + 150.0f) {
            anim->ratoAtivo = false;
            anim->mosquitoAtivo = true;
            anim->mosquitoX = WIDTH + 150.0f;
        }
    }

    // Atualiza posição do mosquito
    if (anim->mosquitoAtivo) {
        anim->mosquitoX -= VELOCIDADE_MOSQUITO;

        // Animação do mosquito
        anim->contadorFrameMosquito++;
        if (anim->contadorFrameMosquito >= FRAMES_POR_SPRITE) {
            anim->contadorFrameMosquito = 0;
            anim->frameMosquito = (anim->frameMosquito + 1) % 2;  // 2 frames do mosquito
        }

        // Se saiu da tela pela esquerda, reinicia o ciclo
        if (anim->mosquitoX < -150.0f) {
            anim->mosquitoAtivo = false;
            anim->jogadorAtivo = true;
            anim->jogadorX = -150.0f;
        }
    }
}

void desenharAnimacaoMenu(AnimacaoMenu* anim, Bitmaps* bitmap) {
    // Desenha o mapa de fundo com scrolling infinito
    if (bitmap->mapaMenu1 && bitmap->mapaMenu2) {
        // Cria o efeito de loopp
        // Usa fmodf para garantir transição suave sem gaps
        float offset = fmodf(anim->offsetMapaX, WIDTH);

        float pos1X = -offset;
        float pos2X = WIDTH - offset;

        // Desenha o primeiro mapa (escala para preencher a tela)
        al_draw_scaled_bitmap(bitmap->mapaMenu1,
            0, 0,
            al_get_bitmap_width(bitmap->mapaMenu1),
            al_get_bitmap_height(bitmap->mapaMenu1),
            pos1X, 0,
            WIDTH, HEIGHT,
            0);

        // Desenha o segundo mapa (logo após o primeiro para continuidade)
        al_draw_scaled_bitmap(bitmap->mapaMenu2,
            0, 0,
            al_get_bitmap_width(bitmap->mapaMenu2),
            al_get_bitmap_height(bitmap->mapaMenu2),
            pos2X, 0,
            WIDTH, HEIGHT,
            0);
    }

    // Desenha jogador (se ativo) - por cima do mapa
    if (anim->jogadorAtivo && anim->jogadorX > -150.0f && anim->jogadorX < WIDTH + 150.0f) {
        ALLEGRO_BITMAP* spriteJogador = bitmap->sprite_andando_direita;

        if (spriteJogador) {
            int larguraTotal = al_get_bitmap_width(spriteJogador);
            int alturaFrame = al_get_bitmap_height(spriteJogador);
            int larguraFrame = larguraTotal / 2;  // 2 frames
            int sx = anim->frameJogador * larguraFrame;

            al_draw_bitmap_region(spriteJogador, sx, 0, larguraFrame, alturaFrame,
                anim->jogadorX, anim->jogadorY, 0);
        }
    }

    // Desenha zumbi (se ativo)
    if (anim->zumbiAtivo && anim->zumbiX > -150.0f && anim->zumbiX < WIDTH + 150.0f) {
        ALLEGRO_BITMAP* spriteZumbi = bitmap->zumbi_esquerda;

        if (spriteZumbi) {
            int larguraTotal = al_get_bitmap_width(spriteZumbi);
            int alturaFrame = al_get_bitmap_height(spriteZumbi);
            int larguraFrame = larguraTotal / 2;  // 2 frames
            int sx = anim->frameZumbi * larguraFrame;

            al_draw_bitmap_region(spriteZumbi, sx, 0, larguraFrame, alturaFrame,
                anim->zumbiX, anim->zumbiY, 0);
        }
    }

    // Desenha rato (se ativo)
    if (anim->ratoAtivo && anim->ratoX > -150.0f && anim->ratoX < WIDTH + 150.0f) {
        ALLEGRO_BITMAP* spriteRato = bitmap->rato_direita;

        if (spriteRato) {
            int larguraTotal = al_get_bitmap_width(spriteRato);
            int alturaFrame = al_get_bitmap_height(spriteRato);
            int larguraFrame = larguraTotal / 2;  // 2 frames
            int sx = anim->frameRato * larguraFrame;

            al_draw_bitmap_region(spriteRato, sx, 0, larguraFrame, alturaFrame,
                anim->ratoX, anim->ratoY, 0);
        }
    }

    // Desenha mosquito (se ativo)
    if (anim->mosquitoAtivo && anim->mosquitoX > -150.0f && anim->mosquitoX < WIDTH + 150.0f) {
        ALLEGRO_BITMAP* spriteMosquito = bitmap->mosquito_esquerda;

        if (spriteMosquito) {
            int larguraTotal = al_get_bitmap_width(spriteMosquito);
            int alturaFrame = al_get_bitmap_height(spriteMosquito);
            int larguraFrame = larguraTotal / 2;  // 2 frames
            int sx = anim->frameMosquito * larguraFrame;

            al_draw_bitmap_region(spriteMosquito, sx, 0, larguraFrame, alturaFrame,
                anim->mosquitoX, anim->mosquitoY, 0);
        }
    }
}

void menu_principal(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao, ALLEGRO_FONT* fonte, Bitmaps* bitmap, SistemaSom* sons) {
    ALLEGRO_EVENT event;

    // Inicializa animação do menu
    AnimacaoMenu animMenu;
    inicializarAnimacaoMenu(&animMenu);

    // Inicia a música do menu
    tocarMusicaMenu(sons);

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

        // Atualiza animação do menu a cada frame do timer
        if (event.type == ALLEGRO_EVENT_TIMER) {
            atualizarAnimacaoMenu(&animMenu);
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Botão Jogar
            if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
                tocarSomClick(sons);
                *menuEstado->telaMenu = false;
                *menuEstado->jogando = true;
                break;
            }

            // Botão Regras
            if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
                tocarSomClick(sons);
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

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "R - Trocar Arma");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Arma 1 - Vassoura (Efetivo contra Ratos)");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Arma 2 - Veneno (Efetivo contra Mosquitos)");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Arma 3 - Vacina (Efetivo contra Zumbis)");
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
                            tocarSomClick(sons);
                            *menuEstado->regrasAberta = false;
                        }
                    }
                }
            }

            // Botão Sair
            if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
                tocarSomClick(sons);
                *menuEstado->telaMenu = false;
                *menuEstado->jogando = false;
                break;
            }
        }

        // ========== DESENHO DO MENU PRINCIPAL ==========

        // Desenha animação de fundo com mapa scrolling e personagens
        desenharAnimacaoMenu(&animMenu, bitmap);

        // Desenha o fundo do menu (menu.png) por cima do mapa e personagens
        al_draw_scaled_bitmap(menuImg->fundoMenu, 0, 0, menuBotao->fundoMenuLargura, menuBotao->fundoMenuAltura, 0, 0, WIDTH, HEIGHT, 0);

        // Desenha botões por cima de tudo

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

    // Para a música do menu ao sair
    pararMusicaMenu(sons);
}

void menu_pausa(MenuEstados* menuEstado, MenuEvents* menuEvent, MenuImagens* menuImg, MenuBotoes* menuBotao, ALLEGRO_FONT* fonte, SistemaSom* sons) {
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
        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            *menuEstado->jogoPausado = false;
            *menuEstado->esc = false;
            break;
        }

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Continuar
            if (*menuEvent->mouseX >= menuBotao->botaoJogarX && *menuEvent->mouseX <= menuBotao->botaoJogarX + menuBotao->botaoJogarLargura && *menuEvent->mouseY >= menuBotao->botaoJogarY && *menuEvent->mouseY <= menuBotao->botaoJogarY + menuBotao->botaoJogarAltura) {
                tocarSomClick(sons);
                *menuEstado->jogoPausado = false;
                *menuEstado->esc = false;
                break;
            }

            // Regras
            if (*menuEvent->mouseX >= menuBotao->botaoRegrasX && *menuEvent->mouseX <= menuBotao->botaoRegrasX + menuBotao->botaoRegrasLargura && *menuEvent->mouseY >= menuBotao->botaoRegrasY && *menuEvent->mouseY <= menuBotao->botaoRegrasY + menuBotao->botaoRegrasAltura) {
                tocarSomClick(sons);

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
                            tocarSomClick(sons);
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

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "R - Trocar Arma");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Arma 1 - Vassoura (Efetivo contra Ratos)");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Arma 2 - Veneno (Efetivo contra Mosquitos)");
                        inicioY += espacamento;

                        al_draw_text(fonte, al_map_rgb(255, 255, 255), centroX, inicioY, ALLEGRO_ALIGN_CENTER, "Arma 3 - Vacina (Efetivo contra Zumbis)");
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
            }

            // Botão Sair
            if (*menuEvent->mouseX >= menuBotao->botaoSairX && *menuEvent->mouseX <= menuBotao->botaoSairX + menuBotao->botaoSairLargura && *menuEvent->mouseY >= menuBotao->botaoSairY && *menuEvent->mouseY <= menuBotao->botaoSairY + menuBotao->botaoSairAltura) {
                tocarSomClick(sons);
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

void desenhar_tela_gameOver(GameOver* gameover, Barra* infec, MenuEvents* menuEvent, MenuEstados* menuEstado, ALLEGRO_FONT* fonte) {
    ALLEGRO_EVENT event;

    // Reseta flag de reiniciar
    gameover->reiniciar = false;

    // Limpa fila de eventos
    al_flush_event_queue(menuEvent->fila_eventos);

    // Define cores
    ALLEGRO_COLOR corFundoRoxo = al_map_rgb(45, 29, 46); // Roxo
    ALLEGRO_COLOR corTexto = al_map_rgb(255, 255, 255); // Branco

    // DESENHA IMEDIATAMENTE antes de entrar no loop
    al_clear_to_color(corFundoRoxo);

    const char* titulo = "FIM DE JOGO";
    int larguraTitulo = al_get_text_width(fonte, titulo);
    int posXTitulo = (WIDTH - larguraTitulo) / 2;
    int posYTitulo = 250;
    al_draw_text(fonte, corTexto, posXTitulo, posYTitulo, 0, titulo);

    // Desenha botões inicialmente
    al_draw_bitmap(gameover->botaoJogarNovamente, gameover->botaoJogarNovamenteX, gameover->botaoJogarNovamenteY, 0);
    al_draw_bitmap(gameover->botaoSairDoJogo, gameover->botaoSairDoJogoX, gameover->botaoSairDoJogoY, 0);

    al_flip_display();

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
            // Jogar Novamente
            if (*menuEvent->mouseX >= gameover->botaoJogarNovamenteX &&
                *menuEvent->mouseX <= gameover->botaoJogarNovamenteX + gameover->botaoJogarNovamenteLargura &&
                *menuEvent->mouseY >= gameover->botaoJogarNovamenteY &&
                *menuEvent->mouseY <= gameover->botaoJogarNovamenteY + gameover->botaoJogarNovamenteAltura) {

                gameover->reiniciar = true;
                *menuEstado->fimDeJogo = false;
                break;
            }

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

        // Redesenha apenas quando necessário
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES || event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(corFundoRoxo);

            // Desenha título "FIM DE JOGO" centralizado
            al_draw_text(fonte, corTexto, posXTitulo, posYTitulo, 0, titulo);

            // Botão Jogar Novamente (hover)
            if (*menuEvent->mouseX >= gameover->botaoJogarNovamenteX &&
                *menuEvent->mouseX <= gameover->botaoJogarNovamenteX + gameover->botaoJogarNovamenteLargura &&
                *menuEvent->mouseY >= gameover->botaoJogarNovamenteY &&
                *menuEvent->mouseY <= gameover->botaoJogarNovamenteY + gameover->botaoJogarNovamenteAltura) {
                al_draw_bitmap(gameover->botaoJogarNovamente2, gameover->botaoJogarNovamenteX, gameover->botaoJogarNovamenteY, 0);
            }
            else {
                al_draw_bitmap(gameover->botaoJogarNovamente, gameover->botaoJogarNovamenteX, gameover->botaoJogarNovamenteY, 0);
            }

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
}

void inicializarMenuEstados(MenuEstados* estado, bool* telaMenu, bool* jogando, bool* regrasAberta, bool* esc, bool* jogoPausado, bool* fimDeJogo) {
    if (!estado) return;

    estado->telaMenu = telaMenu;
    estado->jogando = jogando;
    estado->regrasAberta = regrasAberta;
    estado->esc = esc;
    estado->jogoPausado = jogoPausado;
    estado->fimDeJogo = fimDeJogo;
}

void inicializarMenuEvents(MenuEvents* events, ALLEGRO_EVENT_QUEUE* fila, ALLEGRO_TIMER* timer, ALLEGRO_TRANSFORM* camera, float* mouseX, float* mouseY) {
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

    gameOver->botaoSairDoJogo = bitmap->botaoSair;
    gameOver->botaoSairDoJogo2 = bitmap->botaoSair2;
    gameOver->botaoJogarNovamente = bitmap->botaoJogar;
    gameOver->botaoJogarNovamente2 = bitmap->botaoJogar2;

    gameOver->botaoSairDoJogoLargura = al_get_bitmap_width(bitmap->botaoSair);
    gameOver->botaoSairDoJogoAltura = al_get_bitmap_height(bitmap->botaoSair);

    gameOver->botaoJogarNovamenteLargura = al_get_bitmap_width(bitmap->botaoJogar);
    gameOver->botaoJogarNovamenteAltura = al_get_bitmap_height(bitmap->botaoJogar);

    // Posições centralizadas dos botões (um acima do outro)
    gameOver->botaoJogarNovamenteX = (WIDTH - gameOver->botaoJogarNovamenteLargura) / 2;
    gameOver->botaoJogarNovamenteY = 350;

    gameOver->botaoSairDoJogoX = (WIDTH - gameOver->botaoSairDoJogoLargura) / 2;
    gameOver->botaoSairDoJogoY = 450;

    gameOver->reiniciar = false;
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