#ifndef UTILS_H
#define UTILS_H

#include "arvore_b.h"

// Cores ANSI
#define COR_AMARELO "\033[1;33m"
#define COR_AZUL "\033[1;34m"
#define COR_RESET "\033[0m"

// Protótipos das funções utilitárias
void limpar_buffer_entrada();
void pausar_tela();
int obter_opcao_menu(const char* menu, int opcao_min, int opcao_max);
void exibir_planos_disponiveis();

#endif