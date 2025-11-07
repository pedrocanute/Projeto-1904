#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include "inimigo.h"
#include "personagem.h"
#include "fases.h"

void inicializar_inimigo(Inimigo* inimigo, TipoInimigo tipo, float x, float y, ALLEGRO_BITMAP* sprite_dir, ALLEGRO_BITMAP* sprite_esq, SistemaSom* sons) {
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
    inimigo->somTocado = false;
    inimigo->somID._id = -1;
    inimigo->somID._index = -1;

    inimigo->colidindo_caravana = false;
    inimigo->timer_colisao_inicio = 0.0;
    inimigo->ultimo_dano_aplicado = 0.0; // Inicializa timer de dano contínuo

    // Inicializa sistema de minions e buffs
    inimigo->timer_ultimo_spawn_minion = 0.0;
    inimigo->intervalo_spawn_minion = 3.0; // 3 segundos
    inimigo->buff_metade_vida_aplicado = false;
    inimigo->eh_minion_de_boss = false;

    // ATRIBUTOS DOS INIMIGOS
    switch (tipo) {
    case TIPO_ZUMBI:
        inimigo->larguraBot = 60.0f;
        inimigo->alturaBot = 80.0f;
        inimigo->velocidade = 2.5f;
        inimigo->dano = 6.0f;
        inimigo->vida = 1;
        inimigo->vidaMaxima = 1;
        break;
    case TIPO_RATO:
        inimigo->larguraBot = 60.0f;
        inimigo->alturaBot = 50.0f;
        inimigo->velocidade = 2.8f;
        inimigo->dano = 4.f;
        inimigo->vida = 1;
        inimigo->vidaMaxima = 1;
        break;
    case TIPO_MOSQUITO:
        inimigo->larguraBot = 80.0f;
        inimigo->alturaBot = 80.0f;
        inimigo->frames_por_sprite = 6;
        inimigo->velocidade = 2.9f;
        inimigo->dano = 5.f;
        inimigo->vida = 1;
        inimigo->vidaMaxima = 1;

        break;
    case TIPO_BOSS:
        inimigo->larguraBot = 220.0f;
        inimigo->alturaBot = 220.0f;
        inimigo->frames_por_sprite = 15;
        inimigo->velocidade = 2.0f;
        inimigo->velocidade_original = 2.0f;
        inimigo->vida = 50;
        inimigo->vidaMaxima = 50;
        inimigo->dano = 60;
        inimigo->dano_original = 10;
        break;
    case TIPO_BOSS_RATO:
        inimigo->larguraBot = 348.0f;
        inimigo->alturaBot = 156.0f;
        inimigo->frames_por_sprite = 11;
        inimigo->velocidade = 1.5f;
        inimigo->velocidade_original = 1.5f;
        inimigo->vida = 30;
        inimigo->vidaMaxima = 30;
        inimigo->dano = 60;
        inimigo->dano_original = 15;
        break;
    case TIPO_BOSS_MOSQUITO:
        inimigo->larguraBot = 220.0f;
        inimigo->alturaBot = 220.0f;
        inimigo->frames_por_sprite = 5;
        inimigo->velocidade = 2.0f;
        inimigo->velocidade_original = 2.0f;
        inimigo->vida = 40;
        inimigo->vidaMaxima = 40;
        inimigo->dano = 60;
        inimigo->dano_original = 12;
        break;
    }
}

// SPAWNA INIMIGOS COM BASE NA CAMERA
void respawn_inimigo_na_camera(Inimigo* inimigo, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera, SistemaSom* sons) {
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
        inicializar_inimigo(inimigo, TIPO_ZUMBI, x, y, zumbi_dir, zumbi_esq, sons);
        break;
    case TIPO_RATO:
        inicializar_inimigo(inimigo, TIPO_RATO, x, y, rato_dir, rato_esq, sons);
        break;
    case TIPO_MOSQUITO:
        inicializar_inimigo(inimigo, TIPO_MOSQUITO, x, y, mosquito_dir, mosquito_esq, sons);
        break;
    }
}

