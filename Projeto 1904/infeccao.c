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

void desenhar_barra_infeccao(float infeccaoX, float infeccaoY, float infeccaoLargura, float infeccaoAltura, float* posicaoCamera){
    al_draw_filled_rectangle(infeccaoX + posicaoCamera[0], infeccaoY, infeccaoLargura + posicaoCamera[0], infeccaoAltura, al_map_rgb(0, 255, 0));
};

void desenhar_barra_vida_personagem(float vidaX, float vidaY, float vidaLargura, float vidaAltura, float* posicaoCamera) {
    al_draw_filled_rectangle(vidaX + posicaoCamera[0], vidaY, vidaLargura + posicaoCamera[0], vidaAltura, al_map_rgb(255, 0, 0));
};

void desenhar_fundo_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura) {
    al_draw_filled_rectangle(vidaX, vidaY, vidaLargura, vidaAltura, al_map_rgb(0, 0, 0));
};

void desenhar_barra_vida_boss(float vidaX, float vidaY, float vidaLargura, float vidaAltura) {
    al_draw_filled_rectangle(vidaX, vidaY, vidaLargura, vidaAltura, al_map_rgb(255, 0, 0));
};
