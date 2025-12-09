#include <stdio.h>
#include "../include/munit.h"
#include "../include/estadia.h" // Precisa ter calcularDiarias exposto aqui

// --- TESTES ---

MunitResult teste_calculo_dias_simples(const MunitParameter params[], void* data) {
    // De 01/01/2025 a 05/01/2025 = 4 diárias
    int dias = calcularDiarias(1, 1, 2025, 5, 1, 2025);
    
    munit_assert_int(dias, ==, 4);
    
    return MUNIT_OK;
}

MunitResult teste_calculo_virada_mes(const MunitParameter params[], void* data) {
    // De 30/01/2025 a 02/02/2025 
    // Jan tem 31 dias. 30->31(1), 31->01(2), 01->02(3). Total 3 dias.
    int dias = calcularDiarias(30, 1, 2025, 2, 2, 2025);
    
    munit_assert_int(dias, ==, 3);
    
    return MUNIT_OK;
}

MunitResult teste_data_invalida(const MunitParameter params[], void* data) {
    // Saída antes da entrada (Erro)
    // A função retorna 0 ou 1 dependendo da sua implementação de erro, 
    // mas geralmente retorna <= 0 se data for errada.
    int dias = calcularDiarias(10, 1, 2025, 5, 1, 2025);
    
    // Esperamos 0 ou tratado como 1 minímo dependendo da lógica, 
    // mas matematicamente é negativo/zero.
    munit_assert_int(dias, <=, 1); 
    
    return MUNIT_OK;
}

// --- SUÍTE ---
static MunitTest testes[] = {
    { (char*) "/calculo-simples", teste_calculo_dias_simples, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/calculo-mes", teste_calculo_virada_mes, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/data-invalida", teste_data_invalida, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
    (char*) "Estadia-Tests", testes, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, (void*) "µnit", argc, argv);
}