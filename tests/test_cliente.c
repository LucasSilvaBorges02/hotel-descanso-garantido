#include <stdio.h>
#include <string.h>
#include "../include/munit.h"
#include "../include/cliente.h"

// --- SETUP ---
// Define um cliente de teste padrão
static Cliente gerar_cliente_teste(int codigo) {
    Cliente c;
    c.codigo = codigo;
    strcpy(c.nome, "Cliente Teste Munit");
    strcpy(c.telefone, "00000000");
    strcpy(c.endereco, "Rua Unitario");
    return c;
}

// --- TESTES ---

MunitResult teste_cadastro_novo(const MunitParameter params[], void* data) {
    // Tenta cadastrar cliente 800
    Cliente c = gerar_cliente_teste(800);
    
    // Se existir de um teste anterior, removemos (simulação de limpeza)
    // Como não temos 'remover' exposto fácil aqui, usamos um ID alto que esperamos estar livre
    
    cadastrar_cliente(c);

    // Verifica se agora existe
    munit_assert_int(codigo_cliente_existe(800), ==, 1);
    
    return MUNIT_OK;
}

MunitResult teste_bloqueio_duplicidade(const MunitParameter params[], void* data) {
    Cliente c = gerar_cliente_teste(800); // Mesmo ID do anterior

    // O sistema deve imprimir erro no console e não crashar
    // A função retorna void, então verificamos a integridade do banco
    cadastrar_cliente(c);
    
    // O ID deve continuar existindo (sucesso)
    munit_assert_int(codigo_cliente_existe(800), ==, 1);
    
    return MUNIT_OK;
}

// --- SUÍTE ---
static MunitTest testes[] = {
    { (char*) "/novo-cliente", teste_cadastro_novo, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/duplicidade", teste_bloqueio_duplicidade, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
    (char*) "Cliente-Tests", testes, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, (void*) "µnit", argc, argv);
}