#include "transicoes.h"
#include <allegro5/allegro_primitives.h>
#include "configuracoes.h"
#include "cenario.h"
#include "jogo.h"
#include "personagem.h"
#include "caravana.h"

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
    adicionar_tela(&intro, 0, tela1, 2, 3.0f);

    // TELA 2 - Epidemias
    char* tela2[] = {
        "Varíola, febre amarela e peste bubônica,",
        "espalham medo nas ruas e afastam navios e visitantes"
    };
    adicionar_tela(&intro, 1, tela2, 2, 3.0f);

    // TELA 3 - Protagonista
    char* tela3[] = {
        "O governo convoca Oswaldo Cruz,",
        "para liderar uma campanha sanitária sem precedentes."
    };
    adicionar_tela(&intro, 2, tela3, 2, 3.0f);

    bool concluido = false;
    float tempoAnterior = al_get_time();

    // Loop da introdução
    while (!concluido && *menuEstado->jogando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(menuEvent->fila_eventos, &event);

        // Verifica fechamento de janela
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *menuEstado->jogando = false;
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
    return *menuEstado->jogando;
}

bool executarFadeTransicao(MenuEstados* menuEstado, MenuEvents* menuEvent, double duracao, bool fadeIn) {
    SistemaFade fade;
    inicializarFade(&fade, duracao, fadeIn);

    while (!fadeCompleto(&fade) && *menuEstado->jogando) {
        ALLEGRO_EVENT event;
        bool temEvento = al_wait_for_event_timed(menuEvent->fila_eventos, &event, 0.016);

        if (temEvento && event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            *menuEstado->jogando = false;
            return false;
        }

        atualizarFade(&fade);

        // Desenha tela preta
        al_clear_to_color(al_map_rgb(0, 0, 0));
        desenharFade(&fade, WIDTH, HEIGHT);
        al_flip_display();
    }

    return *menuEstado->jogando;
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
    else if (sistemaFase->faseAtual == 4) {
        // Diálogo de vitória - sempre exibe quando fase == 4
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

    // Retoma o timer (só se não for vitória)
    if (*menuEstado->jogando && sistemaFase->faseAtual < 4) {
        al_start_timer(menuEvent->timer);
    }

    return *menuEstado->jogando;
}

// ============= CUTSCENE =============

void inicializarCutscene(SistemaCutscene* cutscene) {
    cutscene->estado = CUTSCENE_JOGADOR_ENTRADA;
    cutscene->progresso = 0.0f;
    cutscene->completa = false;
}

bool executarCutsceneInicial(JogoEntidades* entidades, JogoCamera* jogoCamera, JogoAnimacao* animacao, MenuEvents* menuEvent, MenuEstados* menuEstado, Bitmaps* bitmap, JogoControle* controle, ALLEGRO_FONT* fonte) {
    
    SistemaCutscene cutscene;
    inicializarCutscene(&cutscene);

    // Posiciona jogador fora da tela à esquerda
    entidades->jogador.jogadorX = -150.0f;
    entidades->jogador.jogadorY = 550.0f;
    entidades->jogador.paraDireita = true;
    entidades->jogador.paraEsquerda = false;

    // Posiciona caravana fora da tela à esquerda
    entidades->caravana.caravanaX = -400.0f;
    entidades->caravana.caravanaY = 305.0f;

    // Configurações de velocidade
    const float VELOCIDADE_JOGADOR_CUTSCENE = 2.5f;
    const float VELOCIDADE_CARAVANA_CUTSCENE = 2.0f;
    const float DESTINO_JOGADOR = 200.0f;
    const float DESTINO_CARAVANA = 0.0f;

    // Reseta a câmera para posição fixa no início
    jogoCamera->posicaoCamera[0] = 0.0f;
    jogoCamera->posicaoCamera[1] = 0.0f;

    // Variáveis de animação
    animacao->virado_direita = true;
    animacao->frame_atual = 0;
    animacao->contador_frame = 0;
    animacao->frames_por_sprite = 8;  // Velocidade da animação

    // Para o timer do jogo
    al_stop_timer(menuEvent->timer);

    // Limpa eventos
    al_flush_event_queue(menuEvent->fila_eventos);

    bool cutsceneAtiva = true;

    while (cutsceneAtiva && *menuEstado->jogando) {
        ALLEGRO_EVENT event;
        bool temEvento = al_get_next_event(menuEvent->fila_eventos, &event);

        if (temEvento) {
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                *menuEstado->jogando = false;
                return false;
            }

            // APENAS ENTER pula a cutscene
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    // Posiciona diretamente nas posições finais
                    entidades->jogador.jogadorX = DESTINO_JOGADOR;
                    entidades->jogador.jogadorY = 550.0f;
                    entidades->caravana.caravanaX = DESTINO_CARAVANA;
                    cutsceneAtiva = false;
                    break;
                }
            }
        }

        // Estado 1: Jogador entra
        if (cutscene.estado == CUTSCENE_JOGADOR_ENTRADA) {
            entidades->jogador.jogadorX += VELOCIDADE_JOGADOR_CUTSCENE;

            // Atualiza animação do jogador
            animacao->contador_frame++;
            if (animacao->contador_frame >= animacao->frames_por_sprite) {
                animacao->contador_frame = 0;
                animacao->frame_atual = (animacao->frame_atual + 1) % 2;  
            }

            // Verifica se chegou ao destino
            if (entidades->jogador.jogadorX >= DESTINO_JOGADOR) {
                entidades->jogador.jogadorX = DESTINO_JOGADOR;
                entidades->jogador.jogadorY = 550.0f;
                cutscene.estado = CUTSCENE_CARAVANA_ENTRADA;
                animacao->frame_atual = 0;  // Frame parado
                al_rest(0.5); // Pequena pausa de 5 segundos
            }
        }
        // Estado 2: Caravana entra
        else if (cutscene.estado == CUTSCENE_CARAVANA_ENTRADA) {
            entidades->caravana.caravanaX += VELOCIDADE_CARAVANA_CUTSCENE;

            // Verifica se chegou ao destino
            if (entidades->caravana.caravanaX >= DESTINO_CARAVANA) {
                entidades->caravana.caravanaX = DESTINO_CARAVANA;
                cutscene.estado = CUTSCENE_COMPLETA;
                cutsceneAtiva = false;
            }
        }

        // ========== DESENHO ==========
        
        // Limpa a tela
        al_clear_to_color(al_map_rgb(45, 29, 46));

        // Aplicar transformação da câmera
        ALLEGRO_TRANSFORM transform;
        al_identity_transform(&transform);
        al_translate_transform(&transform, -jogoCamera->posicaoCamera[0], -jogoCamera->posicaoCamera[1]);
        al_use_transform(&transform);

        // Desenha cenário
        desenhar_cenario(bitmap->cenario1, bitmap->cenario2, entidades->jogador.jogadorX, jogoCamera->posicaoCamera);

        // Desenha caravana (se já deve aparecer)
        if (cutscene.estado == CUTSCENE_CARAVANA_ENTRADA) {
            desenhar_caravana(bitmap->soldado, entidades->caravana.caravanaX, entidades->caravana.caravanaY,
                            entidades->caravana.caravanaLargura, entidades->caravana.caravanaAltura, 
                            al_map_rgb(255, 255, 255));
        }

        // Desenha jogador
        ALLEGRO_BITMAP* spriteAtual = bitmap->sprite_andando_direita;
        
        // Dimensões corretas: sprite tem 2 frames lado a lado
        int larguraTotal = al_get_bitmap_width(spriteAtual);
        int alturaFrame = al_get_bitmap_height(spriteAtual);
        int larguraFrame = larguraTotal / 2;  // 2 colunas
        
        // Seleciona o frame correto
        int frame = (cutscene.estado == CUTSCENE_JOGADOR_ENTRADA) ? animacao->frame_atual : 0;
        int sx = frame * larguraFrame;
        
        // Desenha o jogador
        al_draw_bitmap_region(spriteAtual, sx, 0, larguraFrame, alturaFrame, entidades->jogador.jogadorX, entidades->jogador.jogadorY, 0);

        // ========== DESENHA TEXTO DE COMANDOS ==========
        ALLEGRO_TRANSFORM hud;
        al_identity_transform(&hud);
        al_use_transform(&hud);

        // Caixa de fundo semi-transparente para os comandos
        float caixaX = 20.0f;
        float caixaY = 20.0f;
        float caixaLargura = 420.0f;
        float caixaAltura = 200.0f;
        
        al_draw_filled_rectangle(caixaX, caixaY, caixaX + caixaLargura, caixaY + caixaAltura, al_map_rgba(0, 0, 0, 180));
        al_draw_rectangle(caixaX, caixaY, caixaX + caixaLargura, caixaY + caixaAltura, al_map_rgb(255, 255, 255), 2.0f);

        // Textos dos comandos
        float textoX = caixaX + 15.0f;
        float textoY = caixaY + 15.0f;
        float espacamentoLinha = 25.0f;

        if (fonte) {
            al_draw_text(fonte, al_map_rgb(255, 215, 0), textoX, textoY, 0, "COMANDOS");
            textoY += espacamentoLinha + 5;

            al_draw_text(fonte, al_map_rgb(255, 255, 255), textoX, textoY, 0, "WASD - Mover");
            textoY += espacamentoLinha;

            al_draw_text(fonte, al_map_rgb(255, 255, 255), textoX, textoY, 0, "SHIFT - Correr");
            textoY += espacamentoLinha;

            al_draw_text(fonte, al_map_rgb(255, 255, 255), textoX, textoY, 0, "ESPAÇO - Atirar");
            textoY += espacamentoLinha;

            al_draw_text(fonte, al_map_rgb(255, 255, 255), textoX, textoY, 0, "1/2/3 - Trocar Arma");
            textoY += espacamentoLinha;

            al_draw_text(fonte, al_map_rgb(255, 255, 255), textoX, textoY, 0, "ESC - Pausar");
            textoY += espacamentoLinha + 10;

        }

        al_flip_display();
        al_rest(0.016); // 60 FPS
    }

    // Verifica se foi fechado
    if (!*menuEstado->jogando) {
        return false;
    }

    // Garante que as posições finais estão corretas
    entidades->jogador.jogadorX = DESTINO_JOGADOR;
    entidades->jogador.jogadorY = 550.0f;
    entidades->caravana.caravanaX = DESTINO_CARAVANA;

    // ATIVA O SISTEMA DE SPAWN APÓS A CUTSCENE
    controle->cutscene_concluida = true;

    // Limpa eventos acumulados
    al_flush_event_queue(menuEvent->fila_eventos);

    // Retoma o timer do jogo
    if (*menuEstado->jogando) {
        al_start_timer(menuEvent->timer);
    }

    return true;
}