#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inimigo.h"
#include "personagem.h"

void perseguir(Inimigo* bot, Jogador* jogador, float larguraJogador, float alturaJogador, float velocidadeInimigo) {
    float jogadorEsquerda = jogador->jogadorX;
    float jogadorDireita = jogador->jogadorX + larguraJogador;
    float jogadorTopo = jogador->jogadorY;
    float jogadorBaixo = jogador->jogadorY + alturaJogador;

    // Movimento horizontal
    if (bot->posicaoX < jogadorEsquerda) {
        // Bot à esquerda - sua borda DIREITA deve parar na borda ESQUERDA do jogador
        float bordaDireitaBot = bot->posicaoX + bot->largura;
        float distancia = jogadorEsquerda - bordaDireitaBot;

        if (distancia <= velocidadeInimigo) {
            // Posiciona a borda direita do bot exatamente na borda esquerda do jogador
            bot->posicaoX = jogadorEsquerda - bot->largura;
        }
        else {
            // Move normalmente
            bot->posicaoX += velocidadeInimigo;
        }
    }
    else if (bot->posicaoX > jogadorDireita) {
        // Bot à direita - sua borda ESQUERDA deve parar na borda DIREITA do jogador
        float distancia = bot->posicaoX - jogadorDireita;

        if (distancia <= velocidadeInimigo) {
            // Posiciona a borda esquerda do bot exatamente na borda direita do jogador
            bot->posicaoX = jogadorDireita;
        }
        else {
            // Move normalmente
            bot->posicaoX -= velocidadeInimigo;
        }
    }


    // Movimento vertical
    if (bot->posicaoY < jogadorTopo) {
        // Bot acima - move para baixo até a borda
        float distancia = jogadorTopo - bot->posicaoY;
        if (distancia <= velocidadeInimigo) {
            // Se pode chegar na borda neste frame, vai direto para a borda
            bot->posicaoY = jogadorTopo;
        }
        else {
            // Senão, move a velocidade normal
            bot->posicaoY += velocidadeInimigo;
        }
    }
    else if (bot->posicaoY > jogadorBaixo) {
        // Bot abaixo - move para cima até a borda
        float distancia = bot->posicaoY - jogadorBaixo;
        if (distancia <= velocidadeInimigo) {
            // Se pode chegar na borda neste frame, vai direto para a borda
            bot->posicaoY = jogadorBaixo;
        }
        else {
            // Senão, move a velocidade normal
            bot->posicaoY -= velocidadeInimigo;
        }
    }
}
