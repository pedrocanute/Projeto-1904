#ifndef INTRODUCAO_H
#define INTRODUCAO_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>


typedef enum {

	INTRO_FADEIN,
	INTRO_EXIBINDO,
	INTRO_FADEOUT,
	INTRO_COMPLETO
}EstadoIntroducao;

typedef struct {
	char** linhas;           // Array de strings (linhas do texto)
	int numeroLinhas;        // Quantidade de linhas
	float tempoExibicao;     // Tempo que a tela fica visível
} TelaTexto;


typedef struct {

	EstadoIntroducao estado;
	float alfa;
	float tempoAtual;
	float tempoFadeIn;
	float tempoExibicao;
	float velocidadeFade;
	ALLEGRO_FONT* fonte;
	TelaTexto* telas;
	int numeroTelas;
	int telaAtual;
	bool concluido;
}TelaIntroducao;

void inicializar_introducao(TelaIntroducao* intro, const char* caminhoFonte, int tamanhoFonte, int numeroTelas);

void adicionar_tela(TelaIntroducao* intro, int indiceTela, char** linhas, int numeroLinhas, float tempoExibicao);

void atualizar_introducao(TelaIntroducao* intro, float deltaTime);

void desenhar_introducao(TelaIntroducao* intro, int larguraTela, int alturaTela);

void destruir_introducao(TelaIntroducao* intro);

#endif