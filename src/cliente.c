#include <stdio.h>
#include <string.h>
#include "cliente.h"

#define ARQ_CLIENTES "data/clientes.dat"

int codigo_cliente_existe(int codigo) {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (f == NULL) return 0; // arquivo ainda não existe

    Cliente c;
    while (fread(&c, sizeof(Cliente), 1, f) == 1) {
        if (c.codigo == codigo) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int cadastrar_cliente(Cliente novoCliente) {
    if (codigo_cliente_existe(novoCliente.codigo)) {
        printf("Ja existe cliente com codigo %d.\n", novoCliente.codigo);
        return 1;
    }

    FILE *f = fopen(ARQ_CLIENTES, "ab");
    if (f == NULL) {
        printf("Erro ao abrir arquivo de clientes.\n");
        return 2;
    }

    fwrite(&novoCliente, sizeof(Cliente), 1, f);
    fclose(f);
    return 0;
}

void listar_clientes() {
    FILE *f = fopen(ARQ_CLIENTES, "rb");
    if (f == NULL) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }

    Cliente c;
    printf("=== Clientes cadastrados ===\n");
    while (fread(&c, sizeof(Cliente), 1, f) == 1) {
        printf("Codigo: %d | Nome: %s | Tel: %s | Endereco: %s\n",
               c.codigo, c.nome, c.telefone, c.endereco);
    }
    fclose(f);
}
