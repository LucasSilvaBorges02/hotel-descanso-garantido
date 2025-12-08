#include <stdio.h>
#include <string.h>
#include "../include/quarto.h" 

#define ARQ_QUARTOS "data/quartos.dat"

// Verifica se o número do quarto já existe no arquivo
int quarto_existe(int numero) {
    FILE *f = fopen(ARQ_QUARTOS, "rb");
    if (!f) return 0;

    Quarto q;
    while (fread(&q, sizeof(Quarto), 1, f)) {
        if (q.ativo && q.numero == numero) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void cadastrarQuarto(Quarto *novoQuarto) {
    // Validação de unicidade
    if (quarto_existe(novoQuarto->numero)) {
        printf("Erro: Quarto numero %d ja existe!\n", novoQuarto->numero);
        return;
    }

    FILE *f = fopen(ARQ_QUARTOS, "ab");
    if (!f) {
        printf("Erro ao abrir arquivo de quartos.\n");
        return;
    }
    
    novoQuarto->ativo = 1;
    novoQuarto->status = DESOCUPADO; // Padrão ao criar
    
    fwrite(novoQuarto, sizeof(Quarto), 1, f);
    fclose(f);
    printf("Quarto %d cadastrado com sucesso!\n", novoQuarto->numero);
}

void listarQuartos() {
    FILE *f = fopen(ARQ_QUARTOS, "rb");
    if (!f) {
        printf("Nenhum quarto cadastrado.\n");
        return;
    }

    Quarto q;
    printf("\n--- Lista de Quartos ---\n");
    while (fread(&q, sizeof(Quarto), 1, f)) {
        if (q.ativo) {
            printf("Quarto: %d | Cap: %d | Diaria: R$%.2f | Status: %s\n", 
                q.numero, q.capacidade, q.valorDiaria, 
                (q.status == OCUPADO ? "Ocupado" : "Livre"));
        }
    }
    fclose(f);
}

// Busca e retorna o valor da diária (ou -1 se erro)
float buscarValorDiaria(int numeroQuarto) {
    FILE *f = fopen(ARQ_QUARTOS, "rb");
    if (!f) return -1;

    Quarto q;
    while (fread(&q, sizeof(Quarto), 1, f)) {
        if (q.ativo && q.numero == numeroQuarto) {
            fclose(f);
            return q.valorDiaria;
        }
    }
    fclose(f);
    return -1;
}

// Atualiza o status do quarto no arquivo
void atualizarStatusQuarto(int numeroQuarto, StatusQuarto novoStatus) {
    FILE *f = fopen(ARQ_QUARTOS, "rb+");
    if (!f) return;

    Quarto q;
    while (fread(&q, sizeof(Quarto), 1, f)) {
        if (q.ativo && q.numero == numeroQuarto) {
            q.status = novoStatus;
           fseek(f, -(long)sizeof(Quarto), SEEK_CUR);
            fwrite(&q, sizeof(Quarto), 1, f);
            break;
        }
    }
    fclose(f);
}