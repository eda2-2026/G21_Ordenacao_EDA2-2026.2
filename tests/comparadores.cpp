#include "ordenacao.h"

#include <array>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace {
    const std::array<CriterioOrdenacao, 5> criterios = {
        CriterioOrdenacao::Titulo, CriterioOrdenacao::Autor,
        CriterioOrdenacao::AnoPublicacao, CriterioOrdenacao::Isbn,
        CriterioOrdenacao::QuantidadeVendidos
    };
    const std::array<Direcao, 2> direcoes = {
        Direcao::Crescente, Direcao::Decrescente
    };

    void verificar(bool condicao, const char *mensagem) {
        if (!condicao) {
            throw std::runtime_error(mensagem);
        }
    }

    void verificarOrdem(const Livro &antes, const Livro &depois,
                        CriterioOrdenacao criterio, Direcao direcao) {
        verificar(compararLivros(antes, depois, criterio, direcao),
                  "Ordem esperada não foi respeitada");
        verificar(!compararLivros(depois, antes, criterio, direcao),
                  "Comparação inversa deveria ser falsa");
    }
}

int main() {
    // Todos os campos principais variam; ISBN é textual: "10" precede "2".
    const Livro menor("10", "Algoritmos", "Ana", "A", 1990, 0);
    const Livro maior("2", "Zoologia", "Zelia", "B", 2025, 200);
    for (auto criterio : criterios) {
        verificarOrdem(menor, maior, criterio, Direcao::Crescente);
        verificarOrdem(maior, menor, criterio, Direcao::Decrescente);
    }

    // Campos não usados pelo comparador não quebram equivalência.
    const Livro equivalenteA(" 1-0x ", "  MESMO\t titulo ", " ANA  Souza ", "A", 2020, 10);
    const Livro equivalenteB("10X", "mesmo titulo", "ana souza", "B", 2020, 10);
    for (auto criterio : criterios) {
        for (auto direcao : direcoes) {
            verificar(!compararLivros(equivalenteA, equivalenteB, criterio, direcao) &&
                      !compararLivros(equivalenteB, equivalenteA, criterio, direcao),
                      "Normalização ou equivalência incorreta");
        }
    }

    const Livro base("20", "Beta", "Maria", "A", 2020, 10);
    for (auto direcao : direcoes) {
        // Título empatado: autor antes do ISBN; ano e quantidade são ignorados.
        verificarOrdem(Livro("99", " BETA ", "Ana", "B", 2030, 100), base,
                       CriterioOrdenacao::Titulo, direcao);
        // Autor empatado: título antes do ISBN.
        verificarOrdem(Livro("99", "Alfa", " MARIA ", "B", 2030, 100), base,
                       CriterioOrdenacao::Autor, direcao);
        // Ano e quantidade empatados: título antes de autor e ISBN.
        for (auto criterio : {CriterioOrdenacao::AnoPublicacao,
                              CriterioOrdenacao::QuantidadeVendidos}) {
            verificarOrdem(Livro("99", "Alfa", "Zelia", "B", 2020, 10), base,
                           criterio, direcao);
            verificarOrdem(Livro("99", "Beta", "Ana", "B", 2020, 10), base,
                           criterio, direcao);
        }
        // ISBN como último desempate de todos os outros critérios.
        for (auto criterio : criterios) {
            if (criterio != CriterioOrdenacao::Isbn) {
                verificarOrdem(Livro("10", "Beta", "Maria", "B", 2020, 10), base,
                               criterio, direcao);
            }
        }
        const Livro mesmoIsbn("2-0", "Outro", "Outro", "B", 1900, 999);
        verificar(!compararLivros(base, mesmoIsbn, CriterioOrdenacao::Isbn, direcao) &&
                  !compararLivros(mesmoIsbn, base, CriterioOrdenacao::Isbn, direcao),
                  "ISBN equivalente não deve ter desempate");
    }

    // Valores extremos de int não devem causar overflow na comparação numérica.
    const Livro extremoMenor("99", "Z", "Z", "A", std::numeric_limits<int>::min(), 0);
    const Livro extremoMaior("10", "A", "A", "A", std::numeric_limits<int>::max(),
                             std::numeric_limits<int>::max());
    for (auto criterio : {CriterioOrdenacao::AnoPublicacao,
                          CriterioOrdenacao::QuantidadeVendidos}) {
        verificarOrdem(extremoMenor, extremoMaior, criterio, Direcao::Crescente);
        verificarOrdem(extremoMaior, extremoMenor, criterio, Direcao::Decrescente);
    }

    // A normalização atual distingue letras acentuadas maiúsculas e minúsculas.
    verificarOrdem(Livro("10", "Árvore", "A", "A", 2020, 0),
                   Livro("10", "árvore", "A", "A", 2020, 0),
                   CriterioOrdenacao::Titulo, Direcao::Crescente);

    const std::vector<Livro> amostra = {
        menor, maior, base, equivalenteA, equivalenteB, extremoMenor, extremoMaior,
        Livro("99", "Alfa", "Zelia", "B", 2020, 10),
        Livro("10", "Beta", "Maria", "B", 2020, 10),
        Livro("99", "Beta", "Ana", "B", 2020, 10)
    };
    for (auto criterio : criterios) {
        for (auto direcao : direcoes) {
            const auto antes = [=](const Livro &a, const Livro &b) {
                return compararLivros(a, b, criterio, direcao);
            };
            for (const auto &a : amostra) {
                verificar(!antes(a, a), "Comparador não é irreflexivo");
                for (const auto &b : amostra) {
                    verificar(!(antes(a, b) && antes(b, a)), "Comparador não é assimétrico");
                    for (const auto &c : amostra) {
                        if (antes(a, b) && antes(b, c)) {
                            verificar(antes(a, c), "Ordem não é transitiva");
                        }
                        if (!antes(a, b) && !antes(b, a) && !antes(b, c) && !antes(c, b)) {
                            verificar(!antes(a, c) && !antes(c, a),
                                      "Equivalência não é transitiva");
                        }
                    }
                }
            }
        }
    }
    std::cout << "Comparadores: todos os testes passaram.\n";
}
