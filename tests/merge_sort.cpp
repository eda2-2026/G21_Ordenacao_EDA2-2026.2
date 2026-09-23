#include "ordenacao.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

namespace {
    void verificar(bool condicao, const char *mensagem) {
        if (!condicao) {
            throw std::runtime_error(mensagem);
        }
    }

    bool mesmoLivro(const Livro &a, const Livro &b) {
        return std::make_tuple(a.getIsbn(), a.getTitulo(), a.getAutor(), a.getEditora(),
                               a.getAnoPublicacao(), a.getQuantidadeVendidos()) ==
               std::make_tuple(b.getIsbn(), b.getTitulo(), b.getAutor(), b.getEditora(),
                               b.getAnoPublicacao(), b.getQuantidadeVendidos());
    }

    void verificarResultado(const std::vector<Livro> &entrada,
                            CriterioOrdenacao criterio, Direcao direcao) {
        auto esperado = entrada;
        std::stable_sort(esperado.begin(), esperado.end(), [=](const Livro &a, const Livro &b) {
            return compararLivros(a, b, criterio, direcao);
        });
        auto resultado = entrada;
        const auto metricas = mergeSort(resultado, criterio, direcao);
        verificar(resultado.size() == esperado.size(), "Quantidade de livros alterada");
        verificar(std::equal(resultado.begin(), resultado.end(), esperado.begin(), mesmoLivro),
                  "Ordem, estabilidade ou conteúdo diferente da referência");
        verificar(std::isfinite(metricas.tempoMicrossegundos) && metricas.tempoMicrossegundos >= 0,
                  "Tempo inválido");
        if (entrada.size() <= 1) {
            verificar(metricas.comparacoes == 0 && metricas.movimentacoes == 0 &&
                      metricas.tempoMicrossegundos == 0.0, "Caso-base deve ter métricas zeradas");
        } else {
            verificar(metricas.comparacoes > 0 && metricas.movimentacoes > 0,
                      "Ordenação deve contabilizar suas operações");
        }
    }

    void verificarContagem(std::initializer_list<int> anos,
                           std::uint64_t comparacoes, std::uint64_t movimentacoes) {
        std::vector<Livro> livros;
        for (int ano : anos) {
            livros.emplace_back(std::to_string(ano), "Título", "Autor", "Editora", ano, 0);
        }
        const auto metricas = mergeSort(livros, CriterioOrdenacao::AnoPublicacao, Direcao::Crescente);
        verificar(metricas.comparacoes == comparacoes, "Contagem de comparações incorreta");
        verificar(metricas.movimentacoes == movimentacoes, "Contagem de movimentações incorreta");
    }
}

int main() {
    const std::array<CriterioOrdenacao, 5> criterios = {
        CriterioOrdenacao::Titulo, CriterioOrdenacao::Autor,
        CriterioOrdenacao::AnoPublicacao, CriterioOrdenacao::Isbn,
        CriterioOrdenacao::QuantidadeVendidos
    };
    const std::array<Direcao, 2> direcoes = {Direcao::Crescente, Direcao::Decrescente};
    std::mt19937 gerador(2026);

    for (std::size_t tamanho : {0u, 1u, 2u, 3u, 5u, 8u, 17u, 64u, 127u}) {
        std::vector<Livro> entrada;
        for (std::size_t i = 0; i < tamanho; ++i) {
            // Registros repetidos nos campos comparados, identificados pela editora.
            const auto chave = i % 7;
            entrada.emplace_back(std::to_string(100 + chave),
                                 "Titulo " + std::to_string(chave % 3),
                                 "Autor " + std::to_string(chave % 2),
                                 "Registro " + std::to_string(i),
                                 2000 + static_cast<int>(chave % 4),
                                 static_cast<int>(chave % 3));
        }
        std::shuffle(entrada.begin(), entrada.end(), gerador);
        for (auto criterio : criterios) {
            for (auto direcao : direcoes) {
                verificarResultado(entrada, criterio, direcao);
                auto ordenado = entrada;
                std::stable_sort(ordenado.begin(), ordenado.end(), [=](const Livro &a, const Livro &b) {
                    return compararLivros(a, b, criterio, direcao);
                });
                verificarResultado(ordenado, criterio, direcao);
                std::reverse(ordenado.begin(), ordenado.end());
                verificarResultado(ordenado, criterio, direcao);
            }
        }
    }

    // Equivalência normalizada atravessando as duas metades, com identidade visível.
    const std::vector<Livro> equivalentes = {
        Livro("1-0x", " MESMO  titulo ", " ANA ", "Primeiro", 2020, 10),
        Livro("10X", "mesmo titulo", "ana", "Segundo", 2020, 10),
        Livro(" 10X ", "Mesmo Titulo", "Ana", "Terceiro", 2020, 10),
        Livro("10x", "mesmo titulo", "ana", "Quarto", 2020, 10)
    };
    for (auto criterio : criterios) {
        for (auto direcao : direcoes) {
            verificarResultado(equivalentes, criterio, direcao);
        }
    }

    verificarContagem({}, 0, 0);
    verificarContagem({1}, 0, 0);
    verificarContagem({1, 2}, 1, 4);
    verificarContagem({2, 1}, 1, 4);
    verificarContagem({1, 2, 3}, 2, 10);
    verificarContagem({3, 2, 1}, 3, 10);
    verificarContagem({1, 2, 3, 4}, 4, 16);
    verificarContagem({4, 3, 2, 1}, 4, 16);
    verificarContagem({2, 4, 1, 3}, 5, 16);
    verificarContagem({1, 1, 1, 1}, 4, 16);

    // Desempates internos não devem aumentar o número de chamadas contado.
    auto empatados = equivalentes;
    for (int repeticao = 0; repeticao < 2; ++repeticao) {
        const auto metricas = mergeSort(empatados, CriterioOrdenacao::QuantidadeVendidos,
                                       Direcao::Decrescente);
        verificar(metricas.comparacoes == 4 && metricas.movimentacoes == 16,
                  "Desempates foram contados separadamente ou métricas acumularam");
    }
    std::cout << "Merge Sort: todos os testes passaram.\n";
}
