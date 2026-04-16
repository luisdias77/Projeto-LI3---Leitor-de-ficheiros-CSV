#define _GNU_SOURCE
#include "gestor_entidades/gestor_reservas.h"
#include "gestor_entidades/gestor_passageiros.h"
#include "gestor_entidades/gestor_voos.h"
#include "entidades/reservas.h"
#include "utils/utils.h"
#include <stdio.h>

// --- PARSER RESERVAS ---
Reservas* parser_reserva(char* linha) {
    
    // Array para guardar os ponteiros para os 8 campos
    char *tokens[10] = {NULL}; 

    // 2. Chamar a função genérica
    int num_colunas = parse_csv_line(linha, tokens, 8);

    // O CSV de Reservas tem obrigatoriamente 8 colunas. Se tiver menos, descartamos.
    if (num_colunas < 7) return NULL; 

    // Atribuir os tokens às variáveis correspondentes
    char *id = tokens[0];
    char *flight_ids = tokens[1];
    char *doc = tokens[2];
    char *seat = tokens[3];
    char *price_str = tokens[4];
    char *lug_str = tokens[5];
    char *priority_str = tokens[6];
    char *qrcode = tokens[7];

    // Validações
    if (!validar_reservation_id(id) || !validar_doc_number(doc) || !valida_formato_lista_csv(flight_ids)) {
        return NULL;
    }

    // Converter "extra_luggage" para 0 ou 1
    unsigned char lug_val = 0;
    if (strcasecmp(lug_str, "true") == 0) {
        lug_val = 1;
    } else if (strcasecmp(lug_str, "false") == 0) {
        lug_val = 0;
    } else {
        return NULL; 
    }

    // Converter "priority_boarding" para 0 ou 1
    unsigned char priority_val = 0;
    if (priority_str && (strcasecmp(priority_str, "true") == 0)) {
        priority_val = 1;
    } else if (strcasecmp(priority_str, "false") == 0) {
        priority_val = 0;
    } else {
        return NULL;
    }
    
    // Criar a reserva usando os campos lidos
    // A função atof converte string para double
    return criar_reserva(id, flight_ids, doc, seat, atof(price_str), lug_val, priority_val, qrcode);
}


// Função que vai carregar as Reservas a partir de um ficheiro csv 
int carregar_reservas_csv (GestorReservas* gr, GestorVoos* gv, GestorPassageiros* gp, const char* filename) {
    
    // Abrir ficheiro
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o ficheiro: %s\n", filename);
        return -1;
    }

    // variáveis auxiliares para o getline
    char *line = NULL;  // O getline vai alocar o tamanho necessário aqui
    size_t len = 0;     // Tamanho do buffer
    ssize_t read;       // Quantos caracteres leu

    int count = 0;
    const char* errorFilePath = "resultados/reservations_errors.csv";

    // Ler e Ignorar a primeira linha (Cabeçalho)
    if ((read = getline(&line, &len, file)) != -1) {
        // Remover (\n) se existir
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';
        registarErro(errorFilePath, line, true);
    }
    
    // Ler cada linha do ficheiro
    while ((read = getline(&line, &len, file)) != -1) {
        
        // Limpeza de caracteres de nova linha (\n)
        if (read > 0 && line[read - 1] == '\n') line[read - 1] = '\0';

        char *linha_original = strdup(line);

        // Chamar o parser (agora recebe a linha inteira)
        Reservas* reserva = parser_reserva(line);
        
        if (reserva == NULL) {
            registarErro(errorFilePath, linha_original, false);
            free(linha_original);
            continue;
        }

        const char *id_reserva = get_reservation_id(reserva);

        // Se este ID já existe na tabela, é uma linha duplicada/inválida
        if (get_reserva(gr, id_reserva) != NULL) {
            registarErro(errorFilePath, linha_original, false);
            libertar_reserva(reserva);
            free(linha_original);
            continue;
        }

        // Validação lógica (Verificar se passageiro e voos existem)
        if (!valida_reserva_logica(reserva, gv, gp)) {
            registarErro(errorFilePath, linha_original, false);
            libertar_reserva(reserva);
            free(linha_original);
            continue;
        }

        adiciona_reservas(gr, reserva);
        count++;
        free(linha_original);
    }
    
    free(line); // libertar a memória que o getline alocou
    fclose(file);
    
    return count;
}