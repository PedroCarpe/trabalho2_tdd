#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <fstream>
#include <string>

#include "backup.hpp"

namespace fs = std::filesystem;

TEST_CASE("R1 - Backup.parm nao existe", "[backup][R1]") {

    // ARRANGE: preparar o cenário
    const fs::path diretorioTeste = "test_data_r1";
    const fs::path backupParm = diretorioTeste / "Backup.parm";

    // Novos parâmetros
    const fs::path hd = diretorioTeste / "hd";
    const fs::path pendrive = diretorioTeste / "pendrive";

    fs::create_directories(diretorioTeste);
    fs::remove(backupParm);

    // Verifica a precondição do teste
    REQUIRE_FALSE(fs::exists(backupParm));

    // ACT: executar a função
    Resultado resultado = executarBackup(
        backupParm.string(),
        hd.string(),
        pendrive.string());

    // ASSERT: verificar o comportamento esperado
    REQUIRE(resultado == Resultado::ERRO);

    // Limpeza
    fs::remove_all(diretorioTeste);
}

TEST_CASE("R2 - Salvar A.txt no pendrive", "[backup][R2]") {

    namespace fs = std::filesystem;

    // ARRANGE
    const fs::path base = "test_data_r2";
    const fs::path hd = base / "hd";
    const fs::path pendrive = base / "pendrive";
    const fs::path parm = base / "Backup.parm";

    // Garantir ambiente inicial limpo
    fs::remove_all(base);

    fs::create_directories(hd);
    fs::create_directories(pendrive);

    // Criar Backup.parm
    {
        std::ofstream arquivo(parm);
        arquivo << "A.txt\n";
    }

    // Criar A.txt no HD
    {
        std::ofstream arquivo(hd / "A.txt");
        arquivo << "Conteudo original";
    }

    REQUIRE(fs::exists(parm));
    REQUIRE(fs::exists(hd / "A.txt"));
    REQUIRE_FALSE(fs::exists(pendrive / "A.txt"));

    // ACT
    Resultado resultado = executarBackup(
        parm.string(),
        hd.string(),
        pendrive.string()
    );

    // ASSERT
    REQUIRE(resultado == Resultado::SALVAR);
    REQUIRE(fs::exists(pendrive / "A.txt"));

    std::ifstream arquivoCopiado(pendrive / "A.txt");
    std::string conteudo;
    std::getline(arquivoCopiado, conteudo);

    REQUIRE(conteudo == "Conteudo original");

    // Limpeza
    fs::remove_all(base);
}