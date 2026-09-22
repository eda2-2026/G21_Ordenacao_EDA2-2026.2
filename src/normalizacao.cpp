#include "normalizacao.h"

#include <cctype>

std::string normalizarIsbn(const std::string &isbn) {
	std::string isbnNormalizado;
	isbnNormalizado.reserve(isbn.size());

	for (unsigned char caractere : isbn) {
		if (caractere == '-' || std::isspace(caractere)) {
			continue;
		}

		if (caractere == 'x') {
			isbnNormalizado += 'X';
		} else {
			isbnNormalizado += static_cast<char>(caractere);
		}
	}

	return isbnNormalizado;
}

std::string normalizarTitulo(const std::string &titulo) {
	std::string tituloNormalizado;
	tituloNormalizado.reserve(titulo.size());

	bool adicionarEspaco = false;

	for (unsigned char caractere : titulo) {
		if (std::isspace(caractere)) {
			adicionarEspaco = !tituloNormalizado.empty();
			continue;
		}

		if (adicionarEspaco) {
			tituloNormalizado += ' ';
			adicionarEspaco = false;
		}

		if (caractere <= 127) {
			tituloNormalizado += static_cast<char>(std::tolower(caractere));
		} else {
			tituloNormalizado += static_cast<char>(caractere);
		}
	}

	return tituloNormalizado;
}
