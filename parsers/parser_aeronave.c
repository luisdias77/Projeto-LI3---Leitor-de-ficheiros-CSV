#define _GNU_SOURCE
#include "gestor_entidades/gestor_aeronaves.h"
#include "utils/utils.h"
#include <stdio.h>

// --- PARSER AERONAVES ---
Aeronaves* parser_aeronave(char* linha) {
    
    // Array para guardar os ponteiros dos campos
    char *tokens[10] = {NULL}; 

    int num_colunas = parse_csv_line(linha, tokens, 6); 

    // Verifica se leu os 6 campos 
    if (num_colunas < 6) return NULL; 

    // Atribuir os tokens às variáveis correspondentes
    char *identifier     = tokens[0];
    char *manufacturer   = tokens[1];
    char *model          = tokens[2];
    char *year_str       = tokens[3];
    char *capacity_str   = tokens[4];
    char *range_str      = tokens[5];

    // Validações dos campos
    if (!validar_ano(year_str)) {
         return NULL;
    }

    // Conversao de tipos
    int year = atoi(year_str);
    int capacity = atoi(capacity_str);
    int range = atoi(range_str);

    // Criação da struct 
    return criar_aeronave(identifier, manufacturer, model, year, capacity, range);
}

// Função que vai carregar as aeronaves a partir de um ficheiro csv
int carregar_aeronaves_csv(GestorAeronaves* ga, const char* filename) {
    
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
    const char* errorFilePath = "resultados/aircrafts_errors.csv";

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
        Aeronaves* aeronave = parser_aeronave(line);
        
        if (aeronave == NULL) {
            registarErro(errorFilePath, linha_original, false); // Erro de parsing ou validação
            free(linha_original);
            continue;
        }

        // Adicionar ao gestor
        adiciona_aeronave(ga, aeronave);
        count++;
        free(linha_original);
    }

    free(line);
    fclose(file);
    
    return count;
}