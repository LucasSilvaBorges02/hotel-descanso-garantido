#ifndef ESTADIA_H
#define ESTADIA_H

#include "cliente.h"
#include "quarto.h"

typedef struct {
    int codigo;
    int codigoCliente;
    int numeroQuarto;
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;
    int qtdDiarias;
    int ativo; // 1 = ativa, 0 = finalizada
} Estadia;

// Funções ajustadas para acesso direto a arquivo (sem vetores)
void cadastrarEstadia();
void baixarEstadia(); 
void listarEstadias();
// Se quiser implementar a listagem depois:
// void listarEstadiasDeCliente(char nomeOuCodigo[]); 

int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2);

#endif