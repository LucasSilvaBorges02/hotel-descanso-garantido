#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionarios.h"

#define ARQ_FUNCIONARIOS "data/funcionarios.dat"

// ------------------------------------------
// Função para cadastrar um funcionário
// ------------------------------------------
void cadastrarFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIOS, "ab");
    Funcionarios f;

    if (!file) {
        printf("Erro ao abrir o arquivo de funcionarios!\n");
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

    fwrite(&f, sizeof(Funcionarios), 1, file);
    fclose(file);

    printf("Funcionário cadastrado com sucesso!\n");
}

// ------------------------------------------
// Lista todos os funcionários cadastrados
// ------------------------------------------
void listarFuncionarios() {
    FILE *file = fopen(ARQ_FUNCIONARIOS, "rb");
    Funcionarios f;

    if (!file) {
        printf("Nenhum funcionário encontrado.\n");
        return;
    }

    printf("\n--- Lista de Funcionários ---\n");

    while (fread(&f, sizeof(Funcionarios), 1, file)) {
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
    FILE *file = fopen(ARQ_FUNCIONARIOS, "rb");
    Funcionarios f;
    int idBusca, encontrado = 0;

    if (!file) {
        printf("Arquivo inexistente.\n");
        return;
    }

    printf("Digite o ID do funcionário: ");
    scanf("%d", &idBusca);

    while (fread(&f, sizeof(Funcionarios), 1, file)) {
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
    FILE *file = fopen(ARQ_FUNCIONARIOS, "rb");
    FILE *temp = fopen("data/temp.dat", "wb");

    Funcionarios f;
    int idRemove, removido = 0;

    if (!file || !temp) {
        printf("Erro ao acessar arquivos.\n");
        return;
    }

    printf("ID do funcionário a remover: ");
    scanf("%d", &idRemove);

    while (fread(&f, sizeof(Funcionarios), 1, file)) {
        if (f.id == idRemove) {
            removido = 1;
        } else {
            fwrite(&f, sizeof(Funcionarios), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(ARQ_FUNCIONARIOS);
    rename("data/temp.dat", ARQ_FUNCIONARIOS);

    if (removido)
        printf("Funcionário removido com sucesso!\n");
    else
        printf("Funcionário não encontrado.\n");
}
