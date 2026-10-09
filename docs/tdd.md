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
