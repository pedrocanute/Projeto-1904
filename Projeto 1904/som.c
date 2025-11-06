#include "som.h"
#include <stdio.h>

bool inicializarSistemaAudio() {
    // Instala o sistema de áudio
    if (!al_install_audio()) {
        fprintf(stderr, "Falha ao inicializar áudio!\n");
        return false;
    }

    // Inicializa o addon de codecs para carregar arquivos .wav, .ogg
    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Falha ao inicializar codecs de áudio!\n");
        return false;
    }

    // Reserva 10 canais para efeitos sonoros simultâneos
    // Isso cria automaticamente um mixer padrão
    if (!al_reserve_samples(10)) {
        fprintf(stderr, "Falha ao reservar samples de áudio!\n");
        return false;
    }

    return true;
}

bool carregarSons(SistemaSom* sons) {
    if (!sons) {
        return false;
    }

    // Inicializa todos os ponteiros como NULL
    sons->somTiro = NULL;
    sons->somClick = NULL;
    sons->somAndando = NULL;
    sons->somMosquito = NULL;
    sons->somVassoura = NULL;
    sons->somZumbi = NULL;
    sons->somVeneno = NULL;
    sons->musicaMenu = NULL;

    // Carrega o som de tiro do caminho especificado
    sons->somTiro = al_load_sample("sons/Tiro.wav");

    if (!sons->somTiro) {
        fprintf(stderr, "Falha ao carregar sons/Tiro.wav!\n");
        return false;
    }

    // Carrega o som de clique
    sons->somClick = al_load_sample("sons/Som Click.wav");

    if (!sons->somClick) {
        fprintf(stderr, "Falha ao carregar sons/Som Click.wav!\n");
        return false;
    }

    // Carrega o som de passos
    sons->somAndando = al_load_sample("sons/andando.wav");

    if (!sons->somAndando) {
        fprintf(stderr, "Falha ao carregar sons/andando.wav!\n");
        return false;
    }

    // Carrega o som do mosquito
    sons->somMosquito = al_load_sample("sons/mosquito.wav");

    if (!sons->somMosquito) {
        fprintf(stderr, "Falha ao carregar sons/mosquito.wav!\n");
        return false;
    }

    // Carrega o som da vassoura
    sons->somVassoura = al_load_sample("sons/Vassoura.wav");

    if (!sons->somVassoura) {
        fprintf(stderr, "Falha ao carregar sons/Vassoura.wav!\n");
        return false;
    }

    // Carrega o som do zumbi
    sons->somZumbi = al_load_sample("sons/Zumbi.wav");

    if (!sons->somZumbi) {
        fprintf(stderr, "Falha ao carregar sons/Zumbi.wav!\n");
        return false;
    }

    // Carrega o som do veneno
    sons->somVeneno = al_load_sample("sons/Veneno.wav");

    if (!sons->somVeneno) {
        fprintf(stderr, "Falha ao carregar sons/Veneno.wav!\n");
        return false;
    }

    // Carrega a música do menu como stream
    sons->musicaMenu = al_load_audio_stream("sons/MusicaMenu.wav", 4, 2048);

    if (!sons->musicaMenu) {
        fprintf(stderr, "Falha ao carregar sons/MusicaMenu.wav!\n");
        return false;
    }

    // Configura o modo de reprodução para loop infinito
    al_set_audio_stream_playmode(sons->musicaMenu, ALLEGRO_PLAYMODE_LOOP);

    // Anexa a stream ao mixer padrão
    al_attach_audio_stream_to_mixer(sons->musicaMenu, al_get_default_mixer());

    printf("Som de tiro carregado com sucesso!\n");
    printf("Som de clique carregado com sucesso!\n");
    printf("Som de passos carregado com sucesso!\n");
    printf("Som de mosquito carregado com sucesso!\n");
    printf("Som de vassoura carregado com sucesso!\n");
    printf("Som de zumbi carregado com sucesso!\n");
    printf("Som de veneno carregado com sucesso!\n");
    printf("Música do menu carregada com sucesso!\n");
    return true;
}

void tocarSomTiro(SistemaSom* sons) {
    if (!sons || !sons->somTiro) {
        return;
    }

    al_play_sample(
        sons->somTiro,
        0.8f, // Volume 80%
        0.0f, // Centro (stereo)
        1.0f, // Velocidade normal
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );
}

