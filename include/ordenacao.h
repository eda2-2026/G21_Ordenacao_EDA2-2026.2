#ifndef ORDENACAO_H
#define ORDENACAO_H

#include <cstdint>

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

#endif
