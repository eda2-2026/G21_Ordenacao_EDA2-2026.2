#ifndef TABELA_HASH_EXTENSIVEL_ISBN_H
#define TABELA_HASH_EXTENSIVEL_ISBN_H

#include "livro.h"

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <vector>

struct BucketIsbn {
	explicit BucketIsbn(std::size_t profundidadeLocal, std::size_t capacidade);

	std::size_t profundidadeLocal;
	std::size_t capacidade;
	std::vector<Livro> livros;
};

class TabelaHashExtensivelIsbn {

public:
	explicit TabelaHashExtensivelIsbn(std::size_t capacidadeBucket = 4);

	std::size_t getProfundidadeGlobal() const;
	std::size_t getCapacidadeBucket() const;
	std::size_t getTamanhoDiretorio() const;

	bool inserir(const Livro &livro);
	std::optional<Livro> buscar(const std::string &isbn) const;
	bool atualizar(const Livro &livro);
	bool remover(const std::string &isbn);
	std::vector<Livro> listarTodos() const;
	void visualizarEstrutura(std::ostream &saida) const;

private:
	std::size_t profundidadeGlobal;
	std::size_t capacidadeBucket;
	std::vector<std::shared_ptr<BucketIsbn>> diretorio;

	static bool isbnValido(const std::string &isbnNormalizado);
	std::size_t calcularIndice(std::uint64_t hash) const;
	void duplicarDiretorio();
	void dividirBucket(const std::shared_ptr<BucketIsbn> &bucket);
};

#endif
