#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cliente.h"
#include "../include/estadia.h" 

#define ARQ_CLIENTE "data/clientes.dat"

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
        int pontos = calcularPontosFidelidade(c.codigo);

        printf("Codigo: %d\n", c.codigo);
        printf("Nome: %s\n", c.nome);
        printf("Telefone: %s\n", c.telefone);
        printf("Endereco: %s\n", c.endereco);
        printf("Pontos Fidelidade: %d\n", pontos);
        printf("-------------------------\n");
    }
    fclose(file);
}

// --- FUNÇÃO ATUALIZADA COM "VOLTAR" ---
void remover_cliente() {
    FILE *file = fopen(ARQ_CLIENTE, "rb");
    
    if (!file) {
        printf("Nenhum cliente cadastrado para remover.\n");
        return;
    }

    Cliente c;

    // 1. MOSTRAR A LISTA
    printf("\n=== ESCOLHA UM CLIENTE PARA REMOVER ===\n");
    while (fread(&c, sizeof(Cliente), 1, file)) {
        printf("Codigo: %d \t| Nome: %s\n", c.codigo, c.nome);
    }
    printf("---------------------------------------\n");

    // 2. PEDIR O CÓDIGO (Agora com opção de sair)
    int idRemove;
    printf("Digite o codigo do cliente (ou 0 para VOLTAR): ");
    
    if (scanf("%d", &idRemove) != 1) {
        printf("Entrada invalida.\n");
        int ch; while ((ch = getchar()) != '\n' && ch != EOF); 
        fclose(file);
        return;
    }

    // SE O USUÁRIO DIGITAR 0, CANCELA TUDO E VOLTA
    if (idRemove == 0) {
        printf("Operacao cancelada. Voltando ao menu...\n");
        fclose(file);
        return;
    }

    // 3. AGORA SIM, CRIA O TEMPORÁRIO E REMOVE
    rewind(file); // Volta pro começo do arquivo original

    FILE *temp = fopen("data/temp_clientes.dat", "wb"); 
    if (!temp) {
        printf("Erro ao criar arquivo temporario.\n");
        fclose(file);
        return;
    }

    int removido = 0;
    while(fread(&c, sizeof(Cliente), 1, file)) {
        if (c.codigo == idRemove) {
            removido = 1; 
        } else {
            fwrite(&c, sizeof(Cliente), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    remove(ARQ_CLIENTE);
    rename("data/temp_clientes.dat", ARQ_CLIENTE);

    if (removido)
        printf("Cliente %d removido com sucesso!\n", idRemove);
    else
        printf("Cliente nao encontrado na lista.\n");
}