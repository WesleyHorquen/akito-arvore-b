#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "cliente.h"

// Salva um único contrato no final do arquivo (modo append)
void salvar_contrato_no_arquivo(Contrato* contrato) {
    if (contrato == NULL) {
        return;
    }
    
    FILE* arquivo = fopen(ARQUIVO_CONTRATOS, "a");
    if (arquivo == NULL) {
        printf(">>> Erro: Nao foi possivel abrir o arquivo %s para escrita. <<<\n", ARQUIVO_CONTRATOS);
        return;
    }
    
    fprintf(arquivo, "%d|%s|%s|%s|%s|%s\n",
            contrato->numero_contrato,
            contrato->cpf,
            contrato->nome_completo,
            contrato->telefone,
            contrato->endereco_completo,
            contrato->plano_contratado);
    
    fclose(arquivo);
}

// Função auxiliar recursiva para percorrer a árvore e escrever no arquivo
void escrever_no_arquivo_recursivo(NoArvoreB* no_atual, FILE* arquivo) {
    if (no_atual == NULL) {
        return;
    }
    
    for (int indice = 0; indice < no_atual->quantidade_chaves; indice++) {
        // Primeiro escreve os filhos à esquerda
        if (!no_atual->eh_folha) {
            escrever_no_arquivo_recursivo(no_atual->filhos[indice], arquivo);
        }
        
        // Depois escreve o contrato atual
        if (no_atual->contratos[indice] != NULL) {
            Contrato* contrato_atual = no_atual->contratos[indice];
            fprintf(arquivo, "%d|%s|%s|%s|%s|%s\n",
                    contrato_atual->numero_contrato,
                    contrato_atual->cpf,
                    contrato_atual->nome_completo,
                    contrato_atual->telefone,
                    contrato_atual->endereco_completo,
                    contrato_atual->plano_contratado);
        }
    }
    
    // Escreve o último filho
    if (!no_atual->eh_folha) {
        escrever_no_arquivo_recursivo(no_atual->filhos[no_atual->quantidade_chaves], arquivo);
    }
}

// Reescreve todo o arquivo com os dados atuais da árvore
void reescrever_arquivo_completo(NoArvoreB* raiz) {
    FILE* arquivo = fopen(ARQUIVO_CONTRATOS, "w");
    if (arquivo == NULL) {
        printf(">>> Erro: Nao foi possivel abrir o arquivo %s para reescrita. <<<\n", ARQUIVO_CONTRATOS);
        return;
    }
    
    escrever_no_arquivo_recursivo(raiz, arquivo);
    fclose(arquivo);
}

// Salva todos os contratos da árvore no arquivo (substitui o conteúdo)
void salvar_todos_contratos_no_arquivo(NoArvoreB* raiz) {
    reescrever_arquivo_completo(raiz);
    printf("> Todos os contratos foram salvos no arquivo %s\n", ARQUIVO_CONTRATOS);
}

// Carrega os contratos do arquivo e insere na árvore
void carregar_contratos_do_arquivo(NoArvoreB** raiz, int* ultimo_numero_contrato) {
    FILE* arquivo = fopen(ARQUIVO_CONTRATOS, "r");
    if (arquivo == NULL) {
        printf(">> Arquivo %s nao encontrado. Iniciando com nenhum contrato.\n", ARQUIVO_CONTRATOS);
        return;
    }
    
    char linha[500];
    int quantidade_carregada = 0;
    int maior_contrato = 0;
    
    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove o caractere de nova linha do final
        linha[strcspn(linha, "\n")] = 0;
        
        // Divide a linha usando '|' como separador
        char* token = strtok(linha, "|");
        if (token == NULL) continue;
        
        int numero_contrato = atoi(token);
        
        token = strtok(NULL, "|");
        char cpf[12];
        if (token) strcpy(cpf, token);
        
        token = strtok(NULL, "|");
        char nome[100];
        if (token) strcpy(nome, token);
        
        token = strtok(NULL, "|");
        char telefone[20];
        if (token) strcpy(telefone, token);
        
        token = strtok(NULL, "|");
        char endereco[200];
        if (token) strcpy(endereco, token);
        
        token = strtok(NULL, "|");
        char plano[20];
        if (token) strcpy(plano, token);
        
        // Cria o contrato
        Contrato* novo_contrato = criar_novo_contrato(numero_contrato, cpf, nome, telefone, endereco, plano);
        if (novo_contrato != NULL) {
            inserir_contrato_arvore_b(raiz, novo_contrato);
            quantidade_carregada++;
            
            if (numero_contrato > maior_contrato) {
                maior_contrato = numero_contrato;
            }
        }
    }
    
    fclose(arquivo);
    
    // Atualiza o contador para o próximo contrato
    if (ultimo_numero_contrato != NULL) {
        *ultimo_numero_contrato = maior_contrato;
    }
    
    if (quantidade_carregada > 0) {
        printf("> Carregados %d contratos do arquivo %s\n", quantidade_carregada, ARQUIVO_CONTRATOS);
    } else {
        printf("> Arquivo %s esta vazio ou corrompido.\n", ARQUIVO_CONTRATOS);
    }
}