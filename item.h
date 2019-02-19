#ifndef ITEM_H
#define ITEM_H
#include <QMainWindow>
#include "historico.h"
#include "emprestimos.h"
#include <QObject>

class Item
{
public:
    Item();
    Item (QString n, QString tag); // construtor da classe, inicializado com o nome e a tag do item

    QString nomeDoItem; // nome do item
    QString tagDoItem; // tag do item
    bool dispo; // disponibilidade do item
    Historico historico; // histórico do item

    void inserirHistorico(Emprestimos x); //  introduz um empréstimo no histórico
    void setarDataDevolucao(); // seta a data em que o item foi devolvido
    void setarMudancaNome (QString n); // altera o nome da pessoa que pegou o item emprestado pela última vez
    // abaixo get e set para todas as variáveis
    QString getNome() const;
    void setNome(const QString &value);

    QString getTagDoItem() const;
    void setTagDoItem(const QString &value);

    bool getDispo() const;
    void setDispo(bool value);

    Historico getHistorico() const;
    void setHistorico(const Historico &value);
};

#endif // ITEM_H
