#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "inimigo.h"
#include "personagem.h"
#include "fases.h"

void inicializar_inimigo(Inimigo* inimigo, TipoInimigo tipo, float x, float y, ALLEGRO_BITMAP* sprite_dir, ALLEGRO_BITMAP* sprite_esq) {

    inimigo->tipo = tipo;
    inimigo->botX = x;
    inimigo->botY = y;
    inimigo->sprite_direita = sprite_dir;
    inimigo->sprite_esquerda = sprite_esq;
    inimigo->frame_atual = 0;
    inimigo->contador_frame = 0;
    inimigo->frames_por_sprite = 11;
    inimigo->virado_direita = false;
    inimigo->em_movimento = true;
    inimigo->ativo = false;
    inimigo->vida = 1;
    inimigo->vidaMaxima = 1;  
    inimigo->timer_intangibilidade = 0.0f;

    // ATRIBUTOS DOS INIMIGOS
    switch (tipo) {
    case TIPO_ZUMBI:
        inimigo->larguraBot = 60.0f;
        inimigo->alturaBot = 80.0f;
        inimigo->velocidade = 2.5f;
        inimigo->dano = 5.0f;
        inimigo->vida = 1;
        inimigo->vidaMaxima = 1;
        break;
    case TIPO_RATO:
        inimigo->larguraBot = 60.0f;
        inimigo->alturaBot = 50.0f;
        inimigo->velocidade = 2.8f;
        inimigo->dano = 3.0f;
        inimigo->vida = 1;
        inimigo->vidaMaxima = 1;
        break;
    case TIPO_MOSQUITO:
        inimigo->larguraBot = 80.0f;
        inimigo->alturaBot = 80.0f;
        inimigo->frames_por_sprite = 6;
        inimigo->velocidade = 2.9f;
        inimigo->dano = 2.0f;
        inimigo->vida = 1;
        inimigo->vidaMaxima = 1;
        break;
    case TIPO_BOSS:
        inimigo->larguraBot = 220.0f;
        inimigo->alturaBot = 220.0f;
        inimigo->frames_por_sprite = 15;
        inimigo->velocidade = 2.0f;
        inimigo->vida = 50;
        inimigo->vidaMaxima = 50;  
        inimigo->dano = 10;
        break;
    case TIPO_BOSS_RATO:
        inimigo->larguraBot = 180.0f;  
        inimigo->alturaBot = 150.0f;   
        inimigo->frames_por_sprite = 11;  
        inimigo->velocidade = 1.5f;     
        inimigo->vida = 30;             
        inimigo->vidaMaxima = 30;       
        inimigo->dano = 15;             
        break;

    }
}

// SPAWNA INIMIGOS COM BASE NA CAMERA
void respawn_inimigo_na_camera(Inimigo* inimigo, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera) {

    // Largura da tela (campo de visão do jogador)
    const float LARGURA_TELA = 1280.0f;
    
    // Calcula onde termina o campo de visão da câmera (baseado na posição X atual da câmera)
    float limite_direito_camera = posicaoCamera[0] + LARGURA_TELA;
    
    // Spawna SEMPRE fora do campo de visão (além do limite direito)
    
    float spawn_x_min = limite_direito_camera + 50.0f;   // Mínimo 50px fora da visão
    float spawn_x_max = limite_direito_camera + 300.0f;  // Máximo 300px fora da visão

    // Define área vertical de spawn
    float spawn_y_min = (720.0f / 2.0f) + 52.0f;
    float spawn_y_max = 720.0f - 100.0f;

    // Gera posição aleatória dentro dos limites
    float x = spawn_x_min + rand() % (int)(spawn_x_max - spawn_x_min + 1);
    float y = spawn_y_min + rand() % (int)(spawn_y_max - spawn_y_min + 1);

    TipoInimigo tipo = (TipoInimigo)(rand() % 3);

    switch (tipo) {
    case TIPO_ZUMBI:
        inicializar_inimigo(inimigo, TIPO_ZUMBI, x, y, zumbi_dir, zumbi_esq);
        break;
    case TIPO_RATO:
        inicializar_inimigo(inimigo, TIPO_RATO, x, y, rato_dir, rato_esq);
        break;
    case TIPO_MOSQUITO:
        inicializar_inimigo(inimigo, TIPO_MOSQUITO, x, y, mosquito_dir, mosquito_esq);
        break;
    }
}

