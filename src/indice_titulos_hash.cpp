#include "indice_titulos_hash.h"

#include "funcao_hash.h"
#include "normalizacao.h"

#include <algorithm>
#include <ostream>
#include <stdexcept>

IndiceTitulosHash::IndiceTitulosHash(std::size_t quantidadeBuckets)
	: quantidadeBuckets(quantidadeBuckets), buckets(quantidadeBuckets) {
	if (quantidadeBuckets == 0) {
		throw std::invalid_argument("A quantidade de buckets deve ser maior que zero");
	}
}

bool IndiceTitulosHash::adicionar(const std::string &titulo, const std::string &isbn) {
	const std::string tituloNormalizado = normalizarTitulo(titulo);
	const std::string isbnNormalizado = normalizarIsbn(isbn);

	if (tituloNormalizado.empty() || isbnNormalizado.empty()) {
		return false;
	}

	auto &bucket = buckets[calcularIndice(tituloNormalizado)];

	for (EntradaTitulo &entrada : bucket) {
		if (entrada.tituloNormalizado != tituloNormalizado) {
			continue;
		}

		const auto isbnExistente = std::find(
			entrada.isbns.begin(), entrada.isbns.end(), isbnNormalizado);

		if (isbnExistente != entrada.isbns.end()) {
			return false;
		}

		entrada.isbns.push_back(isbnNormalizado);
		return true;
	}

	bucket.push_back({tituloNormalizado, {isbnNormalizado}});
	return true;
}

std::vector<std::string> IndiceTitulosHash::buscar(
	const std::string &titulo) const {
	const std::string tituloNormalizado = normalizarTitulo(titulo);

	if (tituloNormalizado.empty()) {
		return {};
	}

	const auto &bucket = buckets[calcularIndice(tituloNormalizado)];

	for (const EntradaTitulo &entrada : bucket) {
		if (entrada.tituloNormalizado == tituloNormalizado) {
			return entrada.isbns;
		}
	}

	return {};
}

bool IndiceTitulosHash::remover(const std::string &titulo, const std::string &isbn) {
	const std::string tituloNormalizado = normalizarTitulo(titulo);
	const std::string isbnNormalizado = normalizarIsbn(isbn);

	if (tituloNormalizado.empty() || isbnNormalizado.empty()) {
		return false;
	}

	auto &bucket = buckets[calcularIndice(tituloNormalizado)];

	for (auto entrada = bucket.begin(); entrada != bucket.end(); ++entrada) {
		if (entrada->tituloNormalizado != tituloNormalizado) {
			continue;
		}

		const auto isbnEncontrado = std::find(
			entrada->isbns.begin(), entrada->isbns.end(), isbnNormalizado);

		if (isbnEncontrado == entrada->isbns.end()) {
			return false;
		}

		entrada->isbns.erase(isbnEncontrado);

		if (entrada->isbns.empty()) {
			bucket.erase(entrada);
		}

		return true;
	}

	return false;
}

void IndiceTitulosHash::mostrarEstrutura(std::ostream &saida) const {
	bool possuiEntradas = false;

	for (std::size_t indice = 0; indice < buckets.size(); ++indice) {
		const auto &bucket = buckets[indice];

		if (bucket.empty()) {
			continue;
		}

		possuiEntradas = true;
		saida << "Bucket " << indice << ":\n";

		for (const EntradaTitulo &entrada : bucket) {
			saida << "  " << entrada.tituloNormalizado << " -> ";

			for (std::size_t posicao = 0; posicao < entrada.isbns.size();
				 ++posicao) {
				if (posicao > 0) {
					saida << ", ";
				}

				saida << entrada.isbns[posicao];
			}

			saida << '\n';
		}
	}

	if (!possuiEntradas) {
		saida << "Índice de títulos vazio.\n";
	}
}

std::size_t IndiceTitulosHash::getQuantidadeBuckets() const {
	return quantidadeBuckets;
}

std::size_t IndiceTitulosHash::calcularIndice(
	const std::string &tituloNormalizado) const {
	return calcularHash(tituloNormalizado) % quantidadeBuckets;
}
