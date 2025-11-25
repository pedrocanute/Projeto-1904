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
        dialogo->textos[0] = "Sou Oswaldo Cruz, Diretor-Geral de Saúde Pública desde 1903.";
        dialogo->textos[1] = "O presidente Rodrigues Alves me convocou para modernizar o Rio de Janeiro, que vive uma crise sanitária profunda.";
        dialogo->textos[2] = "A cidade sofre com a varíola, a febre amarela e a peste bubônica — doenças que matam milhares todos os anos.";
        dialogo->textos[3] = "A varíola, em especial, se espalha rapidamente. Por isso, decretamos a vacinação obrigatória em todo o Brasil.";
        dialogo->textos[4] = "A medida gerou medo, boatos e protestos nas ruas.";
        dialogo->textos[5] = "A mudança foi brusca para a população, que ainda não entende o que é uma vacina nem como ela funciona.";
        dialogo->textos[6] = "Boatos, medo e desinformação começaram a ganhar força nas ruas da capital.";
        dialogo->textos[7] = "Mesmo assim, não podemos recuar: limpeza urbana, combate aos vetores e vacinação são essenciais para salvar vidas.";
        dialogo->textos[8] = "Preciso de você no campo para três tarefas: vacinar, desinfetar e controlar os animais transmissores.";
        dialogo->textos[9] = "Cada ferramenta funciona para um tipo específico de ameaça.";
        dialogo->textos[10] = "A vassoura extermina os ratos da peste bubônica. O veneno elimina mosquitos transmissores da febre amarela. A vacina protege o povo da varíola.";
        dialogo->textos[11] = "Proteja os bairros rapidamente. A saúde pública depende de ações firmes e ágeis. Vamos mostrar que ciência salva vidas.";
        dialogo->numeroTextos = 12;
        break;

    case 2:
        dialogo->textos[0] = "Excelente trabalho! Eliminamos muitos vetores da peste bubônica, mas a crise está longe de acabar.";
        dialogo->textos[1] = "Embora as ações de saneamento estejam avançando, novos focos de infecção continuam surgindo.";
        dialogo->textos[2] = "A população, porém, não recebeu informações claras sobre o que é a vacina ou por que ela é necessária.";
        dialogo->textos[3] = "Sem explicações, surgiram boatos dizendo que a vacina era perigosa, que invadia a casa das pessoas ou que causava doenças — tudo isso alimentou revolta.";
        dialogo->textos[4] = "Mesmo assim, precisamos continuar: vacinar, informar e combater os vetores é a única forma de controlar as epidemias.";
        dialogo->textos[5] = "A ciência pode enfrentar doenças, mas precisa superar também a desinformação.";
        dialogo->numeroTextos = 6;
        break;

    case 3:
        dialogo->textos[0] = "Estamos chegando à etapa final. Depois de tudo isso, eu te prometo que seu valor será reconhecido.";
        dialogo->textos[1] = "Com os protestos crescendo, o governo respondeu com forte repressão: prisões, violência e até deportações.";
        dialogo->textos[2] = "A Revolta da Vacina não é apenas sobre saúde. É também sobre desigualdade, falta de informação e choque entre governo e população.";
        dialogo->textos[3] = "Ainda assim, precisamos concluir o trabalho: eliminar os últimos focos de varíola e os vetores remanescentes.";
        dialogo->textos[4] = "Juntos, vamos transformar o Rio de Janeiro em uma cidade mais segura e livre das grandes epidemias.";
        dialogo->numeroTextos = 5;
        break;

    case 4:  // === DIÁLOGO DE VITÓRIA ===
        dialogo->textos[0] = "VITÓRIA!";
        dialogo->textos[1] = "Após a Revolta da Vacina, o governo recuou e a vacinação deixou de ser obrigatória naquele momento.";
        dialogo->textos[2] = "Mesmo assim, as campanhas continuaram e a população, aos poucos, passou a confiar mais na ciência.";
        dialogo->textos[3] = "As epidemias no Rio foram controladas graças ao saneamento, ao combate aos mosquitos, à eliminação dos ratos e à vacinação.";
        dialogo->textos[4] = "Essas ações transformaram a saúde pública brasileira e se tornaram referência internacional.";
        dialogo->textos[5] = "Milhares de vidas foram salvas porque escolhemos enfrentar o medo com ciência e trabalho.";
        dialogo->textos[6] = "Parabéns por completar sua missão e ajudar a reescrever essa história!";
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