# Sistema de Biblioteca com Hashing

Trabalho acadêmico da disciplina de Estruturas de Dados. O projeto implementa um sistema simples de biblioteca em C++ para demonstrar o uso de algoritmos de busca e tabelas hash.

Os livros ficam armazenados em memória durante a execução. O ISBN é usado como chave principal e o título como chave secundária, permitindo consultar um livro de duas formas diferentes.

## Sumário

- [Sobre](#sobre)
- [Funcionalidades](#funcionalidades)
- [Algoritmos e estruturas de dados](#algoritmos-e-estruturas-de-dados)
- [Como o sistema funciona](#como-o-sistema-funciona)
- [Complexidade das operações](#complexidade-das-operações)
- [Estrutura do projeto](#estrutura-do-projeto)
- [Compilação e execução](#compilação-e-execução)
- [Como usar](#como-usar)

## Sobre

### Apresentação do trabalho: [assistir ao vídeo](https://youtu.be/aIuzGhLyyEw)

### Desenvolvedores

<div align="center">
  <table>
    <tr>
      <td align="center">
        <a href="https://github.com/libruna">
          <img src="https://avatars.githubusercontent.com/u/83987201?v=4" width="100px;" alt="Bruna Lima"/><br>
          <sub><b>Bruna Lima</b></sub>
        </a>
      </td>
      <td align="center">
        <a href="https://github.com/">
          <img src="https://avatars.githubusercontent.com/u/98625860?v=4" width="100px;" alt="Thiago Gomes"/><br>
          <sub><b>Thiago Gomes</b></sub>
        </a>
      </td>
    </tr>
  </table>
</div>

## Funcionalidades

- Cadastrar um livro com ISBN, título, autor, editora e ano de publicação;
- buscar um livro pelo ISBN;
- buscar um ou mais livros pelo título;
- atualizar os dados de um livro, mantendo o ISBN como identificador;
- remover um livro pelo ISBN;
- listar todos os livros cadastrados;
- visualizar o diretório e os buckets do hashing extensível por ISBN;
- carregar livros de demonstração que provocam uma divisão de bucket.

## Algoritmos e estruturas de dados

O material da disciplina apresenta a busca como a localização de um registro por meio de uma chave. Também diferencia a chave primária, que não pode se repetir, da chave secundária, que pode possuir valores repetidos. Essa separação foi aplicada da seguinte forma:

- **ISBN:** chave primária, pois identifica um único livro no sistema;
- **título:** chave secundária, pois livros diferentes podem possuir o mesmo título.

### Função hash

Uma função hash transforma uma chave em um valor usado para encontrar seu bucket. Ela deve ser determinística: a mesma chave sempre precisa gerar o mesmo resultado.

O projeto usa o algoritmo **FNV-1a de 64 bits** para calcular o hash das strings. Essa função é usada tanto para o ISBN quanto para o título. O hashing não impede colisões, portanto cada índice possui uma estratégia para tratá-las.

Antes do cálculo do hash, as chaves são normalizadas:

- no ISBN, espaços e hífens são removidos e `x` é convertido para `X`;
- no título, espaços repetidos são reduzidos e letras ASCII maiúsculas são convertidas para minúsculas.

### Índice principal por ISBN: hashing extensível

O ISBN é armazenado em uma tabela de **hashing extensível**, um tipo de hashing dinâmico estudado no material da disciplina. O espaço de endereçamento pode crescer quando novos elementos são inseridos.

A implementação possui:

- um diretório com referências para os buckets;
- profundidade global, que indica quantos bits do hash formam o índice do diretório;
- profundidade local para cada bucket;
- capacidade padrão de quatro livros por bucket;
- divisão do bucket quando não há mais espaço;
- duplicação do diretório antes da divisão quando um bucket cheio possui profundidade local igual à global.

Para localizar um ISBN, o sistema normaliza a entrada, calcula seu hash e usa os bits menos significativos correspondentes à profundidade global para acessar diretamente uma posição do diretório. Depois, compara o ISBN apenas com os livros do bucket encontrado. Assim, não é necessário percorrer todos os livros.

A opção 7 do menu mostra a profundidade global, as entradas do diretório, a profundidade local e o conteúdo de cada bucket. Essa visualização ajuda a observar as divisões durante os cadastros.

### Índice secundário por título: hashing aberto

O título utiliza uma tabela hash estática com **encadeamento separado**, chamado de hashing aberto no material da disciplina. O índice possui 101 buckets e cada bucket contém uma lista de entradas.

O hash do título normalizado é calculado e o resto da divisão por 101 determina o bucket. Quando títulos diferentes caem na mesma posição, as entradas permanecem na lista daquele bucket. Além disso, cada título guarda uma lista de ISBNs, pois pode existir mais de um livro com o mesmo título.

Depois de encontrar os ISBNs no índice de títulos, o sistema consulta a tabela principal para recuperar os dados completos dos livros.

### Por que escolhemos hashing

A busca sequencial teria custo linear, pois poderia precisar examinar todos os livros. A busca binária reduziria o custo da consulta, mas exigiria que os dados permanecessem ordenados, aumentando o trabalho de inserção e remoção.

Como as consultas deste projeto são "exatas" usando ISBN ou título, o hashing é adequado: oferece acesso próximo de constante no caso médio e permite atualizar o catálogo sem manter todos os registros ordenados.

## Como o sistema funciona

A classe `Catalogo` coordena as duas tabelas hash:

1. Ao cadastrar um livro, a tabela principal e o índice secundário são atualizados. Se uma das inserções falhar, a outra alteração é desfeita.
2. Na busca por ISBN, o livro é recuperado diretamente no hashing extensível.
3. Na busca por título, o índice secundário retorna os ISBNs relacionados e a tabela principal recupera os livros.
4. Ao alterar um título, o ISBN é removido da entrada antiga e adicionado à nova entrada do índice de títulos.
5. Ao remover um livro, suas informações são retiradas das duas estruturas.

## Complexidade das operações

As complexidades abaixo representam o comportamento esperado com uma função hash que distribui bem as chaves:

| Operação           | Custo médio esperado       | Observação                                                                                                    |
| ------------------ | -------------------------- | ------------------------------------------------------------------------------------------------------------- |
| Buscar por ISBN    | `O(1)`                     | Acessa o diretório e procura apenas no bucket calculado.                                                      |
| Cadastrar livro    | `O(1 + k)` amortizado      | Uma inserção pode percorrer ISBNs do mesmo título ou dividir um bucket.                                       |
| Atualizar por ISBN | `O(1)` ou `O(1 + k1 + k2)` | O primeiro caso ocorre quando o título não muda; `k1` e `k2` representam os livros nos títulos antigo e novo. |
| Remover por ISBN   | `O(1 + k)`                 | `k` é a quantidade de ISBNs associados ao título removido.                                                    |
| Buscar por título  | `O(1 + k)`                 | `k` é a quantidade de livros com o título encontrado.                                                         |
| Listar todos       | `O(D + n)`                 | São visitadas as `D` entradas do diretório e os `n` livros.                                                   |

Em um caso ruim, com muitas colisões no mesmo bucket, as buscas podem se aproximar de `O(n)`. Portanto, hashing oferece `O(1)` esperado, mas não garante esse custo para qualquer conjunto de dados.

## Estrutura do projeto

```text
.
├── include/
│   ├── catalogo.h
│   ├── funcao_hash.h
│   ├── indice_titulos_hash.h
│   ├── livro.h
│   ├── livros_exemplo.h
│   ├── menu.h
│   ├── normalizacao.h
│   └── tabela_hash_extensivel_isbn.h
├── src/
│   ├── catalogo.cpp
│   ├── funcao_hash.cpp
│   ├── indice_titulos_hash.cpp
│   ├── livro.cpp
│   ├── livros_exemplo.cpp
│   ├── main.cpp
│   ├── menu.cpp
│   ├── normalizacao.cpp
│   └── tabela_hash_extensivel_isbn.cpp
├── CMakeLists.txt
├── Makefile
└── README.md
```

- `include/`: declarações das classes e funções;
- `src/`: implementação da aplicação;
- `CMakeLists.txt`: configuração de compilação com CMake;
- `Makefile`: alternativa de compilação direta com GNU Make.

## Compilação e execução

### Pré-requisitos

Escolha uma das formas de compilação:

- CMake 3.16 ou superior e um compilador compatível com C++17; ou
- GNU Make e `g++` compatível com C++17.

O Ninja é recomendado ao usar CMake no Windows com MSYS2.

### Com CMake

Na raiz do projeto, execute:

```bash
cmake -S . -B build
cmake --build build
```

No Linux ou macOS:

```bash
./build/biblioteca
```

No Windows com Ninja ou MinGW:

```bash
./build/biblioteca.exe
```

Quando o CMake usa o gerador do Visual Studio, o executável pode ficar em `build/Debug/biblioteca.exe`.

### Windows com MSYS2 UCRT64

Abra o terminal **MSYS2 UCRT64** e instale as ferramentas, caso ainda não estejam disponíveis:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

Depois, compile usando o Ninja:

```bash
cmake -S . -B build -G Ninja
cmake --build build
./build/biblioteca.exe
```

### Com GNU Make

Se `g++` e `make` estiverem instalados:

```bash
make
make run
```

Para remover o executável gerado pelo Makefile:

```bash
make clean
```

## Como usar

Ao iniciar a aplicação, o seguinte menu é exibido:

```text
[1] Cadastrar livro
[2] Buscar por ISBN
[3] Buscar por título
[4] Atualizar livro
[5] Remover livro
[6] Listar todos os livros
[7] Visualizar hashing por ISBN
[8] Carregar livros de demonstração
[0] Sair
```

Digite o número da operação e informe os campos solicitados. A busca por título é exata depois da normalização. Por exemplo, espaços repetidos e diferenças entre letras ASCII maiúsculas e minúsculas não alteram o resultado.

### Demonstração da divisão de bucket

Com o catálogo vazio, selecione a opção 8 para cadastrar cinco livros preparados para chegar inicialmente ao mesmo bucket. Como cada bucket aceita quatro livros, o quinto cadastro provoca a duplicação do diretório e a divisão do bucket.

| ISBN | Título |
| --- | --- |
| `9780000000002` | Estruturas de Dados |
| `9780000000019` | Estruturas de Dados |
| `9780000000026` | Algoritmos em C++ |
| `9780000000033` | Banco de Dados |
| `9780000000040` | Redes de Computadores |

Em seguida, selecione a opção 7. A visualização deverá mostrar profundidade global 2 e quatro entradas no diretório. A opção 8 também inclui dois livros com o título `Estruturas de Dados`, permitindo demonstrar uma busca por título que retorna mais de um resultado.
