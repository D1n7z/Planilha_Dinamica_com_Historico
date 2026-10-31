# EP1 – Planilha Esparsa com Histórico de Alterações

Implementação em **C** de uma planilha esparsa de inteiros com suporte a desfazer operações. Somente as células com valor diferente de zero são armazenadas, o que economiza memória em planilhas grandes e quase vazias.

## Estruturas de dados

- **Listas ortogonais (multilistas):** cada célula não nula pertence simultaneamente a uma lista encadeada da sua **linha** e a uma lista encadeada da sua **coluna**, ambas ordenadas.
- **Lista de fileiras:** linhas e colunas existentes ficam em listas encadeadas ordenadas por índice (`fileira_t`). Uma fileira só existe enquanto tiver ao menos uma célula e é removida automaticamente quando fica vazia.
- **Pilha de histórico:** cada alteração efetiva empilha uma operação (`operacao_t`), permitindo desfazer em ordem inversa. A operação guarda o valor anterior da célula ou, no caso de transposição, a região transposta (`union` entre `valor_anterior` e `tamanho`).

| Estrutura | Função |
|---|---|
| `celula_t` | Célula com linha, coluna, valor e ponteiros para a próxima da linha e da coluna |
| `fileira_t` | Cabeça de uma linha ou coluna, com índice e primeira célula |
| `operacao_t` | Registro de uma alteração (definição/remoção ou transposição) |
| `pilha_t` / `elo_pilha_t` | Pilha do histórico |
| `planilha_t` | Planilha completa: listas de linhas e colunas, total de células e histórico |

## Funcionalidades

- Definir, atualizar e remover células (`definir_celula`, `remover_celula`)
- Consultar o valor de uma célula (`obter_valor`), com retorno 0 para células inexistentes
- Somar os valores de um intervalo retangular (`somar_intervalo`)
- Contar as células não nulas (`contar_nao_nulas`)
- Transpor um bloco quadrado da planilha (`transpor`)
- Desfazer a última alteração, inclusive transposições (`desfazer`)
- Exibir a planilha e o histórico (`exibir_planilha`, `exibir_historico`)
- Liberar toda a memória alocada (`liberar_tudo`)

Operações sem efeito (definir uma célula com o mesmo valor que ela já tem, definir 0 em uma célula inexistente, ou transpor um bloco que não muda) retornam `false` e **não** entram no histórico.

## Compilação e execução

```bash
gcc -Wall -Wextra -o ep1 ep1.c
./ep1 arquivo_entrada.txt arquivo_saida.txt
```

> Ajuste `ep1.c` para o nome real do seu arquivo-fonte.

Para verificar vazamentos de memória:

```bash
valgrind --leak-check=full ./ep1 entrada.txt saida.txt
```

## Formato da entrada

A primeira linha contém um inteiro `n`. Em seguida vêm os comandos, um por linha:

| Comando | Descrição |
|---|---|
| `DEF lin col valor` | Define o valor da célula (cria, atualiza ou remove se `valor` for 0) |
| `REM lin col` | Remove a célula |
| `GET lin col` | Imprime o valor da célula |
| `SOMA lin_ini lin_fim col_ini col_fim` | Imprime a soma dos valores no intervalo (inclusivo) |
| `CONT` | Imprime a quantidade de células não nulas |
| `TRANS lin col tam` | Transpõe o bloco quadrado de `tam × tam` com canto superior esquerdo em `(lin, col)` |
| `DESFAZER` | Desfaz a última alteração efetiva |
| `EXIBIR` | Imprime todas as células não nulas |
| `HIST` | Imprime o histórico de alterações, do topo para a base |

## Formato da saída

```
GET lin col valor
SOMA lin_ini lin_fim col_ini col_fim resultado
CONT quantidade
```

`EXIBIR` imprime `PLANILHA` seguido de uma linha `linha coluna valor` por célula, em ordem crescente de linha e, dentro de cada linha, de coluna. Se não houver células, imprime `PLANILHA VAZIA`.

`HIST` imprime `HISTORICO` seguido de uma linha por operação, do topo para a base: `linha coluna valor_anterior` para definições e remoções, ou `T linha coluna tamanho` para transposições. Se a pilha estiver vazia, imprime `HISTORICO VAZIO`. O mesmo texto é impresso se `DESFAZER` for chamado com o histórico vazio.

### Exemplo

Entrada:

```
7
DEF 1 2 5
DEF 3 4 7
SOMA 0 5 0 5
CONT
DESFAZER
EXIBIR
HIST
```

Saída:

```
SOMA 0 5 0 5 12
CONT 2
PLANILHA
1 2 5
HISTORICO
1 2 0
```

## Organização do código

O arquivo segue o template da disciplina: as structs e as assinaturas das funções obrigatórias foram mantidas. Funções auxiliares adicionadas:

- `auxiliar_definir_celula`: faz a criação, atualização ou remoção física da célula e das fileiras, sem mexer no histórico.
- `auxiliar_transpor`: executa a transposição. Como a transposição é a sua própria inversa, `desfazer` reutiliza a mesma função.
- `igual`: comparação de strings usada na leitura dos comandos.

## Complexidade

- **Busca de célula:** O(L + C + k), onde L e C são o número de linhas e colunas existentes e k é o tamanho da fileira percorrida.
- **Soma de intervalo:** percorre apenas as linhas e células dentro do intervalo.
- **Desfazer:** O(1) para empilhar/desempilhar, mais o custo da operação revertida.

## Autor

Eduardo Diniz da Silva