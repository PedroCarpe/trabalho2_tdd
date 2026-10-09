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

Correção do requisito de lista completa
Dois testes adicionais revelaram RED: a segunda cópia não ocorria em nenhum
modo, nem se processava arquivo após um item inválido. processarArquivo agora
retorna o resultado de cada item e executarBackup percorre toda a lista.
Política explicitada (a tabela não define agregação): ERRO prevalece, mas os
outros itens continuam; sem erros, retorna a ação do modo se houve alguma cópia,
ou NADA. IMPOSSIVEL cancela antes de ler a lista. Não há rollback de cópias.

Validação adicional — datas, I/O, diagnóstico e infraestrutura
O teste de repetição encontrou RED: copy_file atribuía data nova, tornando a
cópia mais recente que a origem. A cópia agora preserva last_write_time;
regressão GREEN: 16 casos e 179 assertivas. Lista vazia e destino diretório
foram testados; o diretório revelava NADA indevido em BACKUP. Corrigido para
ERRO; 18 casos, 199 assertivas. Diagnósticos opcionais preservam as chamadas
anteriores e passaram após RED com mensagens vazias: 19 casos/209 assertivas.

Refatoração de estilo e contratos: indentação Google, includes, header guard,
comentários Javadoc e assertivas de entrada/saída. Temporários dos testes
passaram para build/test_data, dentro do projeto. CPPLINT.cfg explica as
exceções para C++17, macros Catch2, includes e ausência de atribuição inventada.

O histórico original foi preservado. R8, R11 e R12 não ganharam commits GREEN
artificiais; os testes já passaram. A avaliação de refatoração foi documentada
quando não existia extração justificada. A exigência acadêmica literal de três
commits por teste não foi fabricada retroativamente. O total já supera 30.
