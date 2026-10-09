#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>
#include <system_error>

#include "backup.hpp"

namespace fs = std::filesystem;

namespace {
struct Cenario {
    fs::path base;
    fs::path hd;
    fs::path pen;
    fs::path parm;

    explicit Cenario(const std::string& nome)
        : base(fs::temp_directory_path() / ("backup_tdd_" + nome)),
          hd(base / "hd"), pen(base / "pendrive"), parm(base / "Backup.parm") {
        fs::remove_all(base);
        fs::create_directories(hd);
        fs::create_directories(pen);
    }

    ~Cenario() {
        std::error_code ec;
        fs::remove_all(base, ec);
    }

    void Lista(const std::string& nomes = "A.txt\n") {
        Escrever(parm, nomes);
    }

    static void Escrever(const fs::path& arquivo, const std::string& texto) {
        std::ofstream saida(arquivo, std::ios::binary);
        REQUIRE(static_cast<bool>(saida));
        saida << texto;
        REQUIRE(static_cast<bool>(saida));
    }

    static std::string Ler(const fs::path& arquivo) {
        std::ifstream entrada(arquivo, std::ios::binary);
        REQUIRE(static_cast<bool>(entrada));
        return std::string(std::istreambuf_iterator<char>(entrada),
                           std::istreambuf_iterator<char>());
    }

    static void Data(const fs::path& arquivo, int horas) {
        fs::last_write_time(arquivo, fs::file_time_type::clock::now() +
                           std::chrono::hours(horas));
    }

    Resultado Executar(Operacao operacao = Operacao::BACKUP) {
        return executarBackup(parm.string(), hd.string(), pen.string(), operacao);
    }
};
}  // namespace

TEST_CASE("R1 - Backup.parm nao existe", "[backup][R1]") {
    Cenario c("r1");

    REQUIRE_FALSE(fs::exists(c.parm));
    REQUIRE(c.Executar() == Resultado::IMPOSSIVEL);
}

TEST_CASE("R2 - Salvar A.txt no pendrive", "[backup][R2]") {
    Cenario c("r2");
    c.Lista();
    c.Escrever(c.hd / "A.txt", "Conteudo original");

    REQUIRE(fs::exists(c.parm));
    REQUIRE(fs::exists(c.hd / "A.txt"));
    REQUIRE_FALSE(fs::exists(c.pen / "A.txt"));

    REQUIRE(c.Executar() == Resultado::SALVAR);
    REQUIRE(fs::exists(c.pen / "A.txt"));
    REQUIRE(c.Ler(c.pen / "A.txt") == "Conteudo original");
}

TEST_CASE("R3 - Atualizar A.txt mais antigo no pendrive", "[backup][R3]") {
    Cenario c("r3");
    c.Lista();
    c.Escrever(c.hd / "A.txt", "Conteudo atualizado");
    c.Escrever(c.pen / "A.txt", "Conteudo antigo");
    c.Data(c.hd / "A.txt", 0);
    c.Data(c.pen / "A.txt", -24);

    REQUIRE(fs::is_regular_file(c.parm));
    REQUIRE(fs::is_regular_file(c.hd / "A.txt"));
    REQUIRE(fs::is_regular_file(c.pen / "A.txt"));
    REQUIRE(fs::last_write_time(c.pen / "A.txt") <
            fs::last_write_time(c.hd / "A.txt"));

    const Resultado resultado = c.Executar();

    // CHECK permite observar ambas as falhas esperadas da fase RED.
    CHECK(resultado == Resultado::SALVAR);
    CHECK(c.Ler(c.pen / "A.txt") == "Conteudo atualizado");
}

TEST_CASE("R4 - Backup: datas iguais: NADA", "[R4]") {
    Cenario c("r4"); c.Lista();
    c.Escrever(c.hd / "A.txt", "HD"); c.Escrever(c.pen / "A.txt", "Pen");
    fs::last_write_time(c.pen / "A.txt", fs::last_write_time(c.hd / "A.txt"));
    REQUIRE(c.Executar() == Resultado::NADA);
    REQUIRE(c.Ler(c.pen / "A.txt") == "Pen");
}

TEST_CASE("R5 - Backup: pendrive recente: ERRO", "[R5]") {
    Cenario c("r5");
    c.Lista();
    c.Escrever(c.hd / "A.txt", "HD");
    c.Escrever(c.pen / "A.txt", "Pen");
    c.Data(c.hd / "A.txt", -2);
    c.Data(c.pen / "A.txt", 2);

    REQUIRE(fs::is_regular_file(c.hd / "A.txt"));
    REQUIRE(fs::is_regular_file(c.pen / "A.txt"));
    REQUIRE(fs::last_write_time(c.pen / "A.txt") >
            fs::last_write_time(c.hd / "A.txt"));

    CHECK(c.Executar() == Resultado::ERRO);
    CHECK(c.Ler(c.pen / "A.txt") == "Pen");
}

TEST_CASE("R6 - Restaurar: apenas HD: ERRO", "[R6]") {
    Cenario c("r6"); c.Lista(); c.Escrever(c.hd / "A.txt", "HD");
    REQUIRE(c.Executar(Operacao::RESTAURAR) == Resultado::ERRO);
    REQUIRE(c.Ler(c.hd / "A.txt") == "HD");
}

TEST_CASE("R7 - Restaurar: pendrive antigo: ERRO", "[backup][R7]") {
    Cenario c("r7");
    c.Lista();
    c.Escrever(c.hd / "A.txt", "HD recente");
    c.Escrever(c.pen / "A.txt", "Pen antigo");
    c.Data(c.hd / "A.txt", 0);
    c.Data(c.pen / "A.txt", -24);
    const auto dataHd = fs::last_write_time(c.hd / "A.txt");
    const auto dataPen = fs::last_write_time(c.pen / "A.txt");
    REQUIRE(dataPen < dataHd);
    CHECK(c.Executar(Operacao::RESTAURAR) == Resultado::ERRO);
    CHECK(c.Ler(c.hd / "A.txt") == "HD recente");
    CHECK(c.Ler(c.pen / "A.txt") == "Pen antigo");
    CHECK(fs::last_write_time(c.hd / "A.txt") == dataHd);
    CHECK(fs::last_write_time(c.pen / "A.txt") == dataPen);
}