void inicializar_array_inimigos(Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera) {
    srand(time(NULL));
    for (int i = 0; i < quantidade; i++) {
        respawn_inimigo_na_camera(&inimigos[i], zumbi_dir, zumbi_esq, rato_dir, rato_esq, mosquito_dir, mosquito_esq, posicaoCamera);
    }
}

void atualizar_movimento_inimigos(Caravana* caravana,Inimigo* inimigos, int quantidade) {

    for (int i = 0; i < quantidade; i++) {
        if (inimigos[i].ativo == false)
            continue;
        if (inimigos[i].tipo == TIPO_BOSS || inimigos[i].tipo == TIPO_BOSS_RATO)
            continue;

        inimigos[i].botX -= inimigos[i].velocidade;

        if (inimigos[i].botX < caravana->caravanaX) {
            inimigos[i].botX = caravana->caravanaX;
        }
    }
}

void desenhar_inimigo(Inimigo* inimigo, bool em_movimento) {
    if (inimigo->ativo == false) return;

    // Escolhe sprite pela orientacao
    ALLEGRO_BITMAP* sprite_atual = inimigo->virado_direita ? inimigo->sprite_direita : inimigo->sprite_esquerda;

    int num_colunas;
    if (inimigo->tipo == TIPO_BOSS) { //Verifica se o inimigo eh o Boss ou inimigo normal (Boss tem 3 colunas na spritesheet ao inves de 2)
        num_colunas = 3;
    }
    else {
        num_colunas = 2;
    }

    if (em_movimento) {
        inimigo->contador_frame++;
        if (inimigo->contador_frame >= inimigo->frames_por_sprite) {
            inimigo->frame_atual = (inimigo->frame_atual + 1) % num_colunas;
            inimigo->contador_frame = 0;
        }
    }

    int largura_total = al_get_bitmap_width(sprite_atual);
    int altura_frame = al_get_bitmap_height(sprite_atual);
    int largura_frame = largura_total / num_colunas;

    int sx = inimigo->frame_atual * largura_frame;
    int sy = 0;

    if (inimigo->tipo == TIPO_BOSS_RATO) {
        al_draw_scaled_bitmap(sprite_atual, sx, sy, largura_frame, altura_frame, inimigo->botX, inimigo->botY, largura_frame * 3, altura_frame * 3, 0);
    }
    else {

        al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame,inimigo->botX, inimigo->botY, 0);
    }

}


void desenhar_todos_inimigos(Inimigo* inimigos, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        desenhar_inimigo(&inimigos[i], inimigos[i].em_movimento);
    }
}

int contarInimigosAtivos(Inimigo* inimigos, int maxInimigos) {
    int inimigosAtivos = 0;
    for (int i = 0; i < maxInimigos; i++) {
        if (inimigos[i].ativo)
            inimigosAtivos++;
    }
    return inimigosAtivos;
}

void aplicar_buffs_por_fase(Inimigo* inimigos, int quantidade, int faseAtual) {
    for (int i = 0; i < quantidade; i++) {
        
        // Só aplica buffs em inimigos normais (não bosses)
        if (inimigos[i].tipo == TIPO_BOSS || inimigos[i].tipo == TIPO_BOSS_RATO) {
            continue;
        }

        // Buffs progressivos baseados na fase
        // Fase 1: sem buff adicional (valores base)
        // Fase 2: +1 de multiplier
        // Fase 3: +2 de multiplier
        float multiplicador = faseAtual;
        
        // Aumenta vida máxima
        inimigos[i].vidaMaxima = inimigos[i].vidaMaxima * multiplicador;
        if (inimigos[i].vidaMaxima < 1) inimigos[i].vidaMaxima = 1;
        inimigos[i].vida = inimigos[i].vidaMaxima;
        
        // Aumenta dano (menos agressivo que vida)
        inimigos[i].dano += (faseAtual - 1) * 2.0f;
        
        // Aumenta velocidade levemente
        inimigos[i].velocidade += (faseAtual - 1) * 0.3f;
    }
}

