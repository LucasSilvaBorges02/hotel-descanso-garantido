#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    int codigo;
    char nome[100];
    char telefone[20];
    char endereco[150];
} Cliente;

int cadastrar_cliente(Cliente novoCliente);
void listar_clientes();
int codigo_cliente_existe(int codigo);

#endif
