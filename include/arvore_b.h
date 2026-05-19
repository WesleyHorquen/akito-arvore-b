#ifndef ARVORE_B_H
#define ARVORE_B_H

#include "cliente.h"

// Configurações da árvore B
#define ORDEM_DA_ARVORE 4                                // Grau da árvore (t)
#define MAXIMO_CHAVES_POR_NO (ORDEM_DA_ARVORE * 2 - 1)   // Cálculo do máximo de chaves por nó (2t - 1)
#define MINIMO_CHAVES_POR_NO (ORDEM_DA_ARVORE - 1)       // Cálculo do mínimo de chaves por nó (t - 1)

// Estrutura do nó da árvore B
typedef struct NoArvoreB {
    int quantidade_chaves;                           // Quantas chaves o nó possui atualmente
    int chaves[MAXIMO_CHAVES_POR_NO];                
    Contrato* contratos[MAXIMO_CHAVES_POR_NO];       // Vetor com ponteiros para os contratos
    struct NoArvoreB* filhos[MAXIMO_CHAVES_POR_NO + 1]; 
    int eh_folha;                                    // 1 se for folha, 0 se for nó interno
} NoArvoreB;

// Protótipos das funções da árvore B
NoArvoreB* criar_no_arvore_b(int folha);
void inserir_contrato_arvore_b(NoArvoreB** raiz, Contrato* contrato);
int remover_contrato_arvore_b(NoArvoreB** raiz, int numero_contrato);
Contrato* buscar_por_numero_contrato(NoArvoreB* no, int numero_contrato);
void buscar_por_nome_ou_cpf(NoArvoreB* no, const char* termo_busca, 
                            Contrato* resultados[], int* quantidade_encontrada);
void percorrer_arvore_em_ordem(NoArvoreB* no, Contrato* lista_contratos[], int* indice_atual);
void exibir_arvore_estrutura(NoArvoreB* no, int nivel_atual);
void liberar_arvore_b(NoArvoreB* no);
int contar_total_contratos(NoArvoreB* no);
int verificar_cpf_existente(NoArvoreB* no, const char* cpf, int contrato_ignorar);

#endif