#include "almoxarifado.h"

Almoxarifado::Almoxarifado() //construtor da classe, não serve para nada
{

}

void Almoxarifado::pushBackItem(Item a) //coloca mais um item no almoxarifado
{
    almoxarifado.push_back(a);
}

int Almoxarifado::compareItem(QString tag)
{
    for(int i=0; i < almoxarifado.size(); i++){ // um for para percorrer todo o almoxarifado
        if(almoxarifado[i].getTagDoItem() == tag){ // se a tag do almoxarifado na posição i for igual a tag lida pelo arduino
            return i; //retorna a posição em que se encontra o item
        }
    }
    return -1; // se não, retorna -1
}

int Almoxarifado::compareNomeItem(QString n)
{
    for(int i = 0; i<almoxarifado.size(); i++){ // um for para percorrer todo o almoxarifado
        if(almoxarifado[i].getNome() == n){ // se o nome do item do almoxarifado na posição i for igual ao nome que foi passado como parâmetro
            return i; // retorna a posição do item no almoxarifado
        }
    }
    return -1; // se não, retorna -1
}

void Almoxarifado::disponibilidade(int x)
{
    if(almoxarifado[x].getDispo()){ // se o item que se encontra na posição x do almoxarifado estiver disponível
        almoxarifado[x].setDispo(false); // seta a disponibilidade do item como falsa
    }else{ // se não
        almoxarifado[x].setDispo(true); // seta como disponível
    }

}

void Almoxarifado::devolucao(int i)
{
    almoxarifado[i].setarDataDevolucao(); // seta a data de devolução no item encontrado na posição i do almoxarifado
}

void Almoxarifado::inserirHisto(Emprestimos x, int i)
{
    almoxarifado[i].inserirHistorico(x); // introduz um empréstimo no histórico do item
}



Item *Almoxarifado::retornoItem(int x)
{
    return &almoxarifado[x]; // retorna o item encontrado na posição x do almoxarifado
}

void Almoxarifado::clear()
{
    almoxarifado.clear(); // limpa o vector
}

int Almoxarifado::tamanho()
{
    return almoxarifado.size(); // retorna o tamanho do almoxarifado
}

void Almoxarifado::alterarNomeItem(int linha, QString nome)
{
    almoxarifado[linha].setNome(nome); // altera o nome do item encontrado na posição linha
}

void Almoxarifado::alterarDadoEmprestimo(int linha, QString nome)
{
        almoxarifado[linha].setarMudancaNome(nome); // altera o nome da pessoa que pegou o item emprestado

}
