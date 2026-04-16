/**
 * @file gestor_aeronaves.h
 * @brief Ficheiro de cabeçalho para as funções associadas ao gestor de aeronaves.
 *
 * Este ficheiro contém as declarações das funções utilizadas para criar, destruir,
 * adicionar, remover e consultar aeronaves no gestor de aeronaves, bem como os getters usados para aceder às aeronaves armazenadas,
 * ou a campos específicos das mesmas.
 */

#ifndef _GESTOR_AERONAVES_H_
#define _GESTOR_AERONAVES_H_

#include <glib.h>
#include "entidades/aeronaves.h"

/**
 * @brief Estrutura que representa o gestor de aeronaves.
 *
 * Esta estrutura contém uma tabela hash que armazena as aeronaves,
 * onde a chave é o identificador da aeronave (char*) e o valor é um apontador para a estrutura Aeronaves*.
 */

typedef struct gestor_aeronaves GestorAeronaves;


/**
 * @brief Função para criar o gestor de aeronaves.
 *
 * Esta função aloca memória para uma nova estrutura GestorAeronaves
 * inicializando a tabela hash que irá armazenar as aeronaves.
 * 
 * @return Apontador para a nova estrutura GestorAeronaves criada.
 */

GestorAeronaves* criar_gestor_aeronaves();


/** 
 * @brief Função para destruir o gestor de aeronaves.
 * 
 * Esta função liberta a memória ocupada pela estrutura GestorAeronaves,
 * incluindo a tabela hash e todas as aeronaves que estão armazenadas nela.
 *
 * @param g Apontador para a estrutura GestorAeronaves que vai destruída.
 */

void destruir_gestor_aeronaves(GestorAeronaves* g);


/** 
 * @brief Função para adicionar uma aeronave ao gestor de aeronaves.
 *
 * Esta função insere uma nova aeronave na tabela hash do gestor,
 * usando o identificador da aeronave como chave.
 *
 * @param g Apontador para a estrutura GestorAeronaves onde será adiconada a respetiva aeronave.
 * @param a Apontador para a estrutura Aeronaves que será adicionada ao gestor.
 */

void adiciona_aeronave(GestorAeronaves* g, Aeronaves* a);

/**
 * @brief Função para verificar se uma aeronave existe no gestor de aeronaves.
 *
 * Esta função verifica se uma aeronave com um identificador fornecido
 * existe na tabela hash do gestor.
 *
 * @param g Apontador para a estrutura GestorAeronaves onde será irá ser verificado se a aeronave existe.
 * @param id_aeronave String que representa o identificador da aeronave a ser verificada.
 * @return true se a aeronave existir, ou false, no caso de a aeronave não existir.
 */

bool aeronave_existe(GestorAeronaves* g, const char* id_aeronave);

/**
 * @brief Função que remove uma aeronave do gestor de aeronaves.
 *
 * Esta função remove a aeronave com um identificador fornecido
 * da tabela hash do gestor.
 *
 * @param g Apontador para a estrutura GestorAeronaves onde será irá ser removida a respetiva aeronave.
 * @param id_aeronave String que representa o identificador da aeronave a ser removida.
 */

void remove_aeronave(GestorAeronaves* g, char* id_aeronave);

/**
 * @brief Função que conta o número de aeronaves no gestor de aeronaves.
 *
 * Esta função retorna o número total de aeronaves armazenadas
 * na tabela hash do gestor de aeronaves.
 *
 * @param g Apontador para a estrutura GestorAeronaves onde será feita a contagem das aeronaves.
 * @return Inteiro que representa o número de aeronaves existentes no gestor.
 */

int conta_aeronaves(GestorAeronaves* g);
/** 
 * @brief Getters para aceder a uma aeronave com um determinado identifier (id da aeronave).
 *
 * Esta função retorna um apontador para a estrutura Aeronaves
 * associada ao identificador fornecido.
 *
 * @param g Apontador para a estrutura GestorAeronaves onde será feita a procura da aeronave.
 * @param id String que representa o identificador da aeronave que vai ser procurada.
 * @return Apontador para a estrutura Aeronaves correspondente ao identificador fornecido.
 */

Aeronaves* get_aeronave(GestorAeronaves* g, const char* id);

/**
 * @brief Getters para aceder a tabela de aeronaves.
 *
 * Esta função retorna um apontador para a tabela hash
 * que armazena todas as aeronaves no gestor.
 *
 * @param g Apontador para a estrutura GestorAeronaves onde será acedida a tabela de aeronaves.
 * @return Apontador para a tabela hash de aeronaves. 
 */

GHashTable* get_tabela_aeronaves(GestorAeronaves* g);

#endif