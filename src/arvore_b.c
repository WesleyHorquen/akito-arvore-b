#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_b.h"

// Cria um novo nó da árvore B
NoArvoreB* criar_no_arvore_b(int folha) {
    NoArvoreB* novo_no = (NoArvoreB*)malloc(sizeof(NoArvoreB));
    if (novo_no == NULL) {
        printf(">>> Erro: Falha ao alocar memoria para noh da arvore B.\n");
        return NULL;
    }
    
    novo_no->quantidade_chaves = 0;
    novo_no->eh_folha = folha;
    
    // Inicializa todos os ponteiros como NULL
    for (int indice = 0; indice < MAXIMO_CHAVES_POR_NO; indice++) {
        novo_no->contratos[indice] = NULL;
        novo_no->filhos[indice] = NULL;
    }
    novo_no->filhos[MAXIMO_CHAVES_POR_NO] = NULL;
    
    return novo_no;
}

// Divide um nó filho que está cheio
void dividir_no_filho(NoArvoreB* no_pai, int posicao_filho, NoArvoreB* filho_cheio) {
    // Cria um novo nó que vai receber a metade direita do filho cheio
    NoArvoreB* novo_no = criar_no_arvore_b(filho_cheio->eh_folha);
    novo_no->quantidade_chaves = MINIMO_CHAVES_POR_NO;
    
    // Copia a segunda metade das chaves para o novo nó
    for (int indice = 0; indice < MINIMO_CHAVES_POR_NO; indice++) {
        novo_no->chaves[indice] = filho_cheio->chaves[indice + MINIMO_CHAVES_POR_NO + 1];
        novo_no->contratos[indice] = filho_cheio->contratos[indice + MINIMO_CHAVES_POR_NO + 1];
    }
    
    // Copia os filhos se não for folha
    if (!filho_cheio->eh_folha) {
        for (int indice = 0; indice <= MINIMO_CHAVES_POR_NO; indice++) {
            novo_no->filhos[indice] = filho_cheio->filhos[indice + MINIMO_CHAVES_POR_NO + 1];
        }
    }
    
    // Reduz a quantidade de chaves do filho cheio
    filho_cheio->quantidade_chaves = MINIMO_CHAVES_POR_NO;
    
    // Abre espaço no pai para a nova chave que vai subir
    for (int indice = no_pai->quantidade_chaves; indice > posicao_filho; indice--) {
        no_pai->filhos[indice + 1] = no_pai->filhos[indice];
    }
    no_pai->filhos[posicao_filho + 1] = novo_no;
    
    // Move as chaves no pai para abrir espaço
    for (int indice = no_pai->quantidade_chaves; indice > posicao_filho; indice--) {
        no_pai->chaves[indice] = no_pai->chaves[indice - 1];
        no_pai->contratos[indice] = no_pai->contratos[indice - 1];
    }
    
    // Sobe a chave mediana para o pai
    no_pai->chaves[posicao_filho] = filho_cheio->chaves[MINIMO_CHAVES_POR_NO];
    no_pai->contratos[posicao_filho] = filho_cheio->contratos[MINIMO_CHAVES_POR_NO];
    no_pai->quantidade_chaves++;
}

// Insere uma chave em um nó que não está cheio
void inserir_em_no_nao_cheio(NoArvoreB* no_atual, int numero_contrato, Contrato* contrato) {
    int indice = no_atual->quantidade_chaves - 1;
    
    if (no_atual->eh_folha) {
        // Procura a posição correta para inserir (mantendo ordem)
        while (indice >= 0 && no_atual->chaves[indice] > numero_contrato) {
            no_atual->chaves[indice + 1] = no_atual->chaves[indice];
            no_atual->contratos[indice + 1] = no_atual->contratos[indice];
            indice--;
        }
        
        // Insere o novo contrato
        no_atual->chaves[indice + 1] = numero_contrato;
        no_atual->contratos[indice + 1] = contrato;
        no_atual->quantidade_chaves++;
    } else {
        // Encontra o filho apropriado para inserir
        while (indice >= 0 && no_atual->chaves[indice] > numero_contrato) {
            indice--;
        }
        indice++;
        
        // Verifica se o filho está cheio
        if (no_atual->filhos[indice]->quantidade_chaves == MAXIMO_CHAVES_POR_NO) {
            dividir_no_filho(no_atual, indice, no_atual->filhos[indice]);
            
            // Após a divisão, decide qual filho usar
            if (numero_contrato > no_atual->chaves[indice]) {
                indice++;
            }
        }
        
        // Insere recursivamente no filho escolhido
        inserir_em_no_nao_cheio(no_atual->filhos[indice], numero_contrato, contrato);
    }
}

