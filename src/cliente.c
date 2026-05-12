#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cliente.h"

// Cria um novo contrato com os dados fornecidos
Contrato* criar_novo_contrato(int numero_contrato, const char* cpf, 
                               const char* nome_completo, const char* telefone, 
                               const char* endereco_completo, const char* plano) {
    Contrato* novo_contrato = (Contrato*)malloc(sizeof(Contrato));
    if (novo_contrato == NULL) {
        printf(">>> Erro: Falha ao alocar memoria para o contrato.\n");
        return NULL;
    }
    
    novo_contrato->numero_contrato = numero_contrato;
    strcpy(novo_contrato->cpf, cpf);
    novo_contrato->nome_completo = malloc(strlen(nome_completo) + 1);

    if (novo_contrato->nome_completo == NULL) {
        free(novo_contrato);
    return NULL;
    }
    strcpy(novo_contrato->nome_completo, nome_completo);
    novo_contrato->endereco_completo = malloc(strlen(endereco_completo) + 1);

    if (novo_contrato->endereco_completo == NULL) {

        free(novo_contrato->nome_completo);
        free(novo_contrato);
    return NULL;
}

    strcpy(novo_contrato->endereco_completo, endereco_completo);
    strcpy(novo_contrato->telefone, telefone);
    strcpy(novo_contrato->plano_contratado, plano);
    
    return novo_contrato;
}

// Libera a memória alocada para um contrato
void liberar_contrato(Contrato* contrato) {
    if (contrato != NULL) {
        free(contrato->nome_completo);
        free(contrato->endereco_completo);
        free(contrato);
    }
}

// Exibe todos os dados de um contrato na tela
void exibir_dados_contrato(Contrato* contrato) {
    if (contrato == NULL) {
        printf(">> Contrato invalido (NULL).\n");
        return;
    }
    
    printf("===========================================\n");
    printf("| Numero do Contrato: %d\n", contrato->numero_contrato);
    printf("| Nome: %s\n", contrato->nome_completo);
    printf("| CPF: %s\n", contrato->cpf);
    printf("| Telefone: %s\n", contrato->telefone);
    printf("| Endereco: %s\n", contrato->endereco_completo);
    printf("| Plano: %s\n", contrato->plano_contratado);
    printf("===========================================\n");
}

// Valida se o CPF está no formato correto
int validar_formato_cpf(const char* cpf) {
    // Verifica o tamanho
    if (strlen(cpf) != 14) return 0;
    
    // Verifica os pontos e traço nas posições corretas
    if (cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-') return 0;
    
    // Verifica se os demais caracteres são dígitos
    for (int indice = 0; indice < 14; indice++) {
        if (indice == 3 || indice == 7 || indice == 11) continue;
        if (cpf[indice] < '0' || cpf[indice] > '9') return 0;
    }
    
    return 1;
}

// Verifica se o plano escolhido é válido
int validar_plano(const char* plano) {
    return (strcmp(plano, "200 mbps") == 0 ||
            strcmp(plano, "400 mbps") == 0 ||
            strcmp(plano, "900 mbps") == 0);
}