#include <stdio.h>
#include <string.h>
#include "cliente.h"

int main() {
    int opcao;

    do {
        printf("\n=== Hotel Descanso Garantido ===\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Listar clientes\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpar \n

        if (opcao == 1) {
            Cliente c;

            printf("Codigo: ");
            scanf("%d", &c.codigo);
            getchar();

            // VERIFICA ANTES DE PEDIR O RESTO
            if (codigo_cliente_existe(c.codigo)) {
                printf("Ja existe cliente com codigo %d.\n", c.codigo);
                continue;  // volta pro menu
            }

            printf("Nome: ");
            fgets(c.nome, sizeof(c.nome), stdin);
            c.nome[strcspn(c.nome, "\n")] = '\0';

            printf("Telefone: ");
            fgets(c.telefone, sizeof(c.telefone), stdin);
            c.telefone[strcspn(c.telefone, "\n")] = '\0';

            printf("Endereco: ");
            fgets(c.endereco, sizeof(c.endereco), stdin);
            c.endereco[strcspn(c.endereco, "\n")] = '\0';

            int r = cadastrar_cliente(c);
            if (r == 0)
                printf("Cliente cadastrado!\n");
            else
                printf("Erro ao cadastrar cliente.\n");

        } else if (opcao == 2) {
            listar_clientes();
        } else if (opcao != 0) {
            printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    return 0;
}
