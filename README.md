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
estão declarados em `include/ordenacao.h`, e os comparadores com desempates estão
implementados em `src/ordenacao.cpp`, pela função `compararLivros()`. A função
`mergeSort()` implementa a ordenação recursiva estável e retorna suas métricas.
O catálogo integra a ordenação por `listarOrdenados()`, disponível na opção 9 do menu.

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

As estruturas de busca continuam responsáveis pelo armazenamento e pelas consultas. O método `listarTodos()` recupera os livros das tabelas hash e os retorna em um vetor independente. O método `listarOrdenados()` aplica o Merge Sort sobre esse vetor, de acordo com o critério e a direção escolhidos, e devolve um `ResultadoOrdenacao` contendo os livros ordenados e as métricas. O catálogo e seus índices permanecem inalterados.

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

#### Windows no terminal MinGW64 (MSYS2 ou Git Bash)

Use CMake, o compilador MinGW `g++` e Ninja disponíveis no `PATH` do terminal.
Confira as ferramentas:

```bash
cmake --version
g++ --version
ninja --version
```

Se estiver no **MSYS2 MinGW64** e faltar alguma dessas ferramentas, instale os
pacotes correspondentes:

```bash
pacman -S --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-ninja
```

O Git Bash não inclui `pacman`: nele, as ferramentas precisam estar instaladas
e acessíveis pelo `PATH` antes de continuar.

Entre na pasta onde clonou o projeto. Por exemplo, para o diretório local usado
no desenvolvimento:

```bash
cd /c/Users/thiag/Desktop/projetos/G21_Ordenacao_EDA2-2026.2
```

Configure, compile e execute:

```bash
cmake -S . -B build-mingw -G Ninja -DCMAKE_CXX_COMPILER=g++
cmake --build build-mingw
./build-mingw/biblioteca.exe
```

O primeiro comando configura a compilação; o segundo gera o executável; o
terceiro abre a aplicação. Após alterar o código, repita os dois últimos comandos.
A pasta `build-mingw` deve ser exclusiva dessa configuração; se ela já tiver
sido configurada com outro gerador ou compilador, escolha outra pasta nos três
comandos.

O Ninja é o gerador recomendado na [documentação do MSYS2 sobre CMake](https://www.msys2.org/docs/cmake/).
O compilador continua sendo o MinGW `g++`.

No menu, use **8** para carregar os livros de demonstração e **9** para ordenar.
Escolha um critério: **1** título, **2** autor, **3** ano, **4** ISBN ou **5**
quantidade de vendidos. Em seguida, escolha **1** crescente ou **2** decrescente.
A aplicação exibe todos os campos dos livros ordenados, o total, as comparações,
as movimentações e o tempo em microssegundos. **0** em qualquer etapa do submenu
volta ao menu principal. Entradas inválidas são rejeitadas e solicitadas novamente.

Por exemplo, a sequência **8 → 9 → 5 → 2** lista os livros mais vendidos primeiro.
A opção **6** continua listando os livros na ordem original recuperada do catálogo.
Catálogos vazios exibem uma mensagem e métricas zeradas; um único livro também
retorna métricas zeradas.

#### Linux ou macOS

Na raiz do projeto, configure, compile e execute:

```bash
cmake -S . -B build
cmake --build build
./build/biblioteca
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

## Testes dos comparadores

Os testes verificam os cinco critérios nas duas direções, normalização,
desempates crescentes, equivalência, limites numéricos e propriedades da ordem.
Eles validam os comparadores independentemente do algoritmo de ordenação.

Na raiz do projeto, com `g++` disponível, compile e execute:

```bash
g++ -Wall -Wextra -pedantic -std=c++17 -Iinclude tests/comparadores.cpp src/ordenacao.cpp src/livro.cpp src/normalizacao.cpp -o /tmp/g21-testes-comparadores
/tmp/g21-testes-comparadores
```

O comando acima usa `/tmp` no Linux. No Windows, escolha um caminho de saída
local com a extensão `.exe` e execute o arquivo correspondente.


## Merge Sort e métricas

`mergeSort()` recebe um `std::vector<Livro>&`, o critério e a direção, ordena o
próprio vetor recebido e devolve uma nova `MetricasOrdenacao`. Na integração
com o catálogo, `listarOrdenados()` passa a cópia obtida por `listarTodos()`.

```cpp
auto livros = catalogo.listarTodos();
const auto metricas = mergeSort(livros, CriterioOrdenacao::QuantidadeVendidos,
                               Direcao::Decrescente);
```

A implementação divide intervalos `[inicio, fim)` até restar no máximo um livro.
Um único vetor auxiliar é preparado antes da medição e reutilizado nas
intercalações. Quando dois livros são equivalentes, o da metade esquerda é
escolhido primeiro, preservando a estabilidade em ambas as direções.

Cada intercalação de `m` livros realiza `2 * m` movimentações: `m` escritas no
auxiliar e `m` no vetor de destino. Cada escolha entre duas metades ainda não
esgotadas conta uma comparação. Por exemplo, dois livros exigem uma comparação
e quatro movimentações; vetores vazios e unitários retornam métricas zeradas.
O cronômetro engloba apenas a chamada recursiva, conforme as convenções acima.

### Testes do Merge Sort

Os testes comparam o resultado com `std::stable_sort` (usado somente como
referência nos testes), verificando todos os campos dos livros. Cobrem vetores
vazios, unitários, ordenados, inversos, tamanhos ímpares, equivalências nas duas
direções e contagens conhecidas de comparações e movimentações.

```bash
g++ -Wall -Wextra -pedantic -std=c++17 -Iinclude tests/merge_sort.cpp src/ordenacao.cpp src/livro.cpp src/normalizacao.cpp -o /tmp/g21-testes-merge-sort
/tmp/g21-testes-merge-sort
```

Assim como nos testes dos comparadores, no Windows adapte o caminho de saída
para um executável local com extensão `.exe`.


## Testes de integração da ordenação

O teste do catálogo verifica os cinco critérios nas duas direções, os casos
vazio e unitário e a preservação da ordem original, dos campos e dos índices.
No Git Bash/MinGW64, compile e execute na raiz do projeto:

```bash
g++ -Wall -Wextra -pedantic -std=c++17 -Iinclude tests/catalogo_ordenacao.cpp src/catalogo.cpp src/funcao_hash.cpp src/indice_titulos_hash.cpp src/livro.cpp src/livros_exemplo.cpp src/normalizacao.cpp src/ordenacao.cpp src/tabela_hash_extensivel_isbn.cpp -o testes-catalogo.exe
./testes-catalogo.exe
```

O teste do menu requer Python 3 e o executável da aplicação já compilado.
Ele percorre os critérios e direções, verifica a ordem exibida e as métricas e
exercita entradas inválidas, cancelamento e fim de entrada. Usando a compilação
CMake descrita acima:

```bash
python tests/menu_ordenacao.py ./build-mingw/biblioteca.exe
```

Se compilou diretamente com `g++`, use `./biblioteca.exe` como argumento. No
Linux, use `python3` e o caminho do executável correspondente.
