# Sistema de Biblioteca com Merge Sort

## Alunos

| Matrícula | Aluno |
| -- | -- |
| 211041105 | Bruna de Lima Santos |
| 232014333 | Thiago Gomes Pereira de Abreu |

## Sobre

Projeto desenvolvido para o Trabalho 2 da disciplina de Estruturas de Dados 2, com base no sistema de biblioteca em C++ implementado no Trabalho 1. Nesta etapa, o objetivo é aplicar o algoritmo Merge Sort para ordenar os livros de acordo com diferentes atributos.

### Problema

Os livros do catálogo possuem informações de tipos diferentes, como título, autor, ISBN, ano de publicação e quantidade de exemplares vendidos. O sistema organiza esses registros de acordo com o atributo e a direção escolhidos pelo usuário.

### Solução

O usuário escolhe o campo usado como critério e informa se deseja a ordenação crescente ou decrescente. A partir dessas opções, o sistema retorna uma listagem ordenada dos livros, sem modificar os índices internos ou a disposição original dos registros.

Além da coleção ordenada, o sistema exibe métricas da execução, como a quantidade de comparações, a quantidade de movimentações e o tempo de execução do algoritmo.

### Algoritmo

A ordenação é realizada com o **Merge Sort**, algoritmo baseado na estratégia de **divisão e conquista**. A coleção é dividida recursivamente em duas partes até que cada intervalo possua, no máximo, um elemento. Depois, as partes são intercaladas em ordem.

A mesma implementação é utilizada para ordenar todos os campos. Um comparador determina quando um livro deve aparecer antes de outro, permitindo ordenar por:

- **Título:** ordem alfabética;
- **Autor:** ordem alfabética;
- **Ano de publicação:** ordem numérica;
- **ISBN:** ordem textual do identificador normalizado;
- **Quantidade de vendidos:** ordem numérica.

Em caso de empate no critério principal, são utilizados critérios secundários para produzir um resultado determinístico. Quando os livros são equivalentes em todos os critérios considerados, a estabilidade do algoritmo preserva sua ordem relativa original.

### Regras de ordenação, desempate e métricas

As regras abaixo definem o contrato para a implementação da ordenação. Os tipos
estão declarados em `include/ordenacao.h`; os comparadores, o Merge Sort e a
integração com o menu ainda serão implementados.

#### Critérios e normalização

| Critério (`CriterioOrdenacao`) | Comparação do campo principal | Desempates, nesta ordem |
| -- | -- | -- |
| `Titulo` | Textual, após normalização | Autor → ISBN |
| `Autor` | Textual, após normalização | Título → ISBN |
| `AnoPublicacao` | Numérica, pelo ano | Título → Autor → ISBN |
| `Isbn` | Textual, pelo ISBN normalizado | Nenhum |
| `QuantidadeVendidos` | Numérica, pela quantidade de exemplares vendidos | Título → Autor → ISBN |

- Título e autor seguem as regras de `normalizarTitulo()`: remover espaços nas
  extremidades, reduzir sequências de espaços a um e converter letras ASCII
  para minúsculas. A comparação textual usa a ordem lexicográfica de
  `std::string`, sem alterar o texto armazenado ou exibido.
- A normalização atual preserva os bytes de caracteres não ASCII. Portanto,
  acentos não são removidos, `Á` e `á` não são equivalentes e não há garantia de
  ordem alfabética segundo as regras do português.
- O ISBN segue `normalizarIsbn()`: remover hífens e espaços e converter `x` para
  `X`. O identificador é comparado como texto, sem conversão para número.
- As mesmas regras de normalização valem para os campos usados nos desempates.
- A editora não participa da comparação.

#### Direção, desempates e estabilidade

`Direcao::Crescente` coloca os menores valores do campo principal primeiro;
`Direcao::Decrescente` coloca os maiores primeiro. A direção afeta somente o
campo principal: **todos os desempates permanecem em ordem crescente**.
Por exemplo, na ordenação decrescente por quantidade, livros com a mesma
quantidade são ordenados por título crescente, depois autor crescente e ISBN
crescente.

