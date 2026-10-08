#ifndef BACKUP_HPP
#define BACKUP_HPP

#include <string>

enum class Resultado {
    SALVAR,
    RESTAURAR,
    EXCLUIR,
    NADA,
    ERRO
};

Resultado executarBackup(const std::string& caminhoParm);

#endif