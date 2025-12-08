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

// ---------------------------------------------------------
// Funções Auxiliares (Data e Diárias)
// ---------------------------------------------------------

// Converte data para timestamp (tempo em segundos)
static time_t data_para_time(int d, int m, int a, int hora) {
    struct tm t = {0};
    t.tm_mday = d; t.tm_mon = m - 1; t.tm_year = a - 1900;
    t.tm_hour = hora;
    return mktime(&t);
}

// Calcula diferença de dias entre entrada (14h) e saída (12h)
int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2) {
    time_t t_entrada = data_para_time(d1, m1, a1, 14);
    time_t t_saida = data_para_time(d2, m2, a2, 12);
    double diff = difftime(t_saida, t_entrada);
    if (diff <= 0) return 0;
    int diarias = (int)ceil(diff / 86400.0);
    return (diarias < 1) ? 1 : diarias;
}

// Verifica se já existe reserva para aquele quarto nas datas informadas
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

            // Verifica sobreposição de datas
            if (difftime(novo_inicio, fim_existente) < 0 && difftime(inicio_existente, novo_fim) < 0) {
                fclose(f);
                return 1; // Conflito encontrado!
            }
        }
    }
    fclose(f);
    return 0;
}

// ---------------------------------------------------------
// Funções Principais do Módulo
// ---------------------------------------------------------

void cadastrarEstadia() {
    Estadia nova;
    int qtdHospedes;
    
    // 1. Validar Cliente
    printf("Codigo do Cliente: ");
    scanf("%d", &nova.codigoCliente);
    
    if (!codigo_cliente_existe(nova.codigoCliente)) {
        printf("Cliente nao encontrado!\n");
        return;
    }

    // 2. Coletar dados da Estadia
    printf("Qtd Hospedes: ");
    scanf("%d", &qtdHospedes);
    
    if (qtdHospedes <= 0) {
        printf("Qtd de hospedes invalida.\n");
        return;
    }

    printf("Entrada (DD MM AAAA): ");
    scanf("%d %d %d", &nova.diaEntrada, &nova.mesEntrada, &nova.anoEntrada);
    printf("Saida (DD MM AAAA): ");
    scanf("%d %d %d", &nova.diaSaida, &nova.mesSaida, &nova.anoSaida);

    nova.qtdDiarias = calcularDiarias(nova.diaEntrada, nova.mesEntrada, nova.anoEntrada, 
                                      nova.diaSaida, nova.mesSaida, nova.anoSaida);
    
    if (nova.qtdDiarias <= 0) {
        printf("Data invalida (saida antes da entrada).\n");
        return;
    }

    // 3. Buscar Quarto Disponível (Lendo arquivo de Quartos)
    FILE *fQuartos = fopen(ARQ_QUARTOS, "rb");
    if (!fQuartos) { printf("Sem quartos cadastrados.\n"); return; }
    
    Quarto q;
    int quartoEncontrado = -1;
    
    while(fread(&q, sizeof(Quarto), 1, fQuartos)) {
        // Regra: Quarto ativo, Capacidade suporta, e sem conflito de agenda
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
    nova.ativo = 1; // Check-in Ativo
    nova.codigo = (int)time(NULL); // Gera ID único baseado no relógio

    FILE *fEst = fopen(ARQ_ESTADIAS, "ab");
    fwrite(&nova, sizeof(Estadia), 1, fEst);
    fclose(fEst);

    // Atualiza status visual do quarto
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
            e.ativo = 0; // Marca como Check-out realizado
            
            // Volta cursor para sobrescrever o registro atualizado
            fseek(f, -(long)sizeof(Estadia), SEEK_CUR);
            fwrite(&e, sizeof(Estadia), 1, f);
            
            // Calcula preço total
            float valorDiaria = buscarValorDiaria(e.numeroQuarto);
            float total = e.qtdDiarias * valorDiaria;
            
            // Libera quarto
            atualizarStatusQuarto(e.numeroQuarto, DESOCUPADO);
            
            printf("Baixa realizada! Total a pagar: R$ %.2f\n", total);
            // Mostra os pontos ganhos nesta estadia específica
            printf("Pontos ganhos nesta estadia: %d\n", e.qtdDiarias * 10);
            achou = 1;
            break;
        }
    }
    fclose(f);
    if (!achou) printf("Estadia nao encontrada ou ja baixada.\n");
}

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

// ---------------------------------------------------------
// NOVA FUNÇÃO: PONTOS DE FIDELIDADE
// ---------------------------------------------------------
int calcularPontosFidelidade(int codCliente) {
    FILE *f = fopen(ARQ_ESTADIAS, "rb");
    if (!f) return 0; // Se não tem arquivo, cliente tem 0 pontos

    Estadia e;
    int totalPontos = 0;

    while(fread(&e, sizeof(Estadia), 1, f)) {
        // Soma pontos apenas se a estadia for do cliente E já estiver finalizada (check-out feito)
        if (e.codigoCliente == codCliente && e.ativo == 0) {
            totalPontos += (e.qtdDiarias * 10);
        }
    }
    fclose(f);
    return totalPontos;
}