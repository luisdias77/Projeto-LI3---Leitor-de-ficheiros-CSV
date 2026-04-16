#include <stdio.h>
#include "gestor_entidades/gestor_aeroportos.h"
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_reservas.h"
#include "entidades/aeroportos.h"
#include "entidades/reservas.h"
#include "queries/query1.h"
#include <string.h>
#include <glib.h>
#include <strings.h>

// Esta função calcula as estatisticas que precisamos e guarda os resultados dentro da struct Aeroporto
void calcula_estatisticas_aeroportos (GestorAeroportos* ga, GestorReservas* gr, GestorVoos* gv) {
    
    GHashTable* tabela_reservas = get_tabela_reservas(gr);
    GHashTableIter iter;
    gpointer chave, valor;

    g_hash_table_iter_init(&iter, tabela_reservas);
    
    while (g_hash_table_iter_next(&iter, &chave, &valor)) {
        Reservas* reserva_atual = (Reservas*) valor;
        const char* lista_ids = get_flight_ids(reserva_atual);
        
        char id_voo[20];
        int idx = 0;

        while (extrair_id_voo(lista_ids, idx, id_voo)) {
            Voos* voo = get_voo(gv, id_voo);

            if (voo != NULL && strcasecmp(get_status(voo), "Cancelled") != 0) {
                
                // Atualiza o Aeroporto de Origem
                Aeroportos* origem = get_aeroporto(ga, get_origin(voo));
                if (origem) incrementa_partidas(origem); 

                // Atualiza o Aeroporto de Destino
                Aeroportos* destino = get_aeroporto(ga, get_destination(voo));
                if (destino) incrementa_chegadas(destino); 
            }
            idx++;
        }
    }
}

void query1(const char* code, GestorAeroportos* ga, char separador, FILE *output) {
    Aeroportos *a = get_aeroporto(ga, code);
    
    if (a != NULL) {
        fprintf(output, "%s%c%s%c%s%c%s%c%s%c%d%c%d\n", 
            get_code_aeroporto(a), separador,
            get_name_aeroporto(a), separador,
            get_city_aeroporto(a), separador,
            get_country_aeroporto(a), separador,
            get_type_aeroporto(a), separador,
            get_total_chegadas(a), separador,
            get_total_partidas(a)
        );
    } else {
        fprintf(output, "\n");
    }
}