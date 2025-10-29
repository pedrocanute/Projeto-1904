#ifndef JOGO_H
#define JOGO_H

#include "configuracoes.h"
#include "personagem.h"
#include "inimigo.h"
#include "projetil.h"
#include "caravana.h"
#include "infeccao.h"
#include "fases.h"
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
    const float TEMPO_REGEN_INFECCAO;
    const float TEMPO_INTANGIBILIDADE;
    float timer_spawn_inimigos;
    bool spawn_ativo;
    const float TEMPO_SPAWN;
    bool boss_spawnado;
    bool fase_boss_ativa;
    bool mostrar_dialogo_transicao;  // NOVO
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
void inicializarJogoEntidades(JogoEntidades* entidades, Bitmaps* bitmap, float* posicaoCamera);
void inicializarJogoBarras(JogoBarras* barras);
void inicializarJogoControle(JogoControle* controle);
void inicializarJogoAnimacao(JogoAnimacao* animacao);

// Funções de atualização de boss
void atualizarBossAtivo(JogoEntidades* entidades, JogoBarras* barras);
void posicionarBarraVidaBoss(JogoBarras* barras, Inimigo* boss);

// Funções de colisão e dano
ResultadoColisao processarColisoes(JogoEntidades* entidades, JogoBarras* barras, JogoControle* controle, JogoCamera* jogoCamera);
void processarRegeneracaoVida(JogoBarras* barras, JogoControle* controle, bool colisaoOcorreu);

// Funções de spawn e fases
void processarSpawnInimigos(JogoEntidades* entidades, JogoControle* controle, Bitmaps* bitmap, float* posicaoCamera);
void verificarMorteBoss(JogoEntidades* entidades, JogoControle* controle);

#endif