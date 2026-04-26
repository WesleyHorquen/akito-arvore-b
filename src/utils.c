#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

// Limpa o buffer do teclado para evitar problemas com scanf
void limpar_buffer_entrada() {
    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF);
}

// Aguarda o usuário pressionar Enter para continuar
void pausar_tela() {
    printf("\nPressione ENTER para continuar...");
    limpar_buffer_entrada();
    getchar();
}

// Limpa a tela do console (funciona em Windows e Linux/Mac)
void limpar_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Exibe um menu e obtém uma opção válida do usuário
int obter_opcao_menu(const char* menu, int opcao_min, int opcao_max) {
    int opcao_escolhida;
    
    printf("%s", menu);
    printf("> Escolha uma opcao (%d-%d): ", opcao_min, opcao_max);
    
    while (1) {
        if (scanf("%d", &opcao_escolhida) == 1) {
            if (opcao_escolhida >= opcao_min && opcao_escolhida <= opcao_max) {
                limpar_buffer_entrada();
                return opcao_escolhida;
            }
        } else {
            limpar_buffer_entrada();
        }
        printf(">> Opcao invalida! Digite um numero entre %d e %d: ", opcao_min, opcao_max);
    }
}

// Exibe a lista de planos disponíveis
void exibir_planos_disponiveis() {
    printf("\n============ PLANOS DISPONIVEIS ============\n");
    printf("|  1. 200 mbps - R$ 79,90/mes              |\n");
    printf("|  2. 400 mbps - R$ 99,90/mes              |\n");
    printf("|  3. 900 mbps - R$ 129,90/mes             |\n");
    printf("=========================================e\n");
}