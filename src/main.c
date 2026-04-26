#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_b.h"
#include "persistencia.h"
#include "cliente.h"
#include "utils.h"

#define AMARELO "\033[1;33m"
#define AZUL "\033[1;34m"
#define RESET "\033[0m"

// Variáveis globais
NoArvoreB* raiz_arvore = NULL;
int ultimo_numero_contrato = 0;

// Protótipos das funções do sistema
void cadastrar_novo_contrato();
void excluir_contrato_existente();
void buscar_contrato_por_nome_ou_cpf();
void listar_todos_contratos();
void visualizar_estrutura_arvore();
void salvar_e_sair();

// Função para gerar novo número de contrato
int gerar_proximo_numero_contrato() {
    return ++ultimo_numero_contrato;
}

// Função para validar e obter um CPF válido
void obter_cpf_valido(char* cpf_destino, int contrato_ignorar) {
    int cpf_valido = 0;
    
    while (!cpf_valido) {
        printf("CPF (formato 000.000.000-00): ");
        scanf(" %[^\n]", cpf_destino);
        
        if (!validar_formato_cpf(cpf_destino)) {
            printf("> Formato de CPF inválido! Use o formato 000.000.000-00\n");
            continue;
        }
        
        // Verifica se o CPF já existe (opcional - pode ter múltiplos contratos)
        // Como você pediu que pode haver inúmeros contratos num mesmo CPF,
        // não vamos bloquear - apenas avisar
        if (verificar_cpf_existente(raiz_arvore, cpf_destino, contrato_ignorar)) {
            printf(">>> ATENCAO: Este CPF ja possui contrato(s) cadastrado(s). <<<\n");
            printf("   Voce pode cadastrar outro contrato para o mesmo CPF.\n");
        }
        
        cpf_valido = 1;
    }
}

// Função para obter o plano escolhido
void obter_plano_escolhido(char* plano_destino) {
    int opcao_plano;
    
    exibir_planos_disponiveis();
    
    do {
        printf("> Escolha o plano (1, 2 ou 3): ");
        scanf("%d", &opcao_plano);
        
        switch (opcao_plano) {
            case 1:
                strcpy(plano_destino, "200mbps");
                break;
            case 2:
                strcpy(plano_destino, "400mbps");
                break;
            case 3:
                strcpy(plano_destino, "900mbps");
                break;
            default:
                printf("> Opcao invalida! Escolha 1, 2 ou 3.\n");
                break;
        }
    } while (opcao_plano < 1 || opcao_plano > 3);
}

// CADASTRAR NOVO CONTRATO
void cadastrar_novo_contrato() {
    printf("\n=================================================================\n");
    printf("|                    CADASTRAR NOVO CONTRATO                    |\n");
    printf("=================================================================\n\n");
    
    // Gera número automático do contrato
    int numero_contrato = gerar_proximo_numero_contrato();
    printf("> Numero do contrato: %d\n", numero_contrato);
    
    // Coleta os dados do cliente
    char cpf[12];
    char nome_completo[100];
    char telefone[20];
    char endereco_completo[200];
    char plano_escolhido[20];
    
    obter_cpf_valido(cpf, -1);
    
    printf("Nome completo: ");
    scanf(" %[^\n]", nome_completo);
    
    printf("Telefone (formato (99) 99999-9999): ");
    scanf(" %[^\n]", telefone);
    
    printf("Endereco completo: ");
    scanf(" %[^\n]", endereco_completo);
    
    obter_plano_escolhido(plano_escolhido);
    
    // Cria o contrato
    Contrato* novo_contrato = criar_novo_contrato(numero_contrato, cpf, nome_completo, 
                                                   telefone, endereco_completo, plano_escolhido);
    
    if (novo_contrato == NULL) {
        printf(">>> Erro: Nao foi possivel criar o contrato.\n");
        return;
    }
    
    // Insere na árvore B
    inserir_contrato_arvore_b(&raiz_arvore, novo_contrato);
    
    // Salva no arquivo
    salvar_contrato_no_arquivo(novo_contrato);
    
    printf("\n>> CONTRATO CADASTRADO COM SUCESSO! <<\n");
    printf("   Numero do contrato: %d\n", numero_contrato);
    printf("   Cliente: %s\n", nome_completo);
    printf("   Plano: %s\n", plano_escolhido);
}

