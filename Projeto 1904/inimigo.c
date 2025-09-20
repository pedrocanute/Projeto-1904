#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inimigo.h"
#include "personagem.h"

// FUNCAO COLISAO
bool detectarColisao(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador) {
    float botEsquerda = bot->posicaoX;
    float botDireita = bot->posicaoX + bot->largura;
    float botTopo = bot->posicaoY;
    float botBaixo = bot->posicaoY + bot->altura;

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

    float botCentroX = bot->posicaoX + (bot->largura / 2.0f);
    float botCentroY = bot->posicaoY + (bot->altura / 2.0f);

    // GUARDA POSICOES DO BOT
    float novaPosicaoX = bot->posicaoX;
    float novaPosicaoY = bot->posicaoY;

    // EIXO X - CENTRO SEGUE O CENTRO
    if (botCentroX < jogadorCentroX) {
        float distancia = jogadorCentroX - botCentroX;
        if (distancia <= velocidadeInimigo) {
            novaPosicaoX = jogadorCentroX - (bot->largura / 2.0f);
        }
        else {
            novaPosicaoX = bot->posicaoX + velocidadeInimigo;
        }
    }
    else if (botCentroX > jogadorCentroX) {
        float distancia = botCentroX - jogadorCentroX;
        if (distancia <= velocidadeInimigo) {
            novaPosicaoX = jogadorCentroX - (bot->largura / 2.0f);
        }
        else {
            novaPosicaoX = bot->posicaoX - velocidadeInimigo;
        }
    }

    // EIXO Y
    if (botCentroY < jogadorCentroY) {
        float distancia = jogadorCentroY - botCentroY;
        if (distancia <= velocidadeInimigo) {
            novaPosicaoY = jogadorCentroY - (bot->altura / 2.0f);
        }
        else {
            novaPosicaoY = bot->posicaoY + velocidadeInimigo;
        }
    }
    else if (botCentroY > jogadorCentroY) {
        float distancia = botCentroY - jogadorCentroY;
        if (distancia <= velocidadeInimigo) {
            novaPosicaoY = jogadorCentroY - (bot->altura / 2.0f);
        }
        else {
            novaPosicaoY = bot->posicaoY - velocidadeInimigo;
        }
    }

    // TESTA MOVIMENTO EIXO X
    float posicaoXOriginal = bot->posicaoX;
    bot->posicaoX = novaPosicaoX;

    // COLIDIU? PARA O MOVIMENTO
    if (detectarColisao(bot, jogador, larguraJogador, alturaJogador)) {
        bot->posicaoX = posicaoXOriginal;
    }

    // TESTA MOVIMENTO EIXO Y
    float posicaoYOriginal = bot->posicaoY;
    bot->posicaoY = novaPosicaoY;

    // COLIDIU? PARA O MOVIMENTO
    if (detectarColisao(bot, jogador, larguraJogador, alturaJogador)) {
        bot->posicaoY = posicaoYOriginal;
    }
}