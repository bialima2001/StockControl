#ifndef EMPRESTIMOS_H
#define EMPRESTIMOS_H
#include <QMainWindow>
#include <QDate>

class Emprestimos
{
public:
    Emprestimos();
    Emprestimos (QString n, QDate i, QDate d);
    Emprestimos(QString n, QDate i); // construtor para criar um empréstimo inicializado com o nome do usuário e data de retirada do item no almoxarifado

    QString nome; // nome da pessoa que pegou o item emprestado
    QDate inicio, devolucao; // data de retirada e de devolução, respectivamente
    //abaixo get e set para todas a variáveis
    QString getNome() const;
    void setNome(const QString &value);

    QDate getInicio() const;
    void setInicio(const QDate &value);

    QDate getDevolucao() const;
    void setDevolucao(const QDate &value);
};

#endif // EMPRESTIMOS_H
