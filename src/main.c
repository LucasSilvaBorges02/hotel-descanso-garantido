#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>

#include "../include/cliente.h"
#include "../include/funcionario.h"
#include "../include/quarto.h"
#include "../include/estadia.h"

// Função para limpar o lixo de memória do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    setlocale(LC_ALL, "Portuguese");
    int opcao = -1; 

    do {
        opcao = -1; 
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
        printf("10. Remover Cliente\n");
        printf("11. Remover Funcionario\n"); // <--- AQUI ESTÁ ELA!
        printf("0. SAIR\n");
        printf("Escolha: ");
        
        // Blindagem do menu contra letras
        if (scanf("%d", &opcao) != 1) {
            printf("\n--> Entrada invalida! Digite apenas numeros.\n");
            limparBuffer();
            continue;
        }
        limparBuffer(); // Consome o Enter

        switch(opcao) {
            case 1: {
                Cliente c;
                printf("Codigo: "); 
                if(scanf("%d", &c.codigo) == 1) {
                    limparBuffer();
                    printf("Nome: "); fgets(c.nome, 100, stdin); c.nome[strcspn(c.nome, "\n")] = 0;
                    printf("Tel: "); fgets(c.telefone, 20, stdin); c.telefone[strcspn(c.telefone, "\n")] = 0;
                    printf("End: "); fgets(c.endereco, 150, stdin); c.endereco[strcspn(c.endereco, "\n")] = 0;
                    cadastrar_cliente(c);
                } else { printf("Codigo invalido.\n"); limparBuffer(); }
                break;
            }
            case 2: listar_clientes(); break;
            
            case 3: { 
                // Lógica de cadastro no Main (para permitir teste automatizado na função salvar)
                Funcionario f;
                printf("--- Cadastro de Funcionario ---\n");
                printf("ID: ");
                if(scanf("%d", &f.id) == 1) {
                    limparBuffer();
                    if(funcionarioExiste(f.id)) {
                        printf("Erro: Ja existe funcionario com este ID.\n");
                    } else {
                        printf("Nome: "); fgets(f.nome, 50, stdin); f.nome[strcspn(f.nome, "\n")] = 0;
                        printf("Cargo: "); fgets(f.cargo, 30, stdin); f.cargo[strcspn(f.cargo, "\n")] = 0;
                        printf("Salario: "); 
                        if(scanf("%f", &f.salario) == 1) {
                            limparBuffer();
                            cadastrarFuncionario(f); // Salva
                        } else {
                            printf("Erro: Salario invalido.\n"); limparBuffer();
                        }
                    }
                } else {
                    printf("Erro: ID invalido.\n"); limparBuffer();
                }
                break;
            }

            case 4: listarFuncionario(); break;
            case 5: {
                Quarto q;
                printf("Numero Quarto: "); 
                if(scanf("%d", &q.numero) == 1) {
                    printf("Capacidade: "); scanf("%d", &q.capacidade);
                    printf("Valor Diaria: "); scanf("%f", &q.valorDiaria);
                    cadastrarQuarto(&q);
                } else { printf("Numero invalido.\n"); limparBuffer(); }
                break;
            }
            case 6: listarQuartos(); break;
            case 7: cadastrarEstadia(); break;
            case 8: baixarEstadia(); break;
            case 9: listarEstadias(); break;
            case 10: remover_cliente(); break;
            case 11: removerFuncionario(); break; // <--- CHAMADA DA FUNÇÃO
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
    return 0;
}