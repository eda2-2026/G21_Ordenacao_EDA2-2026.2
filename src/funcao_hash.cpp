#include "funcao_hash.h"

// calcula a hash de uma chave com o algoritmo FNV-1a
std::uint64_t calcularHash(const std::string& chave) {
	constexpr std::uint64_t OFFSET_BASIS = 14695981039346656037ULL;	// valor inicial definido pelo algoritmo
	constexpr std::uint64_t FNV_PRIME = 1099511628211ULL; // constante de multiplicação do algoritmo

	std::uint64_t hash = OFFSET_BASIS;

	for (unsigned char caractere : chave) {
		hash ^= caractere; // XOR bit a bit
		hash *= FNV_PRIME;
	}

	return hash;
}
