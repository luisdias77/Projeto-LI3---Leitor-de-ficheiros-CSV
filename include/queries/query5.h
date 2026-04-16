/**
 * @file query5.h
 * @brief Ficheiro de cabeçalho para as funções da Query 5.
 *
 * Este ficheiro contém as declarações das funções utilizadas para processar a Query 5,
 * que envolve o cálculo de atrasos médios de voos por companhia aérea.
 */

#ifndef QUERY5_H
#define QUERY5_H

#include "gestor_entidades/gestor_voos.h"

/**
 * @brief Estrutura para armazenar informações sobre companhias aéreas.
 * 
 * Esta estrutura contém informações relativas a uma companhia aérea, 
 * essenciais à execução da query 5 e que são ocultadas a quem utiliza este módulo.
 */

typedef struct InformaçãoAirlines InformaçãoAirlines;

/**
* @brief Converte uma string de data e hora para minutos.
*
* Esta função recebe uma string de data e hora em um formato específico e a converte
* no total de minutos correspondente.
*
* @param data_hora String que representa a data e hora.
* @return          Total de minutos como um inteiro.
*/
int converte_para_minutos(const char* data_hora);

/**
 * @brief Encontra uma companhia aérea na lista de companhias aéreas.
 *
 * Esta função procura uma companhia aérea específica na lista fornecida de companhias aéreas
 * e retorna o seu índice, se encontrada.
 *
 * @param listAirline   Apontador para a lista de informações das companhias aéreas.
 * @param airlineusadas Número de companhias aéreas atualmente em uso.
 * @param airline       String que representa a companhia aérea a ser procurada.
 * @return              Índice da companhia aérea, se encontrada, ou -1 se não encontrada.
 */
int encontra_airline(InformaçãoAirlines* listAirline, int airlineusadas, const char* airline);

/**
 * @brief Compara dois elementos para ordenação na Query 5.
 *
 * Esta função é utilizada como função de comparação para operações de ordenação
 * na Query 5. Ela compara dois elementos e determina sua ordem.
 *
 * @param a Aponatdor para o primeiro elemento.
 * @param b Apontador para o segundo elemento.
 * @return  Inteiro menor, igual ou maior que zero se o primeiro elemento for considerado
 *          respectivamente menor, igual ou maior que o segundo.
 */
int comparar_q5(const void* a, const void* b);

/**
 * @brief Lista as Top N companhias aéreas com maior tempo médio de atraso por voo.
 *
 * Esta função percorre todos os voos, filtra aqueles que tem estado "Delayed" e calcula o atraso acumulado
 * para cada companhia aérea. O atraso de um voo é calculado como a diferença em minutos entre a 'actual_arrival' 
 * e a 'actual_departure'. Os resultados são ordenados por ordem decrescente de média de atraso. Em caso de
 * empate, as companhias são ordenadas alfabeticamente.
 *
 * @param top         Número de resultados principais a serem obtidos.
 * @param gestor_voos Apontador para o gestor de voos.
 * @param separador   Caractere a utilizar como separador de campos (',' ou '=').
 * @param output      Apontador para o ficheiro onde o resultado da query será escrito.
 */
void query5(int top, GestorVoos* gestor_voos, char separador, FILE* output);


#endif