// EXCLUIR CONTRATO EXISTENTE
void excluir_contrato_existente() {
    printf("\n=================================================================\n");
    printf("|                      EXCLUIR CONTRATO                         |\n");
    printf("=================================================================\n\n");
    
    char termo_busca[100];
    printf("Digite o nome ou CPF do cliente: ");
    scanf(" %[^\n]", termo_busca);
    
    Contrato* resultados[100];
    int quantidade_encontrada = 0;
    
    buscar_por_nome_ou_cpf(raiz_arvore, termo_busca, resultados, &quantidade_encontrada);
    
    if (quantidade_encontrada == 0) {
        printf("> Nenhum contrato encontrado com o termo '%s'\n", termo_busca);
        pausar_tela();
        return;
    }
    
    printf("\n============ CONTRATOS ENCONTRADOS ============\n");
    for (int indice = 0; indice < quantidade_encontrada; indice++) {
        printf("| %2d. Contrato #%-6d | %-20s |\n", 
               indice + 1, 
               resultados[indice]->numero_contrato,
               resultados[indice]->nome_completo);
        printf("|    CPF: %-11s | Plano: %-8s |\n",
               resultados[indice]->cpf,
               resultados[indice]->plano_contratado);
        printf("===========================================\n");
    }
    printf("===========================================\n");
    
    int opcao_escolhida;
    printf("\n> Qual contrato deseja excluir? (1-%d): ", quantidade_encontrada);
    scanf("%d", &opcao_escolhida);
    
    if (opcao_escolhida < 1 || opcao_escolhida > quantidade_encontrada) {
        printf(">> Opcao invalida!\n");
        pausar_tela();
        return;
    }
    
    int contrato_para_remover = resultados[opcao_escolhida - 1]->numero_contrato;
    
    // Remove da árvore
    if (remover_contrato_arvore_b(&raiz_arvore, contrato_para_remover)) {
        // Reescreve o arquivo sem o contrato removido
        reescrever_arquivo_completo(raiz_arvore);
        printf("\n> Contrato #%d excluido com sucesso!\n", contrato_para_remover);
    } else {
        printf("\n>> Erro ao excluir o contrato!\n");
    }
    
    pausar_tela();
}

// BUSCAR CONTRATO POR NOME OU CPF
void buscar_contrato_por_nome_ou_cpf() {
    printf("\n==============================================================\n");
    printf("|                    BUSCAR CONTRATO                           |\n");
    printf("==============================================================\n\n");
    
    char termo_busca[100];
    printf("> Digite o nome ou CPF para busca: ");
    scanf(" %[^\n]", termo_busca);
    
    Contrato* resultados[100];
    int quantidade_encontrada = 0;
    
    buscar_por_nome_ou_cpf(raiz_arvore, termo_busca, resultados, &quantidade_encontrada);
    
    if (quantidade_encontrada == 0) {
        printf(">> Nenhum contrato encontrado com o termo '%s'\n", termo_busca);
        pausar_tela();
        return;
    }
    
    printf("\n==================== RESULTADOS DA BUSCA ====================\n");
    for (int indice = 0; indice < quantidade_encontrada; indice++) {
        printf("\n");
        exibir_dados_contrato(resultados[indice]);
    }
    printf("==============================================================\n");
    printf("\n> Total de contratos encontrados: %d\n", quantidade_encontrada);
    
    pausar_tela();
}

// LISTAR TODOS OS CONTRATOS
// LISTAR TODOS OS CONTRATOS - Versão ASCII simples (sem caracteres especiais)
void listar_todos_contratos() {
    printf("\n");
    printf("=============================================================================\n");
    printf("                    TODOS OS CONTRATOS CADASTRADOS                          \n");
    printf("=============================================================================\n\n");
    
    int total_contratos = contar_total_contratos(raiz_arvore);
    
    if (total_contratos == 0) {
        printf(">>> Nenhum contrato cadastrado no sistema.\n");
        pausar_tela();
        return;
    }
    
    // Coleta todos os contratos em ordem
    Contrato* lista_ordenada[1000];
    int indice_atual = 0;
    
    percorrer_arvore_em_ordem(raiz_arvore, lista_ordenada, &indice_atual);
    
    // Cabeçalho da tabela
    printf("+----+------------+------------------------------------------+------------+\n");
    printf("| #  | Contrato   | Nome do Cliente                          | Plano      |\n");
    printf("+----+------------+------------------------------------------+------------+\n");
    
    for (int indice = 0; indice < total_contratos; indice++) {
        Contrato* contrato_atual = lista_ordenada[indice];
        
        // Limita o nome a 40 caracteres para caber na tabela
        char nome_curto[41];
        strncpy(nome_curto, contrato_atual->nome_completo, 40);
        nome_curto[40] = '\0';
        
        printf("| %2d | #%-8d  | %-40s | %-10s |\n",
               indice + 1,
               contrato_atual->numero_contrato,
               nome_curto,
               contrato_atual->plano_contratado);
        
        if (indice < total_contratos - 1) {
            printf("+----+------------+------------------------------------------+------------+\n");
        }
    }
    
    printf("+----+------------+------------------------------------------+------------+\n");
    printf("\n>>> Total de contratos: %d\n", total_contratos);
    
    pausar_tela();
}

