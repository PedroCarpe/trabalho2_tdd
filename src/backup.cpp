#include "backup.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace {
    // Função auxiliar para verificar se o arquivo existe
    bool arquivoExiste(const std::string& caminho) {
        return fs::is_regular_file(caminho);
    }
}

Resultado executarBackup(const std::string& caminhoParm) {
    if(!arquivoExiste(caminhoParm)) {
        return Resultado::ERRO;
    }
    
    return Resultado::NADA;
}