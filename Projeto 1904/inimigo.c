#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inimigo.h"
#include "personagem.h"

// FUNCAO COLISAO
bool detectarColisao(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador) {
    float botEsquerda = bot->botX;
    float botDireita = bot->botX + bot->larguraBot;
    float botTopo = bot->botY;
    float botBaixo = bot->botY + bot->alturaBot;

    float jogadorEsquerda = jogador->jogadorX;
    float jogadorDireita = jogador->jogadorX + larguraJogador;
    float jogadorTopo = jogador->jogadorY;
    float jogadorBaixo = jogador->jogadorY + alturaJogador;

    // VER SOBREPOSICAO
    return (botEsquerda < jogadorDireita &&
        botDireita > jogadorEsquerda &&
        botTopo < jogadorBaixo &&
        botBaixo > jogadorTopo);
}

void perseguir(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador, float velocidadeInimigo) {
    // CENTRO JOGADOR E BOT
    float jogadorCentroX = jogador->jogadorX + (larguraJogador / 2.0f);
    float jogadorCentroY = jogador->jogadorY + (alturaJogador / 2.0f);

    float botCentroX = bot->botX + (bot->larguraBot / 2.0f);
    float botCentroY = bot->botY + (bot->alturaBot / 2.0f);

    // GUARDA POSICOES DO BOT
    float novaPosicaoX = bot->botX;
    float novaPosicaoY = bot->botY;

    // EIXO X - CENTRO SEGUE O CENTRO
    if (botCentroX < jogadorCentroX) {
        float distancia = jogadorCentroX - botCentroX;
        if (distancia <= velocidadeInimigo) {
            novaPosicaoX = jogadorCentroX - (bot->larguraBot / 2.0f);
        }
        else {
            novaPosicaoX = bot->botX + velocidadeInimigo;
        }
    }
    else if (botCentroX > jogadorCentroX) {
        float distancia = botCentroX - jogadorCentroX;
        if (distancia <= velocidadeInimigo) {
            novaPosicaoX = jogadorCentroX - (bot->larguraBot / 2.0f);
        }
        else {
            novaPosicaoX = bot->botX - velocidadeInimigo;
        }
    }

    // EIXO Y
    if (botCentroY < jogadorCentroY) {
        float distancia = jogadorCentroY - botCentroY;
        if (distancia <= velocidadeInimigo) {
            novaPosicaoY = jogadorCentroY - (bot->alturaBot / 2.0f);
        }
        else {
            novaPosicaoY = bot->botY + velocidadeInimigo;
        }
    }
    else if (botCentroY > jogadorCentroY) {
        float distancia = botCentroY - jogadorCentroY;
        if (distancia <= velocidadeInimigo) {
            novaPosicaoY = jogadorCentroY - (bot->alturaBot / 2.0f);
        }
        else {
            novaPosicaoY = bot->botY - velocidadeInimigo;
        }
    }

    // TESTA MOVIMENTO EIXO X
    float posicaoXOriginal = bot->botX;
    bot->botX = novaPosicaoX;

    // COLIDIU? PARA O MOVIMENTO
    if (detectarColisao(bot, jogador, larguraJogador, alturaJogador)) {
        bot->botX = posicaoXOriginal;
    }

    // TESTA MOVIMENTO EIXO Y
    float posicaoYOriginal = bot->botY;
    bot->botY = novaPosicaoY;

    // COLIDIU? PARA O MOVIMENTO
    if (detectarColisao(bot, jogador, larguraJogador, alturaJogador)) {
        bot->botY = posicaoYOriginal;
    }
}