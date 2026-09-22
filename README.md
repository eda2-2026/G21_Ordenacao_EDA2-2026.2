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