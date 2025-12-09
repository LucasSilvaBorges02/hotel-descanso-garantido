#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

typedef struct {
    int id;
    char nome[50];
    char cargo[30];
    float salario;
} Funcionario;

// Protótipos
void cadastrarFuncionario(Funcionario f); // Agora recebe a struct
void listarFuncionario();
void buscarFuncionario();
void removerFuncionario(); // Nova função
int funcionarioExiste(int id); // Nova função para testes e validação

#endif