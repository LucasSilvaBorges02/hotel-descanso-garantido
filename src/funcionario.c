#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/funcionario.h"

#define ARQ_FUNCIONARIO "data/funcionario.dat"

void cadastrarFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "ab");
    Funcionario f;

    if (!file) {
        printf("Erro ao abrir o arquivo de funcionario!\n");
        return;
    }

    printf("\n--- Cadastro de Funcionario ---\n");
    printf("ID: ");
    scanf("%d", &f.id);
    getchar(); 

    printf("Nome: ");
    fgets(f.nome, 50, stdin);
    f.nome[strcspn(f.nome, "\n")] = '\0';

    printf("Cargo: ");
    fgets(f.cargo, 30, stdin);
    f.cargo[strcspn(f.cargo, "\n")] = '\0';

    printf("Salario: ");
    scanf("%f", &f.salario);

    fwrite(&f, sizeof(Funcionario), 1, file);
    fclose(file);

    printf("Funcionario cadastrado com sucesso!\n");
}

void listarFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    Funcionario f;

    if (!file) {
        printf("Nenhum funcionario encontrado.\n");
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
    scanf("%d", &idBusca);

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

void removerFuncionario() {
    FILE *file = fopen(ARQ_FUNCIONARIO, "rb");
    FILE *temp = fopen("data/temp_func.dat", "wb"); // Nome temporário seguro

    Funcionario f;
    int idRemove, removido = 0;

    if (!file || !temp) {
        printf("Erro ao acessar arquivos.\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    printf("ID do funcionario a remover: ");
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
    rename("data/temp_func.dat", ARQ_FUNCIONARIO);

    if (removido)
        printf("Funcionario removido com sucesso!\n");
    else
        printf("Funcionario nao encontrado.\n");
}