void inicializar_array_inimigos(Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, float* posicaoCamera, SistemaSom* sons) {
    srand(time(NULL));
    for (int i = 0; i < quantidade; i++) {
        respawn_inimigo_na_camera(&inimigos[i], zumbi_dir, zumbi_esq, rato_dir, rato_esq, mosquito_dir, mosquito_esq, posicaoCamera, sons);
    }
}

void atualizar_movimento_inimigos(Caravana* caravana, Inimigo* inimigos, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (inimigos[i].ativo == false)
            continue;

        // Ignora bosses e minions de boss (eles têm IA própria)
        if (inimigos[i].tipo == TIPO_BOSS || inimigos[i].tipo == TIPO_BOSS_RATO || inimigos[i].tipo == TIPO_BOSS_MOSQUITO || inimigos[i].eh_minion_de_boss)
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
    if (inimigo->tipo == TIPO_BOSS) {
        num_colunas = 3;
    }
    else if (inimigo->tipo == TIPO_BOSS_RATO) {
        num_colunas = 3; 
    }
    else if (inimigo->tipo == TIPO_BOSS_MOSQUITO) {
        num_colunas = 2; 
    }
    else {
        num_colunas = 2;
    }

    // Bosses sempre animam, mesmo quando parados
    bool deve_animar = em_movimento || (inimigo->tipo == TIPO_BOSS) || (inimigo->tipo == TIPO_BOSS_RATO) || (inimigo->tipo == TIPO_BOSS_MOSQUITO);

    if (deve_animar) {
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

    // Boss Rato e Boss Mosquito usam flip horizontal quando virado para a direita
    if (inimigo->tipo == TIPO_BOSS_RATO) {
        if (inimigo->virado_direita) {
            // Quando flipado horizontalmente, desenha espelhado
            al_draw_tinted_scaled_rotated_bitmap_region(sprite_atual,sx, sy, largura_frame, altura_frame,al_map_rgb(255, 255, 255),0, 0,inimigo->botX + largura_frame, inimigo->botY,-1.0f, 1.0f,0, 0);
        }
        else {
            // Desenho normal
            al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame, inimigo->botX, inimigo->botY, 0);
        }
    }
    else if (inimigo->tipo == TIPO_BOSS_MOSQUITO) {
        if (inimigo->virado_direita) {
            // Quando flipado horizontalmente, desenha espelhado
            al_draw_tinted_scaled_rotated_bitmap_region(sprite_atual,sx, sy, largura_frame, altura_frame,al_map_rgb(255, 255, 255), 0, 0, inimigo->botX + largura_frame, inimigo->botY, -1.0f, 1.0f,0,0);
        }
        else {
            // Desenho normal
            al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame, inimigo->botX, inimigo->botY, 0);
        }
    }
    else {
        al_draw_bitmap_region(sprite_atual, sx, sy, largura_frame, altura_frame, inimigo->botX, inimigo->botY, 0);
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
        if (inimigos[i].tipo == TIPO_BOSS || inimigos[i].tipo == TIPO_BOSS_RATO || inimigos[i].tipo == TIPO_BOSS_MOSQUITO) {
            continue;
        }

        // Buffs progressivos baseados na fase
        
        float multiplicador = faseAtual;

        // Aumenta vida máxima
        inimigos[i].vidaMaxima = inimigos[i].vidaMaxima * multiplicador;
        if (inimigos[i].vidaMaxima < 1) inimigos[i].vidaMaxima = 1;
        inimigos[i].vida = inimigos[i].vidaMaxima;

        // Aumenta dano 
        inimigos[i].dano += (faseAtual - 1) * 2.0f;

        // Aumenta velocidade 
        inimigos[i].velocidade += (faseAtual - 1) * 1.3f;
    }
}

