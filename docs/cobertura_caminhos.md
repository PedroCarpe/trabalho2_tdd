# Cobertura de caminhos (caixa aberta)

Base: Aula_Testes_caixa_aberta.pdf, páginas 10, 16 e 20–26. A expressão
regular modela caminhos normais de executarBackup; as verificações internas
cada item são decompostas na tabela R1–R13. Não é uma regex para nomes de arquivos.

## Rótulos e expressão

| Rótulo | Bloco |
|---|---|
| A | Assertivas de entrada e limpeza do diagnóstico |
| I | Lista não regular: IMPOSSIVEL, cancela |
| V | Lista regular |
| O | Falha de abertura: ERRO, cancela |
| E | Inicialização do resultado e início da leitura |
| B | Linha vazia, ignora |
| P | Processamento de um nome não vazio |
| G | Agregação do resultado do item |
| L | Falha de leitura: ERRO |
| S | Assertiva de saída e retorno global |

Expressão regular: `A(I|V(O|E(B|PG)*(L|S)))`.

O estado global tem arrasto 1: um primeiro item pode substituir NADA por uma
cópia ou ERRO; os próximos dependem do valor anterior. A seleção inclui zero,
um e pelo menos dois itens, como pedido nos slides (arrasto + 1).

| Caminho | Teste |
|---|---|
| AI | R1 |
| AVES | Lista vazia, nos dois modos |
| AVEPGS | Cada coluna R2–R13 |
| AVEBPGPGS | Lista com linha vazia e dois arquivos, nos dois modos |
| AVEPGPGS | Erro no primeiro item, cópia no segundo; ERRO prevalece |

Dentro de processarArquivosExistentes, os três resultados da comparação são
mutuamente exclusivos: ANTERIOR, IGUAL, POSTERIOR. R3–R5 e R7–R9 exercitam cada
resultado em cada operação. As combinações de existência são exercitadas por
R2, R6 e R10–R13. O teste de destino inválido e a repetição exercitam I/O e
preservação de versão além das treze colunas.

Os caminhos O e L e algumas falhas de filesystem não são provocados pela
suíte: um arquivo regular ilegível depende de permissões/usuário; erros de
leitura e falhas posteriores à cópia exigem injeção de I/O ou corrida externa.
Não se afirma cobertura integral de caminhos ou decisões. O relatório gcov
mede linhas por módulo; scripts/verificar_cobertura.py verifica o limite de 80%.
