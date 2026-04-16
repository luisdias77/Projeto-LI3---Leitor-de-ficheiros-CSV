#define _GNU_SOURCE
#include "entidades/aeroportos.h"
#include <stdlib.h>          
#include <string.h>
#include <stdio.h>

// Definição da estrutura Aeroportos
struct Aeroportos {
    
    char* code;
    char* name;
    char* city;
    char* country;
    double latitude;
    double longitude;
    char* icao;
    char* type;
    int total_passageiros_chegada; // 
    int total_passageiros_partida;

};

// Função que cria uma estrutura do tipo aeroporto(aloca memória para a estrutura em si e para as strings, isto é, os diferentes parametros da struct aeroporto, exceto os parametros double)
Aeroportos* criar_aeroporto(const char* code, const char* name, const char* city, const char* country, double latitude, double longitude, const char* icao, const char* type) {
    // Aqui é alocada a memória para a struct aeroporto
    Aeroportos* novo_aeroporto = (Aeroportos*)malloc(sizeof(Aeroportos));
    if (novo_aeroporto == NULL) {
        return NULL; // caso malloc falhe, retornará NULL
    }
    
    // O strdup aloca a memória exata e copia a string inteira.
    
    novo_aeroporto->code = strdup(code);
    novo_aeroporto->name = strdup(name);
    novo_aeroporto->city = strdup(city);
    novo_aeroporto->country = strdup(country);
    novo_aeroporto->icao = strdup(icao);
    novo_aeroporto->type = strdup(type);

    // Verificar se algum strdup falhou (falta de memória)
    if (!novo_aeroporto->code || !novo_aeroporto->name || !novo_aeroporto->city || 
        !novo_aeroporto->country || !novo_aeroporto->icao || !novo_aeroporto->type) {
        libertar_aeroporto(novo_aeroporto);
        return NULL;
    }

    // Preencher os campos double e int
    novo_aeroporto->latitude = latitude;
    novo_aeroporto->longitude = longitude;
    novo_aeroporto->total_passageiros_chegada = 0;
    novo_aeroporto->total_passageiros_partida = 0;

    return novo_aeroporto;
}


// Função que liberta a memória da estrutura Aeroportos
void libertar_aeroporto(Aeroportos* aeroporto) {
    if (aeroporto != NULL) {
        // 1. Libertar a memória de cada string primeiro
        free(aeroporto->code);
        free(aeroporto->name);
        free(aeroporto->city);
        free(aeroporto->country);
        free(aeroporto->icao);
        free(aeroporto->type);
        
        // 2. Libertar a memória da própria estrutura
        free(aeroporto);
    }
}


// Implementação de getters para os campos da struct Aeroportos

const char* get_code_aeroporto(Aeroportos* aeroporto) {
    return aeroporto->code;
}

const char* get_name_aeroporto(Aeroportos* aeroporto) {
    return aeroporto->name;
}

const char* get_city_aeroporto(Aeroportos* aeroporto) {
    return aeroporto->city;
}

const char* get_country_aeroporto(Aeroportos* aeroporto) {
    return aeroporto->country;
}


double get_latitude_aeroporto(Aeroportos* aeroporto) {
    return aeroporto->latitude;
}

double get_longitude_aeroporto(Aeroportos* aeroporto) {
    return aeroporto->longitude;
}

const char* get_icao_aeroporto(Aeroportos* aeroporto) {
    return aeroporto->icao;
}

const char* get_type_aeroporto(Aeroportos* aeroporto) {
    return aeroporto->type;
}


int get_total_chegadas(Aeroportos* aeroporto) {
    if (aeroporto == NULL) {
        return 0;
    }
    return aeroporto->total_passageiros_chegada;
}

int get_total_partidas(Aeroportos* aeroporto) {
    if (aeroporto == NULL) {
        return 0;
    }
    return aeroporto->total_passageiros_partida;
}

void incrementa_chegadas(Aeroportos* aeroporto) {
    if (aeroporto != NULL) {
        aeroporto->total_passageiros_chegada++;
    }
}

void incrementa_partidas(Aeroportos* aeroporto) {
    if (aeroporto != NULL) {
        aeroporto->total_passageiros_partida++;
    }
}
