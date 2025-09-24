#include "personagem.h"

void mover(Jogador* p, bool w, bool a, bool s, bool d, float velocidade) {
    if (w)
        p-> jogadorY -= velocidade;
    if (a)  {
        p-> jogadorX -= velocidade;
        p-> paraEsquerda = true;
        p-> paraDireita = false;
    }
    if (s)  
        p-> jogadorY += velocidade;
    if (d){
        p-> jogadorX += velocidade;
        p-> paraDireita = true;
        p-> paraEsquerda = false;
    }
        
}

void restringirPosicao(Jogador* p, float WIDTH, float HEIGHT, float larguraJogador, float alturaJogador) {
    if (p->jogadorX < 0) p->jogadorX = 0;
    if (p->jogadorY < 0) p->jogadorY = 0;
    if (p->jogadorX > WIDTH - larguraJogador)  p->jogadorX = WIDTH - larguraJogador;
    if (p->jogadorY > HEIGHT - alturaJogador)   p->jogadorY = HEIGHT - alturaJogador;

}