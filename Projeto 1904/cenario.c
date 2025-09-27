#include "cenario.h"

void desenhar_cenario(ALLEGRO_BITMAP* cenario1, ALLEGRO_BITMAP* cenario2, float posicaoJogadorX, float* posicaoCamera) {

    int larguraCenario = 1280;

    // VERIFICA A SEÇÃO COM BASE NA POSICAO DA CAMERA (eixo X)
    int secao_atual = (int)(posicaoCamera[0] / larguraCenario);

    // DESENHA 4 CENARIOS: anterior (-1), atual(0), próximo(1), e mais um(2)
    for (int posicao = -1; posicao <= 2; posicao++) {
        int indice = secao_atual + posicao;

        // NO INICIO DO JOGO O INDICE = -1, ENTAO IGNORA ELE
        if (indice < 0)
            continue;

        ALLEGRO_BITMAP* cenario;

        // TROCA CENARIOS SE O INDICE FOR PAR OU IMPAR
        if (indice % 2 == 0)
            cenario = cenario1;
        else
            cenario = cenario2;

        // CALCULA POSICAO DA CAMERA
        float pos_X = indice * larguraCenario;

        // Desenha apenas se estiver próximo da área visível
        if (pos_X >= posicaoCamera[0] - larguraCenario && pos_X <= posicaoCamera[0] + WIDTH + larguraCenario) {
            al_draw_bitmap(cenario, pos_X, 0, 0);
        }
    }
}