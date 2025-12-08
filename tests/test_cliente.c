#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cliente.h"

// Como estamos compilando apenas o teste, não usamos o main.c original.
// Criamos um main específico apenas para rodar os testes.

void teste_cadastro_sucesso() {
    printf("\n[TESTE 1] Tentativa de Cadastro Novo...\n");
    
    Cliente c;
    c.codigo = 999; // Usamos um ID alto para não misturar com os seus
    strcpy(c.nome, "Cliente Teste Automatizado");
    strcpy(c.telefone, "00000000");
    strcpy(c.endereco, "Rua dos Testes, 0");

    // Verifica se já existe antes (para garantir que o teste é limpo)
    if (!codigo_cliente_existe(999)) {
        cadastrar_cliente(c);
        
        // Verificação
        if (codigo_cliente_existe(999)) {
            printf("RESULTADO: SUCESSO (Cliente cadastrado e encontrado).\n");
        } else {
            printf("RESULTADO: FALHA (Cliente nao foi salvo).\n");
        }
    } else {
        printf("AVISO: Cliente 999 ja existia. O teste de sucesso nao pode ser validado corretamente.\n");
    }
}

void teste_duplicidade() {
    printf("\n[TESTE 2] Tentativa de Duplicidade (Deve falhar)...\n");
    
    Cliente c;
    c.codigo = 999; // O mesmo ID do teste anterior
    strcpy(c.nome, "Cliente Duplicado");
    strcpy(c.telefone, "11111111");
    strcpy(c.endereco, "Rua Falsa");

    printf("Esperado: Mensagem de erro de duplicidade abaixo:\n");
    printf("--- SAIDA DO SISTEMA ---\n");
    cadastrar_cliente(c); // Deve imprimir o erro no console
    printf("------------------------\n");
}

int main() {
    printf("=== INICIANDO BATERIA DE TESTES: CLIENTES ===\n");
    
    teste_cadastro_sucesso();
    teste_duplicidade();
    
    printf("\n=== FIM DOS TESTES ===\n");
    return 0;
}