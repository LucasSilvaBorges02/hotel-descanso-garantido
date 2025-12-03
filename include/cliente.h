#ifndef CLIENTE_H
#define CLIENTE_H

typedef struct {
    int codigo;
    char nome[100];
    char telefone[20];
    char endereco[150];
} Cliente;

/**
 * Cadastra um novo cliente no arquivo de clientes.
 * Entrada: struct Cliente com dados preenchidos.
 * Saída: 0 em caso de sucesso, diferente de 0 em erro (ex: código já existe).
 */
int cadastrar_cliente(Cliente novoCliente);

#endif
