#include "personagem.h"
#include "projetil.h"

void mover(Jogador* p, bool w, bool a, bool s, bool d, bool shift, float velocidade, int* frames_por_sprite) {
    float velocidade_caminhada = velocidade;
    float velocidade_corrida = velocidade * 1.5;

    if (w) {
        if (shift) {
            p->jogadorY -= velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorY -= velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }

    if (a) {
        p->paraEsquerda = true;
        p->paraDireita = false;

        if (shift) {
            p->jogadorX -= velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorX -= velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }

    if (s) {
        if (shift) {
            p->jogadorY += velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorY += velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }

    if (d) {
        p->paraDireita = true;
        p->paraEsquerda = false;

        if (shift) {
            p->jogadorX += velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorX += velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }
}

void restringirPosicao(Jogador* p, Caravana* caravana, float maximoX, float maximoY, float larguraJogador, float alturaJogador) {
    if (p->jogadorX < caravana->caravanaX + caravana->caravanaLargura)
        p->jogadorX = caravana->caravanaX + caravana->caravanaLargura;
    if (p->jogadorY < 0)
        p->jogadorY = 0;

    if (p->jogadorY < (HEIGHT / 2) - alturaJogador + 52)
        p->jogadorY = (HEIGHT / 2) - alturaJogador + 52;
    if (p->jogadorY > HEIGHT - alturaJogador)
        p->jogadorY = HEIGHT - alturaJogador;
}

void desenhar_jogador(Jogador* jogador, bool w, bool a, bool s, bool d, bool espaco,SpritesJogador* sprites, int arma_equipada,int* frame_atual, int* contador_frame, int frames_por_sprite,bool* virado_direita, int* frame_tiro, int* contador_frame_tiro) {

    // Determina direção baseado no movimento
    if (d || (d && s) || (d && w)) {
        *virado_direita = true;
    }
    else if (a || (a && s) || (a && w)) {
        *virado_direita = false;
    }

    // Variáveis para armazenar os sprites a serem usados
    ALLEGRO_BITMAP* sprite_andando_dir = NULL;
    ALLEGRO_BITMAP* sprite_andando_esq = NULL;
    ALLEGRO_BITMAP* sprite_atacando_dir = NULL;
    ALLEGRO_BITMAP* sprite_atacando_esq = NULL;
    int frames_ataque = 3; 

    //VERIFICAR CADENCIA DE CADA ARMA E VELOCIDADE DE ANIMACAO
    float velocidade, cadencia;
    configuracoes_armas(arma_equipada, &velocidade, &cadencia);

    int velocidadeAnimacao;

    if (cadencia <= 0.2f) {
        velocidadeAnimacao = 4;
    }
    else if (cadencia <= 0.5f) {
        velocidadeAnimacao = 5;
    }
    else {
        velocidadeAnimacao = (int)(cadencia * 8.0f); 
    }

    // SELECIONA OS SPRITES COM BASE NA ARMA EQUIPADA
    switch (arma_equipada) {
    case ARMA_VASSOURA:
        sprite_andando_dir = sprites->sprite_andando_direita_vassoura;
        sprite_andando_esq = sprites->sprite_andando_esquerda_vassoura;
        sprite_atacando_dir = sprites->atacando_vassoura_direita;
        sprite_atacando_esq = sprites->atacando_vassoura_esquerda;
        frames_ataque = 3; 
        break;

    case ARMA_VENENO:
        sprite_andando_dir = sprites->sprite_andando_direita_veneno;
        sprite_andando_esq = sprites->sprite_andando_esquerda_veneno;
        sprite_atacando_dir = sprites->atacando_veneno_direita;
        sprite_atacando_esq = sprites->atacando_veneno_esquerda;
        frames_ataque = 3; 
        break;

    case ARMA_VACINA:
    default:
        // Vacina é a arma padrão
        sprite_andando_dir = sprites->sprite_andando_direita;
        sprite_andando_esq = sprites->sprite_andando_esquerda;
        sprite_atacando_dir = sprites->sprite_atirando_direita;
        sprite_atacando_esq = sprites->sprite_atirando_esquerda;
        frames_ataque = 3; 
        break;
    }

    // ESCOLHER SPRITE E SISTEMA DE ANIMAÇÃO
    ALLEGRO_BITMAP* sprite_atual;
    bool esta_atirando = espaco;
    bool esta_em_movimento = (a || d || s || w);

    if (esta_atirando) {
        
        if (*virado_direita) {
            sprite_atual = sprite_atacando_dir;
        }
        else {
            sprite_atual = sprite_atacando_esq;
        }

        // ANIMAÇÃO DE ATAQUE - SINCRONIZADA COM CADÊNCIA
       
        (*contador_frame_tiro)++;
        if (*contador_frame_tiro >= velocidadeAnimacao) {  
            *frame_tiro = (*frame_tiro + 1) % frames_ataque; 
            *contador_frame_tiro = 0;
        }

        // Calcula coordenadas
        int largura_frame = al_get_bitmap_width(sprite_atual) / frames_ataque;
        int altura_frame = al_get_bitmap_height(sprite_atual);
        int sx = *frame_tiro * largura_frame;
        int sy = 0;

        al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame,
            jogador->jogadorX, jogador->jogadorY, 0);
    }
    else {
        
        if (*virado_direita) {
            sprite_atual = sprite_andando_dir;
        }
        else {
            sprite_atual = sprite_andando_esq;
        }

        *frame_tiro = 0;
        *contador_frame_tiro = 0;

        // ANIMAÇÃO NORMAL DE CAMINHADA (2 colunas)
        if (esta_em_movimento) {
            (*contador_frame)++;
            if (*contador_frame >= frames_por_sprite) {
                *frame_atual = (*frame_atual + 1) % 2;
                *contador_frame = 0;
            }
        }
        else {
            // Parado - primeiro frame
            *frame_atual = 0;
            *contador_frame = 1;
        }

        // Calcula coordenadas usando 2 colunas para caminhada
        int largura_frame = al_get_bitmap_width(sprite_atual) / 2; // 2 colunas para caminhada
        int altura_frame = al_get_bitmap_height(sprite_atual);
        int sx = *frame_atual * largura_frame;
        int sy = 0;

        al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame,jogador->jogadorX, jogador->jogadorY, 0);
    }
}

void camera_jogador(float* posicaoCamera, Jogador jogador, int larguraTela, int larguraJogador, int alturaJogador, float caravanaX, float caravanaVelocidade) {
    posicaoCamera[0] = jogador.jogadorX - (larguraTela / 2) + (larguraJogador / 2);

    if (posicaoCamera[0] < caravanaX + caravanaVelocidade)
        posicaoCamera[0] = caravanaX + caravanaVelocidade;
}
