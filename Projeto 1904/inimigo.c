#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "inimigo.h"
#include "personagem.h"

void perseguir(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador, float velocidadeInimigo) {
    float deltaX = (jogador->jogadorX + larguraJogador / 2) - (bot->botX + bot->larguraBot / 2);
    float deltaY = (jogador->jogadorY + alturaJogador / 2) - (bot->botY + bot->alturaBot / 2);

    float botEsquerda = bot->botX;
    float botDireita = bot->botX + bot->larguraBot;
    float botCima = bot->botY;
    float botBaixo = bot->botY + bot->alturaBot;

    float jogadorEsquerda = jogador->jogadorX;
    float jogadorDireita = jogador->jogadorX + larguraJogador;
    float jogadorCima = jogador->jogadorY;
    float jogadorBaixo = jogador->jogadorY + alturaJogador;

    float zona = 0.02f;

    // Verifica se está próximo ou colidindo
    bool proximoX = (botDireita >= jogadorEsquerda - zona && botEsquerda <= jogadorDireita + zona);
    bool proximoY = (botBaixo >= jogadorCima - zona && botCima <= jogadorBaixo + zona);
    bool colidindo = proximoX && proximoY;

    if (!colidindo) {
        float novoX = bot->botX, novoY = bot->botY;

        // Move X
        if (fabs(deltaX) > velocidadeInimigo) {
            novoX += (deltaX > 0) ? velocidadeInimigo : -velocidadeInimigo;
        }
        else if (fabs(deltaX) > 0.5f) {
            novoX += deltaX;
        }

        // Move Y
        if (fabs(deltaY) > velocidadeInimigo) {
            novoY += (deltaY > 0) ? velocidadeInimigo : -velocidadeInimigo;
        }
        else if (fabs(deltaY) > 0.5f) {
            novoY += deltaY;
        }

        // Testa nova posição
        float novoEsquerda = novoX;
        float novoDireita = novoX + bot->larguraBot;
        float novoCima = novoY;
        float novoBaixo = novoY + bot->alturaBot;

        // Atualiza posição se não colidir
        if (!(novoDireita > jogadorEsquerda && novoEsquerda < jogadorDireita &&
            botBaixo > jogadorCima && botCima < jogadorBaixo)) {
            bot->botX = novoX;
        }

        if (!(botDireita > jogadorEsquerda && botEsquerda < jogadorDireita &&
            novoBaixo > jogadorCima && novoCima < jogadorBaixo)) {
            bot->botY = novoY;
        }
    }
}
