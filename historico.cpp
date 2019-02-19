#include "historico.h"

Historico::Historico()
{

}

void Historico::pushBack(Emprestimos x) // introduz um empréstimo no histórico
{
    emprestimo.push_back(x);
}

void Historico::devolucao() // seta data de devolução de item
{
    //não sei se entenderam a minha lógica, mas...
    // ... no histórico são introduzidos os empréstimos, e quando o item não estiver disponível,
    //significa que ele está emprestado, logo, está emprestado a última pessoa do histórico, sacaram?
    int i = emprestimo.size()-1; // última posição do vetor
    emprestimo[i].setDevolucao(QDate::currentDate()); //setando devolução
}

int Historico::size()
{
    return emprestimo.size(); // retorna o tamanho do vetor
}

Emprestimos Historico::retornoEmp(int x)
{
    return emprestimo[x]; // retorna um empréstimo localizado na posição x do histórico
}

void Historico::alterarNome(QString n)
{
    int i = emprestimo.size()-1; // último índice o histórico
    emprestimo[i].setNome(n); // seta o nome que foi passado como parâmetro
}
