/**
 * @file utils.h
 * @brief Ficheiro de cabeçalho para funções auxiliares gerais do projeto, ou seja, que não estão diretamente ligadas a um gestor específico.
 *
 * Este ficheiro contém as declarações das funções utilizadas para validação de códigos IATA,
 * registar erros em ficheiros, validação de identificadores de voo (flight_id), números de documentos (doc_number),
 * formatos de listas em CSV, datas, date times (datetime) e ano.
 */

#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdbool.h>

/**
 * @brief Função para registar erros em ficheiros.
 *
 * Esta função regista os erros que ocorreram durante o processamento dos dados
 * em um ficheiro de erros que é especificado.
 *
 * @param errorFilePath Caminho para o ficheiro de erros onde os erros serão registados.
 * @param linha String que representa a linha com o erro a ser registado.
 * @param isHeader Booleano que indica se a linha a ser registada é um cabeçalho (header) ou não.
 */

void registarErro (const char* errorFilePath, const char* linha, bool isHeader);

/**
 * @brief Função para validar um código IATA.
 *
 * Esta função verifica se o código IATA fornecido é válido,
 * ou seja, se possui exatamente 3 letras maiúsculas.
 *
 * @param code String que representa o código IATA que irá ser validado.
 *
 * @return Inteiro que indica se o código IATA é válido (1) ou inválido (0).
 */

int validar_iata_code(const char* code);

/**
 * @brief Função para validar um identificador de voo (flight_id).
 *
 * Esta função verifica se o identificador de voo fornecido (flight_id) é válido,
 * ou seja, se está no formato correto, isto é, 2 letras maiúsculas seguidas de 5 caracteres que devem ser dígitos.
 *
 * @param flight_id String que representa o identificador de voo que irá ser validado.
 *
 * @return Inteiro que indica se o identificador de voo é válido (1) ou inválido (0).
 */

int valida_flight_id(const char* flight_id);

/**
 * @brief Função para validar um número de documento (doc_number).
 *
 * Esta função verifica se o número do documento fornecido é válido,
 * ou seja, se possui exatamente 9 dígitos.
 *
 * @param doc_number String que representa o número do documento que irá ser validado.
 *
 * @return Inteiro que indica se o número do documento é válido (1) ou inválido (0).
 */

int validar_doc_number(const char* doc_number);

/**
 * @brief Função que valida o formato de uma lista em CSV.
 *
 * Esta função verifica se a lista em CSV fornecida está no formato correto,
 * isto é, se começa com '[' e termina com ']'.
 *
 * @param lista_csv String que representa a lista em CSV que irá ser validada pela função.
 *
 * @return Inteiro que indica se o formato da lista em CSV é válido (1) ou inválido (0).
 */

int valida_formato_lista_csv (const char* lista_csv);

/** 
 * @brief Função que valida se a data está no formato aaaa-mm-dd e não é futura.
 *
 * Esta função verifica se a data fornecida está no formato correto (aaaa-mm-dd)
 * e se não é uma data futura em relação à data atual, tendo considerado a data atual como 2025-09-30.
 *
 * @param data String que representa a data que irá ser validada.
 *
 * @return Inteiro que indica se a data é válida (1) ou inválida (0).
 */

int validar_data(const char* data);

/**
 * @brief Função que valida se o date time está no formato "aaaa-mm-dd hh:mm".
 *
 * Esta função verifica se o date time fornecido está no formato correto,
 * ou seja, no formato (aaaa-mm-dd hh:mm) e se os valores de hora e minuto são válidos.
 *
 * @param date_time String que representa o date time que irá ser validado.
 *
 * @return Inteiro que indica se o date time é válido (1) ou inválido (0).
 */

int validar_datetime(const char* date_time);

/**
 * @brief Função que valida se o ano é válido.
 *
 * Esta função verifica se o ano fornecido está no formato correto, ou seja, 
 * se possui exatamente 4 caracteres que são todos dígitos, bem como,
 * se o ano não é maior que o ano atual (2025).
 *
 * @param ano String que representa o ano que irá ser validado.
 *
 * @return Inteiro que indica se o ano é válido (1) ou inválido (0).
 */

int validar_ano(const char* ano);

/** 
* @brief Função Genérica que parte uma linha CSV em tokens.
*
* Esta função divide uma linha de texto no formato CSV em tokens individuais
*
* @param linha A linha de texto no formato CSV a ser dividida.
* @param tokens Um array de strings onde os tokens extraídos serão armazenados.
* @param max_tokens O número máximo de tokens que podem ser armazenados no array tokens.
*/
int parse_csv_line(char* linha, char** tokens, int max_tokens);

#endif