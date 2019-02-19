#ifndef HISTORICO_H
#define HISTORICO_H
#include <QMainWindow>
#include "emprestimos.h"
#include <algorithm>
#include <QVector>
//Histórico é uma classe formada por um vector de empréstimos, ou seja, é um conjunto de empréstimos
class Historico
{
public:
    Historico();
    QVector <Emprestimos> emprestimo; // vector de empréstimos
    void pushBack(Emprestimos x); // introduz no histórico mais um empréstimo
    void devolucao(); // seta devolução de um item
    int size(); // retorna o a quantidade de vezes que o item foi emprestado
    Emprestimos retornoEmp(int x); // retorna um empréstimo encontrado no histórico na posição x
    void alterarNome(QString n); // altera o nome da pessoa que pegou o item emprestado pela última vez
};

#endif // HISTORICO_H
