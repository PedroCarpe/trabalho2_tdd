/**
 * @file
 * @brief Biblioteca de backup dirigida pela tabela de decisão.
 */
#include "backup.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

namespace fs = std::filesystem;

namespace {
/** @brief Verifica arquivo regular; entrada: caminho não vazio. */
bool arquivoExiste(const fs::path& caminho) {
  assert(!caminho.empty());
  return fs::is_regular_file(caminho);
}

/** @brief Ordem da primeira data em relação à segunda. */
enum class OrdemDatas { ANTERIOR, IGUAL, POSTERIOR };

/** @brief Ordena datas; entrada: dois arquivos regulares existentes. */
OrdemDatas compararDatas(const fs::path& arquivo, const fs::path& referencia) {
  assert(arquivoExiste(arquivo));
  assert(arquivoExiste(referencia));
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

/**
 * @brief Copia conteúdo e data de modificação, sobrescrevendo o destino.
 * @pre Origem regular e destino não vazio.
 * @post Em sucesso, destino regular com a mesma data da origem.
 * @return false se a cópia ou a atualização da data falhar (sem rollback).
 */
bool salvarArquivo(const fs::path& origem, const fs::path& destino) {
  assert(arquivoExiste(origem));
  assert(!destino.empty());

  std::error_code erro;

  bool copiado = fs::copy_file(origem, destino,
                               fs::copy_options::overwrite_existing, erro);

  if (!copiado || erro) {
    return false;
  }
  const auto data = fs::last_write_time(origem, erro);
  if (erro) {
    return false;
  }
  fs::last_write_time(destino, data, erro);
  if (!erro) {
    assert(arquivoExiste(destino));
    assert(fs::last_write_time(destino) == data);
  }
  return !erro;
}

/** @brief Aplica R3–R5 e R7–R9; entrada: ambos os arquivos regulares. */
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

/** @brief Processa um item; não lê a lista nem agrega resultados. */
Resultado processarArquivo(const fs::path& origem, const fs::path& destino,
                           Operacao operacao) {
  if (operacao == Operacao::RESTAURAR && !arquivoExiste(destino)) {
    return Resultado::ERRO;
  }

  if (!arquivoExiste(origem)) {
    if (!arquivoExiste(destino)) {
      return Resultado::ERRO;
    }
    if (operacao == Operacao::RESTAURAR) {
      return salvarArquivo(destino, origem) ? Resultado::RESTAURAR
                                            : Resultado::ERRO;
    }
    return Resultado::NADA;
  }

  if (arquivoExiste(destino)) {
    const auto resultado =
        processarArquivosExistentes(origem, destino, operacao);
    if (resultado != Resultado::NADA) {
      return resultado;
    }
    return Resultado::NADA;
  }

  if (!fs::exists(destino)) {
    return salvarArquivo(origem, destino) ? Resultado::SALVAR : Resultado::ERRO;
  }
  return Resultado::ERRO;
}

}  // namespace

Resultado executarBackup(const std::string& caminhoParm,
                         const std::string& diretorioHd,
                         const std::string& diretorioPendrive,
                         Operacao operacao, std::string* erro) {
  assert(!caminhoParm.empty());
  assert(!diretorioHd.empty());
  assert(!diretorioPendrive.empty());
  assert(operacao == Operacao::BACKUP || operacao == Operacao::RESTAURAR);
  if (erro != nullptr) {
    erro->clear();
  }

  // Valida a lista antes de processar qualquer arquivo.
  if (!arquivoExiste(caminhoParm)) {
    if (erro != nullptr) {
      *erro = "Backup.parm ausente ou nao e arquivo regular: " + caminhoParm;
    }
    return Resultado::IMPOSSIVEL;
  }

  std::ifstream parm(caminhoParm);

  if (!parm.is_open()) {
    if (erro != nullptr) {
      *erro = "Nao foi possivel abrir Backup.parm: " + caminhoParm;
    }
    return Resultado::ERRO;
  }

  Resultado resultadoGlobal = Resultado::NADA;
  std::string nomeArquivo;

  while (std::getline(parm, nomeArquivo)) {
    if (nomeArquivo.empty()) {
      continue;
    }

    fs::path origem = fs::path(diretorioHd) / nomeArquivo;
    fs::path destino = fs::path(diretorioPendrive) / nomeArquivo;

    const auto resultado = processarArquivo(origem, destino, operacao);
    if (resultado == Resultado::ERRO) {
      resultadoGlobal = Resultado::ERRO;
      if (erro != nullptr) {
        if (!erro->empty()) {
          *erro += "\n";
        }
        *erro += nomeArquivo +
                 ": origem ausente, versao conflitante ou falha de copia.";
      }
    } else if (resultadoGlobal != Resultado::ERRO &&
               resultado != Resultado::NADA) {
      resultadoGlobal = resultado;
    }
  }

  if (parm.bad()) {
    if (erro != nullptr) {
      *erro += "\nFalha ao ler Backup.parm: " + caminhoParm;
    }
    return Resultado::ERRO;
  }
  assert(resultadoGlobal == Resultado::NADA ||
         resultadoGlobal == Resultado::ERRO ||
         resultadoGlobal == (operacao == Operacao::BACKUP
                                 ? Resultado::SALVAR
                                 : Resultado::RESTAURAR));
  return resultadoGlobal;
}
