#ifndef INDICE_TITULOS_HASH_H
#define INDICE_TITULOS_HASH_H

#include <cstddef>
#include <iosfwd>
#include <list>
#include <string>
#include <vector>

struct EntradaTitulo {
	std::string tituloNormalizado;
	std::vector<std::string> isbns;
};

class IndiceTitulosHash {
public:
	explicit IndiceTitulosHash(std::size_t quantidadeBuckets = 101);

	bool adicionar(const std::string &titulo, const std::string &isbn);
	std::vector<std::string> buscar(const std::string &titulo) const;
	bool remover(const std::string &titulo, const std::string &isbn);
	void mostrarEstrutura(std::ostream &saida) const;
	std::size_t getQuantidadeBuckets() const;

private:
	std::size_t quantidadeBuckets;
	std::vector<std::list<EntradaTitulo>> buckets;

	std::size_t calcularIndice(
		const std::string &tituloNormalizado) const;
};

#endif
