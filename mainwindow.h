#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QSerialPort> // serve para conexão serial
#include <QFileDialog> // utilizada para manipulação de arquivo
#include <QTextStream>// tambem para arquivo
#include <QVector> // vetor de classe
#include <QInputDialog> // comunicação com o usuário
#include "item.h" // classe criada para um item
#include "almoxarifado.h" // vector de itens
#include <QMessageBox> // para informar/advertir o usuário com mensagens
#include "emprestimos.h" // classe criada para monitoramento de empréstimos de cada item
#include "historico.h" // vector de empréstimos
#include <QDate> // manipular data
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QSerialPortInfo> // informa as portas disponíveis para comunicação serial

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QString temp; // variável de armazenamento temporário da tag lida pelo arduino
    QSerialPort serial; // porta serial
    Almoxarifado almoxarifado; // almoxarifado
    Item *novoItem; // novoItem e novoEmp são ponteiros que servem para ajudar a escrever nas tabelas
    Emprestimos *novoEmp;
    QString dadosSerial;
    QString CADASTRO = "\"CADASTRO\":";
    QString NOME = "\"NOME\" : ";
    QString TAG = "\"TAG\" : ";
    QString DISPO = "\"DISPO\" : ";
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString getTemp() const;// get para temp
    void setTemp(const QString &value); // set para temp
    void inserirItemTabela(Item a, int row); // introduz na tabela de controle um item
    void inserirItemTabelaEmprestismos(Item a, int row); // introduz na tabela de empréstimo um item
    void atualizarTabelaHistorico(Item a, int row); // introduz empréstimos realizados pelo item na tabela do histórico
    void inserirItemTabelaBusca(QString n); // introduz na tabela de busca itens encontrados através da busca
    bool salvarAlmoxarifado(QString nomeArquivo, Almoxarifado x); // salva o almoxarifado em um arquivo
    bool carregarAlmoxarifado(QString nomeArquivo); // lê um arquivo que contêm um almoxarifado
    void atualizarTabelas(); // atualiza todas as tabelas


private slots:

    void leitura(); //  lê tag do arduino

    void salvar(); // salvar em um arquivo o almoxarifado

    void carregar(); // carregar um almoxarifado através de um arquivo

    void on_btn_okPortaSerial_clicked(); // quando o botão para se conectar com a porta serial for clicado

    void on_Inserir_clicked(); // quando o botão inserir for clicado (cadastra um item no almoxarifado)

    void on_btn_emprestimo_clicked(); // quando o botão para empréstimos for clicado

    void on_tabela_cellDoubleClicked(int row, int column); // quando clicar duas vezes na tabela de controle

    void on_tabelaEmprestimo_cellDoubleClicked(int row, int column); // quando clicar duas vezes na tabela de empréstimos

    void on_btn_closeSerial_clicked(); // quando clicar para fechar a porta serial

    void on_btn_buscar_clicked(); // quando clicar para buscar um item no almoxarifado

    void on_pushButton_clicked(); // quando clicar para limpar a tabela de buscas

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
