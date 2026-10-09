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

    enum class OrdemDatas { ANTERIOR, IGUAL, POSTERIOR };

    // Compara as datas de modificação de dois arquivos existentes.
    OrdemDatas compararDatas(const fs::path& arquivo, const fs::path& referencia) {
        const auto dataArquivo = fs::last_write_time(arquivo);
        const auto dataReferencia = fs::last_write_time(referencia);
        if (dataArquivo < dataReferencia) {
            return OrdemDatas::ANTERIOR;
        }
        if (dataArquivo > dataReferencia) {
            return OrdemDatas::POSTERIOR;
        }
        return OrdemDatas::IGUAL;
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

        if (!arquivoExiste(origem)) {
            continue;
        }

        bool deveSalvar = !fs::exists(destino);
        if (!deveSalvar && arquivoExiste(destino)) {
            const auto ordem = compararDatas(destino, origem);
            if (ordem == OrdemDatas::POSTERIOR) {
                return Resultado::ERRO;
            }
            deveSalvar = ordem == OrdemDatas::ANTERIOR;
        }

        if (deveSalvar) {

            if (!salvarArquivo(origem, destino)) {
                return Resultado::ERRO;
            }

            return Resultado::SALVAR;
        }
    }
    
    //Comportamento provisório para outros cenários
    return Resultado::NADA;
}
