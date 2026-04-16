#define _GNU_SOURCE
#include "entidades/voos.h"
#include <stdlib.h>          
#include <string.h>

// Definição da estrutura Voos
struct Voos {
    
    char* flight_id;
    char* departure;        
    char* actual_departure;
    char* arrival;          
    char* actual_arrival;  
    char* gate;
    char* status;
    char* origin;           
    char* destination;      
    char* aircraft;       
    char* airline;
    char* tracking_url;

};

// Função que cria uma estrutura do tipo reservas(aloca memória para a estrutura em si e para as strings, isto é, os diferentes parametros da struct voos)
Voos* criar_voo(const char* id, const char* dep, const char* act_dep, const char* arr, const char* act_arr, const char* gt, const char* sts, const char* org, const char* dest, const char* acft, const char* alrn, const char* track_url) {           
    
    // Aqui é alocada a memória para a struct voos
    Voos* novo_voo = (Voos*)malloc(sizeof(Voos));
    if (novo_voo == NULL) {    
        return NULL;     
    }

    // O strdup aloca a memória exata e copia a string inteira.
    novo_voo->flight_id = strdup(id);
    novo_voo->departure = strdup(dep);
    novo_voo->actual_departure = strdup(act_dep);
    novo_voo->arrival = strdup(arr);
    novo_voo->actual_arrival = strdup(act_arr);
    novo_voo->gate = strdup(gt);
    novo_voo->status = strdup(sts);
    novo_voo->origin = strdup(org);
    novo_voo->destination = strdup(dest);
    novo_voo->aircraft = strdup(acft);
    novo_voo->airline = strdup(alrn);
    novo_voo->tracking_url = strdup(track_url);

    // Retornar o ponteiro para a estrutura voos criada
    return novo_voo;
}

// Função que liberta a memória da estrutura Voos
void libertar_voo(Voos* voo) {
    if (voo != NULL) {
        // 1. Libertar a memória de cada string primeiro
        free(voo->flight_id); 
        free(voo->departure);
        free(voo->actual_departure);
        free(voo->arrival);
        free(voo->actual_arrival);
        free(voo->gate);
        free(voo->status);
        free(voo->origin);
        free(voo->destination);
        free(voo->aircraft);
        free(voo->airline);
        free(voo->tracking_url);
        
        // 2. Libertar a memória da própria estrutura
        free(voo);
    }
}

// Implementação de getters para os campos da struct Voos

const char* get_flight_id(Voos* v) {
    return v->flight_id;
}

const char* get_departure(Voos* v) {
    return v->departure;
}

const char* get_actual_departure(Voos* v) {
    return v->actual_departure;
}

const char* get_arrival(Voos* v) {
    return v->arrival;
}

const char* get_actual_arrival(Voos* v) {
    return v->actual_arrival;
}

const char* get_gate(Voos* v) {
    return v->gate;
}

const char* get_status(Voos* v) {
    return v->status;
}

const char* get_origin(Voos* v) {
    return v->origin;
}

const char* get_destination(Voos* v) {
    return v->destination;
}

const char* get_aircraft(Voos* v) {
    return v->aircraft;
}

const char* get_airline(Voos* v) {
    return v->airline;
}

const char* get_tracking_url(Voos* v) {
    return v->tracking_url;
}

