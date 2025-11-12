#include "som.h"
#include <stdio.h>

bool inicializarSistemaAudio() {
    // Instala o sistema de áudio
    al_install_audio();

    // Inicializa o addon de codecs para carregar arquivos .wav, .ogg
    al_init_acodec_addon();

    // Reserva 10 canais para efeitos sonoros simultâneos
    // Isso cria automaticamente um mixer padrão
    al_reserve_samples(10);

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
    sons->somRato = NULL;
    sons->musicaMenu = NULL;

    // Carrega o som de tiro
    sons->somTiro = al_load_sample("sons/Tiro.wav");

    // Carrega o som de clique
    sons->somClick = al_load_sample("sons/Som Click.wav");

    // Carrega o som de passos
    sons->somAndando = al_load_sample("sons/andando.wav");

    // Carrega o som do mosquito
    sons->somMosquito = al_load_sample("sons/mosquito.wav");

    // Carrega o som da vassoura
    sons->somVassoura = al_load_sample("sons/Vassoura.wav");

    // Carrega o som do zumbi
    sons->somZumbi = al_load_sample("sons/Zumbi.wav");

    // Carrega o som do veneno
    sons->somVeneno = al_load_sample("sons/Veneno.wav");

    // Carrega o som do rato
    sons->somRato = al_load_sample("sons/Rato.wav");

    // Carrega a música do menu como stream
    sons->musicaMenu = al_load_audio_stream("sons/MusicaMenu.wav", 4, 2048);

    // Configura o modo de reprodução para loop infinito
    al_set_audio_stream_playmode(sons->musicaMenu, ALLEGRO_PLAYMODE_LOOP);

    // Anexa a stream ao mixer padrão
    al_attach_audio_stream_to_mixer(sons->musicaMenu, al_get_default_mixer());

    return true;
}

void tocarSomTiro(SistemaSom* sons) {
    if (!sons || !sons->somTiro) {
        return;
    }

    al_play_sample(
        sons->somTiro,
        0.5f, // Volume 50%
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
        0.6f, // Volume 60%
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
        0.9f, // Volume 90%
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
        1.5f, // Volume 150%
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
        0.1f, // Volume 40%
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
        0.02f, // Volume 2%
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
        0.1f, // Volume 20%
        0.0f, // Centro (stereo)
        1.0f, // Velocidade normal
        ALLEGRO_PLAYMODE_ONCE,
        NULL
    );
}

void tocarSomRato(SistemaSom* sons) {
    if (!sons || !sons->somRato) {
        return;
    }

  al_play_sample(
        sons->somRato,
        0.02f, // Volume 2%
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

    // Define o volume da música
    al_set_audio_stream_gain(sons->musicaMenu, 1.0f);

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

    // Destroi o som do rato
    if (sons->somRato) {
        al_destroy_sample(sons->somRato);
        sons->somRato = NULL;
    }
}
