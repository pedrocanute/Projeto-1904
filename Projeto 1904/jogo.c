#include "jogo.h"
#include <allegro5/allegro_primitives.h>
#include "inimigo.h"

void inicializarJogoCamera(JogoCamera* camera, ALLEGRO_TRANSFORM* transform) {
    camera->posicaoCamera[0] = 0.0f;
    camera->posicaoCamera[1] = 0.0f;
    camera->cor = al_map_rgb(0, 0, 0);
    camera->corCaravana = al_map_rgb(0, 0, 0);
    camera->camera = transform;
}

void inicializarJogoEntidades(JogoEntidades* entidades, Bitmaps* bitmap, float* posicaoCamera, SistemaSom* sons) {
    // Jogador
    entidades->jogador.jogadorX = 120.0f;
    entidades->jogador.jogadorY = 520.0f;
    entidades->jogador.paraDireita = true;
    entidades->jogador.paraEsquerda = false;
    entidades->jogador.tempoUltimoPasso = 0.0f;
    entidades->jogador.timer_dano_visual = 0.0f; 

    // Sprites do jogador
    entidades->spritesJogador.sprite_andando_direita = bitmap->sprite_andando_direita;
    entidades->spritesJogador.sprite_andando_esquerda = bitmap->sprite_andando_esquerda;
    entidades->spritesJogador.sprite_atirando_direita = bitmap->sprite_atirando_direita;
    entidades->spritesJogador.sprite_atirando_esquerda = bitmap->sprite_atirando_esquerda;
    entidades->spritesJogador.sprite_andando_direita_vassoura = bitmap->sprite_andando_direita_vassoura;
    entidades->spritesJogador.sprite_andando_esquerda_vassoura = bitmap->sprite_andando_esquerda_vassoura;
    entidades->spritesJogador.atacando_vassoura_direita = bitmap->atacando_vassoura_direita;
    entidades->spritesJogador.atacando_vassoura_esquerda = bitmap->atacando_vassoura_esquerda;
    entidades->spritesJogador.sprite_andando_direita_veneno = bitmap->sprite_andando_direita_veneno;
    entidades->spritesJogador.sprite_andando_esquerda_veneno = bitmap->sprite_andando_esquerda_veneno;
    entidades->spritesJogador.atacando_veneno_direita = bitmap->atacando_veneno_direita;
    entidades->spritesJogador.atacando_veneno_esquerda = bitmap->atacando_veneno_esquerda;

    // Projetil
    for (int i = 0; i < 50; i++) {
        entidades->projetil.projetilX[i] = 0.0f;
        entidades->projetil.projetilY[i] = 0.0f;
        entidades->projetil.projetilAtivo[i] = false;
    }
    entidades->projetil.tipo = ARMA_VACINA;

    // Caravana
    entidades->caravana.caravanaX = 0.0f;
    entidades->caravana.caravanaY = 305.0f;
    entidades->caravana.caravanaLargura = 80.0f;
    entidades->caravana.caravanaAltura = 732.0f;
    entidades->caravana.caravanaVelocidade = 1.0f;

    // Sistema de fases
    inicializarSistemaFases(&entidades->sistemaFase, &entidades->inimigos[0]);

    // Inicializa array de inimigos mas não ativa ainda (spawn cadenciado)
    inicializar_array_inimigos(entidades->inimigos, MAX_INIMIGOS, bitmap->zumbi_direita, bitmap->zumbi_esquerda, bitmap->rato_direita, bitmap->rato_esquerda, bitmap->mosquito_direita, bitmap->mosquito_esquerda, posicaoCamera, sons);
}

void inicializarJogoBarras(JogoBarras* barras) {
    // Barra de fundo
    barras->barraFundo.barraX = 75.0f;
    barras->barraFundo.barraY = 50.0f;
    barras->barraFundo.barraLargura = 400.0f;
    barras->barraFundo.barraAltura = 100.0f;

    // Barra de infecção
    barras->barraInfeccao.barraX = 75.0f;
    barras->barraInfeccao.barraY = 50.0f;
    barras->barraInfeccao.barraLargura = 75.0f;
    barras->barraInfeccao.barraAltura = 100.0f;

    // Barras do boss - inicialização explícita
    barras->barraVidaBossFundo.barraX = 0.0f;
    barras->barraVidaBossFundo.barraY = 0.0f;
    barras->barraVidaBossFundo.barraLargura = 0.0f;
    barras->barraVidaBossFundo.barraAltura = 0.0f;
    barras->barraVidaBossFundo.barraVida = 0.0f;

    barras->barraVidaBoss.barraX = 0.0f;
    barras->barraVidaBoss.barraY = 0.0f;
    barras->barraVidaBoss.barraLargura = 0.0f;
    barras->barraVidaBoss.barraAltura = 0.0f;
    barras->barraVidaBoss.barraVida = 0.0f;
}

