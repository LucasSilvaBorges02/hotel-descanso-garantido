#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include "../include/cliente.h"
#include "../include/funcionario.h"
#include "../include/quarto.h"
#include "../include/estadia.h"

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao;

    do {
        printf("\n=== HOTEL DESCANSO GARANTIDO ===\n");
        printf("1. Cadastrar Cliente\n");
        printf("2. Listar Clientes\n");
        printf("3. Cadastrar Funcionario\n");
        printf("4. Listar Funcionarios\n");
        printf("5. Cadastrar Quarto\n");
        printf("6. Listar Quartos\n");
        printf("7. Nova Estadia (Check-in)\n");
        printf("8. Baixa Estadia (Check-out)\n");
        printf("9. Listar Estadias\n");
        printf("0. SAIR\n");
        printf("Escolha: ");
        
        scanf("%d", &opcao);
        getchar(); // Limpa o buffer

        switch(opcao) {
            case 1: {
                Cliente c;
                printf("Codigo: "); scanf("%d", &c.codigo); getchar();
                printf("Nome: "); fgets(c.nome, 100, stdin); c.nome[strcspn(c.nome, "\n")] = 0;
                printf("Tel: "); fgets(c.telefone, 20, stdin); c.telefone[strcspn(c.telefone, "\n")] = 0;
                printf("End: "); fgets(c.endereco, 150, stdin); c.endereco[strcspn(c.endereco, "\n")] = 0;
                cadastrar_cliente(c);
                break;
            }
            case 2: 
                listar_clientes(); 
                break;
            case 3: 
                cadastrarFuncionario(); 
                break;
            case 4: 
                listarFuncionario(); 
                break;
            case 5: {
                Quarto q;
                printf("Numero Quarto: "); scanf("%d", &q.numero);
                printf("Capacidade: "); scanf("%d", &q.capacidade);
                printf("Valor Diaria: "); scanf("%f", &q.valorDiaria);
                cadastrarQuarto(&q);
                break;
            }
            case 6: 
                listarQuartos(); 
                break;
            case 7: 
                cadastrarEstadia(); 
                break;
            case 8: 
                baixarEstadia(); 
                break;
            case 9:
                listarEstadias();
                break;
            case 0: 
                printf("Saindo...\n"); 
                break;
            default: 
                printf("Invalido!\n");
        }
    } while (opcao != 0);

    return 0;
}