// Função principal para inserir um contrato na árvore B
void inserir_contrato_arvore_b(NoArvoreB** raiz, Contrato* contrato) {
    if (*raiz == NULL) {
        // Árvore vazia, cria a raiz
        *raiz = criar_no_arvore_b(1);
        (*raiz)->chaves[0] = contrato->numero_contrato;
        (*raiz)->contratos[0] = contrato;
        (*raiz)->quantidade_chaves = 1;
        return;
    }
    
    // Se a raiz está cheia, a árvore cresce em altura
    if ((*raiz)->quantidade_chaves == MAXIMO_CHAVES_POR_NO) {
        NoArvoreB* nova_raiz = criar_no_arvore_b(0);
        nova_raiz->filhos[0] = *raiz;
        dividir_no_filho(nova_raiz, 0, *raiz);
        *raiz = nova_raiz;
        inserir_em_no_nao_cheio(*raiz, contrato->numero_contrato, contrato);
    } else {
        inserir_em_no_nao_cheio(*raiz, contrato->numero_contrato, contrato);
    }
}

// Busca um contrato pelo número do contrato
Contrato* buscar_por_numero_contrato(NoArvoreB* no_atual, int numero_contrato) {
    if (no_atual == NULL) {
        return NULL;
    }
    
    int indice = 0;
    while (indice < no_atual->quantidade_chaves && numero_contrato > no_atual->chaves[indice]) {
        indice++;
    }
    
    // Verifica se encontrou no nó atual
    if (indice < no_atual->quantidade_chaves && numero_contrato == no_atual->chaves[indice]) {
        return no_atual->contratos[indice];
    }
    
    // Se é folha, não encontrou
    if (no_atual->eh_folha) {
        return NULL;
    }
    
    // Desce para o filho apropriado
    return buscar_por_numero_contrato(no_atual->filhos[indice], numero_contrato);
}

// Função auxiliar para busca por nome ou CPF
void buscar_por_nome_ou_cpf(NoArvoreB* no_atual, const char* termo_busca, 
                            Contrato* resultados[], int* quantidade_encontrada) {
    if (no_atual == NULL || *quantidade_encontrada >= 100) {
        return;
    }
    
    for (int indice = 0; indice < no_atual->quantidade_chaves; indice++) {
        Contrato* contrato_atual = no_atual->contratos[indice];
        
        if (contrato_atual != NULL) {
            // Verifica se o termo aparece no nome ou no CPF
            if (strstr(contrato_atual->nome_completo, termo_busca) != NULL ||
                strstr(contrato_atual->cpf, termo_busca) != NULL) {
                resultados[*quantidade_encontrada] = contrato_atual;
                (*quantidade_encontrada)++;
            }
        }
        
        // Desce para os filhos antes da chave atual
        if (!no_atual->eh_folha) {
            buscar_por_nome_ou_cpf(no_atual->filhos[indice], termo_busca, 
                                  resultados, quantidade_encontrada);
        }
    }
    
    // Desce para o último filho
    if (!no_atual->eh_folha && no_atual->filhos[no_atual->quantidade_chaves] != NULL) {
        buscar_por_nome_ou_cpf(no_atual->filhos[no_atual->quantidade_chaves], 
                              termo_busca, resultados, quantidade_encontrada);
    }
}

// Remove um contrato da árvore
int remover_contrato_arvore_b(NoArvoreB** raiz, int numero_contrato) {
    if (*raiz == NULL) {
        return 0;
    }
    
    NoArvoreB* no_atual = *raiz;
    int indice = 0;
    
    // Encontra a posição da chave
    while (indice < no_atual->quantidade_chaves && numero_contrato > no_atual->chaves[indice]) {
        indice++;
    }
    
    // Se encontrou a chave neste nó
    if (indice < no_atual->quantidade_chaves && numero_contrato == no_atual->chaves[indice]) {
        // Remove a chave e desloca os elementos
        for (int j = indice; j < no_atual->quantidade_chaves - 1; j++) {
            no_atual->chaves[j] = no_atual->chaves[j + 1];
            no_atual->contratos[j] = no_atual->contratos[j + 1];
        }
        no_atual->quantidade_chaves--;
        return 1;
    }
    
    // Se é folha, não encontrou
    if (no_atual->eh_folha) {
        return 0;
    }
    
    // Desce para o filho apropriado
    return remover_contrato_arvore_b(&(no_atual->filhos[indice]), numero_contrato);
}

