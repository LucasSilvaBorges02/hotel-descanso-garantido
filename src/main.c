#include <stdio.h>
#include "cliente.h"

int main() {
    Cliente c;

    c.codigo = 1;
    sprintf(c.nome, "Cliente Teste");
    sprintf(c.telefone, "31999999999");
    sprintf(c.endereco, "Rua X, 123, Bairro Y");

    if (cadastrar_cliente(c) == 0) {
        printf("Cliente cadastrado com sucesso!\n");
    } else {
        printf("Erro ao cadastrar cliente.\n");
    }

    return 0;
}
