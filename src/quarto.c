#include <stdio.h>
#include <stdlib.h>
#include "../include/quarto.h"

#define ARQ_QUARTO "data/quartos.dat"

// Função auxiliar interna para checar duplicidade
int quartoExiste(int numero) {
    FILE *file = fopen(ARQ_QUARTO, "rb");
    if (!file) return 0;

    Quarto q;
    while (fread(&q, sizeof(Quarto), 1, file)) {
        if (q.ativo && q.numero == numero) {
            fclose(file);
            return 1; 
        }
    }
    fclose(file);
    return 0;
}

void cadastrarQuarto(Quarto *q) {
    // Validações de Regra de Negócio
    if (q->capacidade <= 0) {
        printf("ERRO: A capacidade deve ser maior que zero.\n");
        return;
    }
    if (q->valorDiaria < 0) {
        printf("ERRO: O valor da diaria nao pode ser negativo.\n");
        return;
    }
    if (quartoExiste(q->numero)) {
        printf("ERRO: Ja existe um quarto com o numero %d cadastrado.\n", q->numero);
        return;
    }

    FILE *file = fopen(ARQ_QUARTO, "ab");
    if (!file) {
        printf("Erro ao abrir arquivo de quartos.\n");
        return;
    }

    q->ativo = 1; 
    q->status = DESOCUPADO; 

    fwrite(q, sizeof(Quarto), 1, file);
    fclose(file);
    printf("Quarto %d cadastrado com sucesso!\n", q->numero);
}

void listarQuartos() {
    FILE *file = fopen(ARQ_QUARTO, "rb");
    Quarto q;

    if (!file) {
        printf("Nenhum quarto cadastrado.\n");
        return;
    }

    printf("\n--- Lista de Quartos ---\n");
    while (fread(&q, sizeof(Quarto), 1, file)) {
        if (q.ativo) {
            printf("Num: %d | Cap: %d | Valor: R$ %.2f | Status: %s\n", 
                   q.numero, q.capacidade, q.valorDiaria, 
                   (q.status == OCUPADO) ? "OCUPADO" : "DESOCUPADO");
        }
    }
    fclose(file);
}

void atualizarStatusQuarto(int numero, int novoStatus) {
    FILE *file = fopen(ARQ_QUARTO, "rb+");
    Quarto q;

    if (!file) return;

    while (fread(&q, sizeof(Quarto), 1, file)) {
        if (q.numero == numero && q.ativo) {
            q.status = novoStatus;
            // Move cursor para trás para sobrescrever o registro
            fseek(file, -(long)sizeof(Quarto), SEEK_CUR);
            fwrite(&q, sizeof(Quarto), 1, file);
            break;
        }
    }
    fclose(file);
}

float buscarValorDiaria(int numero) {
    FILE *file = fopen(ARQ_QUARTO, "rb");
    Quarto q;
    float valor = 0.0;

    if (!file) return 0.0;

    while (fread(&q, sizeof(Quarto), 1, file)) {
        if (q.numero == numero && q.ativo) {
            valor = q.valorDiaria;
            break;
        }
    }
    fclose(file);
    return valor;
}