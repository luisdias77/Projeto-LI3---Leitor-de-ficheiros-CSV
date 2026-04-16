#define _GNU_SOURCE
#include "entidades/aeronaves.h"
#include <stdlib.h>          
#include <string.h>

// Definição da estrutura Aeronaves
struct Aeronaves {
    
    char* identifier;
    char* manufacturer;
    char* model;
    int year;
    int capacity;
    int range;

};



// Função que cria uma estrutura do tipo aeronave(aloca memória para a estrutura em si e para as strings, isto é, os diferentes parametros da struct aeronave, exceto os parametros int)
Aeronaves* criar_aeronave(const char* id, const char* manuf, const char* model, int year, int cap, int range) {
    // Aqui é alocada a memória para a struct aeronave
    Aeronaves* nova_aeronave = (Aeronaves*)malloc(sizeof(Aeronaves));
    if (nova_aeronave == NULL) { 
        return NULL; // caso malloc falhe, retornará NULL
    }
    
    // O strdup aloca a memória exata e copia a string inteira.
    nova_aeronave->identifier = strdup(id);
    nova_aeronave->manufacturer = strdup(manuf);
    nova_aeronave->model = strdup(model);   

    // Preencher os campos dos parametros inteiros
    nova_aeronave->year = year;
    nova_aeronave->capacity = cap;
    nova_aeronave->range = range;

    // Retornar o ponteiro para a estrutura aeronave criada
    return nova_aeronave;
}


// Função que liberta a memória da estrutura Aeronaves
void libertar_aeronave(Aeronaves* aeronave) {
    if (aeronave != NULL) {
        // 1. Libertar a memória de cada string primeiro
        free(aeronave->identifier);
        free(aeronave->manufacturer);
        free(aeronave->model);
        
        // 2. Libertar a memória da própria estrutura
        free(aeronave);
    }
}

// Getters para os campos da struct Aeronaves
const char* get_identifier_aeronave(Aeronaves* a) {
    return a->identifier;
}

const char* get_manufacturer_aeronave(Aeronaves* a) {
    return a->manufacturer;
}

const char* get_model_aeronave(Aeronaves* a) {
    return a->model;
}

int get_year_aeronave(Aeronaves* a) {
    return a->year;
}

int get_capacity_aeronave(Aeronaves* a) {
    return a->capacity;
}

int get_range_aeronave(Aeronaves* a) {
    return a->range;
}

