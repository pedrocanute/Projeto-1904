#ifndef JOGO_H
#define JOGO_H

#include "configuracoes.h"
#include "personagem.h"
#include "inimigo.h"
#include "projetil.h"
#include "caravana.h"
#include "infeccao.h"
#include "fases.h"
#include "menu.h"
#include "dialogos.h"
#include <allegro5/allegro.h>

// Estrutura para câmera e cores
typedef struct {
    float posicaoCamera[2];
    ALLEGRO_COLOR cor;
    ALLEGRO_COLOR corCaravana;
    ALLEGRO_TRANSFORM* camera;
} JogoCamera;

// Estrutura para entidades do jogo
typedef struct {
    Jogador jogador;
    SpritesJogador spritesJogador;
    ProjetilPosicao projetil;
    Caravana caravana;
    Inimigo inimigos[MAX_INIMIGOS];
    SistemaFases sistemaFase;
} JogoEntidades;

// Estrutura para barras de UI
typedef struct {
    Barra barraFundo;
    Barra barraInfeccao;
    BarraBoss barraVidaBossFundo;
    BarraBoss barraVidaBoss;
} JogoBarras;

// Estrutura para variáveis de controle
typedef struct {
    float timer_regen_infeccao;
    float TEMPO_REGEN_INFECCAO;
    float TEMPO_INTANGIBILIDADE;
    float timer_spawn_inimigos;
    bool spawn_ativo;
    float TEMPO_SPAWN;
    bool boss_spawnado;
    bool fase_boss_ativa;

    // SPAWN CADENCIADO
    int inimigos_spawnados;
    float timer_spawn_individual;
    float INTERVALO_SPAWN_INDIVIDUAL;

    bool mostrar_dialogo_transicao;
    bool cutscene_concluida;   //Flag para controlar início do spawn
    bool game_over_processado; 
} JogoControle;

// Estrutura para animação
typedef struct {
    int frame_atual;
    int contador_frame;
    int frames_por_sprite;
    bool virado_direita;
    int frame_tiro;
    int contador_frame_tiro;
} JogoAnimacao;

// Estrutura para resultado de colisão
typedef struct {
    bool ocorreuColisao;
    ALLEGRO_COLOR corCaravana;
} ResultadoColisao;

// Funções de inicialização
void inicializarJogoCamera(JogoCamera* camera, ALLEGRO_TRANSFORM* transform);
void inicializarJogoEntidades(JogoEntidades* entidades, Bitmaps* bitmap, float* posicaoCamera, SistemaSom* sons);
void inicializarJogoBarras(JogoBarras* barras);
void inicializarJogoControle(JogoControle* controle);
void inicializarJogoAnimacao(JogoAnimacao* animacao);

// Funções de atualização de boss
void atualizarBossAtivo(JogoEntidades* entidades, JogoBarras* barras, Bitmaps* bitmap, SistemaSom* sons);
void posicionarBarraVidaBoss(JogoBarras* barras, Inimigo* boss);

// Funções de colisão e dano
ResultadoColisao processarColisoes(JogoEntidades* entidades, JogoBarras* barras, JogoControle* controle, JogoCamera* jogoCamera);
void processarRegeneracaoVida(JogoBarras* barras, JogoControle* controle, bool colisaoOcorreu);

// Funções de spawn e fases
void processarSpawnInimigos(JogoEntidades* entidades, JogoControle* controle, Bitmaps* bitmap, float* posicaoCamera, SistemaSom* sons);
void verificarMorteBoss(JogoEntidades* entidades, JogoControle* controle);

// Função de verificação de game over e reinício
bool verificarGameOver(JogoBarras* barras, JogoEntidades* entidades, JogoCamera* jogoCamera, JogoControle* controle, JogoAnimacao* animacao, GameOver* gameOver, MenuEvents* menuEvent, MenuEstados* menuEstado, Bitmaps* bitmap, SistemaSom* sons, ALLEGRO_FONT* fonteDialogo, ResultadoColisao* resultadoColisao, bool* redesenhar, bool* w, bool* a, bool* s, bool* d, bool* espaco, bool* shift, bool* esc, bool* seta_cima, bool* seta_direita, bool* seta_baixo, bool* seta_esquerda, Dialogo* dialogo);

#endif