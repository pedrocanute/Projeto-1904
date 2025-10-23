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
    intro->tempoFadeIn = 2.0f;       // 2 segundos para fade in
    intro->tempoExibicao = 3.0f;      // 3 segundos exibindo
    intro->velocidadeFade = 127.5f;    // Velocidade do fade (alfa por segundo)
    intro->concluido = false;

    intro->numeroTelas = numeroTelas;
    intro->telaAtual = 0;
    intro->telas = (TelaTexto*)malloc(sizeof(TelaTexto) * numeroTelas);

    for (int i = 0; i < numeroTelas; i++) {
        intro->telas[i].linhas = NULL;
        intro->telas[i].numeroLinhas = 0;
        intro->telas[i].tempoExibicao = 3.0f;  // Padrão: 3 segundos
    }

    //CARREGA A FONTE 
    intro->fonte = al_load_ttf_font(caminhoFonte, tamanhoFonte, 0);

      /*
        á	\xC3\xA1	
        à	\xC3\xA0	
        â	\xC3\xA2	
        ã	\xC3\xA3	
        é	\xC3\xA9	
        ê	\xC3\xAA	
        í	\xC3\xAD	
        ó	\xC3\xB3	
        ô	\xC3\xB4	
        õ	\xC3\xB5	
        ú	\xC3\xBA	
        ç	\xC3\xA7
        */	

    /*intro->texto = "Rio de Janeiro, 1904...\nA cidade enfrenta uma epidemia mortal.\nVoc\xC3\xAA \xC3\xA9 a \xC3\xBAltima esperan\xC3\xA7""a.";*/

}

void adicionar_tela(TelaIntroducao* intro, int indiceTela, char** linhas, int numeroLinhas, float tempoExibicao) {
    
    TelaTexto* tela = &intro->telas[indiceTela];

    tela->numeroLinhas = numeroLinhas;
    tela->linhas = (char**)malloc(sizeof(char*) * numeroLinhas);
    tela->tempoExibicao = tempoExibicao;

    for (int i = 0; i < numeroLinhas; i++) {
        size_t tamanho = strlen(linhas[i]) + 1;
        tela->linhas[i] = (char*)malloc(tamanho);
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

        if (intro->tempoAtual >= intro->tempoExibicao) {
            intro->estado = INTRO_FADEOUT;
            intro->tempoAtual = 0.0f;
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
   
    //FUNDO PRETO
    al_clear_to_color(al_map_rgb(0, 0, 0));

    if (intro->fonte && intro->telaAtual < intro->numeroTelas) {

        TelaTexto* tela = &intro->telas[intro->telaAtual];

        if (tela->linhas == NULL) return;

        ALLEGRO_COLOR cor = al_map_rgba_f(intro->alfa / 255.0f, intro->alfa / 255.0f, intro->alfa / 255.0f, intro->alfa / 255.0f);

        //CENTRALIZA O TEXTO
        int posX = larguraTela / 2;
        int posY = alturaTela / 2;

        int alturaTotal = tela->numeroLinhas * 40;
        int offsetY = -(alturaTotal / 2) + 20;

        // Desenha cada linha
        for (int i = 0; i < tela->numeroLinhas; i++) {
            al_draw_text(intro->fonte, cor, posX, posY + offsetY,ALLEGRO_ALIGN_CENTRE, tela->linhas[i]);
            offsetY += 40;  // Espaçamento entre linhas
        }
    }
}

void destruir_introducao(TelaIntroducao* intro) {
    if (intro->fonte) {
        al_destroy_font(intro->fonte);
        intro->fonte = NULL;
    }
    if (intro->telas) {
        for (int i = 0; i < intro->numeroTelas; i++) {
            TelaTexto* tela = &intro->telas[i];
            if (tela->linhas) {
                for (int j = 0; j < tela->numeroLinhas; j++) {
                    free(tela->linhas[j]);
                }
                free(tela->linhas);
            }
        }
        free(intro->telas);
        intro->telas = NULL;
    }
}
