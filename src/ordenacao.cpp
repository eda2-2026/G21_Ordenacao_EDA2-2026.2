#include "ordenacao.h"

#include "normalizacao.h"

#include <chrono>
#include <cstddef>
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

    // Intervalos semiabertos: [inicio, meio) e [meio, fim).
    void intercalar(std::vector<Livro> &livros, std::vector<Livro> &auxiliar,
                    std::size_t inicio, std::size_t meio, std::size_t fim,
                    CriterioOrdenacao criterio, Direcao direcao,
                    MetricasOrdenacao &metricas) {
        for (std::size_t i = inicio; i < fim; ++i) {
            auxiliar[i] = livros[i];
            ++metricas.movimentacoes;
        }

        std::size_t esquerda = inicio;
        std::size_t direita = meio;
        for (std::size_t destino = inicio; destino < fim; ++destino) {
            if (esquerda == meio) {
                livros[destino] = auxiliar[direita++];
            } else if (direita == fim) {
                livros[destino] = auxiliar[esquerda++];
            } else {
                ++metricas.comparacoes;
                // Só escolhe a direita se ela vier estritamente antes.
                // Em equivalência, a esquerda preserva a estabilidade.
                if (compararLivros(auxiliar[direita], auxiliar[esquerda], criterio, direcao)) {
                    livros[destino] = auxiliar[direita++];
                } else {
                    livros[destino] = auxiliar[esquerda++];
                }
            }
            ++metricas.movimentacoes;
        }
    }

    void mergeSortRecursivo(std::vector<Livro> &livros, std::vector<Livro> &auxiliar,
                            std::size_t inicio, std::size_t fim,
                            CriterioOrdenacao criterio, Direcao direcao,
                            MetricasOrdenacao &metricas) {
        if (fim - inicio <= 1) {
            return;
        }

        const std::size_t meio = inicio + (fim - inicio) / 2;
        mergeSortRecursivo(livros, auxiliar, inicio, meio, criterio, direcao, metricas);
        mergeSortRecursivo(livros, auxiliar, meio, fim, criterio, direcao, metricas);
        intercalar(livros, auxiliar, inicio, meio, fim, criterio, direcao, metricas);
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

MetricasOrdenacao mergeSort(std::vector<Livro> &livros,
                           CriterioOrdenacao criterio, Direcao direcao) {
    MetricasOrdenacao metricas;
    if (livros.size() <= 1) {
        return metricas;
    }

    // Livro não tem construtor padrão. Esta cópia prepara o único auxiliar,
    // reutilizado em toda a recursão, sem entrar nas métricas da ordenação.
    std::vector<Livro> auxiliar = livros;

    const auto inicio = std::chrono::steady_clock::now();
    mergeSortRecursivo(livros, auxiliar, 0, livros.size(), criterio, direcao, metricas);
    const auto fim = std::chrono::steady_clock::now();

    metricas.tempoMicrossegundos =
        std::chrono::duration<double, std::micro>(fim - inicio).count();
    return metricas;
}
