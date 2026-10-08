#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define LARGURA 30
#define ALTURA 15
#define TAM_MAX 100

int cobraX[TAM_MAX];
int cobraY[TAM_MAX];
int tamanho;

int comidaX, comidaY;

int direcao;

int pontos;
int recorde = 0;
int velocidade;
int fimDeJogo;

/* sorteia um lugar para a comida que nao seja em cima da cobra */
void sortearComida()
{
    int i;
    int emCimaDaCobra;

    do {
        emCimaDaCobra = 0;
        comidaX = rand() % LARGURA;
        comidaY = rand() % ALTURA;

        for (i = 0; i < tamanho; i++) {
            if (cobraX[i] == comidaX && cobraY[i] == comidaY) {
                emCimaDaCobra = 1;
            }
        }
    } while (emCimaDaCobra == 1);
}

void iniciarJogo()
{
    int i;

    tamanho = 3;
    direcao = 4;
    pontos = 0;
    velocidade = 150;
    fimDeJogo = 0;

    /* a cobra comeca no meio do mapa, deitada, olhando para a direita */
    for (i = 0; i < tamanho; i++) {
        cobraX[i] = LARGURA / 2 - i;
        cobraY[i] = ALTURA / 2;
    }

    sortearComida();
}

void desenhar()
{
    int x, y, i;
    int achou;

    system("cls");

    /* parede de cima */
    for (x = 0; x < LARGURA + 2; x++) {
        printf("#");
    }
    printf("\n");

    for (y = 0; y < ALTURA; y++) {
        printf("#");

        for (x = 0; x < LARGURA; x++) {
            if (x == cobraX[0] && y == cobraY[0]) {
                printf("O");
            }
            else if (x == comidaX && y == comidaY) {
                printf("*");
            }
            else {
                /* ve se tem algum pedaco do corpo nessa posicao */
                achou = 0;
                for (i = 1; i < tamanho; i++) {
                    if (cobraX[i] == x && cobraY[i] == y) {
                        achou = 1;
                    }
                }

                if (achou == 1) {
                    printf("o");
                }
                else {
                    printf(" ");
                }
            }
        }

        printf("#\n");
    }

    /* parede de baixo */
    for (x = 0; x < LARGURA + 2; x++) {
        printf("#");
    }
    printf("\n");

    printf("Pontos: %d    Recorde: %d\n", pontos, recorde);
    printf("W A S D para mover, X para sair\n");
}

void lerTeclado()
{
    char tecla;

    if (kbhit()) {
        tecla = getch();

        /* nao deixa a cobra virar para tras, senao ela bate nela mesma */
        if ((tecla == 'w' || tecla == 'W') && direcao != 2) {
            direcao = 1;
        }
        if ((tecla == 's' || tecla == 'S') && direcao != 1) {
            direcao = 2;
        }
        if ((tecla == 'a' || tecla == 'A') && direcao != 4) {
            direcao = 3;
        }
        if ((tecla == 'd' || tecla == 'D') && direcao != 3) {
            direcao = 4;
        }
        if (tecla == 'x' || tecla == 'X') {
            fimDeJogo = 1;
        }
    }
}

void moverCobra()
{
    int i;
    int raboX, raboY;

    /* guarda onde o rabo estava, para usar se a cobra crescer */
    raboX = cobraX[tamanho - 1];
    raboY = cobraY[tamanho - 1];

    /* cada pedaco vai para o lugar do pedaco da frente (de tras para frente) */
    for (i = tamanho - 1; i > 0; i--) {
        cobraX[i] = cobraX[i - 1];
        cobraY[i] = cobraY[i - 1];
    }

 
    if (direcao == 1) {
        cobraY[0] = cobraY[0] - 1;
    }
    if (direcao == 2) {
        cobraY[0] = cobraY[0] + 1;
    }
    if (direcao == 3) {
        cobraX[0] = cobraX[0] - 1;
    }
    if (direcao == 4) {
        cobraX[0] = cobraX[0] + 1;
    }

    
    if (cobraX[0] < 0 || cobraX[0] >= LARGURA || cobraY[0] < 0 || cobraY[0] >= ALTURA) {
        fimDeJogo = 1;
    }

   
    for (i = 1; i < tamanho; i++) {
        if (cobraX[0] == cobraX[i] && cobraY[0] == cobraY[i]) {
            fimDeJogo = 1;
        }
    }

    /* comeu a comida? */
    if (cobraX[0] == comidaX && cobraY[0] == comidaY) {
        pontos = pontos + 10;

        /* o pedaco novo nasce onde o rabo estava */
        if (tamanho < TAM_MAX) {
            cobraX[tamanho] = raboX;
            cobraY[tamanho] = raboY;
            tamanho++;
        }

        /* o jogo vai ficando mais rapido */
        if (velocidade > 60) {
            velocidade = velocidade - 5;
        }

        sortearComida();
    }
}

int main()
{
    char resposta;

    srand(time(NULL));

    do {
        iniciarJogo();

        while (fimDeJogo == 0) {
            desenhar();
            lerTeclado();
            moverCobra();
            Sleep(velocidade);
        }

        if (pontos > recorde) {
            recorde = pontos;
            printf("\nNOVO RECORDE!\n");
        }

        printf("\nFim de jogo! Voce fez %d pontos.\n", pontos);
        printf("Jogar de novo? (s/n): ");

        /* fica esperando ate a pessoa apertar s ou n */
        do {
            resposta = getch();
        } while (resposta != 's' && resposta != 'S' && resposta != 'n' && resposta != 'N');

    } while (resposta == 's' || resposta == 'S');

    printf("\n\nObrigado por jogar!\n");

    return 0;
}
