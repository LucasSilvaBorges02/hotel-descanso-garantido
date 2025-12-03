#ifndef QUARTO_H
#define QUARTO_H

#define MAX_QUARTOS 50

typedef enum { OCUPADO = 1, DESOCUPADO = 0 } StatusQuarto;

typedef struct {
    int numero;
    int capacidade;
    float valorDiaria;
    StatusQuarto status;
    int ativo; // 1 = cadastrado
} Quarto;

void cadastrarQuarto(Quarto quartos[], int *qtQuartos);
int buscarQuartoDisponivel(Quarto quartos[], int qt, int qtdHospedes);
int buscarQuartoPorNumero(Quarto quartos[], int qt, int numero);
void listarQuarto(Quarto q);

#endif

