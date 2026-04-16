CC = gcc

CFLAGS = -Wall -Wextra -Werror -std=c11 -g -Iinclude -Iutils $(shell pkg-config --cflags glib-2.0)
LIBS = $(shell pkg-config --libs glib-2.0)

# ficheiros .c do projeto
SRC = $(wildcard src/*.c src/entidades/*.c src/gestor_entidades/*.c src/parsers/*.c src/queries/*.c src/utils/*.c) 


OBJ = $(SRC:.c=.o)

# Nomes dos executáveis gerados
MAIN = programa-principal
TESTE = programa-testes
INTERATIVO = programa-interativo

TEST_DATASET_PATH = dataset-fase1/dataset-fase1_sem_erros
TEST_INPUT_FILE = inputs_fase1.txt
TEST_EXPECTED_PATH = resultados-esperados/resultados-esperados-fase1
TEST_GENERAL_OUTPUT = test-phase1/test_queries/test_output.txt


# Aqui são excluídos ficheiros main, de forma a que cada programa use apenas o seu, para não ocorrer o erro de ter dois 'main' associados ao mesmo executável
CORE_OBJ = $(filter-out src/main_principal.o src/programa-interativo.o test-phase1/tests-phase1.o, $(SRC:.c=.o))


all: $(MAIN) $(INTERATIVO) $(TESTE)

$(MAIN): src/main_principal.o $(CORE_OBJ)
	$(CC) $(CFLAGS) src/main_principal.o $(CORE_OBJ) -o $(MAIN) $(LIBS)


$(INTERATIVO): src/programa-interativo.o $(CORE_OBJ)
	$(CC) $(CFLAGS) src/programa-interativo.o $(CORE_OBJ) -o $(INTERATIVO) $(LIBS)


$(TESTE): test-phase1/tests-phase1.c $(CORE_OBJ)
	@mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) test-phase1/tests-phase1.c $(CORE_OBJ) -o $(TESTE) $(LIBS) -lm

test: $(MAIN) $(TESTE)
	./$(TESTE) $(TEST_DATASET_PATH) $(TEST_INPUT_FILE) $(TEST_EXPECTED_PATH)


clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(MAIN) $(TESTE) $(INTERATIVO) 
	rm -rf resultados/
	rm -rf test-phase1/test_queries/

re: fclean all

# Permite correr o programa principal com o Valgrind para detetar os leaks de memória 
valgrind: $(MAIN)
	valgrind --leak-check=full --show-leak-kinds=all ./$(MAIN) dataset-fase2/sem_erros inputs_fase2.txt

.PHONY: all clean fclean re valgrind test