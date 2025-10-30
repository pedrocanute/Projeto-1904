#define _CRT_SECURE_NO_WARNINGS
#include "introducao.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <string.h>

void inicializar_introducao(TelaIntroducao* intro, const char* caminhoFonte, int tamanhoFonte, int numeroTelas) {
    if (!intro) return;

    al_init_ttf_addon();

    intro->estado = INTRO_FADEIN;
    intro->alfa = 0.0f;
    intro->tempoAtual = 0.0f;
    intro->tempoFadeIn = 2.0f;
    intro->tempoExibicao = 3.0f;
    intro->velocidadeFade = 127.5f;
    intro->concluido = false;

    intro->numeroTelas = numeroTelas;
    intro->telaAtual = 0;
    intro->telas = (TelaTexto*)malloc(sizeof(TelaTexto) * numeroTelas);

    if (!intro->telas) {
        intro->numeroTelas = 0;
        return;
    }

    for (int i = 0; i < numeroTelas; i++) {
        intro->telas[i].linhas = NULL;
        intro->telas[i].numeroLinhas = 0;
        intro->telas[i].tempoExibicao = 3.0f;
    }

    intro->fonte = al_load_ttf_font(caminhoFonte, tamanhoFonte, 0);
}

void adicionar_tela(TelaIntroducao* intro, int indiceTela, char** linhas, int numeroLinhas, float tempoExibicao) {
    if (!intro || !linhas || numeroLinhas <= 0) return;
    if (indiceTela < 0 || indiceTela >= intro->numeroTelas) return;

    TelaTexto* tela = &intro->telas[indiceTela];

    if (tela->linhas != NULL) {
        for (int j = 0; j < tela->numeroLinhas; j++) {
            if (tela->linhas[j]) free(tela->linhas[j]);
        }
        free(tela->linhas);
        tela->linhas = NULL;
    }

    tela->numeroLinhas = numeroLinhas;
    tela->tempoExibicao = tempoExibicao;
    tela->linhas = (char**)malloc(sizeof(char*) * numeroLinhas);

    if (!tela->linhas) {
        tela->numeroLinhas = 0;
        return;
    }

    for (int i = 0; i < numeroLinhas; i++) {
        tela->linhas[i] = NULL;
    }

    for (int i = 0; i < numeroLinhas; i++) {
        if (!linhas[i]) continue;

        size_t tamanho = strlen(linhas[i]) + 1;
        tela->linhas[i] = (char*)malloc(tamanho);

        if (!tela->linhas[i]) {
            continue;
        }

        strcpy(tela->linhas[i], linhas[i]);
    }
}

void atualizar_introducao(TelaIntroducao* intro, float deltaTime) {
    if (intro->concluido) return;

    intro->tempoAtual += deltaTime;

    switch (intro->estado) {
    case INTRO_FADEIN:
        intro->alfa += intro->velocidadeFade * deltaTime;
        if (intro->alfa >= 255.0f) {
            intro->alfa = 255.0f;
            intro->estado = INTRO_EXIBINDO;
            intro->tempoAtual = 0.0f;
        }
        break;

    case INTRO_EXIBINDO:
        if (intro->telaAtual < intro->numeroTelas) {
            float tempoTela = intro->telas[intro->telaAtual].tempoExibicao;
            if (intro->tempoAtual >= tempoTela) {
                intro->estado = INTRO_FADEOUT;
                intro->tempoAtual = 0.0f;
            }
        }
        break;

    case INTRO_FADEOUT:
        intro->alfa -= intro->velocidadeFade * deltaTime;
        if (intro->alfa <= 0.0f) {
            intro->alfa = 0.0f;
            intro->telaAtual++;
            if (intro->telaAtual >= intro->numeroTelas) {
                intro->estado = INTRO_COMPLETO;
                intro->concluido = true;
            }
            else {
                intro->estado = INTRO_FADEIN;
                intro->tempoAtual = 0.0f;
            }
        }
        break;

    case INTRO_COMPLETO:
        intro->concluido = true;
        break;
    }
}

void desenhar_introducao(TelaIntroducao* intro, int larguraTela, int alturaTela) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (intro->fonte && intro->telaAtual < intro->numeroTelas) {
        TelaTexto* tela = &intro->telas[intro->telaAtual];

        if (tela->linhas == NULL) return;

        ALLEGRO_COLOR cor = al_map_rgba_f(intro->alfa / 255.0f, intro->alfa / 255.0f, intro->alfa / 255.0f, intro->alfa / 255.0f);

        int posX = larguraTela / 2;
        int posY = alturaTela / 2;

        int espacamento = 50;
        int alturaTotal = tela->numeroLinhas * espacamento;
        int offsetY = -alturaTotal / 2;

        for (int i = 0; i < tela->numeroLinhas; i++) {
            if (tela->linhas[i]) {
                al_draw_text(intro->fonte, cor, posX, posY + offsetY, ALLEGRO_ALIGN_CENTRE, tela->linhas[i]);
                offsetY += espacamento;
            }
        }
    }
}

void destruir_introducao(TelaIntroducao* intro) {
    printf("d\n");
    if (intro->fonte) {
        al_destroy_font(intro->fonte);
        intro->fonte = NULL;
    }
    if (intro->telas) {
        for (int i = 0; i < intro->numeroTelas; i++) {
            TelaTexto* tela = &intro->telas[i];
            // Adiciona verificação se tela->linhas foi inicializado
            if (tela->linhas != NULL) {
                for (int j = 0; j < tela->numeroLinhas; j++) {
                    if (tela->linhas[j]) free(tela->linhas[j]);
                }
                free(tela->linhas);
                tela->linhas = NULL;
            }
        }
        free(intro->telas);
        intro->telas = NULL;
    }
}
