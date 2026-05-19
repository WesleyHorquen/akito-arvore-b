#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore_b.h"
#include "persistencia.h"
#include "cliente.h"
#include "utils.h"

// Variáveis globais
NoArvoreB* raiz_arvore = NULL;
int ultimo_numero_contrato = 0;

// Protótipos das funções do sistema
void cadastrar_novo_contrato();
void modificar_contrato_existente();
void excluir_contrato_existente();
void buscar_contrato_por_id();
void buscar_contrato_por_nome_ou_cpf();
void listar_todos_contratos();
void visualizar_estrutura_arvore();
void salvar_e_sair();

// Função para gerar novo número de contrato
int gerar_proximo_numero_contrato() {
    return ++ultimo_numero_contrato;
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
    
    int numero_contrato = gerar_proximo_numero_contrato();
    printf("> Numero do contrato: %d\n", numero_contrato);
    
    char cpf[15];
    char buffer_nome[200];
    char* nome_completo;
    char telefone[16];
    char buffer_endereco[300];
    char* endereco_completo;
    char plano_escolhido[9];
    
    printf("Nome completo: ");
    scanf(" %[^\n]", buffer_nome);
    nome_completo = malloc(strlen(buffer_nome) + 1);
    if (nome_completo == NULL) {
        printf(">> Erro de memoria.\n");
        return;
    }
    strcpy(nome_completo, buffer_nome);

    printf("CPF: ");
    scanf(" %[^\n]", cpf);
    
    printf("Telefone:");
    scanf(" %[^\n]", telefone);
    
    printf("Endereco completo: ");
    scanf(" %[^\n]", buffer_endereco);
    endereco_completo = malloc(strlen(buffer_endereco) + 1);
    if (endereco_completo == NULL) {
        printf(">> Erro de memoria.\n");
        free(nome_completo);
        return;
    }
    strcpy(endereco_completo, buffer_endereco);

    obter_plano_escolhido(plano_escolhido);
    
    Contrato* novo_contrato = criar_novo_contrato(numero_contrato, cpf, nome_completo, 
                                                   telefone, endereco_completo, plano_escolhido);
    
    if (novo_contrato == NULL) {
        printf(">>> Erro: Nao foi possivel criar o contrato.\n");
        free(nome_completo);
        free(endereco_completo);
        return;
    }
    
    inserir_contrato_arvore_b(&raiz_arvore, novo_contrato);
    salvar_contrato_no_arquivo(novo_contrato);
    
    printf("\n>> CONTRATO CADASTRADO COM SUCESSO! <<\n");
    printf("   Numero do contrato: %d\n", numero_contrato);
    printf("   Cliente: %s\n | CPF: %s\n", nome_completo, cpf);
    printf("   Plano: %s\n", plano_escolhido);
    
    free(nome_completo);
    free(endereco_completo);
}

// MODIFICAR CONTRATO EXISTENTE (por chave primária)
void modificar_contrato_existente() {
    printf("\n=================================================================\n");
    printf("|                      MODIFICAR CONTRATO                       |\n");
    printf("=================================================================\n\n");
    
    int id_contrato;
    printf("> Digite o NUMERO DO CONTRATO (chave primaria) para modificar: ");
    scanf("%d", &id_contrato);
    
    Contrato* contrato_encontrado = buscar_por_numero_contrato(raiz_arvore, id_contrato);
    
    if (contrato_encontrado == NULL) {
        printf(">> Nenhum contrato encontrado com o numero %d\n", id_contrato);
        pausar_tela();
        return;
    }
    
    printf("\n>>> CONTRATO ENCONTRADO (DADOS ATUAIS):\n");
    exibir_dados_contrato(contrato_encontrado);
    
    printf("\n>>> O QUE DESEJA MODIFICAR?\n");
    printf("    1. Telefone\n");
    printf("    2. Endereco\n");
    printf("    3. Plano\n");
    printf("    0. Cancelar\n");
    
    int opcao_modificacao;
    printf("\n> Escolha uma opcao: ");
    scanf("%d", &opcao_modificacao);
    
    if (opcao_modificacao == 0) {
        printf("> Modificacao cancelada.\n");
        pausar_tela();
        return;
    }
    
    char novo_valor[300];
    char* temp_endereco;
    
    switch (opcao_modificacao) {
        case 1:
            printf("> Novo telefone: ");
            scanf(" %[^\n]", novo_valor);
            strcpy(contrato_encontrado->telefone, novo_valor);
            break;
            
        case 2:
            printf("> Novo endereco completo: ");
            scanf(" %[^\n]", novo_valor);
            temp_endereco = malloc(strlen(novo_valor) + 1);
            if (temp_endereco != NULL) {
                strcpy(temp_endereco, novo_valor);
                free(contrato_encontrado->endereco_completo);
                contrato_encontrado->endereco_completo = temp_endereco;
            }
            break;
            
        case 3:
            obter_plano_escolhido(contrato_encontrado->plano_contratado);
            break;
            
        default:
            printf(">> Opcao invalida!\n");
            pausar_tela();
            return;
    }
    
    reescrever_arquivo_completo(raiz_arvore);
    
    printf("\n>> CONTRATO #%d MODIFICADO COM SUCESSO!\n", id_contrato);
    printf("\n>>> NOVOS DADOS DO CONTRATO:\n");
    exibir_dados_contrato(contrato_encontrado);
    
    pausar_tela();
}

