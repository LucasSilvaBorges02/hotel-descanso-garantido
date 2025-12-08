#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/funcionario.h"

#define ARQ_FUNCIONARIO "data/funcionario.dat"

// ------------------------------------------
// Função para cadastrar um funcionário
// ------------------------------------------
void cadastrarFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "ab");
    Funcionario f;

    if (!file) {
        printf("Erro ao abrir o arquivo de funcionario!\n");
        return;
    }

    printf("\n--- Cadastro de Funcionário ---\n");
    printf("ID: ");
    scanf("%d", &f.id);
    getchar(); 

    printf("Nome: ");
    fgets(f.nome, 50, stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';

    printf("Cargo: ");
    fgets(f.cargo, 30, stdin);
    f.cargo[strcspn(f.cargo, "\n")] = '\0';

    printf("Salário: ");
    scanf("%f", &f.salario);

    fwrite(&f, sizeof(Funcionario), 1, file);
    fclose(file);

    printf("Funcionário cadastrado com sucesso!\n");
}

// ------------------------------------------
// Lista todos os funcionários cadastrados
// ------------------------------------------
void listarFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    Funcionario f;

    if (!file) {
        printf("Nenhum funcionário encontrado.\n");
        return;
    }

    printf("\n--- Lista de Funcionários ---\n");

    while (fread(&f, sizeof(Funcionario), 1, file)) {
        printf("ID: %d\n", f.id);
        printf("Nome: %s\n", f.nome);
        printf("Cargo: %s\n", f.cargo);
        printf("Salário: R$ %.2f\n", f.salario);
        printf("----------------------------\n");
    }

    fclose(file);
}

// ------------------------------------------
// Buscar funcionário pelo ID
// ------------------------------------------
void buscarFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    Funcionario f;
    int idBusca, encontrado = 0;

    if (!file) {
        printf("Arquivo inexistente.\n");
        return;
    }

    printf("Digite o ID do funcionário: ");
    scanf("%d", &idBusca);

    while (fread(&f, sizeof(Funcionario), 1, file)) {
        if (f.id == idBusca) {
            printf("\n--- Funcionário Encontrado ---\n");
            printf("ID: %d\n", f.id);
            printf("Nome: %s\n", f.nome);
            printf("Cargo: %s\n", f.cargo);
            printf("Salário: R$ %.2f\n", f.salario);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("Funcionário não encontrado.\n");
    }

    fclose(file);
}

// ------------------------------------------
// Remove funcionário
// ------------------------------------------
void removerFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    FILE *temp = fopen("data/temp.dat", "wb");

    Funcionario f;
    int idRemove, removido = 0;

    if (!file || !temp) {
        printf("Erro ao acessar arquivos.\n");
        return;
    }

    printf("ID do funcionário a remover: ");
    scanf("%d", &idRemove);

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
    rename("data/temp.dat", ARQ_FUNCIONARIO);

    if (removido)
        printf("Funcionário removido com sucesso!\n");
    else
        printf("Funcionário não encontrado.\n");
}
