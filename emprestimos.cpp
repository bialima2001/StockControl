#include "emprestimos.h"

Emprestimos::Emprestimos()
{

}

Emprestimos::Emprestimos(QString n, QDate i, QDate d)
{
    setNome(n);
    setInicio(i);
    setDevolucao(d);
}

Emprestimos::Emprestimos(QString n, QDate i) // construtor que seta nome e data de retirada de um item no almoxarifado
{
    setNome(n);
    setInicio(i);
}

QDate Emprestimos::getDevolucao() const // get e set para a data de devolução, retirada e o nome.
{
    return devolucao;
}

void Emprestimos::setDevolucao(const QDate &value)
{
    devolucao = value;
}

QDate Emprestimos::getInicio() const
{
    return inicio;
}

void Emprestimos::setInicio(const QDate &value)
{
    inicio = value;
}

QString Emprestimos::getNome() const
{
    return nome;
}

void Emprestimos::setNome(const QString &value)
{
    nome = value;
}
