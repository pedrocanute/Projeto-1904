#include "personagens.h"

void mover(Jogador* p, bool w, bool a, bool s, bool d, float velocidade) {
    if (w)
        p-> posicaoYp -= velocidade;
    if (a)  
        p-> posicaoXp -= velocidade;
    if (s)  
        p-> posicaoYp += velocidade;
    if (d) 
        p-> posicaoXp += velocidade;
}

void restringirPosicao(Jogador* p, float maximoX, float maximoY, float larguraJogador, float alturaJogador) {
    if (p-> posicaoXp < 0) p-> posicaoXp = 0;
    if (p-> posicaoYp < 0) p-> posicaoYp = 0;
    if (p-> posicaoXp > maximoX - larguraJogador)  p-> posicaoXp = maximoX - larguraJogador;
    if (p-> posicaoYp > maximoY - alturaJogador)   p-> posicaoYp = maximoY - alturaJogador;

}