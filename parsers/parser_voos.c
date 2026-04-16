#define _GNU_SOURCE 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_aeronaves.h"
#include "gestor_entidades/gestor_aeroportos.h"
#include "utils/utils.h"
#include <ctype.h>

// --- PARSER VOOS ---
Voos* parser_voo(char* linha) {
    
    // Array para guardar os ponteiros dos 12 campos
    char *tokens[15] = {NULL}; 

    int num_colunas = parse_csv_line(linha, tokens, 12); 

    // Verifica se leu todos os 12 campos
    if (num_colunas < 12) return NULL; 

    // Atribuir os tokens às variáveis correspondentes
    char *flight_id        = tokens[0];
    char *departure        = tokens[1];
    char *actual_departure = tokens[2];
    char *arrival          = tokens[3];
    char *actual_arrival   = tokens[4];
    char *gate             = tokens[5];
    char *status           = tokens[6];
    char *origin           = tokens[7];
    char *destination      = tokens[8];
    char *aircraft         = tokens[9];
    char *airline          = tokens[10];
    char *tracking_url     = tokens[11];

    // Validações dos campos 
    if (!valida_flight_id(flight_id) || 
        !validar_iata_code(origin) || 
        !validar_iata_code(destination) || 
        !validar_datetime(departure) || 
        !validar_datetime(arrival)) {
        return NULL;
    }

    // Validação do estado do voo(Cancelled vs N/A)
    // Usamos strcasecmp para ignorar maiúsculas/minúsculas 
    bool isCancelled = (strcasecmp(status, "Cancelled") == 0);

    if (isCancelled) {
        // Se cancelado, as datas reais têm de ser "N/A" 
        if (strcasecmp(actual_departure, "N/A") != 0 || strcasecmp(actual_arrival, "N/A") != 0) {
            return NULL;
        }
    } else {
        // Se não cancelado, têm de ser datas válidas
        if (!validar_datetime(actual_departure) || !validar_datetime(actual_arrival)) {
            return NULL;
        }
    }

    // Criar a struct Voos 
    return criar_voo(flight_id, departure, actual_departure, arrival, actual_arrival,
                     gate, status, origin, destination, aircraft, airline, tracking_url);
}

int carregar_voos_csv(GestorVoos* gv, GestorAeronaves* ga, const char* filename) {    
    // Abrir ficheiro
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro: %s\n", filename);
        return -1;
    }

    // Variáveis auxiliares para o getline
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    const char* errorFilePath = "resultados/flights_errors.csv";

    // 2. Ler e Tratar HEADER (Primeira linha)
    if ((read = getline(&line, &len, file)) != -1) {
        // Remover (\n) se existir
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';
        registarErro(errorFilePath, line, true); // true = é header
    }

    // 3. Ler Linhas de dados
    while ((read = getline(&line, &len, file)) != -1) {
        
        // Limpeza de caracteres de nova linha (\n)
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';  

        // Ignorar linhas vazias
        if (read < 5) continue; 

        // Fazemos uma cópia da linha antes do parser a "partir" com \0
        char *linha_original = strdup(line);

        // Chamar o parser
        Voos* voo = parser_voo(line);
        
        if (voo == NULL) {
            registarErro(errorFilePath, linha_original, false); // Erro de parsing
            free(linha_original);
            continue;
        }

        // Validação lógica do voo (verificar aeronave, etc.)
        if (!valida_voo_logica(voo, ga)) {
            registarErro(errorFilePath, linha_original, false); // Erro lógico
            libertar_voo(voo);
            free(linha_original);
            continue;
        }

        adiciona_voos(gv, voo);
        count++;
        free(linha_original);

    }

    free(line);
    fclose(file);
    
    return count;
}