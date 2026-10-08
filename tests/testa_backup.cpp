#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <string>

#include "backup.hpp"

namespace fs = std::filesystem;

TEST_CASE("R1 - Backup.parm nao existe", "[backup][R1]") {

    // ARRANGE: preparar o cenário
    const fs::path diretorioTeste = "test_data_r1";
    const fs::path backupParm = diretorioTeste / "Backup.parm";

    fs::create_directories(diretorioTeste);
    fs::remove(backupParm);

    // Verifica a precondição do teste
    REQUIRE_FALSE(fs::exists(backupParm));

    // ACT: executar a função
    Resultado resultado = executarBackup(backupParm.string());

    // ASSERT: verificar o comportamento esperado
    REQUIRE(resultado == Resultado::ERRO);

    // Limpeza
    fs::remove_all(diretorioTeste);
}