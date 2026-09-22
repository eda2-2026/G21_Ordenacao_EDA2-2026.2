#include "livros_exemplo.h"

std::vector<Livro> criarLivrosExemplo() {
    return {
        Livro("9780000000002", "Estruturas de Dados", "Ana Souza",
              "Editora Academica", 2020),
        Livro("9780000000019", "Estruturas de Dados", "Bruno Lima",
              "Editora Tecnica", 2021),
        Livro("9780000000026", "Algoritmos em C++", "Carla Mendes",
              "Editora Codigo", 2022),
        Livro("9780000000033", "Banco de Dados", "Daniel Alves",
              "Editora Digital", 2023),
        Livro("9780000000040", "Redes de Computadores", "Elisa Rocha",
              "Editora Universitaria", 2024)
    };
}
