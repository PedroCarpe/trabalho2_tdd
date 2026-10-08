#include "backup.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>    

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


    std::ifstream parm(caminhoParm);

    if (!parm.is_open()) {
        return Resultado::ERRO;
    }

    std::string nomeArquivo;

    while (std::getline(parm, nomeArquivo)) {

        if (nomeArquivo.empty()) {
            continue;
        }

        fs::path origem = fs::path(diretorioHd) / nomeArquivo;
        fs::path destino = fs::path(diretorioPendrive) / nomeArquivo;

        if (fs::is_regular_file(origem) && !fs::exists(destino)) {

            std::error_code erro;

            bool copiado = fs::copy_file(
                origem,
                destino,
                fs::copy_options::none,
                erro
            );

            if (!copiado || erro) {
                return Resultado::ERRO;
            }

            return Resultado::SALVAR;
        }
    }
    
    //Comportamento provisório para outros cenários
    return Resultado::NADA;
}