void spawnar_boss(Inimigo* inimigo, ALLEGRO_BITMAP* boss_dir, ALLEGRO_BITMAP* boss_esq, float* posicaoCamera) {

    // Boss spawna mais a frente da camera
    float camera_direita = posicaoCamera[0] + 1280;
    float spawn_x = camera_direita + 500;  // Mais distante que inimigos normais

    // Boss spawna no centro vertical da tela
    float spawn_y = (720 / 2) + 52;

    inicializar_inimigo(inimigo, TIPO_BOSS, spawn_x, spawn_y, boss_dir, boss_esq);
    inimigo->ativo = true;
}

void spawnar_boss_rato(Inimigo* inimigo, ALLEGRO_BITMAP* ratodir, ALLEGRO_BITMAP* ratoesq, float* posicaoCamera) {
    float camera_direita = posicaoCamera[0] + 1280;
    float spawnX = camera_direita + 500;
    float spawnY = (720 / 2) - 100;

    inicializar_inimigo(inimigo, TIPO_BOSS_RATO, spawnX, spawnY, ratodir, ratoesq);
    inimigo->ativo = true;
}

void atualizar_boss_perseguindo(Inimigo* boss, const Jogador* jogador, float distanciaParada) {
    if (!boss || !boss->ativo || (boss->tipo != TIPO_BOSS && boss->tipo != TIPO_BOSS_RATO)) return;

    if (boss->botY + boss->alturaBot >= 720)
        boss->botY = 720 - boss->alturaBot;

    // Calcula distância entre as bordas das hitboxes, não os centros
    float distanciaX = jogador->jogadorX - boss->botX;
    float distanciaY = jogador->jogadorY - boss->botY;

    // Vetor de distancia entre centros (para direção do movimento)
    float centroJogadorX = jogador->jogadorX + (80.0f / 2.0f);
    float centroJogadorY = jogador->jogadorY + (96.0f / 2.0f);
    
    float centroBossX = boss->botX + (boss->larguraBot / 2.0f);
    float centroBossY = boss->botY + (boss->alturaBot / 2.0f);

    float distanciaCentroX = centroJogadorX - centroBossX;
    float distanciaCentroY = centroJogadorY - centroBossY;
    float distanciaTotal = sqrtf(distanciaCentroX * distanciaCentroX + distanciaCentroY * distanciaCentroY);

    // Verifica se as hitboxes já estão sobrepostas ou muito próximas
    
    bool sobreposX = (boss->botX < jogador->jogadorX + 80.0f) && 
                    (boss->botX + boss->larguraBot > jogador->jogadorX);
    
    bool sobreposY = (boss->botY < jogador->jogadorY + 96.0f) && 
                    (boss->botY + boss->alturaBot > jogador->jogadorY);

    // Se já está colidindo ou muito próximo, para de se mover
    if ((sobreposX && sobreposY) || distanciaTotal < 30.0f) {
        boss->em_movimento = false;
        return;
    }

    // Normaliza direcao (evita divisao por zero)
    float direcaoX = 0.0f;
    float direcaoY = 0.0f;
    if (distanciaTotal > 0.0001f) {
        direcaoX = distanciaCentroX / distanciaTotal;
        direcaoY = distanciaCentroY / distanciaTotal;
    }

    // Move em direcao ao jogador
    float velocidadeAtual = boss->velocidade;
    boss->botX += direcaoX * velocidadeAtual;
    boss->botY += direcaoY * velocidadeAtual;

    // Atualiza estado para animacao e orientacao do sprite
    boss->em_movimento = true;
    boss->virado_direita = (direcaoX >= 0.0f);

}