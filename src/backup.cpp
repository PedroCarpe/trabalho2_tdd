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

// Decide e executa a ação quando os dois arquivos existem.
Resultado processarArquivosExistentes(const fs::path& hd, const fs::path& pen,
                                     Operacao operacao) {
    const auto ordem = compararDatas(pen, hd);
    if (ordem == OrdemDatas::IGUAL) {
        return Resultado::NADA;
    }
    if (operacao == Operacao::BACKUP) {
        if (ordem == OrdemDatas::POSTERIOR) {
            return Resultado::ERRO;
        }
        return salvarArquivo(hd, pen) ? Resultado::SALVAR : Resultado::ERRO;
    }
    if (ordem == OrdemDatas::ANTERIOR) {
        return Resultado::ERRO;
    }
    return salvarArquivo(pen, hd) ? Resultado::RESTAURAR : Resultado::ERRO;
}

}

Resultado executarBackup(
    const std::string& caminhoParm,
    const std::string& diretorioHd,
    const std::string& diretorioPendrive,
    Operacao operacao
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

        if (operacao == Operacao::RESTAURAR && !arquivoExiste(destino)) {
            return Resultado::ERRO;
        }

        if (!arquivoExiste(origem)) {
            if (!arquivoExiste(destino)) {
                return Resultado::ERRO;
            }
            continue;
        }

        if (arquivoExiste(destino)) {
            const auto resultado = processarArquivosExistentes(origem, destino,
                                                               operacao);
            if (resultado != Resultado::NADA) {
                return resultado;
            }
            continue;
        }

        if (!fs::exists(destino)) {
            return salvarArquivo(origem, destino)
                ? Resultado::SALVAR : Resultado::ERRO;
        }
    }
    
    //Comportamento provisório para outros cenários
    return Resultado::NADA;
}
