#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inimigo.h"
#include "personagens.h"

void perseguir(Inimigo* bot, Jogador* jogador) {
    if (jogador->posicaoXp + jogador->largurap / 2 < bot->posicaoX + bot->largura / 2)
        bot->posicaoX--;
    else if (jogador->posicaoXp + jogador->largurap / 2 > bot->posicaoX + bot->largura / 2)
        bot->posicaoX++;
    if (jogador->posicaoYp + jogador->alturap / 2 < bot->posicaoY + bot->altura / 2)
        bot->posicaoY--;
    else if (jogador->posicaoYp + jogador->alturap / 2 > bot->posicaoY + bot->altura / 2)
        bot->posicaoY++;
    return;
}