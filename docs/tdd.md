R6 — REFACTOR
A implementação foi revisada e não foram identificadas
melhorias estruturais necessárias nesta etapa.
Os testes R1–R6 permanecem aprovados.

R7 — RED / GREEN / avaliação REFACTOR
O teste distinguiu os modos: RESTAURAR retornava SALVAR, alterava o conteúdo
 e a data do pendrive. GREEN passou com 7 casos e 68 assertivas.
A condição reutiliza compararDatas; nenhuma extração adicional é necessária
nesta etapa. Os arquivos e as duas datas são preservados no erro.

R8 — teste já GREEN / avaliação REFACTOR
O teste passou imediatamente: 8 casos, 82 assertivas. Datas iguais já não
provocavam cópia, inclusive em RESTAURAR. Verificadas as duas datas e conteúdos.
Não foi fabricado RED nem alteração de implementação. O comparador centralizado
já representa igualdade, portanto nenhuma refatoração adicional é necessária.

R9 — RED / GREEN / REFACTOR
RED: 9 casos, somente R9 falhou (ERRO e conteúdo antigo no HD).
GREEN: 9 casos e 95 assertivas aprovadas. A cópia inverte origem e destino.
REFACTOR: processarArquivosExistentes separa decisão por operação da leitura
 da lista, compartilhando comparação e cópia. Regressão: 9/95 aprovados.

R10 — RED / GREEN / avaliação REFACTOR
RED retornou NADA em vez de ERRO; arquivos permaneceram ausentes.
GREEN: 10 casos, 102 assertivas aprovadas. A verificação de existência já
usa o auxiliar comum; nenhuma refatoração adicional foi necessária.

R11 — teste já GREEN / avaliação REFACTOR
11 casos e 112 assertivas passaram imediatamente. O arquivo apenas no
pendrive é preservado, sem criar arquivo no HD. A tabela não manda excluir.
Não houve implementação nem refatoração artificiais.

R12 — teste já GREEN / avaliação REFACTOR
12 casos e 119 assertivas passaram imediatamente. A guarda introduzida em R6
já rejeita RESTAURAR quando o pendrive não contém o arquivo, mesmo sem HD.
Nenhuma mudança de negócio ou extração adicional foi necessária.

R13 — RED / GREEN / avaliação REFACTOR
RED: NADA em vez de RESTAURAR e arquivo não criado. GREEN: 13 casos,
131 assertivas aprovadas. A cópia reutiliza salvarArquivo com sentido invertido.
Nenhuma refatoração adicional necessária nesta regra; o requisito de processar
toda a lista será tratado separadamente por um teste de regressão específico.
