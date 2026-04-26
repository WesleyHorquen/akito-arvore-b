#ifndef CLIENTE_H
#define CLIENTE_H

// Estrutura que representa um contrato de internet
typedef struct Contrato {
    int numero_contrato;        // Chave primária (única)
    char cpf[12];               // Formato: 000.000.000-00
    char nome_completo[100];    // Nome completo do cliente
    char telefone[20];          // Formato: (99) 99999-9999
    char endereco_completo[200];// Endereço residencial ou comercial
    char plano_contratado[20];  // 200mbps, 400mbps ou 900mbps
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