// VISUALIZAR ESTRUTURA DA ÁRVORE B
void visualizar_estrutura_arvore() {
    printf("\n==============================================================\n");
    printf("|                    ESTRUTURA DA ARVORE B                      |\n");
    printf("==============================================================\n\n");
    
    if (raiz_arvore == NULL || raiz_arvore->quantidade_chaves == 0) {
        printf("> Arvore B esta vazia.\n");
        pausar_tela();
        return;
    }
    
    printf(">> Configuração da Arvore B:\n");
    printf("   - Ordem (t): %d\n", ORDEM_DA_ARVORE);
    printf("   - Maximo de chaves por no: %d\n", MAXIMO_CHAVES_POR_NO);
    printf("   - Minimo de chaves por no: %d\n", MINIMO_CHAVES_POR_NO);
    printf("   - Total de contratos: %d\n\n", contar_total_contratos(raiz_arvore));
    
    printf(">> Representacao visual da arvore:\n");
    printf("   (Os numeros dentro dos nos sao os numeros dos contratos)\n\n");
    
    exibir_arvore_estrutura(raiz_arvore, 0);
    
    printf("\n\n> Legenda:\n");
    printf("   - NO [x, y, z] = No com chaves x, y, z\n");
    printf("   - (FOLHA) = No folha (sem filhos)\n");
    printf("   - (INTERNO) = No interno (tem filhos)\n");
    
    pausar_tela();
}

// SALVAR E SAIR DO SISTEMA
void salvar_e_sair() {
    printf("\n==============================================================\n");
    printf("|                      SALVANDO E SAINDO                       |\n");
    printf("==============================================================\n\n");
    
    salvar_todos_contratos_no_arquivo(raiz_arvore);
    
    printf("> Dados salvos em: %s\n", ARQUIVO_CONTRATOS);
    printf("> Encerrando o sistema da Provedora de Internet...\n");
}

// EXIBIR MENU PRINCIPAL
void exibir_menu_principal() {
    limpar_tela();
    printf("\n");
    printf("================================================================================\n");
    printf("\n");
    printf(AMARELO"                                  A K I T O                                     \n" RESET);
    printf("\n");
    printf(AZUL"                     SISTEMA DE GERENCIAMENTO DE CONTRATOS                       \n" RESET);
    printf(AZUL"                            PROVEDORA DE INTERNET                                \n" RESET);
    printf("\n");
    printf("================================================================================\n");
    printf("\n");
    printf("                               MENU PRINCIPAL                                    \n");
    printf("\n");
    printf("                          1. Cadastrar novo contrato                            \n");
    printf("                          2. Excluir contrato                                   \n");
    printf("                          3. Buscar contrato (por nome ou CPF)                  \n");
    printf("                          4. Listar todos os contratos                          \n");
    printf("                          5. Visualizar estrutura da Arvore B                   \n");
    printf("                          6. Salvar dados e sair                                \n");
    printf("\n");
    printf("================================================================================\n");
    printf("\n");
}

// FUNÇÃO PRINCIPAL
int main() {
    // Carrega os dados salvos anteriormente
    printf("\n> Carregando dados do arquivo %s...\n", ARQUIVO_CONTRATOS);
    carregar_contratos_do_arquivo(&raiz_arvore, &ultimo_numero_contrato);
    
    int opcao_selecionada;
    
    do {
        exibir_menu_principal();
        printf("> Digite sua opcao: ");
        scanf("%d", &opcao_selecionada);
        limpar_buffer_entrada();
        
        switch (opcao_selecionada) {
            case 1:
                cadastrar_novo_contrato();
                break;
            case 2:
                excluir_contrato_existente();
                break;
            case 3:
                buscar_contrato_por_nome_ou_cpf();
                break;
            case 4:
                listar_todos_contratos();
                break;
            case 5:
                visualizar_estrutura_arvore();
                break;
            case 6:
                salvar_e_sair();
                break;
            default:
                printf("> Opção invalida! Tente novamente.\n");
                pausar_tela();
                break;
        }
    } while (opcao_selecionada != 6);
    
    // Libera a memória alocada
    liberar_arvore_b(raiz_arvore);
    
    return 0;
}