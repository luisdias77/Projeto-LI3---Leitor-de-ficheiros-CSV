#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <string.h>
#include <time.h>

//gestores
#include "gestor_entidades/gestor_aeronaves.h"
#include "gestor_entidades/gestor_aeroportos.h"
#include "gestor_entidades/gestor_passageiros.h"
#include "gestor_entidades/gestor_reservas.h"
#include "gestor_entidades/gestor_voos.h"
//queries
#include "queries/query1.h"
#include "queries/query2.h"
#include "queries/query3.h"
#include "queries/query4.h"
#include "queries/query5.h"
#include "queries/query6.h"
//parsers
#include "parsers/parser_voos.h"
#include "parsers/parser_aeroporto.h"
#include "parsers/parser_aeronave.h"
#include "parsers/parser_passageiro.h"
#include "parsers/parser_reservas.h"


typedef struct query_stats {
    double total_time_q1;
    int count_q1;
    double total_time_q2;
    int count_q2;
    double total_time_q3;
    int count_q3;
    double total_time_q4;
    int count_q4;
    double total_time_q5;
    int count_q5;
    double total_time_q6;
    int count_q6;
    // ... outras queries se necessario tem de estar aqui
} QueryTimingStats;

void ini_queryTStats( QueryTimingStats *stats) {
    stats->total_time_q1 = 0.0;
    stats->count_q1 = 0;
    stats->total_time_q2 = 0.0;
    stats->count_q2 = 0;
    stats->total_time_q3 = 0.0;
    stats->count_q3 = 0;
    stats->total_time_q4 = 0.0;
    stats->count_q4 = 0;
    stats->total_time_q5 = 0.0;
    stats->count_q5 = 0;
    stats->total_time_q6 = 0.0;
    stats->count_q6 = 0;
    // Inicializar outras queries aqui se for necessario
}