O comparador indica se um livro vem estritamente antes de outro. Quando todos
os campos considerados forem equivalentes, a resposta deve ser falsa nos dois
sentidos. Na intercalação, esse empate deve favorecer a metade esquerda,
preservando a ordem relativa do vetor recebido nas duas direções. A ordem
decrescente deve ser aplicada pelo comparador, sem inverter o resultado ao final.

A ordenação deve atuar sobre a cópia retornada por `listarTodos()`, preservando o
catálogo e seus índices. A ordem original relevante para a estabilidade é a do
vetor recebido, que não necessariamente corresponde à ordem de cadastro.
Como o ISBN é único no catálogo e encerra os desempates, o teste de estabilidade
deve usar um vetor de teste com registros equivalentes nos campos comparados e
identificáveis por um campo não comparado, como a editora.

#### Convenções das métricas

Cada execução começa com uma nova `MetricasOrdenacao`, com todos os campos em zero.

| Campo | Tipo | Regra |
| -- | -- | -- |
| `comparacoes` | `std::uint64_t` | Uma unidade por chamada ao comparador entre dois livros durante a ordenação. As verificações dos campos de desempate fazem parte da mesma chamada. |
| `movimentacoes` | `std::uint64_t` | Uma unidade por escrita de um livro em uma posição do vetor auxiliar ou do vetor ordenado durante a ordenação, seja por cópia ou movimento. |
| `tempoMicrossegundos` | `double` | Tempo decorrido da ordenação em microssegundos, medido com `std::chrono::steady_clock`. |

Comparações de índices, condições de laços e verificações do caso-base não
incrementam `comparacoes`. Leituras, alterações de índices e alocação de memória
não incrementam `movimentacoes`. Copiar um livro para o auxiliar e depois de
volta para o vetor conta como duas movimentações.

A obtenção do vetor por `listarTodos()` e a alocação e inicialização do auxiliar
ocorrem antes do cronômetro e não entram nos contadores. O intervalo medido
inclui as chamadas recursivas, intercalações, comparações (incluindo normalização
e desempates realizados pelo comparador) e atualizações dos contadores.
Entrada do usuário, exibição dos resultados e verificações posteriores ficam
fora desse intervalo.

Vetores vazios e unitários retornam sem executar a ordenação, com todas as
métricas em zero. O tempo é uma medida observada e pode variar entre execuções;
a unidade em microssegundos não implica precisão de um microssegundo.

### Complexidade

O Merge Sort sempre divide a coleção ao meio e realiza `log n` níveis de recursão. Em cada nível, a intercalação percorre os
`n` elementos. Por isso, seu tempo de execução é `O(n log n)` no melhor, no médio e no pior caso.

O vetor auxiliar usado durante a intercalação ocupa `O(n)` de memória. A pilha de chamadas recursivas ocupa `O(log n)`.

### Código-base

O projeto reutiliza o sistema de biblioteca desenvolvido no Trabalho 1, que já continha:

- Cadastro, consulta, atualização e remoção de livros;
- Índice principal por ISBN com hashing extensível;
- Índice secundário por título com encadeamento separado;
- Cormalização de ISBN e título;
- Menu interativo.

As estruturas de busca continuam responsáveis pelo armazenamento e pelas consultas. O método `listarTodos()` recupera os livros das tabelas hash e os retorna em um vetor independente. O método `listarOrdenados()` aplica o algoritmo do Merge Sort sobre esse vetor, de acordo com o critério e a direção escolhidos, e devolve a lista de livros ordenada. 

## Screenshots

## [Clique aqui para assistir à apresentação](#)

## Instalação

### Pré-requisitos

Antes de começar, certifique-se de ter:

- Git;
- CMake 3.16 ou superior e um compilador compatível com C++17; ou
- GNU Make e `g++` compatível com C++17.

Após isso, clone o repositório e acesse a pasta do projeto:

```bash
git clone https://github.com/eda2-2026/G21_Ordenacao_EDA2-2026.2.git
cd G21_Ordenacao_EDA2-2026.2
```

## Execução

### Com CMake

Na raiz do projeto, compile com:

```bash
cmake -S . -B build
cmake --build build
```

No Linux ou macOS, execute:

```bash
./build/biblioteca
```

No Windows com Ninja ou MinGW, execute:

```bash
./build/biblioteca.exe
```

### Com GNU Make

Compile e execute com:

```bash
make
make run
```

Para remover o executável gerado:

```bash
make clean
```