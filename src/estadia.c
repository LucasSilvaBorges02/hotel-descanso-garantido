// src/estadia.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "estadia.h"
#include "cliente.h"
#include "quarto.h"

/*
 * Função auxiliar: converte data+hora em time_t
 * d,m,a são dia, mês, ano
 * hora é a hora do dia (0-23)
 */
static time_t data_para_time(int d, int m, int a, int hora) {
    struct tm t;
    memset(&t, 0, sizeof(struct tm));
    t.tm_mday = d;
    t.tm_mon = m - 1;
    t.tm_year = a - 1900;
    t.tm_hour = hora;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1; // deixar o sistema decidir
    return mktime(&t);
}

/*
 * Calcula a quantidade de diárias entre as datas,
 * considerando que as diárias começam às 14:00 (entrada)
 * e terminam às 12:00 (saída).
 * Retorna 0 se a data de saída for anterior ou igual à de entrada.
 */
int calcularDiarias(
    int d1, int m1, int a1,
    int d2, int m2, int a2
) {
    // entrada 14:00
    time_t t_entrada = data_para_time(d1, m1, a1, 14);
    // saída 12:00
    time_t t_saida = data_para_time(d2, m2, a2, 12);

    double diff = difftime(t_saida, t_entrada); // segundos
    if (diff <= 0) return 0;

    double dias = diff / 86400.0; // segundos por dia
    int diarias = (int)ceil(dias - 1e-9); // arredonda pra cima
    if (diarias < 1) diarias = 1;
    return diarias;
}

/*
 * Verifica conflito de datas para um quarto específico.
 * Retorna 1 se há conflito (ou seja, existe outra estadia ativa no mesmo quarto
 * que sobrepõe o período pedido), 0 se não há conflito.
 *
 * Considere intervalo [entrada, saida) (entrada inclusa, saida exclusiva).
 */
int verificarConflitoDatas(
    Estadia estadias[], int qtEstadias,
    int numeroQuarto,
    int d1, int m1, int a1,
    int d2, int m2, int a2
) {
    time_t novo_inicio = data_para_time(d1, m1, a1, 14);
    time_t novo_fim = data_para_time(d2, m2, a2, 12);

    if (difftime(novo_fim, novo_inicio) <= 0) {
        // período inválido (saída <= entrada) — considerar como conflito
        return 1;
    }

    for (int i = 0; i < qtEstadias; ++i) {
        if (!estadias[i].ativo) continue;
        if (estadias[i].numeroQuarto != numeroQuarto) continue;

        time_t inicio_existente = data_para_time(
            estadias[i].diaEntrada, estadias[i].mesEntrada, estadias[i].anoEntrada, 14
        );
        time_t fim_existente = data_para_time(
            estadias[i].diaSaida, estadias[i].mesSaida, estadias[i].anoSaida, 12
        );

        // Overlap if novo_inicio < fim_existente && inicio_existente < novo_fim
        if (difftime(novo_inicio, fim_existente) < 0 && difftime(inicio_existente, novo_fim) < 0) {
            return 1; // conflito
        }
    }
    return 0; // sem conflito
}

/*
 * Função para cadastrar uma estadia.
 * Recebe arrays de estadias, clientes e quartos, com seus respectivos tamanhos.
 * Solicita ao usuário: código do cliente, quantidade de hóspedes, datas de entrada/saída.
 * Procura um quarto disponível que suporte a quantidade de hóspedes e sem conflito de datas.
 * Se encontrar, cria a estadia, marca quarto como ocupado e incrementa *qtEstadias.
 */
