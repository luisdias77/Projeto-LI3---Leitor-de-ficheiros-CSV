#include <stdio.h>
#include "gestor_entidades/gestor_voos.h"
#include <string.h>


#include "queries/query5.h"

// Definição da estrutura InformaçãoAirlines
struct InformaçãoAirlines {
    char airline[120];
    double tempo_total_atraso;
    int numero_voos_atrasados;
}; 

// Função que converte "aaaa−mm−dd hh : mm" em minutos
int converte_para_minutos (const char* data_hora) {
    
    int ano, mes, dia, hora, minuto;
    
    sscanf(data_hora, "%d-%d-%d %d:%d", &ano, &mes, &dia, &hora, &minuto);
    
    return (((ano * 12 + mes) * 31 + dia) * 24 + hora) * 60 + minuto;

}

// Função que permite verificar se uma airline já existe no array de InformaçãoAirlines

int encontra_airline(InformaçãoAirlines* listAirline, int airlineusadas, const char* airline) {
    
    for (int i = 0; i < airlineusadas ; i++) {
        
        if (strcmp(listAirline[i].airline, airline) == 0) {
            
            return i; // Se encontrar a companhia aérea, retorna o índice onde ela se encontra
        }
    }
    return -1; // Caso nao encontre a companhia aérea no array, retornará -1
}

// Função que compara duas estruturas InformaçãoAirlines para depois ser usada na qsort, para ordenar o array de InformaçãoAirlines
int comparar_q5 (const void* a, const void* b) {
    
        InformaçãoAirlines* i1 = (InformaçãoAirlines*) a;
        InformaçãoAirlines* i2 = (InformaçãoAirlines*) b;

        // Primeiro, vamos comparar de acordo com o tempo médio de atraso em ordem decrescente
        if (i2 ->tempo_total_atraso > i1->tempo_total_atraso) {
            return 1;
        } else if (i2->tempo_total_atraso < i1->tempo_total_atraso) {
            return -1;
        } else {
            // Em caso de empate, comparar pelo nome da companhia aérea em ordem alfabética
            return strcmp(i1->airline, i2->airline);
        }
}


// Função principal da query 5, que indica o Top N companhias aéreas com mais tempo de atraso médio por voo 
void query5 (int N, GestorVoos* gv, char separador, FILE* output) {
    
    InformaçãoAirlines* listAirline = malloc (sizeof(InformaçãoAirlines) * 100000); 
    int airlineusadas = 0; // Contador para o número de companhias aéreas que estão a ser usadas no array 
    // Isto permite aceder à tabela hash de voos, usando o getter definido no gestor de voos
    GHashTable *voo = get_tabela_voos(gv);
    
    // Iterador para percorrer a tabela hash dos voos
    GHashTableIter it_voo;
    gpointer chaveVoo, valorVoo;

    g_hash_table_iter_init (&it_voo, voo);

    while (g_hash_table_iter_next (&it_voo, &chaveVoo, &valorVoo)) {
        Voos* v = (Voos*) valorVoo; 
        

        // Apenas sao considerados voos com estado "Delayed"
        if (strcmp (get_status(v), "Delayed") != 0) {
            continue; 
            }
        

        const char* ad = get_actual_departure(v); // obter o actual_departure
        const char* aa = get_departure(v); // obter o departure
        
        // Converter os campos actual_departure e departure para minutos
        int minutos_actual_departure = converte_para_minutos(ad);
        int minutos_departure = converte_para_minutos(aa);

        // Calcular o tempo de atraso em minutos
        double tempo_atraso = (double)(minutos_actual_departure - minutos_departure);

        // Procurar se a airline em questão já existe no array de InformaçãoAirlines
        int posicao_airline = encontra_airline(listAirline, airlineusadas, get_airline(v));
        
        // Se a airline não existir, vamos adicioná-la ao array 
        if (posicao_airline == -1) { 

            strcpy(listAirline[airlineusadas].airline, get_airline(v));
            
            listAirline[airlineusadas].tempo_total_atraso = tempo_atraso; // o tempo de total de atraso é igual ao tempo de atraso do voo atual
            
            listAirline[airlineusadas].numero_voos_atrasados = 1; // o numero de voos atrasados para esta airline passa a ser 1
            
            airlineusadas++; // o número de airlines usadas é incrementado em 1
        
        } else {
            
        // Se a airline já existir, então apenas são atualizados os valores de tempo_total_atraso e numero_voos_atrasados
            listAirline[posicao_airline].tempo_total_atraso += tempo_atraso; // acumula o tempo de atraso
            
            listAirline[posicao_airline].numero_voos_atrasados += 1; // incrementa o numero de voos atrasados
        }
    }

    // Calcular o tempo médio de atraso por voo para cada companhia aérea
    for (int i = 0; i < airlineusadas; i++) {
        
        // Só calcula caso o numero de voos atrasados seja maior que 0
        if (listAirline[i].numero_voos_atrasados > 0) {
            listAirline[i].tempo_total_atraso /= listAirline[i].numero_voos_atrasados; // calcula o tempo médio de atraso, quociente do tempo total de atraso pelo numero de voos atrasados
        }
    }

    // Ordenar o array InformaçãoAirlines pelo tempo médio de atraso em ordem decrescente, e em caso de empate, por ordem alfabética do nome da companhia aérea

    qsort(listAirline, airlineusadas, sizeof(InformaçãoAirlines), comparar_q5);
    

    // Imprimir as top N companhias aéreas com maior tempo médio de atraso no ficheiro de output

    for (int i = 0; i < N && i < airlineusadas; i++) {

        fprintf (output, "%s%c%d%c%.3f\n", 
            listAirline[i].airline, separador, 
            listAirline[i].numero_voos_atrasados, separador,
            listAirline[i].tempo_total_atraso
        );
    }

    free (listAirline); // Libertar a memória alocada para o array de InformaçãoAirlines
}




