#include "backup.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>    

namespace fs = std::filesystem;

namespace {
    // Função auxiliar para verificar se o arquivo existe
    bool arquivoExiste(const fs::path& caminho) {
        return fs::is_regular_file(caminho);
    }

    // Compara as datas de modificação de dois arquivos existentes.
    bool arquivoMaisAntigo(const fs::path& arquivo, const fs::path& referencia) {
        return fs::last_write_time(arquivo) < fs::last_write_time(referencia);
    }

    // Copia um arquivo da origem para o destino.
// Retorna true se a copia for realizada com sucesso.
bool salvarArquivo(
    const fs::path& origem,
    const fs::path& destino) {

    std::error_code erro;

    bool copiado = fs::copy_file(
        origem,
        destino,
        fs::copy_options::overwrite_existing,
        erro
    );

    return copiado && !erro;
}

}

Resultado executarBackup(
    const std::string& caminhoParm,
    const std::string& diretorioHd,
    const std::string& diretorioPendrive
) {
    
    //Comportamento já implementado em R1
    if(!arquivoExiste(caminhoParm)) {
        return Resultado::IMPOSSIVEL;
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

        if (arquivoExiste(origem) &&
            (!fs::exists(destino) ||
             (arquivoExiste(destino) &&
              arquivoMaisAntigo(destino, origem)))) {

            if (!salvarArquivo(origem, destino)) {
                return Resultado::ERRO;
            }

            return Resultado::SALVAR;
        }
    }
    
    //Comportamento provisório para outros cenários
    return Resultado::NADA;
}