void cadastrarEstadia(
    Estadia estadias[], int *qtEstadias,
    Cliente clientes[], int qtClientes,
    Quarto quartos[], int qtQuartos
) {
    if (qtClientes <= 0) {
        printf("Nenhum cliente cadastrado. Cadastre um cliente antes de registrar uma estadia.\n");
        return;
    }
    if (qtQuartos <= 0) {
        printf("Nenhum quarto cadastrado. Cadastre quartos antes de registrar uma estadia.\n");
        return;
    }

    int codigoCliente;
    printf("Informe o codigo do cliente: ");
    if (scanf("%d", &codigoCliente) != 1) {
        printf("Codigo invalido.\n");
        while (getchar() != '\n'); // limpar stdin
        return;
    }

    int idxCliente = buscarClientePorCodigo(clientes, qtClientes, codigoCliente);
    if (idxCliente < 0) {
        printf("Cliente com codigo %d nao encontrado.\n", codigoCliente);
        return;
    }

    int qtdHospedes;
    printf("Quantidade de hospedes: ");
    if (scanf("%d", &qtdHospedes) != 1 || qtdHospedes <= 0) {
        printf("Quantidade invalida.\n");
        while (getchar() != '\n');
        return;
    }

    int de, me, ae;
    int ds, ms, as;
    printf("Data de entrada (DD MM AAAA): ");
    if (scanf("%d %d %d", &de, &me, &ae) != 3) {
        printf("Data de entrada invalida.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Data de saida (DD MM AAAA): ");
    if (scanf("%d %d %d", &ds, &ms, &as) != 3) {
        printf("Data de saida invalida.\n");
        while (getchar() != '\n');
        return;
    }

    // procurar quarto disponível que suporte qtdHospedes e sem conflito
    int encontrado = 0;
    int numeroQuartoEscolhido = -1;
    for (int i = 0; i < qtQuartos; ++i) {
        if (!quartos[i].ativo) continue;
        if (quartos[i].capacidade < qtdHospedes) continue;
        if (quartos[i].status != DESOCUPADO) continue;

        // verificar conflito de datas com outras estadias para este quarto
        if (verificarConflitoDatas(estadias, *qtEstadias, quartos[i].numero,
                                   de, me, ae, ds, ms, as)) {
            continue; // conflito, testar próximo quarto
        }

        // quarto atende requisitos
        numeroQuartoEscolhido = quartos[i].numero;
        encontrado = 1;
        // marcar como ocupado e usaremos esse quarto
        quartos[i].status = OCUPADO;
        break;
    }

    if (!encontrado) {
        printf("Nao foi possivel encontrar um quarto disponivel para essas datas e quantidade de hospedes.\n");
        return;
    }

    // preparar a nova estadia
    int novoCodigo = 1;
    for (int i = 0; i < *qtEstadias; ++i) {
        if (estadias[i].codigo >= novoCodigo) novoCodigo = estadias[i].codigo + 1;
    }

    int pos = *qtEstadias;
    estadias[pos].codigo = novoCodigo;
    estadias[pos].codigoCliente = codigoCliente;
    estadias[pos].numeroQuarto = numeroQuartoEscolhido;
    estadias[pos].diaEntrada = de;
    estadias[pos].mesEntrada = me;
    estadias[pos].anoEntrada = ae;
    estadias[pos].diaSaida = ds;
    estadias[pos].mesSaida = ms;
    estadias[pos].anoSaida = as;
    estadias[pos].qtdDiarias = calcularDiarias(de, me, ae, ds, ms, as);
    estadias[pos].ativo = 1;

    (*qtEstadias)++;

    printf("Estadia cadastrada com sucesso!\n");
    printf("Codigo estadia: %d\n", novoCodigo);
    printf("Quarto reservado: %d\n", numeroQuartoEscolhido);
    printf("Quantidade de diarias: %d\n", estadias[pos].qtdDiarias);
}

/*
 * Finaliza (faz checkout) de uma estadia identificada pelo seu codigo.
 * Calcula o total a pagar (qtdDiarias * valorDaDiaria) e libera o quarto (status desocupado).
 */
void finalizarEstadia(
    Estadia estadias[], int qtEstadias,
    Quarto quartos[], int qtQuartos,
    int codigoEstadia
) {
    int idx = -1;
    for (int i = 0; i < qtEstadias; ++i) {
        if (estadias[i].ativo && estadias[i].codigo == codigoEstadia) {
            idx = i;
            break;
        }
    }

    if (idx == -1) {
        printf("Estadia com codigo %d nao encontrada ou ja finalizada.\n", codigoEstadia);
        return;
    }

    // localizar o quarto para obter o valor da diaria
    int idxQuarto = buscarQuartoPorNumero(quartos, qtQuartos, estadias[idx].numeroQuarto);
    if (idxQuarto < 0) {
        printf("Erro: quarto %d nao encontrado no cadastro.\n", estadias[idx].numeroQuarto);
        return;
    }

    float valorDiaria = quartos[idxQuarto].valorDiaria;
    int diarias = estadias[idx].qtdDiarias;
    double total = valorDiaria * diarias;

    printf("Informacoes da estadia:\n");
    printf("Codigo: %d\n", estadias[idx].codigo);
    printf("Cliente: %d\n", estadias[idx].codigoCliente);
    printf("Quarto: %d\n", estadias[idx].numeroQuarto);
    printf("Diarias: %d\n", diarias);
    printf("Valor por diaria: %.2f\n", valorDiaria);
    printf("Valor total a pagar: %.2f\n", total);

    // marcar estadia como finalizada (inativa)
    estadias[idx].ativo = 0;

    // liberar o quarto
    quartos[idxQuarto].status = DESOCUPADO;

    printf("Estadia finalizada e quarto %d liberado.\n", estadias[idx].numeroQuarto);
}

/*
 * Lista na tela todas as estadias de um determinado cliente.
 * O parametro nomeOuCodigo pode ser um codigo (string numerica) ou um nome.
 * Usaremos as funções buscarClientePorCodigo e buscarClientePorNome (presumidas em cliente.h)
 */
void listarEstadiasDeCliente(
    Estadia estadias[], int qtEstadias,
    Cliente clientes[], int qtClientes,
    char nomeOuCodigo[]
) {
    int codigoCliente = -1;

    // verificar se a string é um numero (codigo)
    int isNumber = 1;
    for (size_t i = 0; i < strlen(nomeOuCodigo); ++i) {
        if (nomeOuCodigo[i] < '0' || nomeOuCodigo[i] > '9') { isNumber = 0; break; }
    }

    if (isNumber) {
        codigoCliente = atoi(nomeOuCodigo);
        int idx = buscarClientePorCodigo(clientes, qtClientes, codigoCliente);
        if (idx < 0) {
            printf("Cliente com codigo %d nao encontrado.\n", codigoCliente);
            return;
        }
    } else {
        int idxNome = buscarClientePorNome(clientes, qtClientes, nomeOuCodigo);
        if (idxNome < 0) {
            printf("Cliente com nome '%s' nao encontrado.\n", nomeOuCodigo);
            return;
        }
        codigoCliente = clientes[idxNome].codigo;
    }

    int achou = 0;
    printf("Estadias do cliente (codigo %d):\n", codigoCliente);
    for (int i = 0; i < qtEstadias; ++i) {
        if (!estadias[i].ativo) continue;
        if (estadias[i].codigoCliente == codigoCliente) {
            achou = 1;
            printf("Codigo estadia: %d | Quarto: %d | Entrada: %02d/%02d/%04d | Saida: %02d/%02d/%04d | Diarias: %d\n",
                   estadias[i].codigo,
                   estadias[i].numeroQuarto,
                   estadias[i].diaEntrada, estadias[i].mesEntrada, estadias[i].anoEntrada,
                   estadias[i].diaSaida, estadias[i].mesSaida, estadias[i].anoSaida,
                   estadias[i].qtdDiarias);
        }
    }

    if (!achou) {
        printf("Nenhuma estadia ativa encontrada para esse cliente.\n");
    }
}
