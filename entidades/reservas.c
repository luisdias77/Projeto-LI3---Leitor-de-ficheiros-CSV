#define _GNU_SOURCE   
#include "entidades/reservas.h"
#include <stdlib.h>          
#include <string.h>

 struct Reservas {
    
    char* reservation_id;
    char* flight_ids;
    char* document_number;
    char* seat;
    double price;
    unsigned char extra_luggage;      
    unsigned char priority_boarding;
    char* qrcode;

};

// Função que cria uma estrutura do tipo reservas(aloca memória para a estrutura em si e para as strings, isto é, os diferentes parametros da struct reservas, exceto os parametros int)
Reservas* criar_reserva(const char* res_id, const char* flt_ids, const char* doc_number, const char* st, double prc, unsigned char extra_lug, unsigned char priority_brd, const char* qr) {
    // Aqui é alocada a memória para a struct reservas
    Reservas* nova_reserva = (Reservas*)malloc(sizeof(Reservas)); 
    if (nova_reserva == NULL) {    
        return NULL;     
    }
    
    // O strdup aloca a memória exata e copia a string inteira.
    nova_reserva->reservation_id = strdup(res_id);
    nova_reserva->flight_ids = strdup(flt_ids);
    nova_reserva->document_number = strdup(doc_number);
    nova_reserva->seat = strdup(st);
    nova_reserva->qrcode = strdup(qr);
    
    // Preenche o campo preço, double
    nova_reserva->price = prc;
    nova_reserva->extra_luggage = extra_lug;         
    nova_reserva->priority_boarding = priority_brd;

    // Retornar o ponteiro para a estrutura reservas criada
    return nova_reserva;
}


// Função que liberta a memória da estrutura Reservas
void libertar_reserva(Reservas* reserva) {

    if (reserva != NULL) {
        // 1. Libertar a memória de cada string primeiro
        free(reserva->reservation_id);
        free(reserva->flight_ids);
        free(reserva->document_number);
        free(reserva->seat);
        free(reserva->qrcode);
                
        // 2. Libertar a memória da própria estrutura
        free(reserva);
    }
}

// Implementação de getters para os campos da struct Reservas

const char* get_reservation_id(Reservas* r) {
    return r->reservation_id;
}

const char* get_flight_ids(Reservas* r) {
    return r->flight_ids;
}

const char* get_document_number(Reservas* r) {
    return r->document_number;
}

const char* get_seat(Reservas* r) {
    return r->seat;
}

double get_price(Reservas* r) {
    return r->price;
}

unsigned char get_extra_luggage(Reservas* r) {
    return r ? r->extra_luggage : 0;
}

unsigned char get_priority_boarding(Reservas* r) {
    return r ? r->priority_boarding : 0;
}

const char* get_qrcode(Reservas* r) {
    return r->qrcode;
}

