#include "colisao.h"

bool colisao_aabb(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
    float a_esq = x1;
    float a_dir = x1 + w1;
    float a_top = y1;
    float a_baix = y1 + h1;

    float b_esq = x2;
    float b_dir = x2 + w2;
    float b_top = y2;
    float b_baix = y2 + h2;

    return (a_esq < b_dir) && (a_dir > b_esq) && (a_top < b_baix) && (a_baix > b_top);
}

bool colisao_jogador_inimigo(const Inimigo* bot, const Jogador* jogador, float larguraJogador, float alturaJogador) {
    if (!bot || !jogador || !bot->ativo) return false;

    return colisao_aabb(bot->botX, bot->botY, bot->larguraBot, bot->alturaBot, jogador->jogadorX, jogador->jogadorY, larguraJogador, alturaJogador);
}

bool colisao_projetil_inimigo(float projX, float projY, int projW, int projH, const Inimigo* inimigo) {
    if (!inimigo || !inimigo->ativo) return false;

    return colisao_aabb(projX, projY, (float)projW, (float)projH, inimigo->botX, inimigo->botY, inimigo->larguraBot, inimigo->alturaBot);
}

bool colisao_inimigo_caravana(const Inimigo* bot, const Jogador* caravana, float caravanaLargura, float caravanaAltura) {
    if (!bot || !caravana || !bot->ativo) return false;

    return colisao_aabb(bot->botX, bot->botY, bot->larguraBot, bot->alturaBot, caravana->jogadorX, caravana->jogadorY, caravanaLargura, caravanaAltura);
}
