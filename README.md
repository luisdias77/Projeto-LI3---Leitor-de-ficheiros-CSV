# Flight Data Analyzer ✈️📊

Um sistema robusto e de alto desempenho desenvolvido em **C** para a leitura, processamento e análise de grandes volumes de dados da aviação civil (Aeroportos, Aeronaves, Voos, Passageiros e Reservas). 

Este projeto foi desenvolvido no âmbito da unidade curricular de Laboratórios de Informática III da **Universidade do Minho**, com um forte foco em estruturação de dados, gestão de memória e otimização de algoritmos.

## 🎯 Principais Funcionalidades

O sistema processa ficheiros CSV pesados e permite extrair dados estatísticos complexos através de um **Programa Interativo** via terminal ou processamento em *batch*.

**Queries Suportadas:**
* **Q1:** Resumo completo de um aeroporto (incluindo cálculo de tráfego de passageiros a chegar/partir).
* **Q2:** Top $N$ aeronaves com maior número de voos realizados (com filtro opcional por fabricante).
* **Q3:** Identificação do aeroporto com mais partidas num determinado intervalo de datas.
* **Q4:** Passageiro que esteve mais tempo no Top 10 semanal de gastos num determinado período.
* **Q5:** Top $N$ companhias aéreas com o maior tempo médio de atraso por voo.
* **Q6:** Aeroporto de destino mais comum para passageiros de uma determinada nacionalidade.

## 🧠 Arquitetura e Engenharia de Software

Para garantir um desempenho excelente em *datasets* com milhões de registos, o projeto adotou várias estratégias avançadas:

* **Modularidade e Encapsulamento:** Código estritamente dividido em módulos (`main`, `gestor_entidades`, `entidades`, `parsers`, `queries`, `utils`). Os dados das *structs* são opacos, acedidos exclusivamente através de *Getters* para proteger a integridade da memória.
* **Estruturas de Dados Dinâmicas (GLib):** Utilização intensiva de `GHashTable` para indexação rápida (acesso $O(1)$) e `GList` para ordenações.
* **Pré-Cálculo de Dados:** Para queries pesadas (como a Q1, Q4 e Q6), o sistema faz pré-processamento durante a fase de carregamento (ex: criação de *caches* semanais de gastos), permitindo que a consulta pelo utilizador seja executada em frações de milissegundo.
* **Gestão Rigorosa de Memória:** Otimização para zero *memory leaks* (validado exaustivamente com Valgrind), com funções dedicadas à destruição limpa de *Hash Tables* e estruturas encadeadas após o fecho do programa.
* **Validação de Dados:** Sistema robusto de *parsing* que identifica e isola linhas inválidas num ficheiro de erros durante a leitura dos CSVs.

## 🚀 Como Executar

### Pré-requisitos
* Compilador `gcc`
* Biblioteca `GLib`
* Ferramenta `make`

### Compilação
Na raiz do projeto, execute:
```bash
make
```

### Execução
O programa suporta dois modos:

**1. Modo Interativo (Terminal Dinâmico):**
```bash
./programa-interativo
```
*Irá solicitar o caminho para a pasta dos ficheiros CSV e permite a execução de queries em tempo real.*

**2. Modo Batch (Processamento em Lote):**
```bash
./programa-principal <caminho_para_dataset> <ficheiro_de_inputs.txt>
```

## 👥 Autores

Projeto desenvolvido pelo **Grupo 32** de LI3 (Universidade do Minho):
* **Gabriel Matos Rodrigues** (a110987)
* **Luís Pedro Matos Machado** (a110729)
* **Luís Araújo Dias** (a111474)
