#ifndef LIVRO_H
#define LIVRO_H

#include <string>

class Livro {

private:
	std::string isbn;
	std::string titulo;
	std::string autor;
	std::string editora;
	int anoPublicacao;
	int quantidadeVendidos;

public:
	Livro(const std::string &isbn, const std::string &titulo, const std::string &autor,
		  const std::string &editora, int anoPublicacao, int quantidadeVendidos);

	const std::string &getIsbn() const;
	const std::string &getTitulo() const;
	const std::string &getAutor() const;
	const std::string &getEditora() const;
	int getAnoPublicacao() const;
	int getQuantidadeVendidos() const;

	void setTitulo(const std::string &titulo);
	void setAutor(const std::string &autor);
	void setEditora(const std::string &editora);
	void setAnoPublicacao(int anoPublicacao);
	void setQuantidadeVendidos(int quantidadeVendidos);
};

#endif