// EXCLUIR CONTRATO
void excluir_contrato_existente() {
    printf("\n=================================================================\n");
    printf("|                      EXCLUIR CONTRATO                         |\n");
    printf("=================================================================\n\n");
    
    int id_contrato;
    printf("> Digite o NUMERO DO CONTRATO (chave primaria) para excluir: ");
    scanf("%d", &id_contrato);
    
    Contrato* contrato_encontrado = buscar_por_numero_contrato(raiz_arvore, id_contrato);
    
    if (contrato_encontrado == NULL) {
        printf(">> Nenhum contrato encontrado com o numero %d\n", id_contrato);
        pausar_tela();
        return;
    }
    
    printf("\n>>> CONTRATO ENCONTRADO:\n");
    exibir_dados_contrato(contrato_encontrado);
    
    char confirmacao;
    printf("\n> Confirmar exclusao deste contrato? (S/N): ");
    scanf(" %c", &confirmacao);
    
    if (confirmacao == 'S' || confirmacao == 's') {
        if (remover_contrato_arvore_b(&raiz_arvore, id_contrato)) {
            reescrever_arquivo_completo(raiz_arvore);
            printf("\n>> CONTRATO #%d EXCLUIDO COM SUCESSO!\n", id_contrato);
        } else {
            printf("\n>> ERRO AO EXCLUIR O CONTRATO!\n");
        }
    } else {
        printf("\n> Exclusao cancelada.\n");
    }
    
    pausar_tela();
}

// BUSCAR CONTRATO POR ID
void buscar_contrato_por_id() {
    printf("\n==============================================================\n");
    printf("|              BUSCAR CONTRATO POR ID (CHAVE PRIMARIA)        |\n");
    printf("==============================================================\n\n");
    
    int id_contrato;
    printf("> Digite o NUMERO DO CONTRATO: ");
    scanf("%d", &id_contrato);
    
    Contrato* contrato_encontrado = buscar_por_numero_contrato(raiz_arvore, id_contrato);
    
    if (contrato_encontrado == NULL) {
        printf(">> Nenhum contrato encontrado com o numero %d\n", id_contrato);
    } else {
        printf("\n==================== CONTRATO ENCONTRADO ====================\n");
        exibir_dados_contrato(contrato_encontrado);
    }
    
    pausar_tela();
}

// BUSCAR CONTRATO POR NOME OU CPF
void buscar_contrato_por_nome_ou_cpf() {
    printf("\n==============================================================\n");
    printf("|              BUSCAR CONTRATO POR NOME OU CPF                |\n");
    printf("==============================================================\n\n");
    
    char termo_busca[100];
    printf("> Digite o nome ou CPF para busca: ");
    scanf(" %[^\n]", termo_busca);
    
    Contrato* resultados[1000];
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
    printf("\n==============================================================\n");
    printf("> Total de contratos encontrados: %d\n", quantidade_encontrada);
    
    pausar_tela();
}