void spawnar_boss(Inimigo* inimigo, ALLEGRO_BITMAP* boss_dir, ALLEGRO_BITMAP* boss_esq, float* posicaoCamera, SistemaSom* sons) {
    // Boss spawna mais a frente da camera
    float camera_direita = posicaoCamera[0] + 1280;
    float spawn_x = camera_direita + 500;  // Mais distante que inimigos normais

    // Boss spawna no centro vertical da tela
    float spawn_y = (720 / 2) + 52;

    inicializar_inimigo(inimigo, TIPO_BOSS, spawn_x, spawn_y, boss_dir, boss_esq, sons);
    inimigo->ativo = true;
}

void spawnar_boss_rato(Inimigo* inimigo, ALLEGRO_BITMAP* ratodir, ALLEGRO_BITMAP* ratoesq, float* posicaoCamera, SistemaSom* sons) {
    float camera_direita = posicaoCamera[0] + 1280;
    float spawnX = camera_direita + 500;
    float spawnY = (720 / 2) - 100;

    inicializar_inimigo(inimigo, TIPO_BOSS_RATO, spawnX, spawnY, ratodir, ratoesq, sons);
    inimigo->ativo = true;
}

void spawnar_boss_mosquito(Inimigo* inimigo, ALLEGRO_BITMAP* mosquitodir, ALLEGRO_BITMAP* mosquitoesq, float* posicaoCamera, SistemaSom* sons) {
    float camera_direita = posicaoCamera[0] + 1280;
    float spawnX = camera_direita + 500;
    float spawnY = (720 / 2) + 52;

    inicializar_inimigo(inimigo, TIPO_BOSS_MOSQUITO, spawnX, spawnY, mosquitodir, mosquitoesq, sons);
    inimigo->ativo = true;
}

