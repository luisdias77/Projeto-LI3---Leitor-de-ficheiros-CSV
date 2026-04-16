/**
 * @file aeroportos.h
 * @brief Ficheiro de cabeçalho para as funções relacionadas com Aeroportos.
 *
 * Este ficheiro contém as declarações das funções utilizadas para criar, libertar
 * e aceder aos campos da estrutura aeroporto. A estrutura Aeroportos
 * representa um aeroporto, com os campos de código (code), nome (name), cidade (city),
 * país (country), latitude (latitude), longitude (longitude), código ICAO (icao) e tipo (type).
*/

#ifndef _AEOROPORTOS_H_
#define _AEOROPORTOS_H_

#include <stdbool.h>

/**
 * @brief Estrutura que representa um aeroporto.
 *
 * Esta estrutura contém informações sobre um aeroporto, que são ocultadas a quem utiliza este módulo.
 */


typedef struct Aeroportos Aeroportos;

/**
 * @brief Função que cria um novo aeroporto.
 *
 * Esta função aloca memória para uma nova estrutura Aeroportos e inicializa os seus campos com os valores fornecidos.
 *
 * @param code      Código do aeroporto.
 * @param name      Nome do aeroporto.
 * @param city      Cidade onde o aeroporto está localizado.
 * @param country   País onde o aeroporto está localizado.
 * @param latitude  Latitude do aeroporto.
 * @param longitude Longitude do aeroporto.
 * @param icao      Código ICAO do aeroporto.
 * @param type      Tipo do aeroporto.
 * @param total_passageiros_chegada Número total de passageiros que chegaram ao aeroporto.
 * @param total_passageiros_partida Número total de passageiros que partiram do aeroporto.
 * 
 * @return          Apontador para a nova estrutura Aeroportos criada. Caso haja falha na alocação de memória, retorna NULL.
 */

Aeroportos* criar_aeroporto(const char* code, const char* name, const char* city, const char* country, double latitude, double longitude, const char* icao, const char* type);

/**
 * @brief Função que liberta a memória ocupada por um aeroporto.
 *
 * Esta função liberta a memória alocada para a estrutura Aeroportos, bem como para os seus campos de string.
 * 
 * @param aeroporto  Apontador para a estrutura Aeroportos que se pretende libertar.
 */

void libertar_aeroporto(Aeroportos* aeroporto);


/**
 * @brief Getter para o campo code da estrutura Aeroportos.
 *
 * Esta função retorna o código (code) do aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return String com o código do aeroporto.
 */
const char* get_code_aeroporto(Aeroportos* aeroporto);


/**
 * @brief Getter para o campo name da estrutura Aeroportos.
 *
 * Esta função retorna o nome (name) do aeroporto.
 * 
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return String com o nome do aeroporto.
 */
const char* get_name_aeroporto(Aeroportos* aeroporto);


/**
 * @brief Getter para o campo city da estrutura Aeroportos.
 *
 * Esta função retorna a cidade (city) do aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return String com a cidade do aeroporto.
 */
const char* get_city_aeroporto(Aeroportos* aeroporto);

/**
 * @brief Getter para o campo country da estrutura Aeroportos.
 *
 * Esta função retorna o país (country) do aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return String com o país do aeroporto.
 */
const char* get_country_aeroporto(Aeroportos* aeroporto);

/**
 * @brief Getter para o campo latitude da estrutura Aeroportos.
 *
 * Esta função retorna a latitude (latitude) do aeroporto.
 * 
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return Double com a latitude do aeroporto.
 */
double get_latitude_aeroporto(Aeroportos* aeroporto);

/**
 * @brief Getter para o campo longitude da estrutura Aeroportos.
 *
 * Esta função retorna a longitude (longitude) do aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return Double com a longitude do aeroporto.
 */
double get_longitude_aeroporto(Aeroportos* aeroporto);

/**
 * @brief Getter para o campo icao da estrutura Aeroportos.
 *
 * Esta função retorna o código ICAO (icao) do aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return String com o código ICAO do aeroporto.
 */
const char* get_icao_aeroporto(Aeroportos* aeroporto);

/**
 * @brief Getter para o campo type da estrutura Aeroportos.
 *
 * Esta função retorna o tipo (type) do aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return String com o tipo do aeroporto.
 */
const char* get_type_aeroporto(Aeroportos* aeroporto);

/**
 * @brief Getter para o total de passageiros nas chegadas.
 *
 * Esta função retorna o número total de passageiros que chegaram a este aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return Inteiro com o total de chegadas acumulado.
 */
int get_total_chegadas(Aeroportos* aeroporto);

/**
 * @brief Getter para o total de passageiros nas partidas.
 *
 * Esta função retorna o número total de passageiros que partiram deste aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 * @return Inteiro com o total de partidas acumulado.
 */
int get_total_partidas(Aeroportos* aeroporto);

/**
 * @brief Incrementa o contador de chegadas do aeroporto.
 *
 * Esta função aumenta em uma unidade o valor do campo que contabiliza
 * o total de passageiros que chegaram ao aeroporto.
 *
 * @param aeroporto Apontador para a estrutura Aeroportos.
 */
void incrementa_chegadas(Aeroportos* aeroporto);

/**
 * @brief Incrementa o contador de partidas do aeroporto.
 *
 * Esta função aumenta em uma unidade o valor do campo que contabiliza
 * o total de passageiros que partiram do aeroporto.
 *
 * @param a Apontador para a estrutura Aeroportos.
 */
void incrementa_partidas(Aeroportos* aeroporto);

#endif
