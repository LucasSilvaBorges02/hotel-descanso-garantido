#include <stdio.h>
#include <string.h>
#include "cliente.h"

#define ARQ_CLIENTES "data/clientes.dat"

int cadastrar_cliente(Cliente novoCliente) {
    FILE *f = fopen(ARQ_CLIENTES, "ab"); // append binário

    if (f == NULL) {
        printf("Erro ao abrir arquivo de clientes.\n");
        return 1;
    }

    // Por enquanto só grava sem validar código duplicado
    fwrite(&novoCliente, sizeof(Cliente), 1, f);

    fclose(f);
    return 0;
}
