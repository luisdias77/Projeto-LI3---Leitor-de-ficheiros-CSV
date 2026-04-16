/**
 * @file query4.h
 * @brief Ficheiro de cabeçalho para as funções da Query 4.
 *
 * Este ficheiro contém as declarações das funções utilizadas para processar a Query 4,
 * que identifica o Qual o passageiro que esteve mais tempo no top 10 de passageiros
 * que mais gastaram em viagens durante um período específico.
 */

#ifndef QUERY4_H
#define QUERY4_H

#include <stdio.h>
#include "gestor_entidades/gestor_reservas.h"
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_passageiros.h"

/**
 * @brief Inicializa as estruturas auxiliares para a Query 4.
 *
 * Prepara os dados necessários para responder rapidamente, utilizando os dados
 * dos gestores de reservas, voos e passageiros.
 *
 * @param gr Apontador para o Gestor de Reservas.
 * @param gv Apontador para o Gestor de Voos.
 * @param gp Apontador para o Gestor de Passageiros.
 */
void init_query4(GestorReservas *gr, GestorVoos *gv, GestorPassageiros *gp);

/**
 * @brief Executa a Query 4.
 *
 * Processa o pedido com base no intervalo de datas fornecido e escreve
 * o resultado formatado no ficheiro de saída especificado.
 *
 * @param data_inicio   String contendo a data de início do intervalo.
 * @param data_fim      String contendo a data de fim do intervalo.
 * @param separador     Caracter separador a utilizar na formatação do output.
 * @param gp            Apontador para o Gestor de Passageiros (necessário para validações ou detalhes).
 * @param output        Ficheiro onde será escrita a resposta.
 */
void query4(char *data_inicio, char *data_fim, char separador, GestorPassageiros *gp, FILE *output);

/**
 * @brief Liberta a memória associada à Query 4.
 *
 * Destrói e limpa todas as estruturas auxiliares e caches que foram
 * alocadas durante a fase de inicialização ou execução da query.
 */
void destroy_query4();

#endif