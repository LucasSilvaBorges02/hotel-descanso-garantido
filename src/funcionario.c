#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/funcionario.h"

#define ARQ_FUNCIONARIO "data/funcionario.dat"

int funcionarioExiste(int id) {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    if (!file) return 0;

    Funcionario f;
    while(fread(&f, sizeof(Funcionario), 1, file)) {
        if (f.id == id) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void cadastrarFuncionario(Funcionario f) {
    FILE *file = fopen(ARQ_FUNCIONARIO, "ab");
    if (!file) {
        printf("Erro ao abrir o arquivo de funcionario!\n");
        return;
    }
    fwrite(&f, sizeof(Funcionario), 1, file);
    fclose(file);
    printf("Funcionario cadastrado com sucesso!\n");
}

void listarFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    Funcionario f;

    if (!file) {
        printf("Nenhum funcionario cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Funcionarios ---\n");
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        printf("ID: %d\n", f.id);
        printf("Nome: %s\n", f.nome);
        printf("Cargo: %s\n", f.cargo);
        printf("Salario: R$ %.2f\n", f.salario);
        printf("----------------------------\n");
    }
    fclose(file);
}

void buscarFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    Funcionario f;
    int idBusca, encontrado = 0;

    if (!file) {
        printf("Arquivo inexistente.\n");
        return;
    }

    printf("Digite o ID do funcionario: ");
    if(scanf("%d", &idBusca) != 1) {
        printf("ID invalido.\n");
        int c; while ((c = getchar()) != '\n' && c != EOF); 
        fclose(file);
        return;
    }

    while (fread(&f, sizeof(Funcionario), 1, file)) {
        if (f.id == idBusca) {
            printf("\n--- Funcionario Encontrado ---\n");
            printf("ID: %d\n", f.id);
            printf("Nome: %s\n", f.nome);
            printf("Cargo: %s\n", f.cargo);
            printf("Salario: R$ %.2f\n", f.salario);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Funcionario nao encontrado.\n");
    }
    fclose(file);
}

// --- FUNÇÃO ATUALIZADA COM "VOLTAR" ---
void removerFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    if (!file) {
        printf("Nenhum funcionario cadastrado para remover.\n");
        return;
    }

    Funcionario f;
    
    // 1. MOSTRAR A LISTA
    printf("\n=== ESCOLHA UM FUNCIONARIO PARA REMOVER ===\n");
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        printf("ID: %d \t| Nome: %s\n", f.id, f.nome);
    }
    printf("-------------------------------------------\n");

    // 2. PEDIR O ID (Com opção de sair)
    int idRemove;
    printf("Digite o ID do funcionario (ou 0 para VOLTAR): ");
    
    if (scanf("%d", &idRemove) != 1) {
        printf("Entrada invalida.\n");
        int c; while ((c = getchar()) != '\n' && c != EOF); // Limpa buffer
        fclose(file);
        return;
    }

    // SE O USUÁRIO DIGITAR 0, CANCELA TUDO
    if (idRemove == 0) {
        printf("Operacao cancelada. Voltando ao menu...\n");
        fclose(file);
        return;
    }

    // 3. AGORA SIM, PROCESSA A REMOÇÃO
    rewind(file); // Volta pro começo do arquivo

    FILE *temp = fopen("data/temp_func.dat", "wb");
    if (!temp) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(file);
        return;
    }

    int removido = 0;
    while (fread(&f, sizeof(Funcionario), 1, file)) {
        if (f.id == idRemove) {
            removido = 1;
        } else {
            fwrite(&f, sizeof(Funcionario), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(ARQ_FUNCIONARIO);
    rename("data/temp_func.dat", ARQ_FUNCIONARIO);

    if (removido)
        printf("Funcionario ID %d removido com sucesso!\n", idRemove);
    else
        printf("Funcionario nao encontrado na lista.\n");
}