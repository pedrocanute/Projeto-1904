#include "personagem.h"
#include "projetil.h"
#include "configuracoes.h"

void mover(Jogador* p, bool w, bool a, bool s, bool d, bool shift, bool seta_cima, bool seta_direita, bool seta_baixo, bool seta_esquerda, float velocidade, int* frames_por_sprite) {
    float velocidade_caminhada = velocidade;
    float velocidade_corrida = velocidade * 1.5;

    if (w || seta_cima) {
        if (shift) {
            p->jogadorY -= velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorY -= velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }

    if (a || seta_esquerda) {
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

    if (s || seta_baixo) {
        if (shift) {
            p->jogadorY += velocidade_corrida;
            *frames_por_sprite = 4;
        }
        else {
            p->jogadorY += velocidade_caminhada;
            *frames_por_sprite = 11;
        }
    }

    if (d || seta_direita) {
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
    
    const float LIMITE_TELAS = 2.f;
    const float LARGURA_TELA = 1280.f;

    if (p->jogadorX < caravana->caravanaX + caravana->caravanaLargura)
        p->jogadorX = caravana->caravanaX + caravana->caravanaLargura;

    float limite_direita = caravana->caravanaX + (LIMITE_TELAS * LARGURA_TELA);
    if (p->jogadorX + larguraJogador > limite_direita)
        p->jogadorX = limite_direita - larguraJogador;

    // Restricoes verticais
    
    if (p->jogadorY < 0)
        p->jogadorY = 0;

    if (p->jogadorY < (HEIGHT / 2) - alturaJogador + 52)
        p->jogadorY = (HEIGHT / 2) - alturaJogador + 52;
    if (p->jogadorY > HEIGHT - alturaJogador)
        p->jogadorY = HEIGHT - alturaJogador;
}

void desenhar_jogador(Jogador* jogador, bool w, bool a, bool s, bool d, bool espaco, bool seta_cima, bool seta_direita, bool seta_baixo, bool seta_esquerda, SpritesJogador* sprites, int arma_equipada, int* frame_atual, int* contador_frame, int frames_por_sprite, bool* virado_direita, int* frame_tiro, int* contador_frame_tiro, SistemaSom* sons) {

    // Determina direção baseado no movimento
    if (d || (d && s) || (d && w) || (seta_direita || (seta_direita && seta_baixo))) {
        *virado_direita = true;
    }
    else if (a || (a && s) || (a && w) || (seta_esquerda || (seta_esquerda && seta_cima))) {
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
        velocidadeAnimacao = cadencia * 8.0f;
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
    bool esta_em_movimento = (a || d || s || w || seta_cima || seta_direita || seta_baixo || seta_esquerda);

    // VERIFICA SE DEVE APLICAR EFEITO VISUAL DE DANO
    const float DURACAO_EFEITO_VISUAL = 0.15f;
    bool aplicar_efeito_dano = (al_get_time() - jogador->timer_dano_visual) < DURACAO_EFEITO_VISUAL;
    ALLEGRO_COLOR tint_cor = aplicar_efeito_dano ? al_map_rgba_f(1.0f, 0.3f, 0.3f, 1.0f) : al_map_rgb(255, 255, 255);

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

        // Som da vassoura sincronizado com o frame 1 da animação de ataque
        if (arma_equipada == ARMA_VASSOURA) {
            if (*frame_tiro == 1 && !jogador->somVassouraTocado) {
                tocarSomVassoura(sons);
                jogador->somVassouraTocado = true;
            }
            else if (*frame_tiro == 0) {
                jogador->somVassouraTocado = false;
            }
        }

        // Som do veneno sincronizado com o frame 1 da animação de ataque
        if (arma_equipada == ARMA_VENENO) {
            if (*frame_tiro == 1 && !jogador->somVenenoTocado) {
                tocarSomVeneno(sons);
                jogador->somVenenoTocado = true;
            }
            else if (*frame_tiro == 0) {
                jogador->somVenenoTocado = false;
            }
        }

        // Calcula coordenadas
        int largura_frame = al_get_bitmap_width(sprite_atual) / frames_ataque;
        int altura_frame = al_get_bitmap_height(sprite_atual);
        int sx = *frame_tiro * largura_frame;
        int sy = 0;

        // DESENHA COM TINT (VERMELHO SE LEVOU DANO)
        al_draw_tinted_bitmap_region(sprite_atual, tint_cor, sx, sy, largura_frame, altura_frame, jogador->jogadorX, jogador->jogadorY, 0);
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
        jogador->somVassouraTocado = false; // Reseta quando não está atacando
        jogador->somVenenoTocado = false; // Reseta quando não está atacando

        // ANIMAÇÃO NORMAL DE CAMINHADA (2 colunas)
        if (esta_em_movimento) {
            (*contador_frame)++;
            if (*contador_frame >= frames_por_sprite) {
                *frame_atual = (*frame_atual + 1) % 2;
                *contador_frame = 0;
            }

            // Som toca APENAS quando está no frame 1 E em movimento
            if (*frame_atual == 1 && !jogador->somPassoTocado) {
                tocarSomAndando(sons);
                jogador->somPassoTocado = true;
            }
            else if (*frame_atual == 0) {
                jogador->somPassoTocado = false;
            }
        }
        else {
            // Parado - primeiro frame e reseta tudo
            *frame_atual = 0;
            *contador_frame = 0;
            jogador->somPassoTocado = false; // Reseta a flag quando parado
        }

        // Calcula coordenadas usando 2 colunas para caminhada
        int largura_frame = al_get_bitmap_width(sprite_atual) / 2; // 2 colunas para caminhada
        int altura_frame = al_get_bitmap_height(sprite_atual);
        int sx = *frame_atual * largura_frame;
        int sy = 0;

        // VERMELHO SE LEVOU DANO
        al_draw_tinted_bitmap_region(sprite_atual, tint_cor, sx, sy, largura_frame, altura_frame,
            jogador->jogadorX, jogador->jogadorY, 0);
    }
}

void camera_jogador(float* posicaoCamera, Jogador jogador, int larguraTela, int larguraJogador, int alturaJogador, float caravanaX, float caravanaVelocidade) {
    const float LIMITE_TELAS = 2.0f;
    const float LARGURA_TELA = 1280.0f;

    // Centraliza câmera no jogador
    posicaoCamera[0] = jogador.jogadorX - (larguraTela / 2) + (larguraJogador / 2);

    // Limite esquerdo: não pode voltar além da caravana
    if (posicaoCamera[0] < caravanaX + caravanaVelocidade)
        posicaoCamera[0] = caravanaX + caravanaVelocidade;

    // Limite direito: câmera não pode ultrapassar 2 telas à frente da caravana
    float limite_direita_camera = caravanaX + (LIMITE_TELAS * LARGURA_TELA);
    if (posicaoCamera[0] + larguraTela > limite_direita_camera) {
        posicaoCamera[0] = limite_direita_camera - larguraTela;
    }
}