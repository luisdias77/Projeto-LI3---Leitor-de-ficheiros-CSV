/**
 * @file gestor_reservas.h
 * @brief Ficheiro de cabeçalho para as funções associadas ao gestor de reservas.
 * 
 * Este ficheiro contém as declarações das funções utilizadas para criar, destruir,
 * adicionar, remover e consultar reservas no gestor de reservas, bem como os getters usados para aceder a reservas armazenadas,
 * ou a campos específicos das mesmas.
 */

#ifndef GESTOR_RESERVAS_H
#define GESTOR_RESERVAS_H

#include <glib.h>
#include "entidades/reservas.h"
#include "gestor_entidades/gestor_voos.h"
#include "gestor_entidades/gestor_passageiros.h"
#include <stdbool.h>

/**
 * @brief Estrutura que representa o gestor de reservas.
 * 
 * Esta estrutura contém uma tabela hash que armazena as reservas,
 * onde a chave é o id da reserva (char*) e o valor é um apontador para a estrutura Reservas*.
 */

typedef struct gestor_reservas GestorReservas;

/**
 * @brief Função para criar o gestor de reservas.
 * 
 * Esta função aloca memória para uma nova estrutura GestorReservas
 * inicializando a tabela hash que irá armazenar as reservas.
 * 
 * @return Apontador para a nova estrutura GestorReservas criada.
 */

GestorReservas* criar_gestor_reservas();

/**
 * @brief Função para destruir o gestor de reservas.
 * 
 * Esta função liberta a memória ocupada pela estrutura GestorReservas,
 * incluindo a tabela hash e todas as reservas que estão armazenados nela.
 * 
 * @param g Apontador para a estrutura GestorReservas que vai destruída.
 */

void destruir_gestor_reservas(GestorReservas* g);

/**
 * @brief Função para adicionar uma reserva ao gestor de reservas.
 * 
 * Esta função insere uma nova reserva na tabela hash do gestor,
 * usando o id da reserva como chave.
 * 
 * @param g Apontador para a estrutura GestorReservas onde será adiconada o respetivo reserva.
 * @param a Apontador para a estrutura Reservas que será adicionada ao gestor.
 */

void adiciona_reservas(GestorReservas* g, Reservas* a);

/**
 * @brief Função para verificar se uma reserva existe no gestor de reservas.
 * 
 * Esta função verifica se uma reserva com um id_reserva fornecido
 * existe na tabela hash do gestor.
 * 
 * @param g Apontador para a estrutura GestorReservas onde será irá ser verificado se a reserva existe.
 * @param id_reserva String que representa o id_reserva da reserva a ser verificado.
 * @return true se a reserva existir, ou false, no caso de a reserva não existir.
 */
 
bool reserva_existe(GestorReservas* g, char* id_reserva);

/**
 * @brief Função que remove uma reserva do gestor de reservas.
 *
 * Esta função remove uma reserva da tabela hash do gestor,
 * usando o id_reserva da reserva como chave.
 *
 * @param g Apontador para a estrutura GestorReservas onde será removida a respetiva reserva.
 * @param id_reserva String que representa o id_reserva da reserva a ser removida.
 */

void remove_reserva(GestorReservas* g, char* id_reserva);

/** 
 * @brief Função que conta o numero de reservas na hashtable
 *
 * Esta função percorre a tabela hash do gestor de reservas
 * e conta o número total de reservas armazenadas.
 *
 * @param g Apontador para a estrutura GestorReservas onde serão contadas as reservas.
 *
 * @return Inteiro com o número total de reservas.
 */

int conta_reservas(GestorReservas* g);

/**
 * @brief Função que valida logicamente o id de uma reserva
 *
 * Esta função verifica se o id da reserva fornecido é válido
 * de acordo com os critérios que são definidos no enunciado.
 *
 * @param id String que representa o id da reserva a ser validado.
 *
 * @return Inteiro que indica se o id é válido (1) ou inválido (0).
 */

int validar_reservation_id(const char* id);

/**
 * @brief Função que verifica se um passageiro existe, através do document_number da reserva
 *
 * Esta função verifica se um passageiro com um document_number da reserva fornecido
 * existe na tabela hash do gestor de passageiros.
 *
 * @param p Apontador para a estrutura GestorPassageiros onde será irá ser verificado se o passageiro existe.
 * @param document_number String que representa o document_number do passageiro a ser verificado, prsente na estrutura reserva.
 *
 * @return true se o passageiro existir, ou false, no caso de o passageiro não existir. 
 */

bool passageiro_existe (GestorPassageiros* p, const char* document_number);

/**
 * @brief Função que extrai um id de um voo de uma lista de ids presente na reserva
 *
 * Esta função percorre uma string que contém uma lista de ids de voos
 * e extrai o id do voo correspondente ao índice fornecido.
 *
 * @param flight_ids String que representa a lista de ids de voos
 * @param flight_index Índice do voo que queremos extrair da lista
 * @param id_destino Apontador para a string onde o id extraído será armazenado
 *
 * @return Inteiro que indica se a extração ocorreu corretamente (1) ou falhou (0).
 */

int extrair_id_voo(const char* flight_ids, int flight_index, char* id_destino);

/**
 * @brief Função que valida logicamente uma reserva
 *
 * Esta função verifica se uma reserva é logicamente válida
 * de acordo com os critérios definidos no enunciado,
 * utilizando o gestor de voos e o gestor de passageiros para validações que são necessárias
 * e envolvem essas outras entidades.
 *
 * @param r Apontador para a estrutura Reservas que será validada.
 * @param g Apontador para a estrutura GestorVoos que será usada na validação.
 * @param p Apontador para a estrutura GestorPassageiros que será usada na validação.
 *
 * @return Inteiro que indica se a reserva é válida (1) ou inválida (0).
 */

int valida_reserva_logica(Reservas* r, GestorVoos* g, GestorPassageiros* p);

/** 
 * @brief Getters para o gestor de reservas , para aceder a uma reserva com um determinado id_reserva (identificador da reserva)
 *
 * Esta função retorna um apontador para a estrutura Reservas
 * que corresponde ao id_reserva fornecido.
 *
 * @param g Apontador para a estrutura GestorReservas onde será irá ser procurada a reserva.
 * @param id_reserva String que representa o id_reserva da reserva a ser procurada.
 *
 * @return Apontador para a estrutura Reservas correspondente ao id_reserva fornecido. Em caso de não existir, retorna NULL.
 */

Reservas* get_reserva(GestorReservas* g, const char* id_reserva);

/**
 * @brief Getter para aceder à tabela de reservas
 * 
 * Esta função retorna a tabela hash que armazena as reservas
 * no gestor de reservas.
 * 
 * @param g Apontador para a estrutura GestorReservas.
 * @return Apontador para a tabela hash de reservas.
 */

GHashTable* get_tabela_reservas(GestorReservas* g);



#endif