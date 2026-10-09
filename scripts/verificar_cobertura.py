"""Confere cobertura de linhas dos módulos próprios nos relatórios gcov."""
import re
import sys
from pathlib import Path

if "--limpar" in sys.argv:
    for perfil in Path("build/coverage").glob("*.gcda"):
        perfil.unlink()
    raise SystemExit(0)

padrao = re.compile(r"^\s*(\d+\*?|#####|=====):\s*[1-9]\d*:")
falhou = False
for modulo in ("backup.cpp", "testa_backup.cpp"):
    linhas = []
    for linha in Path("build/coverage", modulo + ".gcov").read_text().splitlines():
        encontrado = padrao.match(linha)
        if encontrado:
            linhas.append(encontrado.group(1))
    executadas = sum(
        valor not in ("#####", "=====") and int(valor.rstrip("*")) > 0
        for valor in linhas
    )
    percentual = 100 * executadas / len(linhas) if linhas else 0
    print(f"{modulo}: {percentual:.2f}% ({executadas}/{len(linhas)} linhas)")
    falhou |= percentual < 80
raise SystemExit(1 if falhou else 0)
