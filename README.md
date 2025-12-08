# Hotel Descanso Garantido

Trabalho das disciplinas Algoritmos e Estruturas de Dados (AEDs) e Fundamentos de Engenharia de Software.

O sistema Hotel Descanso Garantido é uma aplicação em linguagem C para gerenciamento básico de um hotel.
O programa permite:
- Cadastro de clientes
- Cadastro de quartos
- Registro de estadias

Integrantes:
- Henrique Fernandes
- Arthur Gomes
- Lucas Silva

## 🚀 Como Compilar e Rodar o Sistema

Para utilizar o sistema principal (Menu do Hotel), utilize o comando abaixo no terminal:

```powershell
gcc src/*.c -I include -o hotel.exe
./hotel.exe


Para rodar o teste de clientes, use este comando específico:

gcc tests/test_cliente.c src/cliente.c -I include -o teste_clientes.exe
./teste_clientes.exe