#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestor_entidades/gestor_aeroportos.h"
#include "utils/utils.h"

// --- PARSER AEROPORTOS ---
Aeroportos* parser_aeroporto(char* linha) {
    
    // Array para guardar os ponteiros dos 8 campos
    char *tokens[10] = {NULL}; 

    int num_colunas = parse_csv_line(linha, tokens, 8); 

    // Verificação de colunas (Tem de ter 8)
    if (num_colunas < 8) return NULL; 

    // Atribuir os tokens às variáveis correspondentes
    char *code    = tokens[0];
    char *name    = tokens[1];
    char *city    = tokens[2];
    char *country = tokens[3];
    char *lat_str = tokens[4];
    char *lon_str = tokens[5];
    char *icao    = tokens[6]; 
    char *type    = tokens[7]; 

    // Validações dos campos
    if (!validar_iata_code(code) || 
        !validar_tipo_aeroporto(type) || 
        validacaoLatitude(lat_str) ||   
        validacaoLongitude(lon_str)) { 
        return NULL;
    }

    // Conversao de tipos
    double latitude = atof(lat_str);
    double longitude = atof(lon_str);

    // Criar a struct Aeroporto
    return criar_aeroporto(code, name, city, country, latitude, longitude, icao, type);
}

// Função que vai carregar os aeroportos a partir de um ficheiro csv
int carregar_aeroportos_csv(GestorAeroportos* ga, const char* filename) {
    
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
    
    // Caminho do ficheiro de erros 
    const char* errorFilePath = "resultados/airports_errors.csv"; 

    // Ler e Tratar HEADER (Primeira linha)
    if ((read = getline(&line, &len, file)) != -1) {
        // Remover (\n) se existir
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';
        registarErro(errorFilePath, line, true); // true = é header
    }

    // Ler Linhas de dados
    while ((read = getline(&line, &len, file)) != -1) {
        
        // Limpeza de caracteres de nova linha (\n)
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';

        char *linha_original = strdup(line);

        // Chamar o parser
        Aeroportos* aeroporto = parser_aeroporto(line);
        
        if (aeroporto == NULL) {
            registarErro(errorFilePath, linha_original, false); // Erro de parsing/validação
            free(linha_original);
            continue;
        }

        // Adicionar ao gestor
        adiciona_aeroporto(ga, aeroporto);
        count++;
        free(linha_original);
    }

    free(line);
    fclose(file);
    
    return count;
}