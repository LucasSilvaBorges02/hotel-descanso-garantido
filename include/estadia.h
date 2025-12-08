#ifndef ESTADIA_H
#define ESTADIA_H

typedef struct {
    int codigo;
    int codigoCliente;
    int numeroQuarto;
    int qtdHospedes;
    int diaEntrada, mesEntrada, anoEntrada;
    int diaSaida, mesSaida, anoSaida;
    int qtdDiarias;
    int ativo; // 1 = Check-in feito, 0 = Check-out realizado
} Estadia;

// Protótipos
int calcularPontosFidelidade(int codCliente);

void cadastrarEstadia();
void baixarEstadia();
void listarEstadias();

#endif