void inicializarJogoControle(JogoControle* controle) {
    controle->timer_regen_infeccao = 0.0f;
    controle->TEMPO_REGEN_INFECCAO = 4.0f;
    controle->TEMPO_INTANGIBILIDADE = 0.7f;
    controle->timer_spawn_inimigos = 0.0f;
    controle->spawn_ativo = false;
    controle->TEMPO_SPAWN = 3.0f;
    controle->boss_spawnado = false;
    controle->fase_boss_ativa = false;
    controle->mostrar_dialogo_transicao = false;

    // =======SPAWN CADENCIADO==========
    controle->inimigos_spawnados = 0;
    controle->timer_spawn_individual = 0.0f;
    controle->INTERVALO_SPAWN_INDIVIDUAL = 1.0f; // Spawna 1 inimigo a cada 1s

    controle->cutscene_concluida = false;  
}

void inicializarJogoAnimacao(JogoAnimacao* animacao) {
    animacao->frame_atual = 0;
    animacao->contador_frame = 0;
    animacao->frames_por_sprite = 11;
    animacao->virado_direita = true;
    animacao->frame_tiro = 0;
    animacao->contador_frame_tiro = 0;
}

// Atualiza boss ativo
void atualizarBossAtivo(JogoEntidades* entidades, JogoBarras* barras, Bitmaps* bitmap, SistemaSom* sons) {
    for (int i = 0; i < MAX_INIMIGOS; i++) {
        if (entidades->inimigos[i].ativo &&
            (entidades->inimigos[i].tipo == TIPO_BOSS || entidades->inimigos[i].tipo == TIPO_BOSS_RATO || entidades->inimigos[i].tipo == TIPO_BOSS_MOSQUITO)) {

            atualizar_boss_perseguindo(&entidades->inimigos[i], &entidades->jogador, 100.0f);
            posicionarBarraVidaBoss(barras, &entidades->inimigos[i]);

            // Sistema de minions e buffs
            atualizar_sistema_boss(&entidades->inimigos[i], entidades->inimigos, MAX_INIMIGOS,bitmap->zumbi_direita, bitmap->zumbi_esquerda,bitmap->rato_direita, bitmap->rato_esquerda,bitmap->mosquito_direita, bitmap->mosquito_esquerda,&entidades->jogador, sons);

            break;
        }
    }
}

// Posiciona barra de vida do boss
void posicionarBarraVidaBoss(JogoBarras* barras, Inimigo* boss) {
    const float LARGURA_BARRA = 300.0f;
    const float OFFSET_Y = 30.0f;
    const float ALTURA_BARRA = 25.0f;

    float centroX = boss->botX + (boss->larguraBot - LARGURA_BARRA) / 2.0f;
    float topoY = boss->botY - OFFSET_Y;

    // Calcula a largura proporcional da barra baseada na vida do boss
    float proporcaoVida = (float)boss->vida / (float)boss->vidaMaxima;
    barras->barraVidaBoss.barraVida = LARGURA_BARRA * proporcaoVida;

    // Posiciona barra de vida do boss
    barras->barraVidaBoss.barraX = centroX;
    barras->barraVidaBoss.barraY = topoY;
    barras->barraVidaBoss.barraLargura = barras->barraVidaBoss.barraX + barras->barraVidaBoss.barraVida;
    barras->barraVidaBoss.barraAltura = topoY + ALTURA_BARRA;

    // Posiciona fundo da barra
    barras->barraVidaBossFundo.barraX = centroX;
    barras->barraVidaBossFundo.barraY = topoY;
    barras->barraVidaBossFundo.barraLargura = centroX + LARGURA_BARRA;
    barras->barraVidaBossFundo.barraAltura = topoY + ALTURA_BARRA;
}

