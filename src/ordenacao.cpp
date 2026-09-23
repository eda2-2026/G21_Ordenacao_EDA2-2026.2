#include "ordenacao.h"

#include "normalizacao.h"

#include <stdexcept>

namespace {
    int compararNumeros(int primeiro, int segundo) {
        // Evita subtração dos valores, que poderia causar overflow.
        return (primeiro > segundo) - (primeiro < segundo);
    }

    int compararCampo(const Livro &primeiro, const Livro &segundo,
                      CriterioOrdenacao criterio) {
        switch (criterio) {
            case CriterioOrdenacao::Titulo:
                return normalizarTitulo(primeiro.getTitulo()).compare(
                    normalizarTitulo(segundo.getTitulo()));
            case CriterioOrdenacao::Autor:
                return normalizarTitulo(primeiro.getAutor()).compare(
                    normalizarTitulo(segundo.getAutor()));
            case CriterioOrdenacao::AnoPublicacao:
                return compararNumeros(primeiro.getAnoPublicacao(),
                                       segundo.getAnoPublicacao());
            case CriterioOrdenacao::Isbn:
                return normalizarIsbn(primeiro.getIsbn()).compare(
                    normalizarIsbn(segundo.getIsbn()));
            case CriterioOrdenacao::QuantidadeVendidos:
                return compararNumeros(primeiro.getQuantidadeVendidos(),
                                       segundo.getQuantidadeVendidos());
        }

        throw std::invalid_argument("Critério de ordenação inválido");
    }
}

bool compararLivros(const Livro &primeiro, const Livro &segundo,
                    CriterioOrdenacao criterio, Direcao direcao) {
    if (direcao != Direcao::Crescente && direcao != Direcao::Decrescente) {
        throw std::invalid_argument("Direção de ordenação inválida");
    }

    const int resultado = compararCampo(primeiro, segundo, criterio);
    if (resultado != 0) {
        return direcao == Direcao::Crescente ? resultado < 0 : resultado > 0;
    }

    // O ISBN é o identificador final: sua equivalência não tem desempates.
    if (criterio == CriterioOrdenacao::Isbn) {
        return false;
    }

    // Ignorar o campo principal produz a sequência prevista para cada critério:
    // título -> autor, ISBN; autor -> título, ISBN; ano/quantidade -> título, autor, ISBN.
    const CriterioOrdenacao desempates[] = {
        CriterioOrdenacao::Titulo,
        CriterioOrdenacao::Autor,
        CriterioOrdenacao::Isbn
    };

    for (CriterioOrdenacao desempate : desempates) {
        if (desempate == criterio) {
            continue;
        }

        const int resultadoDesempate = compararCampo(primeiro, segundo, desempate);
        if (resultadoDesempate != 0) {
            return resultadoDesempate < 0;
        }
    }

    return false;
}
