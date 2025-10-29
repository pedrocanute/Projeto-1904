#include "transicoes.h"
#include <allegro5/allegro_primitives.h>
#include "configuracoes.h"

// ============= IMPLEMENTAÇÃO DO SISTEMA DE FADE =============

void inicializarFade(SistemaFade* fade, double duracao, bool fadeIn) {
    fade->alfa = fadeIn ? 255.0f : 0.0f;
    fade->tempoInicio = al_get_time();
    fade->duracao = duracao;
    fade->completo = false;
    fade->fadeIn = fadeIn;
}

void atualizarFade(SistemaFade* fade) {
    double tempoAtual = al_get_time();
    double progresso = (tempoAtual - fade->tempoInicio) / fade->duracao;

    if (progresso >= 1.0) {
        fade->completo = true;
        fade->alfa = fade->fadeIn ? 0.0f : 255.0f;
    }
    else {
        if (fade->fadeIn) {
            // Clarear: 255 -> 0
            fade->alfa = 255.0f * (1.0f - progresso);
        }
        else {
            // Escurecer: 0 -> 255
            fade->alfa = 255.0f * progresso;
        }
    }
}

void desenharFade(SistemaFade* fade, int largura, int altura) {
    al_draw_filled_rectangle(0, 0, largura, altura,al_map_rgba(0, 0, 0, fade->alfa / 255.0f));
}

bool fadeCompleto(SistemaFade* fade) {
    return fade->completo;
}

// ============= IMPLEMENTAÇÃO DAS SEQUÊNCIAS =============

bool executarIntroducao(MenuEstados* menuEstado, MenuEvents* menuEvent) {
    
    TelaIntroducao intro;
    inicializar_introducao(&intro, "joystix monospace.otf", 18, 3);

    // TELA 1 - Contexto
    char* tela1[] = {
        "Rio de Janeiro, 1904...",
        "a capital é tomada por epidemias e reformas urbanas duras."
    };
    adicionar_tela(&intro, 0, tela1, 2, 5.0f);

    // TELA 2 - Epidemias
    char* tela2[] = {
        "Varíola, febre amarela e peste bubônica,",
        "espalham medo nas ruas e afastam navios e visitantes"
    };
    adicionar_tela(&intro, 1, tela2, 2, 5.0f);

    // TELA 3 - Protagonista
    char* tela3[] = {
        "O governo convoca Oswaldo Cruz,",
        "para liderar uma campanha sanitária sem precedentes."
    };
    adicionar_tela(&intro, 2, tela3, 2, 5.0f);

    bool concluido = false;
    float tempoAnterior = al_get_time();

    // Loop da introdução
    while (!concluido && menuEstado->jogando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(menuEvent->fila_eventos, &event);

        // Verifica fechamento de janela
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            menuEstado->jogando = false;
            break;
        }

        // SPACE ou ENTER pula a intro
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                concluido = true;
            }
        }

        // Atualização da animação
        if (event.type == ALLEGRO_EVENT_TIMER) {
            float tempoAtual = al_get_time();
            float deltaTime = tempoAtual - tempoAnterior;
            tempoAnterior = tempoAtual;

            atualizar_introducao(&intro, deltaTime);
            desenhar_introducao(&intro, WIDTH, HEIGHT);
            al_flip_display();

            if (intro.concluido)
                concluido = true;
        }
    }

    destruir_introducao(&intro);
    return menuEstado->jogando;
}

bool executarFadeTransicao(MenuEstados* menuEstado, MenuEvents* menuEvent, double duracao, bool fadeIn) {
    SistemaFade fade;
    inicializarFade(&fade, duracao, fadeIn);

    while (!fadeCompleto(&fade) && menuEstado->jogando) {
        ALLEGRO_EVENT event;
        bool temEvento = al_wait_for_event_timed(menuEvent->fila_eventos, &event, 0.016);

        if (temEvento && event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            menuEstado->jogando = false;
            return false;
        }

        atualizarFade(&fade);

        // Desenha tela preta
        al_clear_to_color(al_map_rgb(0, 0, 0));
        desenharFade(&fade, WIDTH, HEIGHT);
        al_flip_display();
    }

    return menuEstado->jogando;
}

bool executarDialogoInicial(Dialogo* dialogo, SistemaFases* sistemaFase, MenuEvents* menuEvent, MenuEstados* menuEstado) {
   

    // Verifica qual diálogo deve iniciar
    bool iniciarDialogo = false;
    if (sistemaFase->faseAtual == 1 && !dialogo->dialogo1) {
        iniciarDialogo = true;
    }
    else if (sistemaFase->faseAtual == 2 && !dialogo->dialogo2) {
        iniciarDialogo = true;
    }
    else if (sistemaFase->faseAtual == 3 && !dialogo->dialogo3) {
        iniciarDialogo = true;
    }

    if (!iniciarDialogo) {
        return true;  // Não precisa exibir diálogo
    }

    // Para o timer do jogo
    al_stop_timer(menuEvent->timer);

    // Limpa eventos antigos
    al_flush_event_queue(menuEvent->fila_eventos);

    // Chama a função existente de diálogo
    desenhar_tela_dialogo(dialogo, sistemaFase, menuEvent, menuEstado);

    // Retoma o timer
    if (menuEstado->jogando) {
        al_start_timer(menuEvent->timer);
    }

    return menuEstado->jogando;
}
