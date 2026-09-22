#ifndef CATALOGO_H
#define CATALOGO_H

#include "livro.h"
#include "indice_titulos_hash.h"
#include "tabela_hash_extensivel_isbn.h"

#include <iosfwd>
#include <string>
#include <vector>
#include <optional>

class Catalogo {
    
private:
    TabelaHashExtensivelIsbn tabelaHashIsbn;
    IndiceTitulosHash indiceTitulos;

public:
    Catalogo() = default;

    bool cadastrar(const Livro &livro);
    std::optional<Livro> buscarPorIsbn(const std::string &isbn) const;
    std::vector<Livro> buscarPorTitulo(const std::string &titulo) const;
    bool atualizar(const Livro &livro);
    bool removerPorIsbn(const std::string &isbn);
    std::vector<Livro> listarTodos() const;  
    void visualizarHashIsbn(std::ostream &saida) const;
};

#endif