// Processa colisões e retorna resultado
ResultadoColisao processarColisoes(JogoEntidades* entidades, JogoBarras* barras, JogoControle* controle, JogoCamera* jogoCamera) { //ESSA FUNCAO RETORNA UMA STRUCT DE COLISOES
    ResultadoColisao resultado;
    resultado.ocorreuColisao = false;
    resultado.corCaravana = al_map_rgba_f(1.0f, 1.0f, 1.0f, 1.0f); // branco

    const float DURACAO_DANO_VISUAL = 0.12f;
    const float INFECCAO_MAXIMA = 400.0f;
    const float DANO_CONTINUO_CARAVANA_INTERVALO = 0.5f; // Dano a cada 0.5 segundos na caravana

    if (controle->fase_boss_ativa) {
        // Colisão boss com jogador
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            if (entidades->inimigos[i].ativo && (entidades->inimigos[i].tipo == TIPO_BOSS || entidades->inimigos[i].tipo == TIPO_BOSS_RATO || entidades->inimigos[i].tipo == TIPO_BOSS_MOSQUITO)) {

                if (colisao_jogador_inimigo(&entidades->inimigos[i], &entidades->jogador, LARGURA_JOGADOR, ALTURA_JOGADOR)) {
                    float tempoAtual = al_get_time();

                    if (tempoAtual - entidades->inimigos[i].timer_intangibilidade >= controle->TEMPO_INTANGIBILIDADE) {
                        resultado.ocorreuColisao = true;
                        barras->barraInfeccao.barraLargura += entidades->inimigos[i].dano;

                        // Garante que a barra não ultrapasse o máximo
                        if (barras->barraInfeccao.barraLargura > INFECCAO_MAXIMA) {
                            barras->barraInfeccao.barraLargura = INFECCAO_MAXIMA;
                        }

                        controle->timer_regen_infeccao = tempoAtual;
                        entidades->inimigos[i].timer_intangibilidade = tempoAtual;

                        // Efeito visual de dano - MARCA NO JOGADOR TAMBÉM
                        entidades->jogador.timer_dano_visual = tempoAtual;

                        // Efeito visual na caravana
                        float fimDanoVisual = tempoAtual + DURACAO_DANO_VISUAL;
                        if (al_get_time() < fimDanoVisual) {
                            resultado.corCaravana = al_map_rgba_f(1.0f, 0.3f, 0.3f, 1.0f);
                        }
                    }
                }
            }
        }

        // Colisão de minions de boss com jogador (DURANTE FASE DE BOSS)
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            if (entidades->inimigos[i].ativo && entidades->inimigos[i].eh_minion_de_boss) {
                if (colisao_jogador_inimigo(&entidades->inimigos[i], &entidades->jogador, LARGURA_JOGADOR, ALTURA_JOGADOR)) {
                    float tempoAtual = al_get_time();

                    if (tempoAtual - entidades->inimigos[i].timer_intangibilidade >= controle->TEMPO_INTANGIBILIDADE) {
                        resultado.ocorreuColisao = true;
                        barras->barraInfeccao.barraLargura += entidades->inimigos[i].dano;

                        if (barras->barraInfeccao.barraLargura > INFECCAO_MAXIMA) {
                            barras->barraInfeccao.barraLargura = INFECCAO_MAXIMA;
                        }

                        controle->timer_regen_infeccao = tempoAtual;
                        entidades->inimigos[i].timer_intangibilidade = tempoAtual;

                        // Efeito visual de dano 
                        entidades->jogador.timer_dano_visual = tempoAtual;

                        float fimDanoVisual = tempoAtual + DURACAO_DANO_VISUAL;
                        if (al_get_time() < fimDanoVisual) {
                            resultado.corCaravana = al_map_rgba_f(1.0f, 0.3f, 0.3f, 1.0f);
                        }
                    }
                }
            }
        }
    }
    else {
        // COLISÃO DE INIMIGOS NORMAIS COM A CARAVANA (FORA DA FASE DE BOSS)
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            if (!entidades->inimigos[i].ativo) continue;

            // Ignora bosses e minions
            if (entidades->inimigos[i].tipo == TIPO_BOSS || entidades->inimigos[i].tipo == TIPO_BOSS_RATO || entidades->inimigos[i].tipo == TIPO_BOSS_MOSQUITO || entidades->inimigos[i].eh_minion_de_boss) {
                continue;
            }

            // Verifica colisão com a caravana
            if (colisao_inimigo_caravana(&entidades->inimigos[i], &entidades->caravana, entidades->caravana.caravanaLargura, entidades->caravana.caravanaAltura)) {

                float tempoAtual = al_get_time();

                // Se acabou de colidir, inicia o timer
                if (!entidades->inimigos[i].colidindo_caravana) {
                    entidades->inimigos[i].colidindo_caravana = true;
                    entidades->inimigos[i].timer_colisao_inicio = tempoAtual;
                    entidades->inimigos[i].ultimo_dano_aplicado = tempoAtual; 

                    // Aplica dano inicial
                    resultado.ocorreuColisao = true;
                    barras->barraInfeccao.barraLargura += entidades->inimigos[i].dano;

                    if (barras->barraInfeccao.barraLargura > INFECCAO_MAXIMA) {
                        barras->barraInfeccao.barraLargura = INFECCAO_MAXIMA;
                    }

                    controle->timer_regen_infeccao = tempoAtual;

                    // Efeito visual de dano
                    resultado.corCaravana = al_map_rgba_f(1.0f, 0.3f, 0.3f, 1.0f);
                }
                else {
                    // Já está colidindo - aplica dano contínuo a cada intervalo
                    if (tempoAtual - entidades->inimigos[i].ultimo_dano_aplicado >= DANO_CONTINUO_CARAVANA_INTERVALO) {
                        resultado.ocorreuColisao = true;
                        barras->barraInfeccao.barraLargura += entidades->inimigos[i].dano;

                        if (barras->barraInfeccao.barraLargura > INFECCAO_MAXIMA) {
                            barras->barraInfeccao.barraLargura = INFECCAO_MAXIMA;
                        }

                        controle->timer_regen_infeccao = tempoAtual;
                        entidades->inimigos[i].ultimo_dano_aplicado = tempoAtual;

                        // Efeito visual de dano
                        resultado.corCaravana = al_map_rgba_f(1.0f, 0.3f, 0.3f, 1.0f);
                    }
                }
            }
            else {
                // Não está mais colidindo
                entidades->inimigos[i].colidindo_caravana = false;
            }
        }
    }

    return resultado;
}

