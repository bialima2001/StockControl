#include "item.h"

Item::Item()
{

}

Item::Item(QString n, QString tag)
{
    setNome(n);
    setTagDoItem(tag);
}

Historico Item::getHistorico() const // get e set para o histórico do item
{
    return historico;
}

void Item::setHistorico(const Historico &value)
{
    historico = value;
}

void Item::inserirHistorico(Emprestimos x) // introduz um empréstimo no histórico do item
{
    historico.pushBack(x);
}

void Item::setarDataDevolucao() // seta data de devolução do item ao almoxarifado
{
    historico.devolucao();
}

void Item::setarMudancaNome(QString n) // altera nome da pessoa que está com o item emprestado
{
    historico.alterarNome(n);
}

bool Item::getDispo() const // get e set para a disponibilidade do item
{
    return dispo;
}

void Item::setDispo(bool value)
{
    dispo = value;
}

QString Item::getTagDoItem() const // get e set para tag do item
{
    return tagDoItem;
}

void Item::setTagDoItem(const QString &value)
{
    tagDoItem = value;
}

QString Item::getNome() const // get e set para nome do item
{
    return nomeDoItem;
}

void Item::setNome(const QString &value)
{
    nomeDoItem = value;
}
