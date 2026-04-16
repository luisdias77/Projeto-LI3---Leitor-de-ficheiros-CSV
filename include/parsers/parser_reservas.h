/**
 * @file parser_reservas.h
 * @brief Ficheiro de cabeçalho para as funções de parsing de Reservas.
 *
 * Este ficheiro contém as declarações das funções utilizadas para fazer o parsing
 * das linhas do ficheiro CSV de reservas e carregar os dados para o gestor de reservas.
 *
 */

#ifndef PARSERS_RESERVAS_H
#define PARSERS_RESERVAS_H

#include "gestor_entidades/gestor_reservas.h"
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_passageiros.h"


/**
 * @brief Função que faz o parsing de uma linha do ficheiro CSV de reservas.
 *
 * Esta função lê uma linha do ficheiro CSV, valida os dados e cria uma estrutura Reservas.
 *
 * @param linha String que contém a linha do ficheiro CSV.
 * @return Apontador para a estrutura Reservas criada. Em caso de falha, retorna NULL.
 */
Reservas* parser_reserva(char* linha);

/**
 * @brief Função que carrega as reservas de um ficheiro CSV para o gestor de reservas.
 *
 * Esta função lê o ficheiro CSV linha por linha, faz o parsing de cada linha e adiciona
 * as reservas que são válidas ao gestor de reservas.
 *
 * @param gr Apontador para o gestor de reservas onde os dados serão carregados.
 * @param gv Apontador para o gestor de voos, que é necessário para fazer a vaidação dos voos associados às reservas.
 * @param gp Apontador para o gestor de passageiros, que é necessário para fazer a validação passageiros associados às reservas.
 * @param filename Nome do ficheiro CSV que vai ser lido.
 * @return Número de reservas que foram carregadas corretamente. Caso haja erro, retorna -1.
 */
int carregar_reservas_csv (GestorReservas* gr, GestorVoos* gv, GestorPassageiros* gp, const char* filename);

#endif