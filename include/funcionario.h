#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

typedef struct {
    int id;
    char nome[50];
    char cargo[30];
    float salario;
} Funcionario;

// Protótipos
void cadastrarFuncionario();
void listarFuncionario();
void buscarFuncionario();
void removerFuncionario();

#endif