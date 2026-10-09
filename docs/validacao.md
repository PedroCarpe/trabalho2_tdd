# Validação final — 08/10/2026

## Resultados executados

| Verificação | Resultado |
|---|---|
| `make test` / `make quality` | 19 casos Catch2, 209 assertivas aprovadas |
| R1–R13 | Um caso específico por coluna, todos aprovados |
| `make gcov` | backup.cpp: 91,84% (90/98 linhas) |
| `make gcov` | testa_backup.cpp: 100% (245/245 linhas) |
| Compilação C++17 | Sem avisos com -Wall -Wextra -Wpedantic |
| `make cpplint` | Sem avisos com CPPLINT.cfg documentado |
| `make cppcheck` | Sem avisos; --error-exitcode=1 |
| `make valgrind` | 0 erros, 0 bytes em uso ao sair, todas as alocações liberadas |
| `make debug-check` | Breakpoint em executarBackup, operação RESTAURAR, R6 aprovado |
| Doxygen 1.9.8 | HTML gerado e index.html verificado, sem avisos |
| `git diff --check` | Sem problemas de whitespace |

Ferramentas extras foram obtidas como pacotes locais em build/tools; não
houve instalação global. Para reproduzir a geração local:

```
LD_LIBRARY_PATH=build/tools/local/usr/lib/x86_64-linux-gnu \
  make doc DOXYGEN=build/tools/local/usr/bin/doxygen
```

Em um ambiente com Doxygen instalado, basta `make doc`. Artefatos de ferramentas,
HTML, cobertura, depuração e Valgrind ficam em build/ e não entram no Git.

A cobertura é de linhas dos módulos próprios, não o percentual agregado dos
headers de STL/Catch2. Não se afirma 100% de ramos/caminhos. As falhas não
provocadas e a expressão regular estão em cobertura_caminhos.md.

## Requisitos e pendências acadêmicas

- R7–R13 concluídas sequencialmente, preservando R1–R6; histórico anterior intacto.
- R8, R11 e R12 já passavam: situação registrada sem fabricar RED ou GREEN.
- Refatoração real em R9; nas demais regras, avaliação documentada.
- Mais de 30 commits no histórico; não foram fabricados três commits por teste.
  A exigência literal de três por teste continua sujeita à avaliação docente.
- Biblioteca, programa de testes, Makefile, Catch2, gcov, cpplint, cppcheck,
  Valgrind, GDB, contratos, Javadoc, Doxygen e leiame.txt presentes.
- Revisão dirigida registrada em revisao.md. Inspeção formal por equipe e
  confronto com os checklists específicos devem ser feitos pelo aluno.


## Política e limites da biblioteca

A tabela define decisões por arquivo, mas não agregação da lista. A política
adotada e testada processa todos os itens: ERRO prevalece; caso contrário,
retorna a ação do modo quando houve cópia, ou NADA. IMPOSSIVEL cancela antes
de processar a lista. Não há rollback de cópias em caso de erro.

Não há exclusão: nenhuma coluna marca essa ação. Datas iguais significam
NADA, sem comparação de conteúdo. A cópia preserva a data da origem, evitando
conflito falso na próxima execução. Caminhos não vazios, nomes relativos e
operação válida são precondições. Corridas externas e exceções de consulta
ao filesystem não têm tratamento transacional.

## Histórico

O início desta tarefa foi `3c679f1`. Consulte os commits reais, em ordem:

```
git log --reverse --oneline 3c679f1..HEAD
```

A lista completa até a entrega também será disponibilizada em build/commits.txt.
