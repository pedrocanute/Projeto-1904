#include "movimentacao.h"

void mover(Posicao* p, bool w, bool a, bool s, bool d, float velocidade) {
    if (w)
        p-> jogadorY -= velocidade;
    if (a)  
        p-> jogadorX -= velocidade;
    if (s)  
        p-> jogadorY += velocidade;
    if (d) 
        p-> jogadorX += velocidade;
}

void restringirPosicao(Posicao* p, float maximoX, float maximoY, float larguraJogador, float alturaJogador) {
    if (p-> jogadorX < 0) p-> jogadorX = 0;
    if (p-> jogadorY < 0) p-> jogadorY = 0;
    if (p-> jogadorX > maximoX - larguraJogador)  p-> jogadorX = maximoX - larguraJogador;
    if (p-> jogadorY > maximoY - alturaJogador)   p-> jogadorY = maximoY - alturaJogador;

}