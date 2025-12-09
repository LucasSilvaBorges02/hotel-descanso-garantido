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

// --- NOVO: Necessário para o teste automatizado acessar a lógica matemática ---
int calcularDiarias(int d1, int m1, int a1, int d2, int m2, int a2);

int calcularPontosFidelidade(int codCliente);
void cadastrarEstadia();
void baixarEstadia();
void listarEstadias();

#endif