// Processa regeneração de vida
void processarRegeneracaoVida(JogoBarras* barras, JogoControle* controle, bool colisaoOcorreu) {
    if (!colisaoOcorreu && al_get_time() - controle->timer_regen_infeccao >= controle->TEMPO_REGEN_INFECCAO && barras->barraInfeccao.barraLargura > barras->barraInfeccao.barraX) {
        barras->barraInfeccao.barraLargura--;
    }
}

// Processa spawn de inimigos
void processarSpawnInimigos(JogoEntidades* entidades, JogoControle* controle, Bitmaps* bitmap, float* posicaoCamera, SistemaSom* sons) {
    // NÃO SPAWNA INIMIGOS ATÉ A CUTSCENE TERMINAR
    if (!controle->cutscene_concluida) {
        return;
    }

    if (controle->fase_boss_ativa) { // Não spawna inimigos durante boss
        return; 
    }

    // Verifica se atingiu meta da fase
    if (verificarProgressoDaFase(&entidades->sistemaFase)) {
        // Spawna boss
        if (!controle->boss_spawnado) {
            int idx_boss = -1;
            for (int i = 0; i < MAX_INIMIGOS; ++i) {
                if (!entidades->inimigos[i].ativo) {
                    idx_boss = i;
                    break;
                }
            }

            if (idx_boss >= 0) {
                if (entidades->sistemaFase.faseAtual == 1) {
                    spawnar_boss_rato(&entidades->inimigos[idx_boss], bitmap->boss_rato_direita, bitmap->boss_rato_esquerda, posicaoCamera, sons);
                }
                else if (entidades->sistemaFase.faseAtual == 2) {
                    spawnar_boss_mosquito(&entidades->inimigos[idx_boss], bitmap->boss_mosquito_direita, bitmap->boss_mosquito_esquerda, posicaoCamera, sons);
                }
                else if (entidades->sistemaFase.faseAtual == 3) {
                    spawnar_boss(&entidades->inimigos[idx_boss], bitmap->boss_variola_direita, bitmap->boss_variola_esquerda, posicaoCamera, sons);
                }
                controle->boss_spawnado = true;
                controle->fase_boss_ativa = true;
                controle->spawn_ativo = false;
            }
        }
    }
    else {
        // SPAWN CADENCIADO: Verifica se está no processo de spawn
        if (controle->inimigos_spawnados > 0 && controle->inimigos_spawnados < MAX_INIMIGOS) {
            // Continua ativando inimigos aos poucos
            float tempo_atual = al_get_time();
            if (tempo_atual - controle->timer_spawn_individual >= controle->INTERVALO_SPAWN_INDIVIDUAL) {
                // Ativa o próximo inimigo
                respawn_inimigo_na_camera(&entidades->inimigos[controle->inimigos_spawnados],bitmap->zumbi_direita, bitmap->zumbi_esquerda,bitmap->rato_direita, bitmap->rato_esquerda,bitmap->mosquito_direita, bitmap->mosquito_esquerda,posicaoCamera, sons);

                // Aplica buffs da fase atual
                aplicar_buffs_por_fase(&entidades->inimigos[controle->inimigos_spawnados],1,entidades->sistemaFase.faseAtual);

                // ativa o inimigo
                entidades->inimigos[controle->inimigos_spawnados].ativo = true;

                // Toca o som do inimigo ao ativar
                tocar_som_inimigo(&entidades->inimigos[controle->inimigos_spawnados], sons);

                controle->inimigos_spawnados++;
                controle->timer_spawn_individual = tempo_atual;
            }
            return;
        }

        // Ciclo normal de hordas
        if (contarInimigosAtivos(entidades->inimigos, MAX_INIMIGOS) == 0) {
            if (!controle->spawn_ativo) {
                // Inicia o processo de spawn
                controle->timer_spawn_inimigos = al_get_time();
                controle->spawn_ativo = true;
            }
            else if (al_get_time() - controle->timer_spawn_inimigos >= controle->TEMPO_SPAWN) {
                // Após aguardar TEMPO_SPAWN, inicializa os inimigos
                inicializar_array_inimigos(entidades->inimigos, MAX_INIMIGOS,bitmap->zumbi_direita, bitmap->zumbi_esquerda,bitmap->rato_direita, bitmap->rato_esquerda,bitmap->mosquito_direita, bitmap->mosquito_esquerda,posicaoCamera, sons);

                // Aplica buffs baseado na fase
                aplicar_buffs_por_fase(entidades->inimigos, MAX_INIMIGOS, entidades->sistemaFase.faseAtual);

                // Ativa o primeiro inimigo imediatamente
                entidades->inimigos[0].ativo = true;

                // Toca o som do primeiro inimigo
                tocar_som_inimigo(&entidades->inimigos[0], sons);

                // Reseta contadores para spawn cadenciado (começa no índice 1)
                controle->inimigos_spawnados = 1;
                controle->timer_spawn_individual = al_get_time();
                controle->spawn_ativo = false;
            }
        }
    }
}

// Verifica se boss foi derrotado
void verificarMorteBoss(JogoEntidades* entidades, JogoControle* controle) {
    if (!controle->fase_boss_ativa) {
        return;
    }

    bool bossVivo = false;
    for (int i = 0; i < MAX_INIMIGOS; ++i) {
        if (entidades->inimigos[i].ativo && (entidades->inimigos[i].tipo == TIPO_BOSS || entidades->inimigos[i].tipo == TIPO_BOSS_RATO || entidades->inimigos[i].tipo == TIPO_BOSS_MOSQUITO)) {
            bossVivo = true;
            break;
        }
    }

    if (!bossVivo) {
        
        desativar_minions_boss(entidades->inimigos, MAX_INIMIGOS);

        avancarFase(&entidades->sistemaFase, entidades->inimigos);
        controle->fase_boss_ativa = false;
        controle->boss_spawnado = false;
        controle->spawn_ativo = false;
        controle->inimigos_spawnados = 0; // Reseta contador de spawn cadenciado

        controle->mostrar_dialogo_transicao = true;
    }
}