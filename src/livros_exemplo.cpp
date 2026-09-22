#include "livros_exemplo.h"

std::vector<Livro> criarLivrosExemplo() {
    return {
        Livro("9780000000002", "Estruturas de Dados", "Ana Souza",
              "Editora Academica", 2020, 150),
        Livro("9780000000019", "Estruturas de Dados", "Bruno Lima",
              "Editora Tecnica", 2021, 95),
        Livro("9780000000026", "Algoritmos em C++", "Carla Mendes",
              "Editora Codigo", 2022, 210),
        Livro("9780000000033", "Banco de Dados", "Daniel Alves",
              "Editora Digital", 2023, 120),
        Livro("9780000000040", "Redes de Computadores", "Elisa Rocha",
              "Editora Universitaria", 2024, 75)
    };
}
