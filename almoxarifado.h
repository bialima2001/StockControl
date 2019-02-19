#ifndef ALMOXARIFADO_H
#define ALMOXARIFADO_H

/*Almoxarifado é uma classe que inclui um vector da Item, ou seja,
  o almoxarifado é formado por vários itens*/

#include "item.h" //inclusão de bibliotecas necessárias
#include <QVector>
#include "emprestimos.h"

class Almoxarifado
{
public:
    Almoxarifado();
    QVector <Item> almoxarifado; //vector de itens para formar o almoxarifado

    void pushBackItem(Item a); //Coloca mais um item no almoxarifado, usado para cadastrar novos itens
    int compareItem(QString tag); /*Compara a tag lida pelo arduino com todas as tags já cadastradas no almoxarifado,
                                    caso exista, retorna a posição que o item pode ser encontrado, caso não, retorna -1 */
    int compareNomeItem(QString n); //Mesma coisa do anterior, só que usa o nome de item para a comparação
    void disponibilidade(int x);/*Essa função tem como parâmetro um inteiro para que seja enviado um indice do vector almoxarifado,
                                  e assim, a disponibilidade do item seja trocada no indice (se tiver indisponível, torna-se disponível, e virse-versa)*/
    void devolucao(int i);//seta a data de devolução do item no histórico
    void inserirHisto (Emprestimos x, int i);//Coloca no vetor do histórico do item mais um empréstimo
    Item *retornoItem(int x); // Retorna um item do almoxarifado com indice x
    void clear(); // limpa o almoxarifado
    int tamanho(); //retorna a quantidade de itens cadastrados no almoxarifado
    void alterarNomeItem(int linha, QString nome); // altera o nome do item através da tabela de controle
    void alterarDadoEmprestimo(int linha, QString nome); //altera o nome da pessoa que pegou o item emprestado através da tabela de empréstimos

};
#endif // ALMOXARIFADO_H
