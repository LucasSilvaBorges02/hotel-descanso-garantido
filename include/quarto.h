#ifndef QUARTO_H
#define QUARTO_H

typedef enum { OCUPADO = 1, DESOCUPADO = 0 } StatusQuarto;

typedef struct {
    int numero;
    int capacidade;
    float valorDiaria;
    StatusQuarto status;
    int ativo; 
} Quarto;

void cadastrarQuarto(Quarto *novoQuarto);
void listarQuartos();
float buscarValorDiaria(int numeroQuarto);
void atualizarStatusQuarto(int numeroQuarto, StatusQuarto novoStatus);

#endif