void listar_todos_contratos() {
    printf("\n");
    printf("================================================================================\n");
    printf("                          TODOS OS CONTRATOS CADASTRADOS                        \n");
    printf("                           (ORDENADOS PELA CHAVE PRIMARIA)                      \n");
    printf("================================================================================\n\n");
    
    int total_contratos = contar_total_contratos(raiz_arvore);
    
    if (total_contratos == 0) {
        printf(">>> Nenhum contrato cadastrado no sistema.\n");
        pausar_tela();
        return;
    }
    
    Contrato** lista_ordenada = (Contrato**)malloc(total_contratos * sizeof(Contrato*));
    if (lista_ordenada == NULL) {
        printf(">>> Erro de memoria!\n");
        pausar_tela();
        return;
    }
    
    int indice_atual = 0;
    percorrer_arvore_em_ordem(raiz_arvore, lista_ordenada, &indice_atual);
    
    for (int indice = 0; indice < total_contratos; indice++) {
        Contrato* contrato_atual = lista_ordenada[indice];
        
        printf("================================================================================\n");
        printf("                               CONTRATO #%d\n", contrato_atual->numero_contrato);
        printf("================================================================================\n");
        printf("  Nome Completo:      %s\n", contrato_atual->nome_completo);
        printf("  CPF:                %s\n", contrato_atual->cpf);
        printf("  Telefone:           %s\n", contrato_atual->telefone);
        printf("  Endereco:           %s\n", contrato_atual->endereco_completo);
        printf("  Plano:              %s\n", contrato_atual->plano_contratado);
        printf("\n");
    }
    
    printf("================================================================================\n");
    printf(">>> Total de contratos: %d\n", total_contratos);
    printf("================================================================================\n");
    
    free(lista_ordenada);
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
    
    printf(">> Configuracao da Arvore B:\n");
    printf("   - Ordem (t): %d\n", ORDEM_DA_ARVORE);
    printf("   - Maximo de chaves por no: %d\n", MAXIMO_CHAVES_POR_NO);
    printf("   - Minimo de chaves por no: %d\n", MINIMO_CHAVES_POR_NO);
    printf("   - Total de contratos: %d\n\n", contar_total_contratos(raiz_arvore));
    
    printf(">> Representacao visual da arvore:\n\n");
    
    exibir_arvore_estrutura(raiz_arvore, 0);
    
    pausar_tela();
}

// SALVAR E SAIR DO SISTEMA
void salvar_e_sair() {
    printf("\n==============================================================\n");
    printf("|                      SALVANDO E SAINDO                       |\n");
    printf("==============================================================\n\n");
    
    salvar_todos_contratos_no_arquivo(raiz_arvore);
    
    printf("> Dados salvos em: %s\n\n", ARQUIVO_CONTRATOS);
    printf("> Encerrando o sistema da Provedora de Internet...\n\n");
}

// EXIBIR MENU PRINCIPAL
void exibir_menu_principal() {
    printf("\n");
    printf("================================================================================\n");
    printf("\n");
    printf(COR_AMARELO"                                  A K I T O                                     \n" COR_RESET);
    printf("\n");
    printf(COR_AZUL"                     SISTEMA DE GERENCIAMENTO DE CONTRATOS                       \n" COR_RESET);
    printf(COR_AZUL"                            PROVEDORA DE INTERNET                                \n" COR_RESET);
    printf("\n");
    printf("================================================================================\n");
    printf("\n");
    printf("                               MENU PRINCIPAL                                    \n");
    printf("\n");
    printf("                          1. Cadastrar novo contrato                            \n");
    printf("                          2. Modificar contrato                                 \n");
    printf("                          3. Excluir contrato                                   \n");
    printf("                          4. Buscar contrato por ID                             \n");
    printf("                          5. Buscar contrato por nome ou CPF                    \n");
    printf("                          6. Listar todos os contratos                          \n");
    printf("                          7. Visualizar estrutura da Arvore B                   \n");
    printf("                          8. Salvar dados e sair                                \n");
    printf("\n");
    printf("================================================================================\n");
    printf("\n");
}

// FUNÇÃO PRINCIPAL
int main() {
    printf("\n> Carregando dados do arquivo %s...\n", ARQUIVO_CONTRATOS);
    carregar_contratos_do_arquivo(&raiz_arvore, &ultimo_numero_contrato);
    
    int opcao_selecionada;
    
    do {
        exibir_menu_principal();
        printf("> Digite sua opcao (1-8): ");
        scanf("%d", &opcao_selecionada);
        limpar_buffer_entrada();
        
        switch (opcao_selecionada) {
            case 1:
                cadastrar_novo_contrato();
                break;
            case 2:
                modificar_contrato_existente();
                break;
            case 3:
                excluir_contrato_existente();
                break;
            case 4:
                buscar_contrato_por_id();
                break;
            case 5:
                buscar_contrato_por_nome_ou_cpf();
                break;
            case 6:
                listar_todos_contratos();
                break;
            case 7:
                visualizar_estrutura_arvore();
                break;
            case 8:
                salvar_e_sair();
                break;
            default:
                printf("> Opcao invalida! Tente novamente.\n");
                pausar_tela();
                break;
        }
    } while (opcao_selecionada != 8);
    
    liberar_arvore_b(raiz_arvore);
    
    return 0;
}