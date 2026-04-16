/** 
 * @file reservas.h
 * @brief Ficheiro de cabeçalho para as funções relacionadas com Reservas.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para criar, libertar
 * e aceder aos campos da estrutura reserva. A estrutura Reservas
 * representa uma reserva, com os campos de identificação da reserva (res_id),
 * identificadores dos voos associados (flt_ids), número do documento do passageiro (doc_number),
 * numero do lugar (st), preço (prc), bagagem extra (extra_lug), embarque prioritário (priority_brd) e código QR (qr).
 */

#ifndef _RESERVAS_H_
#define _RESERVAS_H_

#include <stdbool.h>

/**
 * @brief Estrutura que representa uma reserva.
 *
 * Esta estrutura contém informações sobre uma reserva, que são ocultadas a quem utiliza este módulo.
 */
typedef struct Reservas Reservas;

/**
 * @brief Função que cria uma nova reserva.
 *
 * Esta função aloca memória para uma nova estrutura Reservas e inicializa os seus campos com os valores fornecidos.
 *
 * @param res_id        Identificação única da reserva.
 * @param flt_ids      Identificadores dos voos associados à reserva.
 * @param doc_number   Número do documento do passageiro associado à reserva.
 * @param st           Número do lugar reservado.
 * @param prc          Preço da reserva.
 * @param extra_lug    Indicação de bagagem extra.
 * @param priority_brd Indicação de embarque prioritário.
 * @param qr           Código QR associado à reserva.
 *
 * @return              Apontador para a nova estrutura Reservas criada. Caso haja falha na alocação de memória, retorna NULL.
 */
Reservas* criar_reserva(const char* res_id, const char* flt_ids, const char* doc_number, const char* st, double prc, unsigned char extra_lug, unsigned char priority_brd, const char* qr);

/**
 * @brief Função que liberta a memória ocupada por uma reserva.
 *
 * Esta função liberta a memória alocada para a estrutura Reservas, bem como para os seus campos de string.
 * 
 * @param reserva  Apontador para a estrutura Reservas que se pretende libertar.
 */
void libertar_reserva(Reservas* reserva);


/**
 * @brief Getter para o campo res_id da estrutura Reservas.
 *
 * Esta função retorna a identificação da reserva (res_id).
 *
 * @param r Apontador para a estrutura Reservas.
 * @return String com a identificação da reserva.
 */
const char* get_reservation_id(Reservas* r);

/**
 * @brief Getter para o campo flt_ids da estrutura Reservas.
 *
 * Esta função retorna os identificadores dos voos associados à reserva (flt_ids).
 *
 * @param r Apontador para a estrutura Reservas.
 * @return String com os identificadores dos voos.
 */
const char* get_flight_ids(Reservas* r);

/**
 * @brief Getter para o campo doc_number da estrutura Reservas.
 *
 * Esta função retorna o número do documento do passageiro associado à reserva (doc_number).
 *
 * @param r Apontador para a estrutura Reservas.
 * @return String com o número do documento do passageiro.
 */
const char* get_document_number(Reservas* r);

/**
 * @brief Getter para o campo seat da estrutura Reservas.
 *
 * Esta função retorna o número do lugar reservado (seat).
 *
 * @param r Apontador para a estrutura Reservas.
 * @return String com o número do lugar reservado.
 */
const char* get_seat(Reservas* r);

/**
 * @brief Getter para o campo price da estrutura Reservas.
 *
 * Esta função retorna o preço da reserva (price).
 *
 * @param r Apontador para a estrutura Reservas.
 * @return Valor do preço da reserva.
 */
double get_price(Reservas* r);


#endif
