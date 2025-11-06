#include "dialogos.h"
#include "menu.h"
#include <allegro5/allegro_primitives.h>

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
        dialogo->textos[0] = "A situação se agravou. Novos focos de infecção surgem.";
        dialogo->textos[1] = "Os protestos aumentaram, mas não podemos recuar.";
        dialogo->textos[2] = "Continue vacinando e controlando os vetores.";
        dialogo->textos[3] = "A ciência prevalecerá sobre a ignorância.";
        dialogo->numeroTextos = 4;
        break;

    case 3:
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
    else if (fase->faseAtual == 4) {
        iniciarDialogo = true;
        configurarTextosDialogo(dialogo, 4);
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
    if (fase->faseAtual == 4) {
        *menuEstado->jogando = false;  // Encerra o jogo
        return;  
    }

    // Limpa eventos antes de retomar
    al_flush_event_queue(menuEvent->fila_eventos);

    // Retoma o jogo
    if (*menuEstado->jogando) {
        al_start_timer(menuEvent->timer);
    }
}