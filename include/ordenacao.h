#ifndef ORDENACAO_H
#define ORDENACAO_H

#include "livro.h"

#include <cstdint>
#include <vector>

enum class CriterioOrdenacao {
    Titulo,
    Autor,
    AnoPublicacao,
    Isbn,
    QuantidadeVendidos
};

// A direção afeta apenas o campo principal; os desempates são crescentes.
enum class Direcao {
    Crescente,
    Decrescente
};

// Convenções detalhadas na seção de regras de ordenação do README.
struct MetricasOrdenacao {
    // Uma chamada ao comparador, incluindo seus desempates.
    std::uint64_t comparacoes = 0;

    // Uma escrita de livro no vetor ou auxiliar durante a ordenação.
    std::uint64_t movimentacoes = 0;

    // Tempo apenas da ordenação; preparação e exibição ficam de fora.
    double tempoMicrossegundos = 0.0;
};

// Retorna true somente se primeiro deve vir antes de segundo.
// Desempates são crescentes e livros equivalentes retornam false.
// A contagem de métricas fica a cargo do algoritmo de ordenação.
bool compararLivros(const Livro &primeiro, const Livro &segundo,
                    CriterioOrdenacao criterio, Direcao direcao);

// Ordena o vetor recebido e retorna métricas exclusivas desta execução.
// A preparação do auxiliar fica fora do tempo e dos contadores.
MetricasOrdenacao mergeSort(std::vector<Livro> &livros,
                           CriterioOrdenacao criterio, Direcao direcao);

#endif
