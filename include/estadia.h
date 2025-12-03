#ifndef ESTADIA_H
#define ESTADIA_H

#include "cliente.h"
#include "quarto.h"

#define MAX_ESTADIAS 200

typedef struct {
    int codigo;
    int codigoCliente;
    int numeroQuarto;
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;
    int qtdDiarias;
    int ativo; // 1 = estadia registrada
} Estadia;

// Funções principais
void cadastrarEstadia(
    Estadia estadias[], int *qtEstadias,
    Cliente clientes[], int qtClientes,
    Quarto quartos[], int qtQuartos
);

void finalizarEstadia(
    Estadia estadias[], int qtEstadias,
    Quarto quartos[], int qtQuartos,
    int codigoEstadia
);

int calcularDiarias(
    int d1, int m1, int a1,
    int d2, int m2, int a2
);

void listarEstadiasDeCliente(
    Estadia estadias[], int qtEstadias,
    Cliente clientes[], int qtClientes,
    char nomeOuCodigo[]
);

// Funções auxiliares
int verificarConflitoDatas(
    Estadia estadias[], int qtEstadias,
    int numeroQuarto,
    int d1, int m1, int a1,
    int d2, int m2, int a2
);

#endif
