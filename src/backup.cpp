#include "backup.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace {
    // Função auxiliar para verificar se o arquivo existe
    bool arquivoExiste(const std::string& caminho) {
        return fs::is_regular_file(caminho);
    }
}

Resultado executarBackup(
    const std::string& caminhoParm,
    const std::string& diretorioHd,
    const std::string& diretorioPendrive
) {
    (void)diretorioHd; // Evita warnings de variável não utilizada
    (void)diretorioPendrive; // Evita warnings de variável não utilizada

    //Comportamento já implementado em R1
    if(!arquivoExiste(caminhoParm)) {
        return Resultado::ERRO;
    }
    
    //Comportamento ainda não implementado em R2
    return Resultado::NADA;
}