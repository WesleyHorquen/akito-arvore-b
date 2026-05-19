#ifndef CLIENTE_H
#define CLIENTE_H

// Estrutura que representa um contrato de internet
typedef struct Contrato {
    int numero_contrato;        // Chave primária
    char cpf[15];               // CPF (tamanho fixo)
    char* nome_completo;        // Nome completo do cliente (tamanho variável)
    char telefone[16];          // Telefone (tamanho fixo)
    char* endereco_completo;    // Endereço residencial ou comercial (tamanho variável)
    char plano_contratado[9];   // Plano (tamanho fixo)
} Contrato;

// Protótipos das funções
Contrato* criar_novo_contrato(int numero_contrato, const char* cpf, 
                               const char* nome, const char* telefone, 
                               const char* endereco, const char* plano);

void liberar_contrato(Contrato* contrato);
void exibir_dados_contrato(Contrato* contrato);
int validar_formato_cpf(const char* cpf);
int validar_plano(const char* plano);

#endif