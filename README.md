# Hotel Descanso Garantido

Trabalho das disciplinas Algoritmos e Estruturas de Dados (AEDs) e Fundamentos de Engenharia de Software.

O sistema Hotel Descanso Garantido é uma aplicação em linguagem C para gerenciamento básico de um hotel.
O programa permite:
- Cadastro de clientes
- Cadastro de quartos
- Registro de estadias
- Cadastro de Funcionários

Integrantes:
- Henrique Fernandes
- Arthur Gomes
- Lucas Silva

## 🚀 Como Compilar e Rodar o Sistema

Para utilizar o sistema principal (Menu do Hotel), utilize o comando abaixo no terminal:

```powershell
gcc src/*.c -I include -o hotel.exe
./hotel.exe

TESTE MUnit👾


Para rodar o teste de estadia, use este comando específico:

gcc tests/test_estadia.c src/estadia.c src/cliente.c src/quarto.c src/munit.c -I include -o teste_estadia.exe
./teste_estadia.exe



Para rodar o teste de clientes, use este comando específico:

gcc tests/test_cliente.c src/cliente.c src/estadia.c src/quarto.c src/munit.c -I include -o teste_cliente.exe
./teste_cliente.exe



Para rodar o teste de quartos, use este comando específico:

gcc tests/test_quarto.c src/quarto.c src/munit.c -I include -o teste_quarto.exe
./teste_quarto.exe



Para rodar o teste de funcionário, use este comando específico:

gcc tests/test_funcionario.c src/funcionario.c src/munit.c -I include -o teste_funcionario.exe
./teste_funcionario.exe
