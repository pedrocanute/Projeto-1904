# # Projeto 1904

**Projeto 1904** é um jogo 2D side-scrolling desenvolvido em C com a biblioteca Allegro 5. Ambientado no Rio de Janeiro de 1904, durante a Revolta da Vacina, você controla Oswaldo Cruz em uma campanha sanitária contra epidemias como varíola, febre amarela e peste bubônica. Proteja uma caravana de soldados enquanto combate vetores de doenças em cenários urbanos em constante movimento.


# ## Contexto Histórico

O jogo recria o contexto da reforma sanitária liderada por Oswaldo Cruz, convocada pelo presidente Rodrigues Alves. A cidade enfrenta crises epidêmicas agravadas por boatos e protestos contra a vacinação obrigatória. Missões progressivas abordam saneamento, combate a vetores e conscientização, culminando na transformação da saúde pública brasileira.

## ## Mecânicas de Jogo

-   **Movimento**: Plataforma 2D com câmera que segue a caravana em movimento contínuo para a direita.
    
-   **Armas**  (ciclo com tecla R):
Vacina - Projétil à distância - atinge apenas Zumbis (varíola)
Veneno - Projétil com animação - Mosquitos (febre)
Vassoura - Ataque corpo a corpo - Ratos (peste)

-   **Inimigos**: Zumbis, ratos, mosquitos e bosses com minions, buffs por fase e IA de perseguição.
-   **Sistema de Fases**: 3 fases com metas de eliminação (20 inimigos cada), diálogos narrativos e barras de infecção/vida do boss.
- **UI**: Barras de infecção na caravana, vida de bosses, sons sincronizados com animações.

## ## Como Jogar

**Controles Básicos**:
-   **W/A/S/D**  ou  **Setas**: Mover (Shift para corrida).
    
-   **Espaço**: Atacar/disparar (depende da arma equipada).
    
-   **R**: Trocar arma (Vacina/Veneno/Vassoura).
    
-   **ESC**: Pausa/Menu.

**Objetivo**: Avance com a caravana eliminando inimigos para completar fases. Evite colisões que aumentam a barra de infecção (game over em 400). Bosses spawnam automaticamente e liberam minions.
**Dicas**:
-   Use a arma correta por tipo de inimigo para dano efetivo.
    
-   Fique atento a diálogos e cutscenes entre fases.
    
-   A caravana regenera infecção lentamente; priorize defesa.

# ## Instalação e Execução


# Clone o repositório
git clone https://github.com/SEU_USUARIO/Projeto1904.git
cd Projeto1904

# Compile (requer Allegro 5)
make  # ou gcc *.c -o jogo `pkg-config --libs --cflags allegro-5 allegro_dialog-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_image-5 allegro_audio-5 allegro_acodec-5`

# Execute
./jogo
