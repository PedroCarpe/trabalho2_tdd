CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
LDLIBS := -lCatch2Main -lCatch2
DOXYGEN ?= doxygen

BUILD_DIR := build
TARGET := $(BUILD_DIR)/testa_backup
SRC := src/backup.cpp
TEST_SRC := tests/testa_backup.cpp
OBJ := $(BUILD_DIR)/backup.o $(BUILD_DIR)/testa_backup.o

.PHONY: all compile test cpplint cppcheck gcov debug debug-check valgrind doc quality clean

all: test

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/backup.o: $(SRC) include/backup.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/testa_backup.o: $(TEST_SRC) include/backup.hpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ $(LDLIBS)

compile: $(TARGET)

test: $(TARGET)
	./$(TARGET)

cpplint:
	cpplint include/backup.hpp $(SRC) $(TEST_SRC)

cppcheck:
	cppcheck --enable=warning --error-exitcode=1 --std=c++17 -Iinclude $(SRC) $(TEST_SRC)

# Recompila com instrumentacao de cobertura em diretorio separado.
gcov:
	mkdir -p $(BUILD_DIR)/coverage
	python3 scripts/verificar_cobertura.py --limpar
	$(CXX) $(CXXFLAGS) --coverage -fprofile-abs-path -O0 -c $(SRC) -o $(BUILD_DIR)/coverage/backup.o
	$(CXX) $(CXXFLAGS) --coverage -fprofile-abs-path -O0 -c $(TEST_SRC) -o $(BUILD_DIR)/coverage/testa_backup.o
	$(CXX) --coverage $(BUILD_DIR)/coverage/backup.o $(BUILD_DIR)/coverage/testa_backup.o -o $(BUILD_DIR)/coverage/testa_backup $(LDLIBS)
	./$(BUILD_DIR)/coverage/testa_backup
	cd $(BUILD_DIR)/coverage && gcov -b -c -o . ../../$(SRC) ../../$(TEST_SRC) > gcov.log
	python3 scripts/verificar_cobertura.py

# Compila com simbolos de depuracao sem reutilizar objetos de release.
debug:
	mkdir -p $(BUILD_DIR)/debug
	$(CXX) $(CXXFLAGS) -g -O0 $(SRC) $(TEST_SRC) -o $(BUILD_DIR)/debug/testa_backup $(LDLIBS)
	gdb ./$(BUILD_DIR)/debug/testa_backup

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 --log-file=$(BUILD_DIR)/valgrind.rpt ./$(TARGET)

# Sessão reproduzível: inspeciona a biblioteca e executa R6 sob GDB.
debug-check:
	mkdir -p $(BUILD_DIR)/debug
	$(CXX) $(CXXFLAGS) -g -O0 $(SRC) $(TEST_SRC) -o $(BUILD_DIR)/debug/testa_backup $(LDLIBS)
	gdb -q -batch -ex 'set debuginfod enabled off' -ex 'break executarBackup' -ex 'run [R6]' -ex 'print operacao' -ex 'bt' -ex 'continue' $(BUILD_DIR)/debug/testa_backup

doc:
	$(DOXYGEN) Doxyfile

quality: test cpplint cppcheck gcov valgrind

clean:
	rm -rf $(BUILD_DIR) *.gcov valgrind.rpt
