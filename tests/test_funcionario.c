#include <stdio.h>
#include <string.h>
#include "../include/munit.h"
#include "../include/funcionario.h"

// --- TESTES ---

MunitResult teste_cadastro_funcionario(const MunitParameter params[], void* data) {
    Funcionario f;
    f.id = 777; // ID de teste
    strcpy(f.nome, "Tester Munit");
    strcpy(f.cargo, "QA Engineer");
    f.salario = 5000.50;

    // Tenta cadastrar (agora a função aceita a struct direto!)
    cadastrarFuncionario(f);

    // Verifica se foi salvo no arquivo (Deve retornar 1)
    munit_assert_int(funcionarioExiste(777), ==, 1);

    return MUNIT_OK;
}

MunitResult teste_validacao_existencia(const MunitParameter params[], void* data) {
    // Verifica um ID que sabemos que NÃO existe (ex: 99999)
    // O sistema deve retornar 0 (Falso)
    munit_assert_int(funcionarioExiste(99999), ==, 0);
    
    // Verifica o ID 777 que acabamos de criar no teste anterior
    // O sistema deve retornar 1 (Verdadeiro)
    munit_assert_int(funcionarioExiste(777), ==, 1);

    return MUNIT_OK;
}

// --- SUÍTE ---
static MunitTest testes[] = {
    { (char*) "/cadastro-func", teste_cadastro_funcionario, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { (char*) "/validacao-id", teste_validacao_existencia, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL },
    { NULL, NULL, NULL, NULL, MUNIT_TEST_OPTION_NONE, NULL }
};

static const MunitSuite suite = {
    (char*) "Funcionario-Tests", testes, NULL, 1, MUNIT_SUITE_OPTION_NONE
};

int main(int argc, char* argv[]) {
    return munit_suite_main(&suite, (void*) "µnit", argc, argv);
}