// O programa principal irá receber dois argumentos, o caminho para a pasta onde estão os ficheiros csv de entrada e o segundo é o caminho para um ficheiro de texto que contém uma lista de comandos (queries) a serem executados 
int main(int argc, char *argv[]) {
    // Se o numero de argumentos for diferente de 3, ou seja, o nome do programa(programa principal) mais os dois argumentos
    if (argc != 3) {
        printf("Número de argumentos incorretos!\n");
        printf("Número de argumentos correto: ./programa-principal <pasta_dataset> <ficheiro_input>\n");
        return 1;
    }

    const char *pasta_dataset = argv[1];
    const char *ficheiro_input = argv[2];

    printf("Dataset: %s\n", pasta_dataset);
    printf("Input: %s\n", ficheiro_input);
    
    //const char *program_outputs_dir = "resultados/"; 
    const char *query_times_log_file = "test-phase1/query_times.log"; // Aponta para 'test-phase1/'
    //const char *memory_usage_log_file = "test-phase1/memory_usage.log";
    // Funções que criam os gestores para as diferentes entidades 
    GestorAeronaves* gestor_aeronaves = criar_gestor_aeronaves();
    GestorAeroportos* gestor_aeroportos = criar_gestor_aeroportos();
    GestorVoos* gestor_voos = criar_gestor_voos();
    GestorPassageiros* gestor_passageiros = criar_gestor_passageiros();
    GestorReservas* gestor_reservas = criar_gestor_reservas();
    

    // Carregar os dados dos ficheiros CSV para os gestores 
    // Caminho para o ficheiro airports.csv, feito a partir da pasta_dataset recebida como argumento
    char caminho_airports[512]; // array para guardar o caminho completo do ficheiro airports.csv
    // snprintf para criar o caminho, ela recebe o array onde guarda o caminho, o tamanho dele e o formato do airports.csv
    snprintf(caminho_airports, sizeof(caminho_airports), "%s/airports.csv", pasta_dataset); 
    printf("Número de aeroportos carregados: %d\n", carregar_aeroportos_csv(gestor_aeroportos, caminho_airports));
    
    char caminho_aircrafts[512]; // array para guardar o caminho completo do ficheiro aircrafts.csv
    snprintf(caminho_aircrafts, sizeof(caminho_aircrafts), "%s/aircrafts.csv", pasta_dataset);
    printf("Número de aeronaves carregadas: %d\n", carregar_aeronaves_csv(gestor_aeronaves, caminho_aircrafts));

    char caminho_flights[512]; // array para guardar o caminho completo do ficheiro flights.csv
    // snprintf para criar o caminho, ela recebe o array onde guarda o caminho, o tamanho dele e o formato do flights.csv
    snprintf(caminho_flights, sizeof(caminho_flights), "%s/flights.csv", pasta_dataset); 
    printf("Número de voos carregados: %d\n", carregar_voos_csv(gestor_voos, gestor_aeronaves, caminho_flights));
    
    char caminho_passengers[512]; // array para guardar o caminho completo do ficheiro passengers.csv
    snprintf(caminho_passengers, sizeof(caminho_passengers), "%s/passengers.csv", pasta_dataset); 
    printf("Número de passageiros carregados: %d\n", carregar_passageiros_csv(gestor_passageiros, caminho_passengers));

    char caminho_reservations[512]; // array para guardar o caminho completo do ficheiro reservations.csv
    snprintf(caminho_reservations, sizeof(caminho_reservations), "%s/reservations.csv", pasta_dataset); 
    printf("Número de reservas carregadas: %d\n", carregar_reservas_csv(gestor_reservas, gestor_voos, gestor_passageiros, caminho_reservations));

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

    // Abre o ficheiro input 
    FILE* input = fopen(ficheiro_input, "r"); // Usa "r" para ler o ficheiro
    if (!input) {
        perror("Erro ao abrir ficheiro de input");
        destruir_gestor_aeronaves(gestor_aeronaves);
        destruir_gestor_aeroportos(gestor_aeroportos);
        destruir_gestor_voos(gestor_voos);
        destruir_gestor_passageiros(gestor_passageiros);
        destruir_gestor_reservas(gestor_reservas);
        return 1;
    }
    
    // estrutura para guardar as estatisticas de tempo de cada query --- TESTES 
    QueryTimingStats query_stats;
    ini_queryTStats(&query_stats);
    struct timespec start_time_query, end_time_query;
    double elapsed_time_query;

    struct timespec start_time_loop, end_time_loop;
    double elapsed_time_loop;

   
    // Lê o ficheiro input linha a linha e executa a querie 1, 2 ou 3, dependente do comando lido 
    int linha_comando = 1; // contador para o numero da linha do comando no ficheiro input 
    char line[128]; // array que guarda cada linha do ficheiro

    // Estrutura de dados para armazennar o tempode execuçao de cada query

    // Mede o tempo ANTES de começar a processar a primeira query
    clock_gettime(CLOCK_REALTIME, &start_time_loop);

    while (fgets(line, sizeof(line), input)) { // 

        char comando_str[20] = {0}; // array para guardar o comando lido na linha
        int comando = 0; // pode ser 1, 2 ou 3
        char argumento1[65] = {0}; // array para guardar o argumento da querie
        char argumento2[65] = {0}; // array para guardar o segundo argumento da querie (apenas para a querie 3)
        char argumento3[65] = {0}; // array para guardar o terceiro argumento da querie (apenas para a querie 4)
        char separador = ';'; // O separador padrão é ';' 
        
        int n = sscanf(line, "%s %s %s %s", comando_str, argumento1, argumento2, argumento3); // vai ler o comando (1/1S,2/2S .... ) e depois os argumentos da querie

        char nome_output[65]; // array para guardar o nome do ficheiro output
        sprintf(nome_output, "resultados/command%d_output.txt", linha_comando); // cria o nome do ficheiro output, mediante o numero da linha do comando no input
        linha_comando++; // o contador da linha é incrementado
        

        FILE *output = fopen(nome_output, "w"); // abre o ficheiro output e

        clock_gettime(CLOCK_REALTIME, &start_time_query); // inicia a contagem do tempo de execução da query

        // Verifica qual o comando lido (se tem S ou não) e executa a querie correspondente

        if (strchr (comando_str, 'S') != NULL) { // se encontrar o caractere 'S' na string do comando
            separador = '='; // muda o separador para '='
        }

        comando = atoi(comando_str); // converte a string do comando para inteiro

        if (comando == 1 && n >= 2) { // se o comando for 1 e tiver lido o argumento1
            query1 (argumento1, gestor_aeroportos, separador, output); // executa a querie 1
            //fprintf(general_output_file_log, "Q1: OK para %s\n", line);
            query_stats.count_q1++;
        }
        
        else if (comando == 2 && n >= 2) { // se o comando for 2 e tiver lido o argumento1
            int N = atoi(argumento1); // Converte a string do número para inteiro
            query2 (N, argumento2, gestor_voos, gestor_aeronaves, separador, output); // executa a querie 2
            //fprintf (stderr,"Linha %d a ser lida \n", linha_comando -1);
            //fprintf(general_output_file_log, "Q2: OK para %s\n", line); 
            query_stats.count_q2++;
        }
        
        else if (comando == 3 && n >= 3) { // se o comando for 3 e tiver lido os dois argumentos
            query3 (gestor_voos, gestor_aeroportos, argumento1, argumento2, separador, output); // executa a querie 3
            //fprintf(general_output_file_log, "Q3: OK para %s\n", line); 
            query_stats.count_q3++;
        }

        else if (comando == 4) {
            char *p_inicio = NULL; 
            char *p_fim = NULL;

            // 1. Lógica segura para Datas
            // n=1 -> "4" ou "4S" (Sem datas)
            // n=3 -> "4 data1 data2" (Com datas)
            if (n >= 3) {
                p_inicio = argumento1;
                p_fim = argumento2;
                
            } else {
                p_inicio = NULL;
                p_fim = NULL;
            }

            
            query4(p_inicio, p_fim, separador, gestor_passageiros, output);
            
            query_stats.count_q4++;
        }

        else if (comando == 5 && n >= 2) {
            //int top = 0; // variavel que guarda o número N para a querie 5
            int top = atoi(argumento1); // Converte a string do número para inteiro
            query5 (top, gestor_voos, separador, output);
            query_stats.count_q5++;
        }

        else if (comando == 6 && n >= 2) {
            // Procura o primeiro espaço na linha original (logo a seguir ao "6" ou "6S")
            char* inicio_nome = strchr(line, ' '); 
            
            if (inicio_nome != NULL) {
                // Avança ponteiro para saltar os espaços em branco
                while (*inicio_nome == ' ') inicio_nome++;

                // Copia tudo o que sobra da linha para o argumento1
                strcpy(argumento1, inicio_nome);

                // Remover o (\n) que vem com o fgets
                int len = strlen(argumento1);
                while (len > 0 && (argumento1[len-1] == '\n')) {
                    argumento1[len-1] = '\0';
                    len--;
                }
            }
            
            query6(argumento1, gestor_reservas, gestor_voos, gestor_passageiros, separador, output);
            query_stats.count_q6++;
        }

        clock_gettime(CLOCK_REALTIME, &end_time_query);

        elapsed_time_query = (end_time_query.tv_sec - start_time_query.tv_sec) +
                             (end_time_query.tv_nsec - start_time_query.tv_nsec) / 1000000000.0;

        // Somar o tempo total para a query que acabou de ser executada
        if (comando == 1 && n >= 2) { // Verificar as mesmas condições de execução da query
            query_stats.total_time_q1 += elapsed_time_query;
        } else if (comando == 2 && n >= 2) {
            query_stats.total_time_q2 += elapsed_time_query;
        } else if (comando == 3 && n >= 3) {
            query_stats.total_time_q3 += elapsed_time_query;
        } else if (comando == 4) { // Removido o "&& n >= 4" para contar todos os casos
            query_stats.total_time_q4 += elapsed_time_query;
        } else if (comando == 5 && n >= 2) {
            query_stats.total_time_q5 += elapsed_time_query;
        } else if (comando == 6 && n >= 2) {
            query_stats.total_time_q6 += elapsed_time_query;
        }

        
        fclose(output); // fecha o ficheiro output
        //linha++;
    }

    clock_gettime(CLOCK_REALTIME, &end_time_loop);

    fclose (input);
    //fclose(general_output_file_log);

    destroy_query2();
    destroy_query3();
    destroy_query4();
    destroy_query6();

    // Libertar a memória dos gestores
    destruir_gestor_aeroportos(gestor_aeroportos);
    destruir_gestor_voos(gestor_voos);
    destruir_gestor_aeronaves(gestor_aeronaves);
    destruir_gestor_passageiros(gestor_passageiros);
    destruir_gestor_reservas(gestor_reservas);

    // --- LOG DE TEMPOs de execução ---  

    FILE *times_log = fopen(query_times_log_file, "w");
    if (times_log == NULL) {
        perror("Erro ao abrir ficheiro de log de tempos para escrita final");
    } 
    else {
        if (query_stats.count_q1 > 0)
            fprintf(times_log, "Q1_MEDIA: %.6f segundos (de %d execuções)\n",
                    query_stats.total_time_q1 / query_stats.count_q1, query_stats.count_q1);
        if (query_stats.count_q2 > 0)
            fprintf(times_log, "Q2_MEDIA: %.6f segundos (de %d execuções)\n",
                    query_stats.total_time_q2 / query_stats.count_q2, query_stats.count_q2);
        if (query_stats.count_q3 > 0)
            fprintf(times_log, "Q3_MEDIA: %.6f segundos (de %d execuções)\n",
                    query_stats.total_time_q3 / query_stats.count_q3, query_stats.count_q3);
        if (query_stats.count_q4 > 0)
            fprintf(times_log, "Q4_MEDIA: %.6f segundos (de %d execuções)\n",
                    query_stats.total_time_q4 / query_stats.count_q4, query_stats.count_q4);
        if (query_stats.count_q5 > 0)
            fprintf(times_log, "Q5_MEDIA: %.6f segundos (de %d execuções)\n",
                    query_stats.total_time_q5 / query_stats.count_q5, query_stats.count_q5);
        if (query_stats.count_q6 > 0)
            fprintf(times_log, "Q6_MEDIA: %.6f segundos (de %d execuções)\n",
                    query_stats.total_time_q6 / query_stats.count_q6, query_stats.count_q6);

        // Adicionar aqui outras queries (Q4, Q5, etc.)

        // Calcular o tempo total do loop
        elapsed_time_loop = (end_time_loop.tv_sec - start_time_loop.tv_sec) +
                            (end_time_loop.tv_nsec - start_time_loop.tv_nsec) / 1000000000.0;
        
        // Escreve o tempo total no final do mesmo ficheiro de log
        fprintf(times_log, "\nTempo Total de Processamento de Queries: %.6f segundos\n", elapsed_time_loop);
    }
        fclose(times_log);

    // --- LOG DE USO DE MEMÓRIA  ---
    const char *memory_usage_log_file = "test-phase1/memory_usage.log"; 

    FILE *mem_log = fopen(memory_usage_log_file, "w");
    if (mem_log == NULL) {
        perror("Erro ao abrir ficheiro de log de memória");
        return 1; 
    }

    struct rusage usage;
    long peak_rss_kb = 0;

    // Chamar getrusage para o processo atual (RUSAGE_SELF)
    if (getrusage(RUSAGE_SELF, &usage) == 0) {
        peak_rss_kb = usage.ru_maxrss; 
        fprintf(mem_log, "Pico de memória residente (RSS): %ld KB\n", peak_rss_kb);
    } else {
        perror("Erro ao chamar getrusage para obter memória");
        fprintf(mem_log, "Erro ao obter uso de memória.\n");
    }
    fclose(mem_log);


    return 0;

}