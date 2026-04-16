#include "utils/utils.h"
#include <string.h> // Para strlen, strcmp, etc.
#include <ctype.h>  // Para isdigit
#include <stdio.h> // Para sscanf
#include <stdbool.h> // Para bool
#include <stdlib.h>

// Função que vai registar os erros nos ficheiros de erros
void registarErro(const char* errorFilePath, const char* linha, bool isHeader) {
    static bool headerEscrito = false; // controla se o header já foi escrito
    
    const char* mode;
    if (isHeader && !headerEscrito) {
        mode = "w"; // Primeira vez: escreve header
        headerEscrito = true;
    } else {
        mode = "a"; // Sempre append depois do header
    }

    FILE *errorFile = fopen(errorFilePath, mode);
    if (errorFile == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro de erros: %s\n", errorFilePath);
        return;
    }

    fprintf(errorFile, "%s\n", linha);
    fclose(errorFile);
}

// Função para validar um código IATA (deve ter exatamente 3 letras maiúsculas)
int validar_iata_code(const char* code) {
    // Verifica se o código é NULL ou não tem exatamente 3 caracteres
    if (code == NULL || strlen(code) != 3) {
        return 0; // Inválido
    }
    // Verifica se todos os caracteres são letras maiúsculas
    for (int i = 0; i < 3; i++) {
        if (code[i] < 'A' || code[i] > 'Z') {
            return 0; // Inválido
        }
    }
    return 1; // Válido
}

// Função para validar um Flight ID (formato ccddddd ou ccdddddd)
int valida_flight_id(const char* flight_id) {
    if (flight_id == NULL) return 0;
    
    int len = strlen(flight_id);

    // REGRA: ccddddd (7 chars) OU ccdddddd (8 chars)
    if (len != 7 && len != 8) {
        return 0; 
    }

    // As duas primeiras têm de ser Letras Maiúsculas [A-Z]
    if (!isupper(flight_id[0]) || !isupper(flight_id[1])) {
        return 0;
    }
    
    // O resto têm de ser apenas Dígitos [0-9]
    for (int i = 2; i < len; i++) {
        if (!isdigit(flight_id[i])) {
            return 0;
        }
    }

    return 1;
}


// Função auxiliar que verifica se uma string contem apenas dígitos
static int is_all_digits(const char* string) {
    if (string == NULL || *string == '\0') return 0; // string nula ou vazia é inválida
    for (int i = 0; string[i] != '\0'; i++) {
        if (!isdigit(string[i])) 
            return 0;   // Encontrou um caractere que não é dígito
        }
    return 1; // Todos os caracteres são dígitos
}

// Função para validar um número de documento (deve ter exatamente 9 dígitos)
int validar_doc_number(const char* doc_number) {
    // Verifica se o número do documento é NULL ou não tem exatamente 9 caracteres
    if (doc_number == NULL || strlen(doc_number) != 9) {
        return 0; 
    }
    // 2. Verifica se todos os elementos são dígitos 
    return is_all_digits(doc_number);
}

// Função para validar o formato de uma lista CSV
int valida_formato_lista_csv (const char* lista_csv) {

    // Verifica se a string é NULL
    if (lista_csv == NULL) {
        return 0;
    }

    int len = strlen(lista_csv);
    if (len < 2) return 0; // mínimo: "[]"

    if (lista_csv[0] == '[' && lista_csv[len - 1] == ']') {
        return 1; // Formato valido 
    }

    return 0; // Formato inválido
}

// Função para validar uma data, tem de estar no formato aaaa-mm-dd e não pode ser futura
int validar_data(const char* data) {
    // Valida o formato e o comprimento 
    if (data == NULL || strlen(data) != 10) return 0; 
    if (data[4] != '-' || data[7] != '-') return 0;

    int ano, mes, dia; 
    if (sscanf (data, "%4d-%2d-%2d", &ano, &mes, &dia) != 3) {
        return 0; // Falha ao analisar a data
    }

    // Verifica se o mês e dia são válidos
    if (mes < 1 || mes > 12 || dia < 1 || dia > 31) {
        return 0; // Mês ou dia inválido
    }

    // Verifica se a data não é futura (a data atual é 2025-09-30)
    if (ano > 2025 || (ano == 2025 && mes > 9) || (ano == 2025 && mes == 9 && dia > 30)) {
        return 0; // Data futura
    }

    return 1; // Data válida
}

// Função para validar date time, ou seja o formato "aaaa-mm-dd hh:mm"
int validar_datetime(const char* date_time) {
    // Valida o formato e o comprimento 
    if (date_time == NULL || strlen(date_time) != 16) return 0; 
    if (date_time[10] != ' ') return 0;

    // Copia a parte da data para uma string separada e valida
    char data_data[11]; // 10 caracteres + 1 para o '\0'
    strncpy(data_data, date_time, 10);
    data_data[10] = '\0'; // Termina a string

    // Utiliza se a funcao anterior para validar a data
    if (!validar_data(data_data)) {
        return 0; // Data inválida
    }

    // Validar a parte do time
    int hora, minuto; 
    if (date_time[13] != ':' || sscanf (date_time + 11, "%2d:%2d", &hora, &minuto) != 2) {
        return 0; // formato inválido
    }

    // Verifica se a hora e minutos são válidos
    if (hora < 0 || hora > 23 || minuto < 0 || minuto > 59) {
        return 0; // hora ou minuto inválido
    }

    return 1; // Tudo válido
}

// Valida se uma string de ano (aaaa) é válida.
int validar_ano(const char* ano_str) {
    // 1. Validar formato: 4 caracteres e todos dígitos
    if (ano_str == NULL || strlen(ano_str) != 4) {
        return 0; // Erros como "203" ou "20234"
    }
    if (!is_all_digits(ano_str)) {
        return 0; // Erros como "20a3"
    }
    
    int ano = atoi(ano_str);
    
    // 2. Validar se não é futuro (data atual 2025/09/30) 
    if (ano > 2025) {
        return 0; // Ano futuro
    }
    
    return 1; // Ano válido
}

// Função Genérica que parte uma linha CSV em tokens
int parse_csv_line(char* linha, char** tokens, int max_tokens) {
    int coluna = 0;
    bool dentro_aspas = false;
    char *start = linha;

    // Se o primeiro campo começar com aspas, avança já
    if (*start == '"') start++; 

    for (int i = 0; linha[i] != '\0' && coluna < max_tokens; i++) {
        
        if (linha[i] == '"') {
            dentro_aspas = !dentro_aspas;
        }
        else if (linha[i] == ',' && !dentro_aspas) { 
            
            linha[i] = '\0'; // Corta a string
            
            // Tira aspa do fim do token anterior
            if (i > 0 && linha[i-1] == '"') linha[i-1] = '\0';

            tokens[coluna++] = start;
            
            // Prepara próximo token
            start = &linha[i + 1];
            // Tira aspa do início do próximo token
            if (*start == '"') start++; 
        }
    }

    // Tratar o último token (remover \n e aspas finais)
    int len = strlen(start);
    while (len > 0 && (start[len-1] == '\n')) {
        start[len-1] = '\0';
        len--;
    }
    if (len > 0 && start[len-1] == '"') start[len-1] = '\0';
    
    tokens[coluna++] = start;

    return coluna;
}
