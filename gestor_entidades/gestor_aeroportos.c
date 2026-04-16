#include "gestor_entidades/gestor_aeroportos.h"
#include "utils/utils.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/*
 * ===================================================================
 * ===================  FUNÇÕES DO GESTOR  ===========================
 * ===================================================================
 */

// Definição do gestor de aeroportos
struct gestor_aeroportos {
    GHashTable* tabela_aeroportos;  // chave = código do aeroporto, valor = Aeroportos*
};

// Criação e destruição do gestor de aeroportos
GestorAeroportos* criar_gestor_aeroportos() {
    GestorAeroportos* g = malloc(sizeof(GestorAeroportos));
    g->tabela_aeroportos = g_hash_table_new_full(g_str_hash, 
                                                 g_str_equal,
                                                 g_free, 
                                                 (GDestroyNotify) libertar_aeroporto
                                                 );
    return g;
}

// Função para destruir o gestor de aeroportos
void destruir_gestor_aeroportos(GestorAeroportos* g) {
    if (g == NULL) return;
    g_hash_table_destroy(g->tabela_aeroportos);
    free(g);
}

// Função para adicionar um aeroporto
void adiciona_aeroporto(GestorAeroportos* g, Aeroportos* a) {

    if (g == NULL || a == NULL) return;
    
    // Obtem o codigo IATA do aeroporto
    // char* codigo = a->code;
    char* codigo = g_strdup(get_code_aeroporto(a)); // usa o getter para obter o codigo IATA do aeroporto
    if (codigo == NULL) return;
    
    // Adiciona o aeroporto à tabela 
    g_hash_table_replace(g->tabela_aeroportos,codigo,a);

}


// Função para verificar se um aeroporto existe, através do codigo IATA do aeroporto
bool aeroporto_existe(GestorAeroportos* g, char* codigo) {

    if (g == NULL || codigo == NULL) {
        return false;
    }

    if (g_hash_table_contains(g->tabela_aeroportos, codigo)) {
        return true;
    } else {
        return false;
    }

}


// Função que remove um voo
void remove_aeroporto(GestorAeroportos* g, char* codigo) {

    if (g == NULL || codigo == NULL) {
        return;
    }

    g_hash_table_remove(g->tabela_aeroportos, codigo);
    
}


// Função que conta o numero de aeroportos na hashtable 
int conta_aeroportos(GestorAeroportos* g) {
    if (g == NULL) { 
        return 0; 
        }
        else {
            return g_hash_table_size(g->tabela_aeroportos);
        }
    
}

/*
 * ===================================================================
 * ===================  FUNÇÕES DE VALIDAÇÃO  ========================
 * ===================================================================
 */

// Função para validar o tipo de aeroporto
int validar_tipo_aeroporto(const char* type) {
    if (type == NULL) return 0;
    return (strcmp(type, "small_airport" ) == 0 ||        // strcmp retorna 0 se as strings são iguais
            strcmp(type, "medium_airport") == 0 ||
            strcmp(type, "large_airport" ) == 0 ||
            strcmp(type, "heliport"      ) == 0 ||
            strcmp(type, "seaplane_base" ) == 0 );
}


// Função auxiliar para verificar o formato da latitude e longitude. [-]dd.ddddd (max 8 casas).
// Verifica erros como "18.AB123", "12-12345", "45.", ou mais de 8 casas decimais. 
// retorna 1 se o formato for válido, 0 caso contrário.
int valida_formato (const char* token) {
    if (token == NULL || *token == '\0') { // Verifica se a string é nula ou vazia
        return 0;
    }

    int i = 0;
    int ponto_encontrado = 0;
    int digitos_antes_ponto = 0;
    int digitos_depois_ponto = 0;

    // 1 - Verifica o sinal opcional 
    if (token[0] == '-') {
        i = 1;
        if (token[i] == '\0') return 0; // Se a string é apenas o sinal '-', não é válida
    }

    // 2 - Verifica os dígitos antes do ponto 
    while (token[i] != '\0' && token[i] != '.') {
        if (!(isdigit(token[i]))) {
            return 0; // Se não for dígito o caractere é inválido 
        }
        digitos_antes_ponto++;
        i++;
    }

    // 3 - Verifica o ponto
    if (token[i] == '.') {
        ponto_encontrado = 1;
        i++;
    }

    // 4 - Verifica os dígitos depois do ponto 
    while (token[i] != '\0') {
        if (!(isdigit(token[i]))) {
            return 0; // Se não for dígito o caractere é inválido
        }
        digitos_depois_ponto++;
        i++;
    }

    // 5 - Validações finais
    if (digitos_antes_ponto == 0 || (ponto_encontrado && digitos_depois_ponto == 0)) {
        return 0; // Deve haver pelo menos um dígito antes e depois do ponto, se o ponto existir
    }

    if (digitos_depois_ponto > 8) {
        return 0; // Mais de 8 casas decimais não são permitidas
    }

    return 1; // Formato válido
}


// Função para validar a latitude (está compreendida entre -90 e 90)
// Se for inválida, retorna true, senão false
bool validacaoLatitude(const char *token) {
    // Valida o formato da latitude
    if (!valida_formato(token)) {
        return true; // Formato inválido
    }
    // Se o formato for válido, converte para double e verifica o intervalo
    double latitude = strtod(token, NULL);           // strtod converte string (token) para double

    if (latitude <= -90.0 || latitude >= 90.0) {
        return true; // Latitude inválida
    }

    return false; // Latitude válida
}


// Função para validar a longitude (está compreendida entre -180 e 180)
bool validacaoLongitude(const char *token) {
    // Valida o formato da longitude
    if (!valida_formato(token)) {
        return true; // Formato inválido
    }
    // Se o formato for válido, converte para double e verifica o intervalo
    double longitude = strtod(token, NULL);           // strtod converte string (token) para double

    if (longitude <= -180.0 || longitude >= 180.0) {
        return true; // longitude inválida
    }

    return false; // longitude válida
}






/*
 * ===================================================================
 * ===================  GETTERS  =====================================
 * ===================================================================
 */

// Getter para aceder à tabela de aeroportos
GHashTable* get_tabela_aeroportos(GestorAeroportos* g) {
    if (g == NULL) {
        return NULL;
    }
    return g->tabela_aeroportos;
}

// Getters para o gestor de aeroportos , para aceder a um aeroporto com um determinado code ( codigo IATA)

Aeroportos* get_aeroporto(GestorAeroportos* g, const char* code) {
    if (g == NULL || code == NULL) {
        return NULL;
    }
    return g_hash_table_lookup(g->tabela_aeroportos, code);
}


