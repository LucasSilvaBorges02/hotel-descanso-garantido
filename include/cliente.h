#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    int codigo;
    char nome[100];
    char telefone[20];
    char endereco[150];
} Cliente;

// Protótipos
void cadastrar_cliente(Cliente c);
void listar_clientes();
int codigo_cliente_existe(int codigo);
void remover_cliente();

#endif