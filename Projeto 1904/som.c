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

void tocarMusicaMenu(SistemaSom* sons) {
    if (!sons || !sons->musicaMenu) {
        return;
    }

    // Define o volume da música (mais baixo para não sobrepor os efeitos sonoros)
    al_set_audio_stream_gain(sons->musicaMenu, 0.9f);

    // Inicia a reprodução da música
    al_set_audio_stream_playing(sons->musicaMenu, true);
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

    // Aqui você destruiria outros sons no futuro
}
