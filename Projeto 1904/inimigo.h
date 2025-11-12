#ifndef INIMIGO_H
#define INIMIGO_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "personagem.h"
#include "som.h"

#define MAX_INIMIGOS 20

typedef struct SistemaFases SistemaFases;

typedef enum {
    TIPO_ZUMBI,
    TIPO_RATO,
    TIPO_MOSQUITO,
    TIPO_BOSS,
    TIPO_BOSS_RATO,
    TIPO_BOSS_MOSQUITO
} TipoInimigo;

typedef struct Inimigo { //Aqui cria uma tag que indica pra outro include que essa struct existe
    float botX, botY, larguraBot, alturaBot;
    TipoInimigo tipo;
    bool ativo;
    int vida;
    int vidaMaxima; 
    // Variáveis de animação
    int frame_atual;
    int contador_frame;
    int frames_por_sprite;
    bool virado_direita;
    bool em_movimento;

    ALLEGRO_BITMAP* sprite_direita;
    ALLEGRO_BITMAP* sprite_esquerda;

    float velocidade;
    float velocidade_original; // Guarda velocidade original para buff

    // Variáveis de dano
    float dano;
    float dano_original; // Guarda dano original para buff
    float timer_intangibilidade;
  
    bool colidindo_caravana;      
    double timer_colisao_inicio;
    double ultimo_dano_aplicado; // Timer para dano contínuo na caravana
    bool somTocado; // Controla se o som do inimigo já foi tocado
    ALLEGRO_SAMPLE_ID somID; // ID do som em loop para controlar
    
    // Sistema de spawn de minions para bosses
    double timer_ultimo_spawn_minion;
    double intervalo_spawn_minion; // 3 segundos
    bool buff_metade_vida_aplicado; // Flag para aplicar buff uma vez
    bool eh_minion_de_boss; // Flag para diferenciar minions de inimigos normais
} Inimigo;

void desenhar_inimigo(Inimigo* inimigo, bool em_movimento);

void inicializar_inimigo(Inimigo* inimigo, TipoInimigo tipo, float x, float y, ALLEGRO_BITMAP* sprite_dir, ALLEGRO_BITMAP* sprite_esq, SistemaSom* sons);

// DE ACORDO COM A POSICAO DA CAMERA
void inicializar_array_inimigos(Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esquerda, float* posicaoCamera, SistemaSom* sons);

void desenhar_todos_inimigos(Inimigo* inimigos, int quantidade);

// ATUALIZA DE ACORDO COM A CAMERA
void atualizar_movimento_inimigos(Caravana* caravana, Inimigo* inimigos, int quantidade);

int contarInimigosAtivos(Inimigo* inimigos, int maxInimigos);

void aplicar_buffs_por_fase(Inimigo* inimigos, int quantidade, int faseAtual);

void spawnar_boss(Inimigo* inimigo, ALLEGRO_BITMAP* boss_dir, ALLEGRO_BITMAP* boss_esq, float* posicaoCamera, SistemaSom* sons);

void spawnar_boss_rato(Inimigo* inimigo, ALLEGRO_BITMAP* ratodir, ALLEGRO_BITMAP* ratoresq, float* posicaoCamera, SistemaSom* sons);

void spawnar_boss_mosquito(Inimigo* inimigo, ALLEGRO_BITMAP* mosquitodir, ALLEGRO_BITMAP* mosquitoesq, float* posicaoCamera, SistemaSom* sons);

void atualizar_boss_perseguindo(Inimigo* boss, const Jogador* jogador, float distanciaParada);

// Sistema de minions e buffs de boss
void atualizar_sistema_boss(Inimigo* boss, Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, const Jogador* jogador, SistemaSom* sons);

void desativar_minions_boss(Inimigo* inimigos, int quantidade);

void spawnar_minion_boss(Inimigo* boss, Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* sprite_dir, ALLEGRO_BITMAP* sprite_esquerda, TipoInimigo tipo_minion, const Jogador* jogador, SistemaSom* sons);

void verificar_buff_metade_vida(Inimigo* boss);

void atualizar_minions_perseguindo(Inimigo* inimigos, int quantidade, const Jogador* jogador);

void respawn_inimigo_na_camera(Inimigo* inimigo, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera, SistemaSom* sons);

void atualizar_timer_colisao_inimigos(Inimigo* inimigos, int quantidade, SistemaFases* sistemaFase);

// Toca o som do inimigo quando ele é ativado
void tocar_som_inimigo(Inimigo* inimigo, SistemaSom* sons);

// Para o som do inimigo quando ele é desativado
void parar_som_inimigo(Inimigo* inimigo);

void parar_todos_sons_inimigos(Inimigo* inimigos, int quantidade);

#endif