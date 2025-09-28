#include "configuracoes.h"
#include "personagem.h"
#include "fases.h"
#include "inimigo.h"
#include "projetil.h"
#include "input.h"
#include "colisao.h"
#include "cenario.h"
#include "menu.h"

int main() {

    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();
    al_init_primitives_addon();
    al_install_mouse(); 


    ALLEGRO_DISPLAY* janela = al_create_display(WIDTH, HEIGHT);
    al_set_window_title(janela, "Projeto 1904");
    al_set_window_position(janela, 200, 200);

    //--DECLARACAO DE VARIAVEIS--//
#pragma region VARIAVEIS
    
    bool telaMenu = true;
    bool configAberta = false;
    bool jogoPausado = false;
    bool esc = false;
    float mouseX = 0.0f, mouseY = 0.0f;

    bool telaMenu = true;
    bool configAberta = false;
    bool jogando = false;
    bool jogoPausado = false;
    bool redesenhar = false;
    bool w = false, a = false, s = false, d = false, espaco = false, shift = false, esc = false;
    float mouseX = 0, mouseY = 0;
    ProjetilPosicao projetil = { 0 };
    SistemaFases sistemaFase;

    // TIMER PARA SPAWN DE INIMIGOS
    float timer_spawn_inimigos = 0.0;
    bool spawn_ativo = false;
    const float TEMPO_SPAWN = 3.0;


    // CARREGAMENTO DOS SPRITES DOS INIMIGOS
    ALLEGRO_BITMAP* zumbi_direita = al_load_bitmap("ZumbiAndandoDireita.png");
    ALLEGRO_BITMAP* zumbi_esquerda = al_load_bitmap("ZumbiAndandoEsquerda.png");
    ALLEGRO_BITMAP* rato_direita = al_load_bitmap("RatoAndandoDireita.png");
    ALLEGRO_BITMAP* rato_esquerda = al_load_bitmap("RatoAndandoEsquerda.png");
    ALLEGRO_BITMAP* mosquito_direita = al_load_bitmap("MosquitoDireita.png");
    ALLEGRO_BITMAP* mosquito_esquerda = al_load_bitmap("MosquitoEsquerda.png");

    // CAMERA
    float posicaoCamera[2] = { 0, 0 };

    // ARRAY DE INIMIGOS
    Inimigo inimigos[MAX_INIMIGOS];
    inicializarSistemaFases(&sistemaFase, &inimigos[0]);
    inicializar_array_inimigos(inimigos, MAX_INIMIGOS, zumbi_direita, zumbi_esquerda,rato_direita, rato_esquerda,mosquito_direita, mosquito_esquerda,posicaoCamera);

    ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);

    // JOGADOR
    Jogador jogador = { 120.0, 520.0, true, false };
    ALLEGRO_BITMAP* sprite_andando_direita = al_load_bitmap("AndandoDireita.png");
    ALLEGRO_BITMAP* sprite_andando_esquerda = al_load_bitmap("AndandoEsquerda.png");
    ALLEGRO_BITMAP* sprite_atirando_direita = al_load_bitmap("AtirandoDireita.png");
    ALLEGRO_BITMAP* sprite_atirando_esquerda = al_load_bitmap("AtirandoEsquerda.png");

    // ANIMAÇAO
    int frame_atual = 0;
    int contador_frame = 0;
    int frames_por_sprite = 11;

    bool virado_direita = true;
    int  frame_tiro = 0;
    int  contador_frame_tiro = 0;

    // PROJETIL
    ALLEGRO_BITMAP* projetilDireita = al_load_bitmap("imagens/VacinaProjetilDireita.png");
    ALLEGRO_BITMAP* projetilEsquerda = al_load_bitmap("imagens/VacinaProjetilEsquerda.png");

    // CENARIO
    ALLEGRO_BITMAP* cenario1 = al_load_bitmap("Mapa01.png");
    ALLEGRO_BITMAP* cenario2 = al_load_bitmap("Mapa02.png");

    // MENU
    ALLEGRO_BITMAP* fundoMenu = al_load_bitmap("imagens/menu.png");
    ALLEGRO_BITMAP* botaoJogar = al_load_bitmap("imagens/jogar1.png");
    ALLEGRO_BITMAP* botaoJogar2 = al_load_bitmap("imagens/jogar2.png");
    ALLEGRO_BITMAP* botaoConfig = al_load_bitmap("imagens/configuracoes1.png");
    ALLEGRO_BITMAP* botaoConfig2 = al_load_bitmap("imagens/configuracoes2.png");
    ALLEGRO_BITMAP* botaoSair = al_load_bitmap("imagens/sair1.png");
    ALLEGRO_BITMAP* botaoSair2 = al_load_bitmap("imagens/sair2.png");
    ALLEGRO_BITMAP* abaConfig = al_load_bitmap("imagens/abavazia.png");
    ALLEGRO_BITMAP* botaoVoltar = al_load_bitmap("imagens/voltar1.png");
    ALLEGRO_BITMAP* botaoVoltar2 = al_load_bitmap("imagens/voltar2.png");

    // DIMENSÕES E COORDENADAS DOS BOTÕES (menu principal)
    int botaoJogarLargura = al_get_bitmap_width(botaoJogar);
    int botaoJogarAltura = al_get_bitmap_height(botaoJogar);
    int botaoConfigLargura = al_get_bitmap_width(botaoConfig);
    int botaoConfigAltura = al_get_bitmap_height(botaoConfig);
    int botaoSairLargura = al_get_bitmap_width(botaoSair);
    int botaoSairAltura = al_get_bitmap_height(botaoSair);
    int fundoMenuLargura = al_get_bitmap_width(fundoMenu);
    int fundoMenuAltura = al_get_bitmap_height(fundoMenu);
    int abaConfigLargura = al_get_bitmap_width(abaConfig);
    int abaConfigAltura = al_get_bitmap_height(abaConfig);
    int botaoVoltarLargura = al_get_bitmap_width(botaoVoltar);
    int botaoVoltarAltura = al_get_bitmap_height(botaoVoltar);

    int botaoJogarX = 200, botaoJogarY = 620;
    int botaoConfigX = 500, botaoConfigY = 620;
    int botaoSairX = 800, botaoSairY = 620;
    int botaoVoltarX = 520, botaoVoltarY = 500;

    //-----------------//
