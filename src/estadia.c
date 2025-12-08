#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "../include/estadia.h"
#include "../include/cliente.h"
#include "../include/quarto.h"

#define ARQ_ESTADIAS "data/estadias.dat"
#define ARQ_QUARTOS "data/quartos.dat"
#define ARQ_CLIENTES "data/clientes.dat"

// Função auxiliar de data
static time_t data_para_time(int d, int m, int a, int hora) {
    struct tm t = {0};
    t.tm_mday = d; t.tm_mon = m - 1; t.tm_year = a - 1900;
    t.tm_hour = hora;
    return mktime(&t);
}

int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2) {
    time_t t_entrada = data_para_time(d1, m1, a1, 14);
    time_t t_saida = data_para_time(d2, m2, a2, 12);
    double diff = difftime(t_saida, t_entrada);
    if (diff <= 0) return 0;
    int diarias = (int)ceil(diff / 86400.0);
    return (diarias < 1) ? 1 : diarias;
}

// Verifica conflito LENDO DO ARQUIVO
int verificarConflitoArquivo(int numQuarto, int d1, int m1, int a1, int d2, int m2, int a2) {
    FILE *f = fopen(ARQ_ESTADIAS, "rb");
    if (!f) return 0; // Sem estadias, sem conflito

    time_t novo_inicio = data_para_time(d1, m1, a1, 14);
    time_t novo_fim = data_para_time(d2, m2, a2, 12);
    
    Estadia e;
    while(fread(&e, sizeof(Estadia), 1, f)) {
        if (e.ativo && e.numeroQuarto == numQuarto) {
            time_t inicio_existente = data_para_time(e.diaEntrada, e.mesEntrada, e.anoEntrada, 14);
            time_t fim_existente = data_para_time(e.diaSaida, e.mesSaida, e.anoSaida, 12);

            if (difftime(novo_inicio, fim_existente) < 0 && difftime(inicio_existente, novo_fim) < 0) {
                fclose(f);
                return 1; // Conflito!
            }
        }
    }
    fclose(f);
    return 0;
}

void cadastrarEstadia() {
    Estadia nova;
    int qtdHospedes;
    
    // 1. Validar Cliente
    printf("Codigo do Cliente: ");
    scanf("%d", &nova.codigoCliente);
    
    // Como não temos acesso direto ao header do cliente aqui para ver a função, 
    // assumimos que codigo_cliente_existe funciona ou incluimos a lógica.
    // Se der erro de compilação aqui, verifique se clientes.h tem essa função.
    if (!codigo_cliente_existe(nova.codigoCliente)) {
        printf("Cliente nao encontrado!\n");
        return;
    }

    // 2. Coletar dados
    printf("Qtd Hospedes: ");
    scanf("%d", &qtdHospedes);
    printf("Entrada (DD MM AAAA): ");
    scanf("%d %d %d", &nova.diaEntrada, &nova.mesEntrada, &nova.anoEntrada);
    printf("Saida (DD MM AAAA): ");
    scanf("%d %d %d", &nova.diaSaida, &nova.mesSaida, &nova.anoSaida);

    nova.qtdDiarias = calcularDiarias(nova.diaEntrada, nova.mesEntrada, nova.anoEntrada, 
                                      nova.diaSaida, nova.mesSaida, nova.anoSaida);
    
    if (nova.qtdDiarias <= 0) {
        printf("Data invalida.\n");
        return;
    }

    // 3. Buscar Quarto (Lendo arquivo de Quartos)
    FILE *fQuartos = fopen(ARQ_QUARTOS, "rb");
    if (!fQuartos) { printf("Sem quartos cadastrados.\n"); return; }
    
    Quarto q;
    int quartoEncontrado = -1;
    
    while(fread(&q, sizeof(Quarto), 1, fQuartos)) {
        if (q.ativo && q.capacidade >= qtdHospedes) {
            if (!verificarConflitoArquivo(q.numero, nova.diaEntrada, nova.mesEntrada, nova.anoEntrada, 
                                          nova.diaSaida, nova.mesSaida, nova.anoSaida)) {
                quartoEncontrado = q.numero;
                break;
            }
        }
    }
    fclose(fQuartos);

    if (quartoEncontrado == -1) {
        printf("Nenhum quarto disponivel para essa data/capacidade.\n");
        return;
    }

    // 4. Salvar Estadia
    nova.numeroQuarto = quartoEncontrado;
    nova.ativo = 1;
    nova.codigo = (int)time(NULL); // Gera ID

    FILE *fEst = fopen(ARQ_ESTADIAS, "ab");
    fwrite(&nova, sizeof(Estadia), 1, fEst);
    fclose(fEst);

    atualizarStatusQuarto(quartoEncontrado, OCUPADO);

    printf("Estadia agendada! Quarto: %d | Codigo Estadia: %d\n", quartoEncontrado, nova.codigo);
}

void baixarEstadia() {
    int codEstadia;
    printf("Codigo da Estadia para baixa: ");
    scanf("%d", &codEstadia);

    FILE *f = fopen(ARQ_ESTADIAS, "rb+");
    if (!f) {
        printf("Erro ao abrir arquivo de estadias.\n");
        return;
    }

    Estadia e;
    int achou = 0;
    while(fread(&e, sizeof(Estadia), 1, f)) {
        if (e.ativo && e.codigo == codEstadia) {
            e.ativo = 0; // Finaliza
            
            // Volta cursor para sobrescrever
            fseek(f, -(long)sizeof(Estadia), SEEK_CUR);
            fwrite(&e, sizeof(Estadia), 1, f);
            
            // Calcula preço
            float valorDiaria = buscarValorDiaria(e.numeroQuarto);
            float total = e.qtdDiarias * valorDiaria;
            
            // Libera quarto
            atualizarStatusQuarto(e.numeroQuarto, DESOCUPADO);
            
            printf("Baixa realizada! Total a pagar: R$ %.2f\n", total);
            printf("Pontos Fidelidade: %d\n", e.qtdDiarias * 10);
            achou = 1;
            break;
        }
    }
    fclose(f);
    if (!achou) printf("Estadia nao encontrada ou ja baixada.\n");
}

// --- NOVA FUNÇÃO ADICIONADA ---
void listarEstadias() {
    FILE *f = fopen(ARQ_ESTADIAS, "rb");
    if (!f) {
        printf("\nNenhuma estadia cadastrada.\n");
        return;
    }

    Estadia e;
    printf("\n=== LISTA DE ESTADIAS ===\n");
    while(fread(&e, sizeof(Estadia), 1, f)) {
        printf("--------------------------------\n");
        printf("Codigo Estadia: %d\n", e.codigo);
        printf("Cliente: %d | Quarto: %d\n", e.codigoCliente, e.numeroQuarto);
        printf("Periodo: %02d/%02d/%d ate %02d/%02d/%d\n", 
               e.diaEntrada, e.mesEntrada, e.anoEntrada,
               e.diaSaida, e.mesSaida, e.anoSaida);
        printf("Status: %s\n", e.ativo ? "ATIVO (Check-in)" : "FINALIZADO (Check-out)");
    }
    printf("--------------------------------\n");
    fclose(f);
}