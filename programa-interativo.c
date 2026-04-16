#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// gestores
#include "gestor_entidades/gestor_aeroportos.h"
#include "gestor_entidades/gestor_aeronaves.h"
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_passageiros.h"
#include "gestor_entidades/gestor_reservas.h"

// queries
#include "queries/query1.h"
#include "queries/query2.h"
#include "queries/query3.h"
#include "queries/query4.h"
#include "queries/query5.h"
#include "queries/query6.h"

// parsers
#include "parsers/parser_aeroporto.h"
#include "parsers/parser_aeronave.h"
#include "parsers/parser_voos.h"
#include "parsers/parser_passageiro.h"
#include "parsers/parser_reservas.h"


int main() {
    
    printf ("Bem Vindo ao Programa Interativo!\n");
    printf("------------------------------------------\n");
    printf("Se desejar sair do programa, digite 'EXIT'.\n");
    printf("------------------------------------------\n\n");
    // array para guardar o caminho para a pasta do dataset, que é pedido ao utilizador
    char caminho_dataset[256];

    // Funções que criam os gestores para as diferentes entidades
    GestorAeroportos* gestor_aeroportos = criar_gestor_aeroportos();
    GestorAeronaves* gestor_aeronaves = criar_gestor_aeronaves();
    GestorVoos* gestor_voos = criar_gestor_voos();
    GestorPassageiros* gestor_passageiros = criar_gestor_passageiros();
    GestorReservas* gestor_reservas = criar_gestor_reservas();

    while (1) {
        
        int carregamento_correto = 1;
        int count; 
        
        printf("Introduza o caminho dos ficheiros de dados (Pressione ENTER para usar um caminho default): ");
        
        // Lê o caminho do dataset que o utilizador insere, incluindo o caso em que o utilizador apenas pressiona ENTER. Usamos break para sair do ciclo em caso de erro na leitura
        if (fgets(caminho_dataset, sizeof(caminho_dataset), stdin) == NULL) break;

        // Aqui é removido o \n que o fgets pode adicionar ao final da string
        caminho_dataset[strcspn(caminho_dataset, "\n")] = 0;

        // Se o utilizador apenas pressionar ENTER, usar o caminho default
        if (strlen(caminho_dataset) == 0) {
            strcpy(caminho_dataset, "dataset-fase2/dataset-fase-2_sem_erros");
            printf("A usar caminho default: %s\n", caminho_dataset);
        }
    
        // Verificar se o utilizador quer sair do programa
        if (strcmp(caminho_dataset, "EXIT") == 0) {
            printf("A sair do programa interativo...\n");
            
            // Libertar a memória dos gestores antes de sair
            destruir_gestor_aeroportos(gestor_aeroportos);
            destruir_gestor_aeronaves(gestor_aeronaves);
            destruir_gestor_voos(gestor_voos);
            destruir_gestor_passageiros(gestor_passageiros);
            destruir_gestor_reservas(gestor_reservas);
            return 0; 
        }
    
    // Carregar os dados dos ficheiros CSV
    char caminho_airports[512];
    snprintf(caminho_airports, sizeof(caminho_airports), "%s/airports.csv", caminho_dataset);
    printf("Número de aeroportos carregados: %d\n",
    count = carregar_aeroportos_csv(gestor_aeroportos, caminho_airports));
    if (count < 0) carregamento_correto = 0;

    char caminho_aircrafts[512];
    snprintf(caminho_aircrafts, sizeof(caminho_aircrafts), "%s/aircrafts.csv", caminho_dataset);
    printf("Número de aeronaves carregadas: %d\n",
    count = carregar_aeronaves_csv(gestor_aeronaves, caminho_aircrafts));
    if (count < 0) carregamento_correto = 0;

    char caminho_flights[512];
    snprintf(caminho_flights, sizeof(caminho_flights), "%s/flights.csv", caminho_dataset);
    printf("Número de voos carregados: %d\n",
    count = carregar_voos_csv(gestor_voos, gestor_aeronaves, caminho_flights));
    if (count < 0) carregamento_correto = 0;

    char caminho_passengers[512];
    snprintf(caminho_passengers, sizeof(caminho_passengers), "%s/passengers.csv", caminho_dataset);
    printf("Número de passageiros carregados: %d\n",
    count = carregar_passageiros_csv(gestor_passageiros, caminho_passengers));
    if (count < 0) carregamento_correto = 0;

    char caminho_reservations[512];
    snprintf(caminho_reservations, sizeof(caminho_reservations), "%s/reservations.csv", caminho_dataset);
    printf("Número de reservas carregadas: %d\n",
    count = carregar_reservas_csv(gestor_reservas, gestor_voos, gestor_passageiros, caminho_reservations));
    if (count < 0) carregamento_correto = 0;

    // Inicializa as estatísticas da query 1 (pré-processamento)
    calcula_estatisticas_aeroportos (gestor_aeroportos, gestor_reservas, gestor_voos);
    // Inicializa a cache da query 2 (pré-processamento)
    init_query2(gestor_voos, gestor_aeronaves);
    // Inicializa a cache da query 3 (pré-processamento)
    init_query3(gestor_voos);
    // Inicializa a cache da query 4 (pré-processamento)
    init_query4(gestor_reservas, gestor_voos, gestor_passageiros); 
    // Inicializa a cache da query 6 (pré-processamento)
    init_query6(gestor_reservas, gestor_voos, gestor_passageiros);
    
    if (carregamento_correto) {
        printf ("Dataset carregado com sucesso.\n\n");
        break;
    } else {
        printf("Erro no carregamento dos ficheiros. Insira o caminho novamente.\n");
    }
    }


    // Ciclo interativo para executar a querie pedida pelo utilizador 
    while (1) {

        char comando_str[20];
        int q = 0; // variavel que guarda o número da query que o utilizador pretende executar 
        char separador = ','; // O separador padrão é ','

        printf("Que query deseja executar? ");

        if (scanf("%s", comando_str) != 1) {
            printf("Erro na leitura do comando.\n");
            continue;
        }

        if (strcmp(comando_str, "EXIT") == 0) {
            printf("A sair do programa interativo...\n");
            break; // sai do ciclo, terminando o programa
        }

        if (strchr (comando_str, 'S') != NULL) { // se encontrar o caractere 'S' na string do comando
            separador = '='; // muda o separador para '='
        }

        q = atoi(comando_str); // converte a string do comando para inteiro 
        if (q == 1) {

            char code[32];
            printf("Código do aeroporto: ");
            scanf("%s", code);

            printf("Output: ");
            query1(code, gestor_aeroportos, separador, stdout);
        }

        else if (q == 2) {

            int N;
            char aircraft[32];

            printf("Número N: ");
            scanf("%d", &N);

            printf("Modelo da aeronave: ");
            scanf("%s", aircraft);

            printf("Output:\n");
            query2(N, aircraft, gestor_voos, gestor_aeronaves, separador, stdout);
        }

        else if (q == 3) {

            char origin[32], dest[32];

            printf("Data inicial: ");
            scanf("%s", origin);

            printf("Data final: ");
            scanf("%s", dest);

            printf("Output:\n");
            query3(gestor_voos, gestor_aeroportos, origin, dest, separador, stdout);
        }

        else if (q == 4) {
            char data_inicio[32];
            char data_fim[32];
            char *p_inicio = NULL; 
            char *p_fim = NULL;

            // perguntamos se o utilizados vai usar filtros de datas
            printf("Deseja filtrar por intervalo de datas? (s/n): ");
            char opcao;
            scanf(" %c", &opcao); 

            if (opcao == 's' || opcao == 'S') {
                printf("Data Início (YYYY-MM-DD): ");
                scanf("%s", data_inicio);
                
                printf("Data Fim (YYYY-MM-DD): ");
                scanf("%s", data_fim);
                
                
                p_inicio = data_inicio;
                p_fim = data_fim;
            }

            printf("Output:\n");
            
            query4(p_inicio, p_fim, separador, gestor_passageiros, stdout); 
        }
        

        else if (q == 5) {
            int top;

            printf("Top N: ");
            scanf("%d", &top);

            printf("Output:\n");
            query5(top, gestor_voos, separador, stdout);
        }

        else if (q==6) {
            char nacionalidade[64];

            printf("Nacionalidade: ");
            scanf("%s", nacionalidade);

            printf("Output:\n");
            query6(nacionalidade, gestor_reservas, gestor_voos, gestor_passageiros, separador, stdout);
        }

        else {
            printf("Query inválida.\n");
        }
    }

    // Libertar memória
    destruir_gestor_aeroportos(gestor_aeroportos);
    destruir_gestor_aeronaves(gestor_aeronaves);
    destruir_gestor_voos(gestor_voos);
    destruir_gestor_passageiros(gestor_passageiros);
    destruir_gestor_reservas(gestor_reservas);

    return 0;
}