// Percorre a árvore em ordem e preenche um vetor com todos os contratos
void percorrer_arvore_em_ordem(NoArvoreB* no_atual, Contrato* lista_contratos[], int* indice_atual) {
    if (no_atual == NULL) {
        return;
    }
    
    for (int indice = 0; indice < no_atual->quantidade_chaves; indice++) {
        // Primeiro, percorre o filho à esquerda
        if (!no_atual->eh_folha) {
            percorrer_arvore_em_ordem(no_atual->filhos[indice], lista_contratos, indice_atual);
        }
        
        // Depois, adiciona a chave atual
        if (no_atual->contratos[indice] != NULL) {
            lista_contratos[*indice_atual] = no_atual->contratos[indice];
            (*indice_atual)++;
        }
        
        // O filho à direita será percorrido na próxima iteração ou após o loop
    }
    
    // Percorre o último filho (à direita de todas as chaves)
    if (!no_atual->eh_folha) {
        percorrer_arvore_em_ordem(no_atual->filhos[no_atual->quantidade_chaves], 
                                 lista_contratos, indice_atual);
    }
}

// Exibe a estrutura da árvore B visualmente
void exibir_arvore_estrutura(NoArvoreB* no_atual, int nivel_atual) {
    if (no_atual == NULL) {
        return;
    }
    
    // Imprime indentação
    for (int i = 0; i < nivel_atual; i++) {
        printf("    ");
    }
    
    // Imprime as chaves do nó atual
    printf("> Noh [");
    for (int i = 0; i < no_atual->quantidade_chaves; i++) {
        printf("%d", no_atual->chaves[i]);
        if (i < no_atual->quantidade_chaves - 1) {
            printf(", ");
        }
    }
    printf("]");
    
    if (no_atual->eh_folha) {
        printf(" (FOLHA)\n");
    } else {
        printf(" (INTERNO)\n");
        // Imprime recursivamente os filhos
        for (int i = 0; i <= no_atual->quantidade_chaves; i++) {
            if (no_atual->filhos[i] != NULL) {
                exibir_arvore_estrutura(no_atual->filhos[i], nivel_atual + 1);
            }
        }
    }
}

// Libera toda a memória alocada pela árvore
void liberar_arvore_b(NoArvoreB* no_atual) {
    if (no_atual == NULL) {
        return;
    }
    
    // Libera os filhos primeiro
    if (!no_atual->eh_folha) {
        for (int i = 0; i <= no_atual->quantidade_chaves; i++) {
            liberar_arvore_b(no_atual->filhos[i]);
        }
    }
    
    // Libera os contratos
    for (int i = 0; i < no_atual->quantidade_chaves; i++) {
        if (no_atual->contratos[i] != NULL) {
            liberar_contrato(no_atual->contratos[i]);
        }
    }
    
    // Libera o nó
    free(no_atual);
}

// Conta quantos contratos existem na árvore
int contar_total_contratos(NoArvoreB* no_atual) {
    if (no_atual == NULL) {
        return 0;
    }
    
    int total = no_atual->quantidade_chaves;
    
    if (!no_atual->eh_folha) {
        for (int i = 0; i <= no_atual->quantidade_chaves; i++) {
            total += contar_total_contratos(no_atual->filhos[i]);
        }
    }
    
    return total;
}

// Verifica se um CPF já existe na árvore
int verificar_cpf_existente(NoArvoreB* no_atual, const char* cpf, int contrato_ignorar) {
    if (no_atual == NULL) {
        return 0;
    }
    
    for (int indice = 0; indice < no_atual->quantidade_chaves; indice++) {
        Contrato* contrato_atual = no_atual->contratos[indice];
        if (contrato_atual != NULL && contrato_atual->numero_contrato != contrato_ignorar) {
            if (strcmp(contrato_atual->cpf, cpf) == 0) {
                return 1;
            }
        }
    }
    
    if (!no_atual->eh_folha) {
        for (int indice = 0; indice <= no_atual->quantidade_chaves; indice++) {
            if (verificar_cpf_existente(no_atual->filhos[indice], cpf, contrato_ignorar)) {
                return 1;
            }
        }
    }
    
    return 0;
}