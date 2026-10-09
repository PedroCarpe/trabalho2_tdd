# Revisão dirigida do código

Referências lidas: enunciado TP2_trab_2.pdf; tabela da página 26 e descrições
23–25 da aula de caixa fechada; aula de caixa aberta; Aula_Revisoes_Inspecoes.pdf.
A revisão segue os pontos de vista do testador e mantenedor (páginas 14–16).
É uma revisão do código por este agente, não uma inspeção formal por equipe.

| Critério | Constatação e tratamento |
|---|---|
| Uma coluna, um teste | R1–R13 têm testes e tags próprias |
| Operação correta | BACKUP copia HD→pendrive; RESTAURAR copia pendrive→HD |
| Datas iguais | Nenhuma cópia; os testes usam conteúdos diferentes |
| Conflito de versões | Erro antes de copiar; R7 verifica conteúdos e datas |
| Exclusão | Nenhuma coluna marca Excluir; nenhuma exclusão de dados implementada |
| Lista completa | Retorno antecipado descoberto e corrigido por testes |
| Estado entre iterações | ERRO prevalece sem impedir processamento dos demais |
| Repetição de execução | Data da origem é preservada; segunda chamada retorna NADA |
| Diagnóstico | Parâmetro opcional, limpo na entrada; identifica item com erro |
| I/O inválido | Destino diretório gera ERRO sem alterar origem |
| Recursos | Streams automáticos; fixture RAII limpa temporários |
| Estilo | cpplint configurado para C++17/Catch2; sem avisos |
| Build | Avisos do compilador, cppcheck e Valgrind causam verificações reais |
| Histórico | RED real registrado; comportamento já GREEN explicitado |
| Documentação | Contratos Javadoc, Doxyfile, HTML e leiame.txt |

Limites: não há concorrência/rollback, criação automática de subdiretórios ou
proteção contra mudanças externas entre consulta de data e cópia. Caminhos e
operação são precondições do cliente. Exceções de consulta do filesystem
podem propagar; falhas de copy_file e atualização de data são convertidas em
ERRO. A tabela não define essas extensões.

