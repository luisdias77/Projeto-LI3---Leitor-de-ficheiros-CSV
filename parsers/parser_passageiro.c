#define _GNU_SOURCE
#include "gestor_entidades/gestor_passageiros.h"
#include "utils/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- PARSER PASSAGEIROS ---
Passageiros* parser_passageiro(char* linha) {
    
    // Array para guardar os ponteiros dos 10 campos
    char *tokens[12] = {NULL}; 
    
    int num_colunas = parse_csv_line(linha, tokens, 10); 

    // Verificar se leu os 10 campos
    if (num_colunas < 10) return NULL; 

    // Atribuir os campos às variáveis correspondentes
    char *doc_number  = tokens[0];
    char *first_name  = tokens[1];
    char *last_name   = tokens[2];
    char *dob         = tokens[3];
    char *nationality = tokens[4];
    char *gender      = tokens[5];
    char *email       = tokens[6];
    char *phone       = tokens[7];
    char *address     = tokens[8];
    char *photo       = tokens[9];

    // Validar os campos necessários
    if (!validar_doc_number(doc_number) || 
        !validar_data(dob) || 
        !validar_genero(gender) || 
        !validar_email(email)) {
        return NULL;
    }

    // Criar a struct passageiro
    return criar_passageiro(doc_number, first_name, last_name, dob, nationality, gender, email, phone, address, photo);
}

// Função que vai carregar as passageiros a partir de um ficheiro csv
int carregar_passageiros_csv(GestorPassageiros* gp, const char* filename) {
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro: %s\n", filename);
        return -1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int count = 0;
    const char* errorFilePath = "resultados/passengers_errors.csv";

    // Ler e ignorar HEADER
    if ((read = getline(&line, &len, file)) != -1) {
        // Remover (\n) se existir
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';
        registarErro(errorFilePath, line, true); // true = é header
    }

    // Loop de Leitura
    while ((read = getline(&line, &len, file)) != -1) {
        
        // Limpar quebras de linha
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0'; 

        // Ignorar linhas muito curtas
        if (strlen(line) < 5) continue;

        char *linha_original = strdup(line);

        // Chamar Parser
        Passageiros* p = parser_passageiro(line);

        if (p) {
            adiciona_passageiro(gp, p);
            count++;
        } else {
            registarErro(errorFilePath, linha_original, false); // Erro
        }

        free(linha_original);
    }

    free(line);
    fclose(file);
    return count;
}