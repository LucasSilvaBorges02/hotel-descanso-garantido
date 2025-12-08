#ifndef QUARTO_H
#define QUARTO_H

// Definição de constantes para facilitar leitura
#define DESOCUPADO 0
#define OCUPADO 1

typedef struct {
    int numero;
    int capacidade;
    float valorDiaria;
    int status; // 0 = DESOCUPADO, 1 = OCUPADO
    int ativo;  // 1 = Quarto existe, 0 = Removido (logicamente)
} Quarto;

// Protótipos
void cadastrarQuarto(Quarto *q);
void listarQuartos();
void atualizarStatusQuarto(int numero, int novoStatus);
float buscarValorDiaria(int numero);

#endif