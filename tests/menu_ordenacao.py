"""Uso: python tests/menu_ordenacao.py caminho/para/biblioteca.exe"""
import re
import subprocess
import sys


def executar(entrada):
    return subprocess.run(
        [sys.argv[1]], input=entrada, text=True, encoding="utf-8",
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, check=True, timeout=10
    ).stdout


def verificar(condicao, mensagem):
    if not condicao:
        raise AssertionError(mensagem)


# Índices na ordem crescente dos cinco critérios para os livros de demonstração.
isbns = ["9780000000002", "9780000000019", "9780000000026",
         "9780000000033", "9780000000040"]
ordens = [[2, 3, 0, 1, 4], [0, 1, 2, 3, 4], [0, 1, 2, 3, 4],
          [0, 1, 2, 3, 4], [4, 1, 3, 0, 2]]
for criterio in range(1, 6):
    for direcao in (1, 2):
        saida = executar(f"8\n9\n{criterio}\n{direcao}\n0\n")
        resultado = saida.split("========= Livros Ordenados =========", 1)[1]
        encontrados = re.findall(r"^ISBN: (.+)$", resultado, flags=re.MULTILINE)
        indices = ordens[criterio - 1]
        if direcao == 2:
            # Os títulos empatados continuam com autor crescente.
            indices = [4, 0, 1, 3, 2] if criterio == 1 else list(reversed(indices))
        verificar(encontrados == [isbns[i] for i in indices], "Ordem exibida incorreta")
        verificar("Total de livros: 5" in resultado, "Total ausente")
        verificar(re.search(r"Comparações: [1-9]\d*", resultado), "Comparações ausentes")
        verificar(re.search(r"Movimentações: [1-9]\d*", resultado), "Movimentações ausentes")
        verificar(re.search(r"Tempo de ordenação \(microssegundos\): \d+\.\d{3}", resultado),
                  "Tempo ausente ou sem unidade/formatação")

vazio = executar("9\n1\n1\n0\n")
for trecho in ("Nenhum livro cadastrado", "Total de livros: 0", "Comparações: 0",
               "Movimentações: 0", "Tempo de ordenação (microssegundos): 0.000"):
    verificar(trecho in vazio, f"Catálogo vazio: falta {trecho}")

for entrada in ("9\n0\n0\n", "9\n1\n0\n0\n", "9\n", "9\n1\n"):
    verificar("Livros Ordenados" not in executar(entrada), "Cancelamento ou EOF ordenou livros")

invalida = executar("8\n9\nabc\n6\n-1\n1.5\n1abc\n\n1\n3\nx\n2\n0\n")
verificar(invalida.count("[Erro] Opção inválida!") == 8, "Entradas inválidas não rejeitadas")
verificar("Livros Ordenados" in invalida, "Menu não se recuperou após entrada inválida")

print("Menu de ordenação: todos os testes passaram.")
