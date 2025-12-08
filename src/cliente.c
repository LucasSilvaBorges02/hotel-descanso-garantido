#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cliente.h"

#define ARQ_CLIENTE "data/clientes.dat"

// Função auxiliar para verificar existência (usada aqui e no check-in)
int codigo_cliente_existe(int codigo) {
    FILE *file = fopen(ARQ_CLIENTE, "rb");
    if (!file) return 0;
    
    Cliente c;
    while(fread(&c, sizeof(Cliente), 1, file)) {
        if (c.codigo == codigo) {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void cadastrar_cliente(Cliente c) {
    // Validação: Não permitir ID duplicado
    if (codigo_cliente_existe(c.codigo)) {
        printf("ERRO: Ja existe um cliente com o codigo %d.\n", c.codigo);
        return;
    }

    FILE *file = fopen(ARQ_CLIENTE, "ab");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    fwrite(&c, sizeof(Cliente), 1, file);
    fclose(file);
    printf("Cliente cadastrado com sucesso!\n");
}

void listar_clientes() {
    FILE *file = fopen(ARQ_CLIENTE, "rb");
    Cliente c;

    if (file == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Clientes ---\n");
    while (fread(&c, sizeof(Cliente), 1, file)) {
        printf("Codigo: %d\n", c.codigo);
        printf("Nome: %s\n", c.nome);
        printf("Telefone: %s\n", c.telefone);
        printf("Endereco: %s\n", c.endereco);
        printf("-------------------------\n");
    }
    fclose(file);
}