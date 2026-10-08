#include "backup.hpp"
#include <filesystem>

Resultado executarBackup(const std::string& caminhoParm) {
    if(!std::filesystem::is_regular_file(caminhoParm)) {
        return Resultado::ERRO;
    }
    
    return Resultado::NADA;
}