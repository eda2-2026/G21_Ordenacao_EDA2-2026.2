#include "livro.h"

Livro::Livro(const std::string &isbn, const std::string &titulo, const std::string &autor,
			 const std::string &editora, int anoPublicacao, int quantidadeVendidos)
	: isbn(isbn), titulo(titulo), autor(autor), editora(editora),
	  anoPublicacao(anoPublicacao), quantidadeVendidos(quantidadeVendidos) {
}

const std::string &Livro::getIsbn() const {
	return isbn;
}

const std::string &Livro::getTitulo() const {
	return titulo;
}

const std::string &Livro::getAutor() const {
	return autor;
}

const std::string &Livro::getEditora() const {
	return editora;
}

int Livro::getAnoPublicacao() const {
	return anoPublicacao;
}

int Livro::getQuantidadeVendidos() const {
	return quantidadeVendidos;
}

void Livro::setTitulo(const std::string &titulo) {
	this->titulo = titulo;
}

void Livro::setAutor(const std::string &autor) {
	this->autor = autor;
}

void Livro::setEditora(const std::string &editora) {
	this->editora = editora;
}

void Livro::setAnoPublicacao(int anoPublicacao) {
	this->anoPublicacao = anoPublicacao;
}

void Livro::setQuantidadeVendidos(int quantidadeVendidos) {
	this->quantidadeVendidos = quantidadeVendidos;
}