#pragma endregion

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TRANSFORM camera;

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source()); 
    al_start_timer(timer);

    MenuFlags flags = {
        .telaMenu = &telaMenu,
        .jogando = &jogando,
        .configAberta = &configAberta,
        .esc = &esc,
        .jogoPausado = &jogoPausado
    };

    MenuIO io = {
        .fila_eventos = fila_eventos,
        .timer = timer,
        .camera = &camera,
        .mouseX = &mouseX,
        .mouseY = &mouseY
    };

    MenuBitmaps bmp = {
        .fundoMenu = fundoMenu,
        .botaoJogar = botaoJogar, .botaoJogar2 = botaoJogar2,
        .botaoConfig = botaoConfig, .botaoConfig2 = botaoConfig2,
        .botaoSair = botaoSair, .botaoSair2 = botaoSair2,
        .abaConfig = abaConfig,
        .botaoVoltar = botaoVoltar, .botaoVoltar2 = botaoVoltar2
    };

    MenuLayout lay = {
        .botaoJogarX = botaoJogarX, .botaoJogarY = botaoJogarY,
        .botaoConfigX = botaoConfigX, .botaoConfigY = botaoConfigY,
        .botaoSairX = botaoSairX, .botaoSairY = botaoSairY,
        .botaoVoltarX = botaoVoltarX, .botaoVoltarY = botaoVoltarY,

        .botaoJogarLargura = botaoJogarLargura, .botaoJogarAltura = botaoJogarAltura,
        .botaoConfigLargura = botaoConfigLargura, .botaoConfigAltura = botaoConfigAltura,
        .botaoSairLargura = botaoSairLargura, .botaoSairAltura = botaoSairAltura,
        .fundoMenuLargura = fundoMenuLargura, .fundoMenuAltura = fundoMenuAltura,
        .abaConfigLargura = abaConfigLargura, .abaConfigAltura = abaConfigAltura,
        .botaoVoltarLargura = botaoVoltarLargura, .botaoVoltarAltura = botaoVoltarAltura
    };

    //MENU PRINCIPAL
    // Aqui o jogo só começa quando clicar em "Jogar" no menu.
    menu_principal(&flags, &io, &bmp, &lay);
    if (!jogando) {
        // Saiu pelo menu limpa e return
        al_destroy_font(font);
        al_destroy_bitmap(fundoMenu);
        al_destroy_bitmap(botaoJogar);
        al_destroy_bitmap(botaoJogar2);
        al_destroy_bitmap(botaoConfig);
        al_destroy_bitmap(botaoConfig2);
        al_destroy_bitmap(botaoSair);
        al_destroy_bitmap(botaoSair2);
        al_destroy_bitmap(abaConfig);
        al_destroy_bitmap(botaoVoltar);
        al_destroy_bitmap(botaoVoltar2);
        al_destroy_bitmap(sprite_andando_direita);
        al_destroy_bitmap(sprite_andando_esquerda);
        al_destroy_bitmap(cenario1);
        al_destroy_bitmap(cenario2);
        al_destroy_bitmap(projetilDireita);
        al_destroy_bitmap(projetilEsquerda);
        al_destroy_bitmap(zumbi_direita);
        al_destroy_bitmap(zumbi_esquerda);
        al_destroy_bitmap(sprite_atirando_direita);
        al_destroy_bitmap(sprite_atirando_esquerda);
        al_destroy_bitmap(rato_direita);
        al_destroy_bitmap(rato_esquerda);
        al_destroy_bitmap(mosquito_direita);
        al_destroy_bitmap(mosquito_esquerda);
        al_destroy_display(janela);
        al_destroy_event_queue(fila_eventos);
        al_destroy_timer(timer);
        return 0;
    }

    ALLEGRO_EVENT event;

    //LOOP PRINCIPAL 

    while (jogando) {
        al_wait_for_event(fila_eventos, &event);

        // CONDIÇÃO DE PARADA
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogando = false;
        }
        // VERIFICACAO DE TECLAS
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
            verificar_Input(event, &w, &a, &s, &d, &espaco, &shift);
            // ESC para abrir o pause
            if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                esc = true;
            }
        }

        // PAUSE 
        if (esc) {
            
            lay.botaoJogarX = 525;  lay.botaoJogarY = 260;
            lay.botaoConfigX = 525; lay.botaoConfigY = 340;
            lay.botaoSairX = 525;   lay.botaoSairY = 420;
            
            menu_pausa(&flags, &io, &bmp, &lay);
            if (!jogando) 
                break; // saiu pelo "Sair" no pause
            // Se voltou do pause, 'esc' já foi zerado dentro do menu_pausa
        }

        // MOVIMENTAÇÃO / ATUALIZAÇÃO ENQUANTO NÃO PAUSADO
        if (event.type == ALLEGRO_EVENT_TIMER && esc == false) {
            mover(&jogador, w, a, s, d, shift, VELOCIDADE_JOGADOR, &frames_por_sprite);
            restringirPosicao(&jogador, WIDTH, HEIGHT, LARGURA_JOGADOR, ALTURA_JOGADOR);

            // ATUALIZA MOVIMENTO DOS INIMIGOS 
            atualizar_movimento_inimigos(inimigos, MAX_INIMIGOS,zumbi_direita, zumbi_esquerda,rato_direita, rato_esquerda,mosquito_direita, mosquito_esquerda,posicaoCamera);

            camera_jogador(posicaoCamera, jogador, WIDTH, LARGURA_JOGADOR, ALTURA_JOGADOR);

            redesenhar = true;
        }
      
        // VERIFICA COLISAO

        bool colidiu = false;
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            if (colisao_jogador_inimigo(&inimigos[i], &jogador, LARGURA_JOGADOR, ALTURA_JOGADOR)) {
                colidiu = true;
                break;
            }
        }

        // SISTEMA DE RESPAWN COM TIMER
        if (!verificarProgressoDaFase(&sistemaFase)) {
            if (contarInimigosAtivos(inimigos, MAX_INIMIGOS) == 0) { // Não há inimigos ativos
                if (!spawn_ativo) {
                    timer_spawn_inimigos = al_get_time(); // inicia
                    spawn_ativo = true;
                }
                else if (al_get_time() - timer_spawn_inimigos >= TEMPO_SPAWN) {
                    inicializar_array_inimigos(inimigos, MAX_INIMIGOS,zumbi_direita, zumbi_esquerda,rato_direita, rato_esquerda,mosquito_direita, mosquito_esquerda,posicaoCamera);
                    aplicar_buffs_por_fase(inimigos, MAX_INIMIGOS, sistemaFase.faseAtual);
                    spawn_ativo = false;
                }
            }
            else {
                spawn_ativo = false;
            }
        }
        else {
            avancarFase(&sistemaFase, inimigos);
            spawn_ativo = false;
        }

        // CAMERA
        al_identity_transform(&camera);
        al_translate_transform(&camera, -posicaoCamera[0], -posicaoCamera[1]);
        al_use_transform(&camera);

            menu_pausa(&flags, &io, &bmp, &lay);
            if (!jogando) 
                break; // saiu pelo "Sair" do pause
        }
        //  DESENHO 
        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            al_identity_transform(&camera);
            al_translate_transform(&camera, -posicaoCamera[0], -posicaoCamera[1]);
            al_use_transform(&camera);

        // DESENHAR TODOS OS INIMIGOS ATIVOS
        desenhar_jogador(jogador, w, a, s, d, espaco,sprite_andando_direita, sprite_andando_esquerda,sprite_atirando_direita, sprite_atirando_esquerda,&frame_atual, &contador_frame, frames_por_sprite,&virado_direita, &frame_tiro, &contador_frame_tiro);
        desenhar_todos_inimigos(inimigos, MAX_INIMIGOS);

        // ATIRAR
        atirar_multiplos_inimigos(&projetil, jogador, inimigos, MAX_INIMIGOS,projetilDireita, projetilEsquerda, espaco,LARGURA_PROJETIL, ALTURA_PROJETIL,ALTURA_JOGADOR, LARGURA_JOGADOR,WIDTH, VELOCIDADE_PROJETIL, CADENCIA,posicaoCamera,&sistemaFase);

        // TEXTO INFORMATIVO COM TIMER
        char texto[100];
        if (spawn_ativo) {
            float tempo_restante = TEMPO_SPAWN - (al_get_time() - timer_spawn_inimigos);
            if (tempo_restante > 0)
                sprintf_s(texto, sizeof(texto), "PROJETO 1904 - NOVOS INIMIGOS EM: %.1f s", tempo_restante);
            else
                sprintf_s(texto, sizeof(texto), "PROJETO 1904 - SPAWNING...");
        }
        else {
            int inimigos_restantes = sistemaFase.metaEliminacoes - sistemaFase.inimigosMortos;
            if (inimigos_restantes < 0) inimigos_restantes = 0;
            sprintf_s(texto, sizeof(texto), "PROJETO 1904 - INIMIGOS RESTANTES: %d", inimigos_restantes);
        }

        // HUD (fixo na tela)
        ALLEGRO_TRANSFORM world_backup;
        al_copy_transform(&world_backup, al_get_current_transform()); // salva a câmera

        ALLEGRO_TRANSFORM hud;
        al_identity_transform(&hud);
        al_use_transform(&hud);

        al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, texto);

        al_use_transform(&world_backup);
        al_flip_display();
    }

    // LIMPEZA
    al_destroy_font(font);
    al_destroy_bitmap(fundoMenu);
    al_destroy_bitmap(botaoJogar);
    al_destroy_bitmap(botaoJogar2);
    al_destroy_bitmap(botaoConfig);
    al_destroy_bitmap(botaoConfig2);
    al_destroy_bitmap(botaoSair);
    al_destroy_bitmap(botaoSair2);
    al_destroy_bitmap(abaConfig);
    al_destroy_bitmap(botaoVoltar);
    al_destroy_bitmap(botaoVoltar2);
    al_destroy_bitmap(sprite_andando_direita);
    al_destroy_bitmap(sprite_andando_esquerda);
    al_destroy_bitmap(cenario1);
    al_destroy_bitmap(cenario2);
    al_destroy_bitmap(projetilDireita);
    al_destroy_bitmap(projetilEsquerda);
    al_destroy_bitmap(zumbi_direita);
    al_destroy_bitmap(zumbi_esquerda);
    al_destroy_bitmap(sprite_atirando_direita);
    al_destroy_bitmap(sprite_atirando_esquerda);
    al_destroy_bitmap(rato_direita);
    al_destroy_bitmap(rato_esquerda);
    al_destroy_bitmap(mosquito_direita);
    al_destroy_bitmap(mosquito_esquerda);

    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);
    al_destroy_timer(timer);

    return 0;
}
