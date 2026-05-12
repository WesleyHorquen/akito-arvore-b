#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "arvore_b.h"

#define ARQUIVO_CONTRATOS "data/contratos.txt"

// Protótipos das funções de persistência
void salvar_contrato_no_arquivo(Contrato* contrato);
void salvar_todos_contratos_no_arquivo(NoArvoreB* raiz);
void carregar_contratos_do_arquivo(NoArvoreB** raiz, int* ultimo_numero_contrato);
void reescrever_arquivo_completo(NoArvoreB* raiz);

#endif