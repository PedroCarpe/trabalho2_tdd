#ifndef INCLUDE_BACKUP_HPP_
#define INCLUDE_BACKUP_HPP_

/** @file @brief Interface pública da biblioteca de backup. */

#include <string>

/** @brief Sentido solicitado para a sincronização. */
enum class Operacao { BACKUP, RESTAURAR };

/**
 * @brief Ação realizada ou falha. EXCLUIR é reservado e não usado pela tabela.
 */
enum class Resultado { SALVAR, RESTAURAR, EXCLUIR, NADA, ERRO, IMPOSSIVEL };

/**
 * @brief Processa todos os arquivos listados em Backup.parm conforme R1–R13.
 * @param caminhoParm Caminho da lista, um nome relativo por linha.
 * @param diretorioHd Diretório do HD.
 * @param diretorioPendrive Diretório do pendrive ou equivalente.
 * @param operacao BACKUP (padrão) ou RESTAURAR.
 * @param erro Recebe diagnóstico; nullptr dispensa mensagens.
 * @pre Caminhos não vazios e operação válida. Diretórios preparados pelo
 * cliente.
 * @post Em ERRO, itens válidos ainda são processados; não há rollback.
 * @return IMPOSSIVEL cancela por lista ausente; ERRO prevalece; caso contrário,
 * retorna a ação do modo se houve cópia, ou NADA. EXCLUIR não aparece na
 * tabela.
 */
Resultado executarBackup(const std::string& caminhoParm,
                         const std::string& diretorioHd,
                         const std::string& diretorioPendrive,
                         Operacao operacao = Operacao::BACKUP,
                         std::string* erro = nullptr);

#endif  // INCLUDE_BACKUP_HPP_
