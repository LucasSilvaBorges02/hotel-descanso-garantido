#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#define MAX_FUNCIONARIOS 100

typedef struct {
    int codigo;
    char nome[100];
    char telefone[20];
    char cargo[50];
    float salario;
    int ativo; // 1 = cadastrado
} Funcionario;

void cadastrarFuncionario(Funcionario funcionarios[], int *qtFuncionarios);
int buscarFuncionarioPorCodigo(Funcionario funcionarios[], int qt, int codigo);
int buscarFuncionarioPorNome(Funcionario funcionarios[], int qt, char nome[]);
void listarFuncionario(Funcionario f);

#endif
