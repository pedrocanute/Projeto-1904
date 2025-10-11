#include "infeccao.h"

/*bool colisao_aabb(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    float a_esq = x1;
    float a_dir = x1 + w1;
    float a_top = y1;
    float a_baix = y1 + h1;

    float b_esq = x2;
    float b_dir = x2 + w2;
    float b_top = y2;
    float b_baix = y2 + h2;

    return (a_esq < b_dir) && (a_dir > b_esq) && (a_top < b_baix) && (a_baix > b_top);
}*/
void desenhar_barra(float infeccaoX, float infeccaoY, float infeccaoLargura, float infeccaoAltura) {
    al_draw_filled_rectangle(infeccaoX, infeccaoY, infeccaoLargura, infeccaoAltura, al_map_rgb(0, 0, 0));
};

void desenhar_barra_infeccao(float infeccaoX, float infeccaoY, float infeccaoLargura, float infeccaoAltura) {
    al_draw_filled_rectangle(infeccaoX, infeccaoY, infeccaoLargura, infeccaoAltura, al_map_rgb(0, 255, 0));
};

// Desenhar tela de game over ao atingir o limite de infecção
void desenhar_tela_gameOver(GameOver* gameover, float *infeccaoAtual, float infeccaoMaxima) {
    if (*infeccaoAtual >= infeccaoMaxima) {
        al_draw_scaled_bitmap(gameover->telaGameOver, 0, 0, gameover->telaGameOverLargura, gameover->telaGameOverAltura, 0, 0, WIDTH, HEIGHT, 0);
    }
}
