#include "normalizacao.h"

#include <cctype>
#include <string_view>

namespace {
	char removerAcento(std::string_view caractere) {
		if (caractere == "á" || caractere == "à" || caractere == "â" ||
			caractere == "ã" || caractere == "ä" || caractere == "Á" ||
			caractere == "À" || caractere == "Â" || caractere == "Ã" ||
			caractere == "Ä") {
			return 'a';
		}
		if (caractere == "é" || caractere == "è" || caractere == "ê" ||
			caractere == "ë" || caractere == "É" || caractere == "È" ||
			caractere == "Ê" || caractere == "Ë") {
			return 'e';
		}
		if (caractere == "í" || caractere == "ì" || caractere == "î" ||
			caractere == "ï" || caractere == "Í" || caractere == "Ì" ||
			caractere == "Î" || caractere == "Ï") {
			return 'i';
		}
		if (caractere == "ó" || caractere == "ò" || caractere == "ô" ||
			caractere == "õ" || caractere == "ö" || caractere == "Ó" ||
			caractere == "Ò" || caractere == "Ô" || caractere == "Õ" ||
			caractere == "Ö") {
			return 'o';
		}
		if (caractere == "ú" || caractere == "ù" || caractere == "û" ||
			caractere == "ü" || caractere == "Ú" || caractere == "Ù" ||
			caractere == "Û" || caractere == "Ü") {
			return 'u';
		}
		if (caractere == "ç" || caractere == "Ç") {
			return 'c';
		}
		return '\0';
	}
}

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

	for (std::size_t i = 0; i < titulo.size();) {
		const unsigned char caractere = static_cast<unsigned char>(titulo[i]);
		if (std::isspace(caractere)) {
			adicionarEspaco = !tituloNormalizado.empty();
			++i;
			continue;
		}

		if (adicionarEspaco) {
			tituloNormalizado += ' ';
			adicionarEspaco = false;
		}

		if (caractere <= 127) {
			tituloNormalizado += static_cast<char>(std::tolower(caractere));
			++i;
		} else {
			const std::size_t tamanho = (caractere & 0xE0) == 0xC0 ? 2 :
			                            (caractere & 0xF0) == 0xE0 ? 3 :
			                            (caractere & 0xF8) == 0xF0 ? 4 : 1;
			const std::size_t restantes = titulo.size() - i;
			const std::size_t tamanhoDisponivel = tamanho <= restantes ? tamanho : 1;
			const std::string_view caractereUtf8(titulo.data() + i, tamanhoDisponivel);
			const char semAcento = removerAcento(caractereUtf8);

			if (semAcento != '\0') {
				tituloNormalizado += semAcento;
			} else {
				tituloNormalizado.append(caractereUtf8);
			}
			i += tamanhoDisponivel;
		}
	}

	return tituloNormalizado;
}
