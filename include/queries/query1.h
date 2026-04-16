/**
 * @file query1.h
 * @brief Ficheiro de cabeçalho para as funções da Query 1.
 *
 * Este ficheiro contém as declarações das funções utilizadas para processar a Query 1,
 * que lista o resumo de um aeroporto e a contagem de passageiros.
 */

#ifndef QUERIES1_H
#define QUERIES1_H

#include "gestor_entidades/gestor_aeroportos.h"
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_reservas.h"


/**
 * @brief Pré-calcula as estatísticas (chegadas e partidas) para todos os aeroportos.
 *
 * Esta função é chamada uma única vez no início da execução (após o carregamento dos csvs).
 * Ela itera sobre todas as reservas e os respetivos voos para contabilizar o número total de
 * passageiros que partiram e chegaram a cada aeroporto.
 * Os resultados são armazenados diretamente na estrutura de cada Aeroporto
 * 
 * @param ga Apontador para o Gestor de Aeroportos (onde os contadores serão atualizados).
 * @param gr Apontador para o Gestor de Reservas (usado para iterar sobre os passageiros).
 * @param gv Apontador para o Gestor de Voos (usado para validar voos e obter origem/destino).
 */
void calcula_estatisticas_aeroportos(GestorAeroportos* ga, GestorReservas* gr, GestorVoos* gv);

/**
 * @brief Lista o resumo de um aeroporto, consoante o idenitfiador recebido por argumento.
 *
 * Esta função recebe o identificador de um aeroporto e escreve no ficheiro de output:
 * código IATA, nome, cidade, país, tipo, número total de passageiros que aterraram e número total
 * de passageiros que partiram desse aeroporto. 
 * A função acede diretamente aos valores (chegadas e partidas) que já se encontram armazenados na estrutura do aeroporto (valores pré-calculados).
 * - Se o aeroporto não for encontrado, escreve apenas uma linha vazia.
 * 
 * @param code      String com o código IATA do aeroporto a pesquisar.
 * @param ga        Apontador para o Gestor de Aeroportos (para obter os dados do aeroporto).
 * @param separador Caractere a utilizar como separador de campos (',' ou '=').
 * @param output    Apontador para o ficheiro onde o resultado da query será escrito.
 *
 * @return A função não retorna nenhum valor (void). O resultado é escrito no ficheiro apontado por `output`.
*/
void query1 (const char* code, GestorAeroportos* ga, char separador, FILE *output);

#endif
