/** 
 * @file voos.h
 * @brief Ficheiro de cabeçalho para as funções relacionadas com Voos.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para criar, libertar
 * e aceder aos campos da estrutura voo. A estrutura Voos
 * representa um voo, com os campos de identificação do voo (flight_id),
 * hora de partida (departure), hora de partida real (actual_departure), hora de chegada (arrival),
 * hora de chegada real (actual_arrival), portão (gate), estado (status), origem (origin),
 * destino (destination), aeronave (aircraft), companhia aérea (airline), e URL de rastreamento do voo (tracking_url).
 */

#ifndef _VOOS_H_
#define _VOOS_H_

#include <stdbool.h>

/**
 * @brief Estrutura que representa um voo.
 *
 * Esta estrutura contém informações sobre um voo, que são ocultadas a quem utiliza este módulo.
 */
typedef struct Voos Voos;

/**
 * @brief Função que cria um novo voo.
 *
 * Esta função aloca memória para uma nova estrutura Voos e inicializa os seus campos com os valores fornecidos.
 *
 * @param id            Identificação única do voo.
 * @param dep           Hora de partida do voo.
 * @param act_dep       Hora de partida real do voo.
 * @param arr           Hora de chegada do voo.
 * @param act_arr       Hora de chegada real do voo.
 * @param gt            Portão do voo.
 * @param sts           Estado do voo.
 * @param org           Origem do voo.
 * @param dest          Destino do voo.
 * @param acft          Aeronave associada ao voo.
 * @param alrn          Companhia aérea do voo.
 * @param track_url     URL de rastreamento do voo.
 *
 * @return              Apontador para a nova estrutura Voos criada. Caso haja falha na alocação de memória, retorna NULL.
 */
Voos* criar_voo(const char* id, const char* dep, const char* act_dep, const char* arr, const char* act_arr, const char* gt, const char* sts, const char* org, const char* dest, const char* acft, const char* alrn, const char* track_url);

/**
 * @brief Função que liberta a memória ocupada por um voo.
 *
 * Esta função liberta a memória alocada para a estrutura Voos, bem como para os seus campos de string.
 * 
 * @param voo  Apontador para a estrutura Voos que se pretende libertar.
 */
void libertar_voo(Voos* voo);

/**
 * @brief Getter para o campo flight_id da estrutura Voos.
 *
 * Esta função retorna a identificação do voo (flight_id).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a identificação do voo.
 */
const char* get_flight_id(Voos* v);

/**
 * @brief Getter para o campo departure da estrutura Voos.
 *
 * Esta função retorna a hora de partida do voo (departure).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a hora de partida do voo.
 */
const char* get_departure(Voos* v);

/**
 * @brief Getter para o campo actual_departure da estrutura Voos.
 *
 * Esta função retorna a hora de partida real do voo (actual_departure).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a hora de partida real do voo.
 */
const char* get_actual_departure(Voos* v);

/**
 * @brief Getter para o campo arrival da estrutura Voos.
 *
 * Esta função retorna a hora de chegada do voo (arrival).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a hora de chegada do voo.
 */
const char* get_arrival(Voos* v);

/**
 * @brief Getter para o campo actual_arrival da estrutura Voos.
 *
 * Esta função retorna a hora de chegada real do voo (actual_arrival).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a hora de chegada real do voo.
 */
const char* get_actual_arrival(Voos* v);

/**
 * @brief Getter para o campo gate da estrutura Voos.
 *
 * Esta função retorna a porta de embarque do voo (gate).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a porta de embarque do voo.
 */
const char* get_gate(Voos* v);

/**
 * @brief Getter para o campo status da estrutura Voos.
 *
 * Esta função retorna o estado do voo (status).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com o estado do voo.
 */
const char* get_status(Voos* v);

/**
 * @brief Getter para o campo origin da estrutura Voos.
 *
 * Esta função retorna a origem do voo (origin).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a origem do voo.
 */
const char* get_origin(Voos* v);

/**
 * @brief Getter para o campo destination da estrutura Voos.
 *
 * Esta função retorna o destino do voo (destination).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com o destino do voo.
 */
const char* get_destination(Voos* v);

/**
 * @brief Getter para o campo aircraft da estrutura Voos.
 *
 * Esta função retorna a aeronave do voo (aircraft).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a aeronave do voo.
 */
const char* get_aircraft(Voos* v);

/**
 * @brief Getter para o campo airline da estrutura Voos.
 *
 * Esta função retorna a companhia aérea do voo (airline).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a companhia aérea do voo.
 */
const char* get_airline(Voos* v);

/**
 * @brief Getter para o campo tracking_url da estrutura Voos.
 *
 * Esta função retorna a URL de rastreamento do voo (tracking_url).
 *
 * @param v Apontador para a estrutura Voos.
 * @return String com a URL de rastreamento do voo.
 */
const char* get_tracking_url(Voos* v);


#endif