void tocarSomClick(SistemaSom* sons) {
    if (!sons || !sons->somClick) {
        return;
    }

    al_play_sample(
        sons->somClick,
        0.6f, // Volume 60% (mais baixo que o tiro)
        0.0f, // Centro (stereo)
        1.0f, // Velocidade normal
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );
}

void tocarSomAndando(SistemaSom* sons) {
    if (!sons || !sons->somAndando) {
        return;
    }

    al_play_sample(
        sons->somAndando,
        0.5f, // Volume 50%
        0.0f, // Centro (stereo)
        1.0f, // Velocidade normal
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );
}

void tocarSomMosquito(SistemaSom* sons) {
    if (!sons || !sons->somMosquito) {
        return;
    }

    al_play_sample(
        sons->somMosquito,
        0.4f, // Volume 40%
        0.0f, // Centro (stereo)
        1.0f, // Velocidade normal
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );
}

void tocarSomVassoura(SistemaSom* sons) {
    if (!sons || !sons->somVassoura) {
        return;
    }

    al_play_sample(
        sons->somVassoura,
        0.7f, // Volume 70%
        0.0f, // Centro (stereo)
        1.0f, // Velocidade normal
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );
}

void tocarSomZumbi(SistemaSom* sons) {
    if (!sons || !sons->somZumbi) {
        return;
    }

    al_play_sample(
        sons->somZumbi,
        0.4f, // Volume 40%
        0.0f, // Centro (stereo)
        1.0f, // Velocidade normal
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );
}

void tocarSomVeneno(SistemaSom* sons) {
    if (!sons || !sons->somVeneno) {
        return;
    }

    al_play_sample(
        sons->somVeneno,
        0.6f, // Volume 60%
        0.0f, // Centro (stereo)
        1.0f, // Velocidade normal
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );
}

void tocarMusicaMenu(SistemaSom* sons) {
    if (!sons) {
        fprintf(stderr, "ERRO: ponteiro sons é NULL!\n");
        return;
    }
    
    if (!sons->musicaMenu) {
        fprintf(stderr, "ERRO: musicaMenu é NULL!\n");
        return;
    }

    printf("Tentando tocar música do menu...\n");

    // Define o volume da música
  al_set_audio_stream_gain(sons->musicaMenu, 1.0f);
    printf("Volume configurado: 1.0\n");

    // Inicia a reprodução da música
    al_set_audio_stream_playing(sons->musicaMenu, true);
    printf("Comando de play enviado...\n");
    
    // Verifica se está realmente tocando
    if (al_get_audio_stream_playing(sons->musicaMenu)) {
        printf("✓ Música do menu iniciada com sucesso!\n");
    } else {
        fprintf(stderr, "✗ ERRO: Música não está tocando!\n");
        fprintf(stderr, "Verifique se sons/MusicaMenu.wav existe!\n");
    }
}

void pararMusicaMenu(SistemaSom* sons) {
    if (!sons || !sons->musicaMenu) {
        return;
    }

    // Para a reprodução da música
    al_set_audio_stream_playing(sons->musicaMenu, false);
}

void destruirSons(SistemaSom* sons) {
    if (!sons) {
        return;
    }

    // Para a música antes de destruir
    if (sons->musicaMenu) {
        al_set_audio_stream_playing(sons->musicaMenu, false);
        al_destroy_audio_stream(sons->musicaMenu);
        sons->musicaMenu = NULL;
    }

    // Destroi o sample se ele foi carregado
    if (sons->somTiro) {
        al_destroy_sample(sons->somTiro);
        sons->somTiro = NULL;
    }

    // Destroi o som de clique
    if (sons->somClick) {
        al_destroy_sample(sons->somClick);
        sons->somClick = NULL;
    }

    // Destroi o som de passos
    if (sons->somAndando) {
        al_destroy_sample(sons->somAndando);
        sons->somAndando = NULL;
    }

    // Destroi o som do mosquito
    if (sons->somMosquito) {
        al_destroy_sample(sons->somMosquito);
        sons->somMosquito = NULL;
    }

    // Destroi o som da vassoura
    if (sons->somVassoura) {
        al_destroy_sample(sons->somVassoura);
        sons->somVassoura = NULL;
    }

    // Destroi o som do zumbi
    if (sons->somZumbi) {
        al_destroy_sample(sons->somZumbi);
        sons->somZumbi = NULL;
    }

    // Destroi o som do veneno
    if (sons->somVeneno) {
        al_destroy_sample(sons->somVeneno);
        sons->somVeneno = NULL;
    }
}
