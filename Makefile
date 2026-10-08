CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
LDLIBS := -lCatch2Main -lCatch2

BUILD_DIR := build
TARGET := $(BUILD_DIR)/testa_backup
SRC := src/backup.cpp
TEST_SRC := tests/testa_backup.cpp
OBJ := $(BUILD_DIR)/backup.o $(BUILD_DIR)/testa_backup.o

.PHONY: all compile test cpplint cppcheck gcov debug valgrind clean

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
	cppcheck --enable=warning --std=c++17 -Iinclude $(SRC) $(TEST_SRC)

# Recompila com instrumentacao de cobertura em diretorio separado.
gcov:
	mkdir -p $(BUILD_DIR)/coverage
	$(CXX) $(CXXFLAGS) --coverage -O0 -c $(SRC) -o $(BUILD_DIR)/coverage/backup.o
	$(CXX) $(CXXFLAGS) --coverage -O0 -c $(TEST_SRC) -o $(BUILD_DIR)/coverage/testa_backup.o
	$(CXX) --coverage $(BUILD_DIR)/coverage/backup.o $(BUILD_DIR)/coverage/testa_backup.o -o $(BUILD_DIR)/coverage/testa_backup $(LDLIBS)
	./$(BUILD_DIR)/coverage/testa_backup
	gcov -o $(BUILD_DIR)/coverage $(SRC)

# Compila com simbolos de depuracao sem reutilizar objetos de release.
debug:
	mkdir -p $(BUILD_DIR)/debug
	$(CXX) $(CXXFLAGS) -g -O0 $(SRC) $(TEST_SRC) -o $(BUILD_DIR)/debug/testa_backup $(LDLIBS)
	gdb ./$(BUILD_DIR)/debug/testa_backup

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --log-file=valgrind.rpt ./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) *.gcov valgrind.rpt
