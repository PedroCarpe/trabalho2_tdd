#ifndef BACKUP_HPP
#define BACKUP_HPP

#include <string>

enum class Operacao {
    BACKUP,
    RESTAURAR
};

enum class Resultado {
    SALVAR,
    RESTAURAR,
    EXCLUIR,
    NADA,
    ERRO,
    IMPOSSIVEL
};

Resultado executarBackup(
    const std::string& caminhoParm,
    const std::string& diretorioHd,
    const std::string& diretorioPendrive,
    Operacao operacao = Operacao::BACKUP
);

#endif
