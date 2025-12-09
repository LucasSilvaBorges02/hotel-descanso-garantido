#include <stdio.h>
#include "../include/munit.h"
#include "../include/quarto.h"

// --- TESTES ---

MunitResult teste_criar_quarto_valido(const MunitParameter params[], void* data) {
    Quarto q;
    q.numero = 901; // ID alto para teste
    q.capacidade = 2;
    q.valorDiaria = 200.0;
    q.status = DESOCUPADO; // Usando a constante do .h
    q.ativo = 1;

    cadastrarQuarto(&q);

    // Verifica se o valor foi salvo corretamente
    // buscarValorDiaria retorna o valor se achar, ou 0 se não achar
    munit_assert_double(buscarValorDiaria(901), ==, 200.0);

    return MUNIT_OK;
}

MunitResult teste_valor_negativo(const MunitParameter params[], void* data) {
    Quarto q;
    q.numero = 902;
    q.capacidade = 2;
    q.valorDiaria = -50.0; // INVÁLIDO!
    q.status = DESOCUPADO;
    q.ativo = 1;

    // O sistema deve recusar o cadastro
    cadastrarQuarto(&q);

    // Verifica se o quarto NÃO foi salvo (deve retornar 0.0)
    munit_assert_double(buscarValorDiaria(902), ==, 0.0);

    return MUNIT_OK;
}

// --- SUÍTE ---
static MunitTest testes[] = {
    { (char*) "/quarto-valido", teste_criar_quarto_valido, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/validacao-negativo", teste_valor_negativo, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
    (char*) "Quarto-Tests", testes, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, (void*) "µnit", argc, argv);
}