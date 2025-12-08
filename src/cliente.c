#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cliente.h"
#include "../include/estadia.h" // Necessário para ver os pontos

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
        // Busca os pontos acumulados usando a função do módulo Estadia
        int pontos = calcularPontosFidelidade(c.codigo);

        printf("Codigo: %d\n", c.codigo);
        printf("Nome: %s\n", c.nome);
        printf("Telefone: %s\n", c.telefone);
        printf("Endereco: %s\n", c.endereco);
        printf("Pontos Fidelidade: %d\n", pontos); // Mostra os pontos aqui
        printf("-------------------------\n");
    }
    fclose(file);
}

void remover_cliente() {
    FILE *file = fopen(ARQ_CLIENTE, "rb");
    // Cria um arquivo temporário
    FILE *temp = fopen("data/temp_clientes.dat", "wb"); 
    
    Cliente c;
    int idRemove, removido = 0;

    if (!file || !temp) {
        printf("Erro ao abrir arquivos de cliente (verifique se existem clientes cadastrados).\n");
        if (file) fclose(file);
        if (temp) fclose(temp);
        return;
    }

    printf("Digite o codigo do cliente para remover: ");
    scanf("%d", &idRemove);

    // Lê do original e escreve no temporário APENAS se não for o ID removido
    while(fread(&c, sizeof(Cliente), 1, file)) {
        if (c.codigo == idRemove) {
            removido = 1; // Encontrou o alvo, não copia para o novo arquivo
        } else {
            fwrite(&c, sizeof(Cliente), 1, temp);
        }
    }

    fclose(file);
    fclose(temp);

    // Apaga o arquivo antigo e renomeia o temporário para ser o oficial
    remove(ARQ_CLIENTE);
    rename("data/temp_clientes.dat", ARQ_CLIENTE);

    if (removido)
        printf("Cliente %d removido com sucesso!\n", idRemove);
    else
        printf("Cliente nao encontrado.\n");
}