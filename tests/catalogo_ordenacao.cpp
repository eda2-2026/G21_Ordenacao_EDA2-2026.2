#include "catalogo.h"
#include "livros_exemplo.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <tuple>

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

    bool mesmosLivros(const std::vector<Livro> &a, const std::vector<Livro> &b) {
        return a.size() == b.size() && std::equal(a.begin(), a.end(), b.begin(), mesmoLivro);
    }

    std::string estrutura(const Catalogo &catalogo) {
        std::ostringstream saida;
        catalogo.visualizarHashIsbn(saida);
        return saida.str();
    }

    void verificarMetricasZeradas(const MetricasOrdenacao &metricas) {
        verificar(metricas.comparacoes == 0 && metricas.movimentacoes == 0 &&
                  metricas.tempoMicrossegundos == 0, "Caso-base deve ter métricas zeradas");
    }
}

int main() {
    Catalogo catalogo;
    auto vazio = catalogo.listarOrdenados(CriterioOrdenacao::Titulo, Direcao::Crescente);
    verificar(vazio.livros.empty(), "Catálogo vazio deve retornar lista vazia");
    verificarMetricasZeradas(vazio.metricas);

    const auto exemplos = criarLivrosExemplo();
    verificar(catalogo.cadastrar(exemplos.front()), "Falha no cadastro inicial");
    const auto unitario = catalogo.listarOrdenados(CriterioOrdenacao::Titulo, Direcao::Decrescente);
    verificar(unitario.livros.size() == 1 && mesmoLivro(unitario.livros.front(), exemplos.front()),
              "Livro unitário alterado");
    verificarMetricasZeradas(unitario.metricas);
    for (std::size_t i = 1; i < exemplos.size(); ++i) {
        verificar(catalogo.cadastrar(exemplos[i]), "Falha no cadastro de demonstração");
    }

    const auto original = catalogo.listarTodos();
    const auto hashOriginal = estrutura(catalogo);
    for (auto criterio : {CriterioOrdenacao::Titulo, CriterioOrdenacao::Autor,
                          CriterioOrdenacao::AnoPublicacao, CriterioOrdenacao::Isbn,
                          CriterioOrdenacao::QuantidadeVendidos}) {
        for (auto direcao : {Direcao::Crescente, Direcao::Decrescente}) {
            auto esperado = original;
            std::stable_sort(esperado.begin(), esperado.end(), [=](const Livro &a, const Livro &b) {
                return compararLivros(a, b, criterio, direcao);
            });
            auto resultado = catalogo.listarOrdenados(criterio, direcao);
            verificar(mesmosLivros(resultado.livros, esperado), "Resultado ordenado incorreto");
            verificar(resultado.metricas.comparacoes > 0 && resultado.metricas.movimentacoes > 0 &&
                      resultado.metricas.tempoMicrossegundos >= 0, "Métricas ausentes ou inválidas");
            verificar(mesmosLivros(original, catalogo.listarTodos()), "Ordem original alterada");
            verificar(hashOriginal == estrutura(catalogo), "Estrutura hash alterada");

            // Alterar a cópia não pode modificar o livro nem os índices do catálogo.
            resultado.livros.front().setTitulo("Título alterado somente na cópia");
            resultado.livros.front().setQuantidadeVendidos(999);
            for (const auto &livro : original) {
                const auto encontrado = catalogo.buscarPorIsbn(livro.getIsbn());
                verificar(encontrado && mesmoLivro(*encontrado, livro), "Busca por ISBN alterada");
                std::vector<Livro> porTitulo;
                for (const auto &exemplo : exemplos) {
                    if (exemplo.getTitulo() == livro.getTitulo()) {
                        porTitulo.push_back(exemplo);
                    }
                }
                verificar(mesmosLivros(porTitulo, catalogo.buscarPorTitulo(livro.getTitulo())),
                          "Índice de títulos alterado");
            }
        }
    }
    std::cout << "Catálogo ordenado: todos os testes passaram.\n";
}
