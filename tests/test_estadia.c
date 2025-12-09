#include "munit/munit.h"
#include "../include/estadia.h"
#include "../include/quarto.h"
#include "../include/cliente.h"
#include <stdio.h>

static MunitResult test_calcular_diarias(const MunitParameter params[], void *data) {

    int dias = calcularDiarias(1, 1, 2025, 2, 1, 2025);
    munit_assert_int(dias, ==, 1);

    int dias2 = calcularDiarias(1, 1, 2025, 5, 1, 2025);
    munit_assert_int(dias2, ==, 4);

    return MUNIT_OK;
}

static MunitResult test_pontos_fidelidade(const MunitParameter params[], void *data) {

    // ZERAR ARQUIVO
    FILE *f = fopen("data/estadias.dat", "wb");
    fclose(f);

    Estadia e;
    e.codigoCliente = 10;
    e.qtdDiarias = 3;
    e.ativo = 0; // finalizada
    e.codigo = 999;
    e.numeroQuarto = 100;

    f = fopen("data/estadias.dat", "ab");
    fwrite(&e, sizeof(Estadia), 1, f);
    fclose(f);

    int pontos = calcularPontosFidelidade(10);
    munit_assert_int(pontos, ==, 30);

    return MUNIT_OK;
}

static MunitTest tests[] = {
    {"/test_calcular_diarias", test_calcular_diarias, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {"/test_pontos_fidelidade", test_pontos_fidelidade, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL},
    {NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL}
};

static const MunitSuite suite = {
    "/suite_estadia",
    tests,
    NULL,
    1,
    MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char *argv[]) {
    return munit_suite_main(&suite, NULL, argc, argv);
}