void atualizar_boss_perseguindo(Inimigo* boss, const Jogador* jogador, float distanciaParada) {
    if (!boss || !boss->ativo || (boss->tipo != TIPO_BOSS && boss->tipo != TIPO_BOSS_RATO && boss->tipo != TIPO_BOSS_MOSQUITO)) return;

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
    bool sobreposX = (boss->botX < jogador->jogadorX + 80.0f) && (boss->botX + boss->larguraBot > jogador->jogadorX);
    bool sobreposY = (boss->botY < jogador->jogadorY + 96.0f) && (boss->botY + boss->alturaBot > jogador->jogadorY);

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

// Atualiza timer de colisão e desativa inimigos coilidindo na caravana
void atualizar_timer_colisao_inimigos(Inimigo* inimigos, int quantidade) {
    const double TEMPO_VIDA_COLISAO = 2.5; // 2.5 segundos até morrer
    double tempo_atual = al_get_time();

    for (int i = 0; i < quantidade; i++) {
        if (!inimigos[i].ativo) continue;

        if (inimigos[i].colidindo_caravana) {
            double tempo_colidindo = tempo_atual - inimigos[i].timer_colisao_inicio;

            if (tempo_colidindo >= TEMPO_VIDA_COLISAO) {
                parar_som_inimigo(&inimigos[i]); // Para o som antes de desativar
                inimigos[i].ativo = false;
                inimigos[i].colidindo_caravana = false;
            }
        }
    }
}

// Toca o som do inimigo quando ele é ativado
void tocar_som_inimigo(Inimigo* inimigo, SistemaSom* sons) {
    if (!inimigo || !sons || inimigo->somTocado) {
        return;
    }

    // Toca som baseado no tipo do inimigo EM LOOP
    switch (inimigo->tipo) {
    case TIPO_ZUMBI:
        if (sons->somZumbi) {
            al_play_sample(sons->somZumbi, 2.0f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_LOOP, &inimigo->somID);
            inimigo->somTocado = true;
        }
        break;
    case TIPO_RATO:
        if (sons->somRato) {
            al_play_sample(sons->somRato, 0.5f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_LOOP, &inimigo->somID);
            inimigo->somTocado = true;
        }
        break;
    case TIPO_MOSQUITO:
        if (sons->somMosquito) {
            al_play_sample(sons->somMosquito, 1.5f, 0.0f, 1.0f, ALLEGRO_PLAYMODE_LOOP, &inimigo->somID);
            inimigo->somTocado = true;
        }
        break;
    default:
        break;
    }
}

// Para o som do inimigo quando ele é desativado
void parar_som_inimigo(Inimigo* inimigo) {
    if (!inimigo || !inimigo->somTocado) {
        return;
    }

    // Verifica se o ID do sample é válido antes de tentar parar - GPT
    if (inimigo->somID._id != -1 && inimigo->somID._index >= 0) {
        al_stop_sample(&inimigo->somID);
    }

    // Reseta o ID e a flag
    inimigo->somTocado = false;
    inimigo->somID._id = -1;
    inimigo->somID._index = -1;
}

// ===== SISTEMA DE MINIONS E BUFFS DE BOSS =====

// Spawna um minion próximo ao boss
void spawnar_minion_boss(Inimigo* boss, Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* sprite_dir, ALLEGRO_BITMAP* sprite_esq, TipoInimigo tipo_minion, const Jogador* jogador, SistemaSom* sons) {
    int minions_spawnados = 0;

    for (int i = 0; i < quantidade && minions_spawnados < 2; i++) {
        if (!inimigos[i].ativo) {
            // Spawna próximo ao boss com offset aleatório
            float offset_x = (rand() % 200) - 100.0f; // -100 a +100
            float offset_y = (rand() % 100) - 50.0f;  // -50 a +50

            float spawn_x = boss->botX + offset_x;
            float spawn_y = boss->botY + offset_y;

            // Garante que não spawne fora dos limites verticais
            if (spawn_y < 360.0f) spawn_y = 360.0f;
            if (spawn_y > 620.0f) spawn_y = 620.0f;

            inicializar_inimigo(&inimigos[i], tipo_minion, spawn_x, spawn_y, sprite_dir, sprite_esq, sons);
            inimigos[i].ativo = true;
            inimigos[i].eh_minion_de_boss = true; // Marca como minion

            //BUFF DE MINIONS
            inimigos[i].vida *= 1;           // Minion sempre tem 1 de vida            
            inimigos[i].dano *= 3.f;        // Aumenta o dano
            inimigos[i].velocidade *= 1.4f;

            // Toca som do minion
            tocar_som_inimigo(&inimigos[i], sons);

            minions_spawnados++;
        }
    }
}

// Verifica e aplica buff de metade de vida
void verificar_buff_metade_vida(Inimigo* boss) {
    if (!boss || !boss->ativo) return;

    // Só aplica uma vez quando atinge 50% ou menos de vida
    if (!boss->buff_metade_vida_aplicado && boss->vida <= (boss->vidaMaxima / 2)) {
        // Aplica buff de 20% em velocidade e dano
        boss->velocidade = boss->velocidade_original * 1.2f;
        boss->dano = boss->dano_original * 1.2f;
        boss->buff_metade_vida_aplicado = true;
    }
}

// Atualiza minions para perseguir o jogador
void atualizar_minions_perseguindo(Inimigo* inimigos, int quantidade, const Jogador* jogador) {
    for (int i = 0; i < quantidade; i++) {
        if (!inimigos[i].ativo || !inimigos[i].eh_minion_de_boss) continue;

        // Calcula direção para o jogador (usa centro das hitboxes)
        float centroJogadorX = jogador->jogadorX + (80.0f / 2.0f); // 80 = LARGURA_JOGADOR
        float centroJogadorY = jogador->jogadorY + (96.0f / 2.0f); // 96 = ALTURA_JOGADOR

        float centroMinionX = inimigos[i].botX + (inimigos[i].larguraBot / 2.0f);
        float centroMinionY = inimigos[i].botY + (inimigos[i].alturaBot / 2.0f);

        float direcaoX = centroJogadorX - centroMinionX;
        float direcaoY = centroJogadorY - centroMinionY;
        float distancia = sqrtf(direcaoX * direcaoX + direcaoY * direcaoY);

        // Se muito próximo, para de se mover mas mantém animação
        if (distancia < 50.0f) {
            inimigos[i].em_movimento = false;
            continue;
        }

        // Normaliza e move
        if (distancia > 0.0001f) {
            direcaoX /= distancia;
            direcaoY /= distancia;

            // Move o minion na direção do jogador
            inimigos[i].botX += direcaoX * inimigos[i].velocidade;
            inimigos[i].botY += direcaoY * inimigos[i].velocidade;

            // Atualiza orientação baseado na direção X
            inimigos[i].virado_direita = (direcaoX > 0.0f);
            inimigos[i].em_movimento = true;
        }

        // Limita verticalmente ao chão
        if (inimigos[i].botY + inimigos[i].alturaBot >= 720) {
            inimigos[i].botY = 720 - inimigos[i].alturaBot;
        }

        // Limita verticalmente ao topo da área jogável
        if (inimigos[i].botY < 360.0f) {
            inimigos[i].botY = 360.0f;
        }
    }
}

// Sistema principal de atualização de boss (spawn de minions + buffs)
void atualizar_sistema_boss(Inimigo* boss, Inimigo* inimigos, int quantidade, ALLEGRO_BITMAP* zumbi_dir, ALLEGRO_BITMAP* zumbi_esq, ALLEGRO_BITMAP* rato_dir, ALLEGRO_BITMAP* rato_esq, ALLEGRO_BITMAP* mosquito_dir, ALLEGRO_BITMAP* mosquito_esq, const Jogador* jogador, SistemaSom* sons) {
    if (!boss || !boss->ativo) return;

    // Verifica buff de metade de vida
    verificar_buff_metade_vida(boss);

    // Sistema de spawn de minions a cada 3 segundos
    double tempo_atual = al_get_time();

    if (tempo_atual - boss->timer_ultimo_spawn_minion >= boss->intervalo_spawn_minion) {
        // Determina tipo de minion baseado no tipo de boss
        TipoInimigo tipo_minion;
        ALLEGRO_BITMAP* sprite_dir;
        ALLEGRO_BITMAP* sprite_esq;

        switch (boss->tipo) {
        case TIPO_BOSS_RATO:
            tipo_minion = TIPO_RATO;
            sprite_dir = rato_dir;
            sprite_esq = rato_esq;
            break;
        case TIPO_BOSS_MOSQUITO:
            tipo_minion = TIPO_MOSQUITO;
            sprite_dir = mosquito_dir;
            sprite_esq = mosquito_esq;
            break;
        case TIPO_BOSS:
            tipo_minion = TIPO_ZUMBI;
            sprite_dir = zumbi_dir;
            sprite_esq = zumbi_esq;
            break;
        default:
            return;
        }

        // Spawna 2 minions
        spawnar_minion_boss(boss, inimigos, quantidade, sprite_dir, sprite_esq, tipo_minion, jogador, sons);
        boss->timer_ultimo_spawn_minion = tempo_atual;
    }

    // Atualiza minions para perseguir jogador
    atualizar_minions_perseguindo(inimigos, quantidade, jogador);
}

// Desativa todos os minions de boss
void desativar_minions_boss(Inimigo* inimigos, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        if (inimigos[i].ativo && inimigos[i].eh_minion_de_boss) {
            // Para o som do minion antes de desativar
            parar_som_inimigo(&inimigos[i]);

            // Desativa o minion
            inimigos[i].ativo = false;
            inimigos[i].eh_minion_de_boss = false;
        }
    }
}