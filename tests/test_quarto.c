#include "munit/munit.h"
#include "../include/quarto.h"
#include <stdio.h>
#include <string.h>

static MunitResult test_cadastrar_quarto(const MunitParameter params[], void *data) {

    // Limpa arquivo
    FILE *f = fopen("data/quartos.dat", "wb");
    fclose(f);

    Quarto q;
    q.numero = 101;
    q.capacidade = 2;
    q.valorDiaria = 150.0;

    cadastrarQuarto(&q);

    munit_assert_int(quartoExiste(101), ==, 1);
    munit_assert_int(quartoExiste(888), ==, 0);

    float valor = buscarValorDiaria(101);
    munit_assert_double(valor, ==, 150.0);

    return MUNIT_OK;
}

static MunitTest tests[] = {
    {"/test_cadastrar_quarto", test_cadastrar_quarto, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite suite = {
    "/suite_quarto",
    tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}
