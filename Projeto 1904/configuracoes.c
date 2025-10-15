#include "configuracoes.h"

static ALLEGRO_BITMAP* carregar(const char* caminho) { //GPT QUE FEZ essa funcao static
    ALLEGRO_BITMAP* bmp = al_load_bitmap(caminho);
    if (!bmp) fprintf(stderr, "Falha ao carregar: %s\n", caminho);
    return bmp;
}

bool carregar_bitmaps(Bitmaps* bitmap) {
    if (!bitmap) return false;
    memset(bitmap, 0, sizeof(*bitmap));

    bitmap->zumbi_direita = carregar("imagens/ZumbiAndandoDireita.png");
    bitmap->zumbi_esquerda = carregar("imagens/ZumbiAndandoEsquerda.png");
    bitmap->rato_direita = carregar("imagens/RatoAndandoDireita.png");
    bitmap->rato_esquerda = carregar("imagens/RatoAndandoEsquerda.png");
    bitmap->mosquito_direita = carregar("imagens/MosquitoDireita.png");
    bitmap->mosquito_esquerda = carregar("imagens/MosquitoEsquerda.png");
    bitmap->boss_variola_esquerda = carregar("imagens/variolabossEsquerda.png");
    bitmap->boss_variola_direita = carregar("imagens/variolabossDireita.png");

    bitmap->sprite_andando_direita = carregar("imagens/AndandoDireita.png");
    bitmap->sprite_andando_esquerda = carregar("imagens/AndandoEsquerda.png");
    bitmap->sprite_atirando_direita = carregar("imagens/AtirandoDireita.png");
    bitmap->sprite_atirando_esquerda = carregar("imagens/AtirandoEsquerda.png");

    bitmap->projetilDireita = carregar("imagens/VacinaProjetilDireita.png");
    bitmap->projetilEsquerda = carregar("imagens/VacinaProjetilEsquerda.png");

    bitmap->cenario1 = carregar("imagens/Mapa01.png");
    bitmap->cenario2 = carregar("imagens/Mapa02.png");

    bitmap->fundoMenu = carregar("imagens/menu.png");
    bitmap->botaoJogar = carregar("imagens/jogar1.png");
    bitmap->botaoJogar2 = carregar("imagens/jogar2.png");
    bitmap->botaoRegras = carregar("imagens/regras1.png");
    bitmap->botaoRegras2 = carregar("imagens/regras2.png");
    bitmap->botaoSair = carregar("imagens/sair1.png");
    bitmap->botaoSair2 = carregar("imagens/sair2.png");
    bitmap->abaRegras = carregar("imagens/abavazia.png");
    bitmap->botaoVoltar = carregar("imagens/voltar1.png");
    bitmap->botaoVoltar2 = carregar("imagens/voltar2.png");
    bitmap->telaGameOver = carregar("imagens/telaGameOver.png");
    bitmap->botaoSairDoJogo = carregar("imagens/botaoSairDoJogo.png");
    bitmap->botaoSairDoJogo2 = carregar("imagens/botaoSairDoJogo2.png");
    bitmap->oswaldo = carregar("imagens/oswaldo.png");
    bitmap->caixaDialogo = carregar("imagens/caixaDialogo.png");

    return bitmap->sprite_andando_direita && bitmap->sprite_andando_esquerda
        && bitmap->cenario1 && bitmap->fundoMenu;
}

static void destruir_um(ALLEGRO_BITMAP** p) { //GPT QUE FEZ essa funcao static
    if (*p) { al_destroy_bitmap(*p); *p = NULL; }
}

void destruir_bitmaps(Bitmaps* bitmap) {
    if (!bitmap) return;
    destruir_um(&bitmap->zumbi_direita);
    destruir_um(&bitmap->zumbi_esquerda);
    destruir_um(&bitmap->rato_direita);
    destruir_um(&bitmap->rato_esquerda);
    destruir_um(&bitmap->mosquito_direita);
    destruir_um(&bitmap->mosquito_esquerda);
    destruir_um(&bitmap->boss_variola_esquerda);
    destruir_um(&bitmap->boss_variola_direita);
    destruir_um(&bitmap->sprite_andando_direita);
    destruir_um(&bitmap->sprite_andando_esquerda);
    destruir_um(&bitmap->sprite_atirando_direita);
    destruir_um(&bitmap->sprite_atirando_esquerda);
    destruir_um(&bitmap->projetilDireita);
    destruir_um(&bitmap->projetilEsquerda);
    destruir_um(&bitmap->cenario1);
    destruir_um(&bitmap->cenario2);
    destruir_um(&bitmap->fundoMenu);
    destruir_um(&bitmap->botaoJogar);
    destruir_um(&bitmap->botaoJogar2);
    destruir_um(&bitmap->botaoRegras);
    destruir_um(&bitmap->botaoRegras2);
    destruir_um(&bitmap->botaoSair);
    destruir_um(&bitmap->botaoSair2);
    destruir_um(&bitmap->abaRegras);
    destruir_um(&bitmap->botaoVoltar);
    destruir_um(&bitmap->botaoVoltar2);
    destruir_um(&bitmap->telaGameOver);
    destruir_um(&bitmap->botaoSairDoJogo);
    destruir_um(&bitmap->botaoSairDoJogo2);
    destruir_um(&bitmap->oswaldo);
    destruir_um(&bitmap->caixaDialogo);
}