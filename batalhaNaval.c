#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h> 

#define Tamanho_Tabuleiro 10 // Tamanho da Matriz
#define Tamanho_Navio 3 // Tamanho do Navio

int tabuleiro[Tamanho_Tabuleiro][Tamanho_Tabuleiro];

void iniciar_tabuleiro() {   //define o tamanho do tabuleiro
    for (int i = 0; i < Tamanho_Tabuleiro; i++) {
        for (int j = 0; j < Tamanho_Tabuleiro; j++) {
            tabuleiro[i][j] = 0;
        }
    }
}

void setColor(int cor) { // coloquei isso pra deixar a cor do mar azul
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, cor);
}

void imprimir_tabuleiro(){ // imprime o tabuleiro
    printf("   ");  // Espaço extra para alinhar com os números das linhas
    for (int j = 0; j < Tamanho_Tabuleiro; j++) {
        printf("%2d ", j);  // Imprime o índice da coluna
    printf("\n");

    // Imprime o tabuleiro com os números das linhas
    for (int i = 0; i < Tamanho_Tabuleiro; i++){
        printf("%2d ", i);  // Imprime o índice da linha 
        for (int j = 0; j < Tamanho_Tabuleiro; j++){
            int valor = tabuleiro[i][j];
            if (valor == 0) {
                setColor(9); // 
            } else {
                setColor(7); // 
            }

            printf("%2d ", valor);  // Imprime os zeros do tabuleiro, agora coloridos
            
        }
        setColor(7);
        printf("\n");
    }
}

int valido(int x, int y){ //verifica se ta dentro do tabuleiro
    return x >= 0 && x < Tamanho_Tabuleiro && y >= 0 && y < Tamanho_Tabuleiro;
}

int verificar_posicao(int x, int y, int orientacao){
    if(orientacao == 0){  // horizontal
        if(y + Tamanho_Navio > Tamanho_Tabuleiro ) return 0;  // verifica se o navio ultrapassa os limites

        for(int i = 0; i < Tamanho_Navio; i++){
            if(tabuleiro[x][y + i] != 0){  // verifica se o navio sobrepõe outra peça
                return 0;
            }
        }
    }else if(orientacao == 1){  // vertical
        if(x + Tamanho_Navio > Tamanho_Tabuleiro ) return 0; 

        for(int i = 0; i < Tamanho_Navio; i++){
            if(tabuleiro[x + i][y] != 0){  
                return 0;
            }
        }
    }else if (orientacao == 2){ // diagonal
        if(x + Tamanho_Navio > Tamanho_Tabuleiro || y + Tamanho_Navio > Tamanho_Tabuleiro) return 0;

        for(int i = 0; i < Tamanho_Navio; i++){
            if(tabuleiro[x + i][y + i] != 0){  
                return 0;
            }
        }
    } else{
        // sem orientação
        return 0;
    }
    return 1;
}

void colocar_navio(int x, int y, int orientacao) {     // coloca o navio no tabuleiro
    if (verificar_posicao(x, y, orientacao)) {
        for (int i = 0; i < Tamanho_Navio; i++) {
            if (orientacao == 0) { // Horizontal
                tabuleiro[x][y + i] = 3; // Marca a posição com 3 (parte do navio)
            } else if (orientacao == 1){ // Vertical
                tabuleiro[x + i][y] = 3; // Marca a posição com 3 (parte do navio)
            } else if (orientacao == 2){
                tabuleiro[x + i][y + i] = 3;
            }
        }
    } else {
        printf("Posição inválida para o navio.\n");
    }
}

enum Habilidade { CONE, CRUZ, OCTAEDRO }; 

void habilidade_cone(int x, int y) {   // definição do cone. A lógica é ir colocando "1" até formar um cone
    for (int i = 0; i < 3; i++) {      // define quantas linhas o cone vai ter
        for (int j = -i; j <= i; j++) {// para cara linha ele aumenta a largura em 2, na prática ele itera do valor negativo até o valor de I
            int nx = x + i;            // i = 1 -> j = -1, j = 0, j = 1 
            int ny = y + j;

            if (valido(nx, ny)) {
                tabuleiro[nx][ny] = 1; // substitui o 0 por 1
            }
        }
    }
}

void habilidade_cruz(int x, int y) { // definição da cruz. a lógica é basicamente a mesma do cone
    for (int i = -1; i <= 1; i++) {
        for (int j = -2; j <= 2; j++) {
            int nx = x + i;
            int ny = y + j;


            if (i == 0 || (j == 0 && abs(i) == 1)) {
                if (valido(nx, ny)) {
                    tabuleiro[nx][ny] = 1;
                }
            }
        }
    }
}

void habilidade_octaedro(int x, int y) { // definição do octaedro
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (abs(i) + abs(j) <= 2) {
                int nx = x + i;
                int ny = y + j;
                if (valido(nx, ny)) {
                    tabuleiro[nx][ny] = 1;
                }
            }
        }
    }
}

void aplicar_habilidade(int x, int y, enum Habilidade tipo) { //uma função pra selecionar a habilidade
    switch (tipo) {
        case CONE:
            habilidade_cone(x, y);
            break;
        case CRUZ:
            habilidade_cruz(x, y);
            break;
        case OCTAEDRO:
            habilidade_octaedro(x, y);
            break;
    }
}

int main(){
    int B_Naval;
    int x, y, orientacao;

    do{    
        printf("1. Iniciar jogo\n");
        printf("2. Sair do jogo\n");
        scanf("%d", &B_Naval);

        switch (B_Naval) {
            case 1:
                iniciar_tabuleiro();
                imprimir_tabuleiro();

                // Agora o jogo continua com a opção de colocar navios e habilidades
                do {
                    printf("\nEscolha a opção para colocar o navio:\n");
                    printf("1. Colocar navio\n");
                    printf("2. Exibir tabuleiro\n");
                    printf("3. Habilidades.\n");
                    printf("4. Voltar ao menu principal.\n");
                    scanf("%d", &B_Naval);

                    switch (B_Naval) {
                        case 1:
                            printf("Informe a coordenada X (linha) do navio: ");
                            scanf("%d", &x);
                            printf("Informe a coordenada Y (coluna) do navio: ");
                            scanf("%d", &y);
                            printf("Informe a orientação (0 = horizontal, 1 = vertical, 2 = diagonal): ");
                            scanf("%d", &orientacao);

                            colocar_navio(x, y, orientacao);  // Tenta colocar o navio
                            break;

                        case 2:
                            imprimir_tabuleiro(); // Exibe o tabuleiro atual
                            break;

                        case 3:                    
                            printf("Informe a coordenada X (linha) da habilidade: ");
                            scanf("%d", &x);
                            printf("Informe a coordenada Y (coluna) da habilidade: ");
                            scanf("%d", &y);

                            printf("Escolha a habilidade:\n");
                            printf("0. Cone\n");
                            printf("1. Cruz\n");
                            printf("2. Octaedro\n");

                            int tipo_habilidade;
                            scanf("%d", &tipo_habilidade);

                            if (tipo_habilidade >= 0 && tipo_habilidade <= 2) {
                                aplicar_habilidade(x, y, tipo_habilidade);
                            } else {
                                printf("Habilidade inválida.\n");
                            }
                            break;

                        default:
                            printf("Opção inválida. Tente novamente.\n");
                            break;
                    }
                } while (B_Naval != 4);  //isso é pra sair do menu quando digitar 3

                break;

            case 2:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                break;
        }
    } while (B_Naval != 2);  //isso é pra sair do menu quando digitar 2
    return 0;
}
