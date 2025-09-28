#include "configuracoes.h"

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

    bool telaMenu = true;
    bool configAberta = false;
    bool jogando = false;
    bool jogoPausado = false;
    bool redesenhar = false;
    bool w = false, a = false, s = false, d = false, espaco = false, shift = false, esc = false;
    float mouseX = 0, mouseY = 0;
    ProjetilPosicao projetil = { 0 };

    // CARREGAMENTO DOS SPRITES DOS INIMIGOS
    ALLEGRO_BITMAP* zumbi_direita = al_load_bitmap("ZumbiAndandoDireita.png");
    ALLEGRO_BITMAP* zumbi_esquerda = al_load_bitmap("ZumbiAndandoEsquerda.png");
    ALLEGRO_BITMAP* rato_direita = al_load_bitmap("RatoAndandoDireita.png");
    ALLEGRO_BITMAP* rato_esquerda = al_load_bitmap("RatoAndandoEsquerda.png");
    ALLEGRO_BITMAP* mosquito_direita = al_load_bitmap("MosquitoDireita.png");
    ALLEGRO_BITMAP* mosquito_esquerda = al_load_bitmap("MosquitoEsquerda.png");

    // CAMERA
    float posicaoCamera[2] = { 0, 0 };

    // ARRAY DE 20 INIMIGOS
    Inimigo inimigos[MAX_INIMIGOS];
    inicializar_array_inimigos(inimigos, MAX_INIMIGOS,
        zumbi_direita, zumbi_esquerda,
        rato_direita, rato_esquerda,
        mosquito_direita, mosquito_esquerda,
        posicaoCamera);

    ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);

    // JOGADOR
    Jogador jogador = { 120.0, 520.0, true, false };
    ALLEGRO_BITMAP* sprite_andando_direita = al_load_bitmap("AndandoDireita.png");
    ALLEGRO_BITMAP* sprite_andando_esquerda = al_load_bitmap("AndandoEsquerda.png");
    ALLEGRO_BITMAP* sprite_atirando_direita = al_load_bitmap("AtirandoDireita.png");
    ALLEGRO_BITMAP* sprite_atirando_esquerda = al_load_bitmap("AtirandoEsquerda.png");

    // Variáveis de animação normal
    int frame_atual = 0;
    int contador_frame = 0;
    int frames_por_sprite = 11;
    bool virado_direita = true;
    int frame_tiro = 0;
    int contador_frame_tiro = 0;

    //PROJETIL
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
    // DIMENSÕES E COORDENADAS DOS BOTÕES
    int botaoJogarLargura = al_get_bitmap_width(botaoJogar), botaoJogarAltura = al_get_bitmap_height(botaoJogar), botaoConfigLargura = al_get_bitmap_width(botaoConfig), botaoConfigAltura = al_get_bitmap_height(botaoConfig), botaoSairLargura = al_get_bitmap_width(botaoSair), botaoSairAltura = al_get_bitmap_height(botaoSair), fundoMenuLargura = al_get_bitmap_width(fundoMenu), fundoMenuAltura = al_get_bitmap_height(fundoMenu), abaConfigLargura = al_get_bitmap_width(abaConfig), abaConfigAltura = al_get_bitmap_height(abaConfig), botaoVoltarLargura = al_get_bitmap_width(botaoVoltar), botaoVoltarAltura = al_get_bitmap_height(botaoVoltar);
    int botaoJogarX = 200, botaoJogarY = 620, botaoConfigX = 500, botaoConfigY = 620, botaoSairX = 800, botaoSairY = 620, botaoVoltarX = 520, botaoVoltarY = 500;

    //-----------------//

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_TRANSFORM camera;

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_start_timer(timer);

    ALLEGRO_EVENT event;
    
    while (telaMenu) {
        al_wait_for_event(fila_eventos, &event);

        // ATUALIZA COORDENADAS DO MOUSE
        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            mouseX = event.mouse.x;
            mouseY = event.mouse.y;
        }

        // VERIFICA OS CLIQUES
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                // CLICOU NO BOTÃO JOGAR
                if (mouseX >= botaoJogarX && mouseX <= (botaoJogarX + botaoJogarLargura) && mouseY >= botaoJogarY && mouseY <= (botaoJogarY + botaoJogarAltura))
                {
                    // SAI DO MENU E INICIA O JOGO
                    telaMenu = false;
                    jogando = true;
                }

                // CLICOU NO BOTÃO CONFIGURAÇÕES
                if (mouseX >= botaoConfigX && mouseX <= (botaoConfigX + botaoConfigLargura) && mouseY >= botaoConfigY && mouseY <= (botaoConfigY + botaoConfigAltura))
                {
                    configAberta = true;

                    // *****PENDENTE***** DEFINIR ABA DE CONFIGURAÇÕES OU SUBSTITUIR PELO BOTÃO DE REGRAS QUE ESTÁ NA PASTA IMAGENS

                    // ABRIU ABA DE CONFIGURAÇÕES
                    while (configAberta) {

                        al_wait_for_event(fila_eventos, &event);

                        // XY MOUSE
                        if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                            mouseX = event.mouse.x;
                            mouseY = event.mouse.y;
                        }

                        // DESENHA ABA CONFIG E O BOTÃO VOLTAR
                        al_draw_scaled_bitmap(abaConfig, 0, 0, abaConfigLargura, abaConfigAltura, 0, 0, WIDTH, HEIGHT, 0);
                        if (mouseX >= botaoVoltarX && mouseX <= (botaoVoltarX + botaoVoltarLargura) && mouseY >= botaoVoltarY && mouseY <= (botaoVoltarY + botaoVoltarAltura)) {
                            al_draw_bitmap(botaoVoltar2, botaoVoltarX, botaoVoltarY, 0);
                        }
                        else {
                            al_draw_bitmap(botaoVoltar, botaoVoltarX, botaoVoltarY, 0);
                        }
                        al_flip_display();

                        // CLICOU EM VOLTAR
                        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && (mouseX >= botaoVoltarX && mouseX <= (botaoVoltarX + botaoVoltarLargura) && mouseY >= botaoVoltarY && mouseY <= (botaoVoltarY + botaoVoltarAltura))) {
                            // VOLTOU PRO MENU
                            configAberta = false;
                        }
                    }
                }

                // BOTÃO SAIR
                if (mouseX >= botaoSairX && mouseX <= (botaoSairX + botaoSairLargura) && mouseY >= botaoSairY && mouseY <= (botaoSairY + botaoSairAltura))
                {
                    // FECHA O JOGO
                    telaMenu = false;
                }
        }

        // DESENHA O MENU
        al_draw_scaled_bitmap(fundoMenu, 0, 0, fundoMenuLargura, fundoMenuAltura, 0, 0, WIDTH, HEIGHT, 0);
        if (mouseX >= botaoJogarX && mouseX <= (botaoJogarX + botaoJogarLargura) && mouseY >= botaoJogarY && mouseY <= (botaoJogarY + botaoJogarAltura)) {
            al_draw_bitmap(botaoJogar2, botaoJogarX, botaoJogarY, 0);
        }
        else {
            al_draw_bitmap(botaoJogar, botaoJogarX, botaoJogarY, 0);
        }
        if (mouseX >= botaoConfigX && mouseX <= (botaoConfigX + botaoConfigLargura) && mouseY >= botaoConfigY && mouseY <= (botaoConfigY + botaoConfigAltura)) {
            al_draw_bitmap(botaoConfig2, botaoConfigX, botaoConfigY, 0);
        }
        else {
            al_draw_bitmap(botaoConfig, botaoConfigX, botaoConfigY, 0);
        }
        if (mouseX >= botaoSairX && mouseX <= (botaoSairX + botaoSairLargura) && mouseY >= botaoSairY && mouseY <= (botaoSairY + botaoSairAltura)) {
            al_draw_bitmap(botaoSair2, botaoSairX, botaoSairY, 0);
        }
        else {
            al_draw_bitmap(botaoSair, botaoSairX, botaoSairY, 0);
        }
        al_flip_display();
    }

    //LOOP PRINCIPAL
    while (jogando) {
        al_wait_for_event(fila_eventos, &event);

        // CONDICAO DE PARADA
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            jogando = false;
        }

        // VERIFICACAO DE TECLAS
        if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP)
            verificar_Input(event, &w, &a, &s, &d, &espaco, &shift, &esc);

        // MOVIMENTACAO E RESTRICAO DO PERSONAGEM
        if (event.type == ALLEGRO_EVENT_TIMER && esc == false) {
            mover(&jogador, w, a, s, d, shift, VELOCIDADE_JOGADOR, &frames_por_sprite);
            restringirPosicao(&jogador, WIDTH, HEIGHT, LARGURA_JOGADOR, ALTURA_JOGADOR);
            atualizar_movimento_inimigos(inimigos, MAX_INIMIGOS,
                zumbi_direita, zumbi_esquerda,
                rato_direita, rato_esquerda,
                mosquito_direita, mosquito_esquerda,
                posicaoCamera);
            camera_jogador(posicaoCamera, jogador, WIDTH, LARGURA_JOGADOR, ALTURA_JOGADOR);

            redesenhar = true;
        }
        
        // VERIFICA COLISAO
        bool colidiu = false;
        for (int i = 0; i < MAX_INIMIGOS; i++) {
            if (detectarColisao(&inimigos[i], &jogador, LARGURA_JOGADOR, ALTURA_JOGADOR)) {
                colidiu = true;
                break;
            }
        }

        // FICA VERMELHO S ECOLIDIR
        if (colidiu)
            cor = al_map_rgb(255, 0, 0);
        else
            cor = al_map_rgb(0, 0, 0);

        // PAUSAR O JOGO
        if (esc) {
            al_stop_timer(timer);

            // MANTÉM O MENU ACOMPANHANDO A TELA
            al_identity_transform(&camera);
            al_use_transform(&camera);

            jogoPausado = true;
            botaoJogarX = 525, botaoJogarY = 260, botaoConfigX = 525, botaoConfigY = 340, botaoSairX = 525, botaoSairY = 420;

            // JOGO PAUSADO
            while (jogoPausado) {
                al_wait_for_event(fila_eventos, &event);

                    // HABILITA TECLADO PARA SAIR DO PAUSE
                    if (event.type == ALLEGRO_EVENT_KEY_DOWN || event.type == ALLEGRO_EVENT_KEY_UP) {
                        verificar_Input(event, &w, &a, &s, &d, &espaco, &shift, &esc);
                    }

                    // ATUALIZA COORDENADAS DO MOUSE
                    if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                        mouseX = event.mouse.x;
                        mouseY = event.mouse.y;
                    }

                    // VERIFICA OS CLIQUES
                    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

                        // CLICOU NO BOTÃO JOGAR
                        if (mouseX >= botaoJogarX && mouseX <= (botaoJogarX + botaoJogarLargura) && mouseY >= botaoJogarY && mouseY <= (botaoJogarY + botaoJogarAltura))
                        {
                            // VOLTA PRO JOGO
                            jogoPausado = false;
                            esc = false;
                        }

                        // CLICOU NO BOTÃO CONFIGURAÇÕES
                        if (mouseX >= botaoConfigX && mouseX <= (botaoConfigX + botaoConfigLargura) && mouseY >= botaoConfigY && mouseY <= (botaoConfigY + botaoConfigAltura))
                        {
                            configAberta = true;

                            // *****PENDENTE***** DEFINIR ABA DE CONFIGURAÇÕES OU SUBSTITUIR PELO BOTÃO DE REGRAS QUE ESTÁ NA PASTA IMAGENS

                            // ABRIU ABA DE CONFIGURAÇÕES
                            while (configAberta) {

                                al_wait_for_event(fila_eventos, &event);

                                // XY MOUSE
                                if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
                                    mouseX = event.mouse.x;
                                    mouseY = event.mouse.y;
                                }

                                // DESENHA ABA CONFIG E O BOTÃO VOLTAR
                                al_draw_scaled_bitmap(abaConfig, 0, 0, abaConfigLargura, abaConfigAltura, 0, 0, WIDTH, HEIGHT, 0);
                                if (mouseX >= botaoVoltarX && mouseX <= (botaoVoltarX + botaoVoltarLargura) && mouseY >= botaoVoltarY && mouseY <= (botaoVoltarY + botaoVoltarAltura)) {
                                    al_draw_bitmap(botaoVoltar2, botaoVoltarX, botaoVoltarY, 0);
                                }
                                else {
                                    al_draw_bitmap(botaoVoltar, botaoVoltarX, botaoVoltarY, 0);
                                }
                                al_flip_display();

                                // CLICOU EM VOLTAR
                                if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && (mouseX >= botaoVoltarX && mouseX <= (botaoVoltarX + botaoVoltarLargura) && mouseY >= botaoVoltarY && mouseY <= (botaoVoltarY + botaoVoltarAltura))) {
                                    // VOLTOU PRO MENU
                                    configAberta = false;
                                    // COLOCA O CENÁRIO E OS PERSONAGENS DE VOLTA NA TELA
                                    desenhar_cenario(cenario1, cenario2, jogador.jogadorX, posicaoCamera);
                                    desenhar_jogador(jogador, w, a, s, d, espaco, sprite_andando_direita, sprite_andando_esquerda, sprite_atirando_direita, sprite_atirando_esquerda, &frame_atual, &contador_frame, frames_por_sprite, &virado_direita, &frame_tiro, &contador_frame_tiro);
                                    desenhar_todos_inimigos(inimigos, MAX_INIMIGOS);
                                }
                            }
                        }

                        // BOTÃO SAIR
                        if (mouseX >= botaoSairX && mouseX <= (botaoSairX + botaoSairLargura) && mouseY >= botaoSairY && mouseY <= (botaoSairY + botaoSairAltura))
                        {
                            // SAI DO JOGO
                            jogando = false;
                            jogoPausado = false;
                        }
                    }
                    if (mouseX >= botaoJogarX && mouseX <= (botaoJogarX + botaoJogarLargura) && mouseY >= botaoJogarY && mouseY <= (botaoJogarY + botaoJogarAltura)) {
                        al_draw_bitmap(botaoJogar2, botaoJogarX, botaoJogarY, 0);
                    }
                    else {
                        al_draw_bitmap(botaoJogar, botaoJogarX, botaoJogarY, 0);
                    }
                    if (mouseX >= botaoConfigX && mouseX <= (botaoConfigX + botaoConfigLargura) && mouseY >= botaoConfigY && mouseY <= (botaoConfigY + botaoConfigAltura)) {
                        al_draw_bitmap(botaoConfig2, botaoConfigX, botaoConfigY, 0);
                    }
                    else {
                        al_draw_bitmap(botaoConfig, botaoConfigX, botaoConfigY, 0);
                    }
                    if (mouseX >= botaoSairX && mouseX <= (botaoSairX + botaoSairLargura) && mouseY >= botaoSairY && mouseY <= (botaoSairY + botaoSairAltura)) {
                        al_draw_bitmap(botaoSair2, botaoSairX, botaoSairY, 0);
                    }
                    else {
                        al_draw_bitmap(botaoSair, botaoSairX, botaoSairY, 0);
                    }
                    al_flip_display();
                if (esc == false) {
                    jogoPausado = false;
                }
            }
        }
        else if (esc == false) {
            al_start_timer(timer);
        }

        if (redesenhar && al_is_event_queue_empty(fila_eventos)) {
            al_identity_transform(&camera);
            al_translate_transform(&camera, -posicaoCamera[0], -posicaoCamera[1]);
            al_use_transform(&camera);

            al_clear_to_color(cor);
            desenhar_cenario(cenario1, cenario2, jogador.jogadorX, posicaoCamera);

            // DESENHAR TODOS OS INIMIGOS ATIVOS
            desenhar_jogador(jogador, w, a, s, d, espaco, sprite_andando_direita, sprite_andando_esquerda, sprite_atirando_direita, sprite_atirando_esquerda, &frame_atual, &contador_frame, frames_por_sprite, &virado_direita, &frame_tiro, &contador_frame_tiro);
            desenhar_todos_inimigos(inimigos, MAX_INIMIGOS);

            // ATIRAR
            atirar_multiplos_inimigos(&projetil, jogador, inimigos, MAX_INIMIGOS, projetilDireita, projetilEsquerda, espaco, LARGURA_PROJETIL, ALTURA_PROJETIL, ALTURA_JOGADOR, LARGURA_JOGADOR, WIDTH, VELOCIDADE_PROJETIL, CADENCIA, posicaoCamera);

            // Contador de inimigos restantes NAO ESTA FUNCIUONANDO
            int inimigos_restantes = 0;
            for (int i = 0; i < MAX_INIMIGOS; i++) {
                if (inimigos[i].ativo) inimigos_restantes++;
            }

            char texto[100];
            sprintf_s(texto, sizeof(texto), "PROJETO 1904 - INIMIGOS RESTANTES: %d", inimigos_restantes);
            al_draw_text(font, al_map_rgb(255, 255, 255), 10, 10, 0, texto);

            al_flip_display();
            redesenhar = false;
        }
    }

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
