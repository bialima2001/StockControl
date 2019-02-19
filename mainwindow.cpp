#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //seta todos os group box's como false para que o usuário não consiga ter acesso sem conectar com o arduino
    ui->gb_cadastro->setEnabled(false);
    ui->gb_abaEmprestimo->setEnabled(false);
    ui->gb_Emprestimo->setEnabled(false);

    for(auto& item : QSerialPortInfo::availablePorts()){ // verifica as portas disponíveis para comunicação serial
        ui->cb_serial->addItem(item.portName()); // introduz na combo box os nomes das portas para que o usuário escolha
    }

    connect(&serial, SIGNAL(readyRead()), this, SLOT(leitura())); // conecta com a porta serial
    connect(ui->actionSalvar, SIGNAL(triggered()), this, SLOT(salvar())); // conecta com o botão salvar
    connect(ui->actionCarregar,SIGNAL(triggered()), this, SLOT(carregar())); // conecta com o botão carregar
}

MainWindow::~MainWindow() // destrutor
{
    serial.close(); // fecha a porta serial
    ui->lbl_serial->setText("Desconectado!"); // seta uma mensagem de porta serial desconectada
    delete ui;
}


QString MainWindow::getTemp() const // get para temp
{
    return temp;
}

void MainWindow::setTemp(const QString &value) // set para temp
{
    temp = value;
}

void MainWindow::inserirItemTabela(Item a, int row)
{
    QTableWidgetItem *dispo; // temporário, apenas para ajudar a mudar a cor da disponibilidade na tabela
    ui->tabela->setItem(row, 0, new QTableWidgetItem(a.getNome())); // escreve nome e tag do item, repectivamente, na tabela de controle
    ui->tabela->setItem(row, 1, new QTableWidgetItem(a.getTagDoItem()));
    if(a.getDispo()){ // se o item estiver disponivel
        dispo = new QTableWidgetItem ("Disponível"); // dispo recebe a palavra 'disponivel'
        dispo->setTextColor("green"); // seta cor de dispo como verde
        ui->tabela->setItem(row, 2, dispo); // escreve na tabela de controle
    }else{// se não
        dispo = new QTableWidgetItem("Não disponível"); // dispo recebe 'não disponivel'
        dispo->setTextColor("red"); // seta cor de dispo como vermelho
        ui->tabela->setItem(row, 2, dispo); // escreve na tabela de controle
    }
    delete dispo; // deleta dispo
}

void MainWindow::inserirItemTabelaEmprestismos(Item a, int row)
{
    Historico x = a.getHistorico(); // pega o histórico do item a
    Emprestimos y = x.retornoEmp(x.size()-1); // pega o último empréstimo do histórico
    ui->tabelaEmprestimo->setItem(row, 0, new QTableWidgetItem(y.getNome())); // escreve nome da pessoa que pegou o item emprestado, nome do item e a data da retirada, repectivamente na tabela de empréstimos
    ui->tabelaEmprestimo->setItem(row, 1, new QTableWidgetItem(a.getNome()));
    ui->tabelaEmprestimo->setItem(row, 2, new QTableWidgetItem(y.getInicio().toString()));
}

void MainWindow::atualizarTabelaHistorico(Item a, int row)
{
    Historico x = a.getHistorico(); // pega o histórico do item a
    for(int i = 0; i<x.size(); i++){ // um for para percorrer o histórico
        Emprestimos y = x.retornoEmp(i); // pega o empréstimo encontrado na posição i do histórico
        if(y.getDevolucao().toString()!= ""){ //se o item for devolvido
            if(ui->tabelaHistorico->rowCount()<=row+i){// se a quantidade de linhas da tabela do histórico for menor que row + i
                ui->tabelaHistorico->insertRow(row+i); // introduz mais uma linha na tabale
            }
            ui->tabelaHistorico->setItem(row+i, 0, new QTableWidgetItem(a.getNome())); // escreve o nome do item, o nome da pessoa que pegou o item emprestado, da data de retirada e a data de devolução, respectivamente, na tabela do histórico
            ui->tabelaHistorico->setItem(row+i, 1, new QTableWidgetItem(y.getNome()));
            ui->tabelaHistorico->setItem(row+i, 2, new QTableWidgetItem(y.getInicio().toString()));
            ui->tabelaHistorico->setItem(row+i, 3, new QTableWidgetItem(y.getDevolucao().toString()));
        }
    }
}

void MainWindow::inserirItemTabelaBusca(QString n)
{
    int aux = 0; // variável local auxiliar
    QTableWidgetItem *dispo; // só pra ajudar a trocar a cor na tabela de busca
    ui->tabelaBusca->clearContents(); // limpa a tabela de busca
    for(int i = 0; i<almoxarifado.tamanho(); i++){ // for para percorrer o todo o almoxarifado
        if(almoxarifado.retornoItem(i)->getNome().contains(n)){ // se o nome do item encontrado na posição i do almoxarifado conter a string 'n'
            if(ui->tabelaBusca->rowCount()<=aux){ //se a quantidade de linhas da tabela de busca for menor ou igual a aux
                ui->tabelaBusca->insertRow(aux); // insere uma linha
            }
            ui->tabelaBusca->setItem(aux, 0, new QTableWidgetItem(almoxarifado.retornoItem(i)->getNome())); // escreve nome e tag do item na tabela de busca
            ui->tabelaBusca->setItem(aux, 1, new QTableWidgetItem(almoxarifado.retornoItem(i)->getTagDoItem()));
            if(almoxarifado.retornoItem(i)->getDispo()){// se o item estiver disponivel
                dispo = new QTableWidgetItem("Disponível");// dispo recebe a palavra 'disponivel'
                dispo->setTextColor("green"); // seta cor de dispo como verde
                ui->tabelaBusca->setItem(aux, 2, dispo);// escreve na tabela de busca
            }else{// se não
                dispo = new QTableWidgetItem("Não disponível");// dispo recebe 'não disponivel'
                dispo->setTextColor("red"); // seta cor de dispo como vermelho
                ui->tabelaBusca->setItem(aux, 2, dispo);// escreve na tabela de busca
            }
            aux++; // acrescenta mais um a aux
        }
    }
}

bool MainWindow::salvarAlmoxarifado(QString nomeArquivo, Almoxarifado x)
{
    QFile file(nomeArquivo); // define arquivo
    novoItem = new Item;
    if(!file.open(QIODevice::WriteOnly)){ // abre um arquivo de escrita
        return false; // se não conseguir abrir, retorna falso
    }

    QTextStream out(&file);

    for(int i=0;i<x.tamanho();i++){ // um for para percorrer todo o almoxarifado
        novoItem = almoxarifado.retornoItem(i); // novoItem recebe o item que se encontra na posição i do almoxarifado
        Historico novoHistorico = novoItem->getHistorico(); // pega o histórico do item
        out << novoItem->getNome() <<","<<novoItem->getTagDoItem()<<","; // escreve o nome, tag e disponibilidade do item no arquivo
        out <<novoItem->getDispo()<<",";
        for(int j=0; j<novoHistorico.size();j++){// for para percorrer todo o histórico
            Emprestimos aux = novoHistorico.retornoEmp(j); // aux recebe um empréstimo que se encontra na posição j do histórico
            out<<aux.getNome()<<","<<aux.getInicio().toString()<<","<<aux.getDevolucao().toString()<<"\n"; // escreve nome do usuário, data de retirada e de devolução no arquivo
        }
    }

    file.close(); // fecha o arquivo
    delete novoItem; // deleta o ponteiro
    return true; // retorna verdadeiro

}

bool MainWindow::carregarAlmoxarifado(QString nomeArquivo)
{
    QFile file(nomeArquivo); //define o arquivo
    if(!file.open(QIODevice::ReadOnly)){ // abre como arquivo de leitura
        return false; // se não conseguir abrir, retorna como falso
    }else{// se não
        QTextStream in(&file);
        QString linha;
        novoItem = new Item;
        while(!in.atEnd()){// enquanto o arquivo não acabar
            linha = in.readLine(); // ler uma linha do arquivo
            QStringList dados = linha.split(","); //separa os itens através de virgulas em strings

            novoItem->setNome(dados[0]); //seta nome e tag do item
            novoItem->setTagDoItem(dados[1]);
            if(dados[2]=='1'){ //seta disponibilidade
                novoItem->setDispo(true);
            }else{
                novoItem->setDispo(false);
            }
            for(int i=3; i< dados.size(); i+3){ //for para carregar o histórico
                QString nome = dados[i]; //pega o nome
                QDate inicio = QDate:: fromString( dados[i+1],"dd'/'MM'/'yyyy"); // trasnforma a data que estava em string para QDate
                if(dados[i+2]!=""){// se tiver devolução
                    QDate devolucao =QDate:: fromString( dados[i+2],"dd'/'MM'/'yyyy");
                    Emprestimos emp(nome, inicio, devolucao); // inicia um empréstimo
                    novoItem->inserirHistorico(emp); // introduz no histórico
                }else{//se não
                    Emprestimos emp(nome, inicio); // inicia o empréstimo apenas com nome e data de retiraada (item emprestado)
                    novoItem->inserirHistorico(emp); // introduz no histórico
                }

            }

            almoxarifado.pushBackItem(*novoItem); // introduz o novo item ao almoxarifado
        }
    }
    file.close(); // fecha o arquivo
    delete novoItem; // deleta o ponteiro
    return true; // retorna verdadeiro

}

void MainWindow::atualizarTabelas()
{

    int aux = 0, acumulador = 0; // aux é usado para ajudar a remover linhas na tabela de controle e escrever na tabela de empréstimo e acumulador é usado para escrever na tabela do histórico
    int tam = ui->tabela->rowCount(); // quantidade de linhas na tabela de controle
    ui->tabela->clearContents(); // limpa todas as tabelas
    ui->tabelaEmprestimo->clearContents();
    ui->tabelaHistorico->clearContents();
    while(tam-1 > almoxarifado.tamanho()){ //enquanto o número de linhas da tabela for maior que o tamanho do almoxarifado
        ui->tabela->removeRow(almoxarifado.tamanho()-aux); // remove uma linha
        aux++; // soma mais um no aux
    }
    aux = 0; // zera aux
    for(int i = 0; i < almoxarifado.tamanho(); i++){ // for para percorrer todo o almoxarifado
        Item *x = almoxarifado.retornoItem(i); // pega o item na posição i do almoxarifado
        if(tam <= i){ // se a quantidade de linhas da tabela for menor ou igual ao indice do vetor
            ui->tabela->insertRow(i); // acrescenta mais uma linha na tabela
        }

        inserirItemTabela(*x, i); // introduz um item na tabela de controle
        atualizarTabelaHistorico(*x, acumulador); // introduz o histórico do item na tabela dp histórico
        acumulador += x->getHistorico().size(); // acrescenta o tamanho do histórico no acumulador

        if(x->getDispo()==false){ // se o item não tiver disponível
            ui->tabelaEmprestimo->insertRow(aux); // acrescenta uma linha na tabela de empréstimo
            inserirItemTabelaEmprestismos(*x, aux); // introduz o intem na tabela de empréstimo
            aux++; // soma mais um a aux
        }else if(x->getTagDoItem()==getTemp() && x->getDispo()){ // se a tag do item for igual a leitura atual do arduino e o item estiver disponível (devolução)
            ui->tabelaEmprestimo->removeRow(ui->tabelaEmprestimo->rowCount()-1); // remove uma linha da tabela de empréstimos
        }
    }
}


void MainWindow::on_btn_okPortaSerial_clicked()
{
    QString nomePorta = ui->cb_serial->currentText(); // pega o atual texto na combo box da porta serial
    serial.setPortName(nomePorta); // seta o nome da porta serial
    serial.setBaudRate(QSerialPort::Baud115200); // seta a velocidade da comunicação

    if(serial.open(QIODevice::ReadWrite)){ // abre a porta serial e informa que vai ler e escrever na serial
        ui->lbl_serial->setText("Conectado! Velocidade: 115200"); // informa o status da conexão  e a velocidade ao usuário
        ui->lbl_leitura->setText("Aproxime o item do leitor... "); // seta mensagens nas labels pedindo a leitura dos tags
        ui->lbl_msgEmprestimo->setText("Aproxime o item do leitor... ");
        ui->gb_cadastro->setEnabled(true); // seta os group box como verdadeiro (menos o do emprestimos porque ele só vai ser setado como verdadeiro quando o arduino ler uma tag que já foi cadastrada e quando o item estiver disponível)
        ui->gb_abaEmprestimo->setEnabled(true);
    }else{ // se não
        ui->gb_Emprestimo->setEnabled(false); // seta os group box como falso e uma mensagem de desconectado
        ui->gb_abaEmprestimo->setEnabled(false);
        ui->gb_cadastro->setEnabled(false);
        ui->lbl_serial->setText("Desconectado!");
    }
}


void MainWindow::leitura()
{
    int temp; // inteiro temporário
    QString tag = serial.readAll();// lê tudo que estiver escrito na serial
    dadosSerial = dadosSerial+tag;
    qDebug()<<dadosSerial;
    if(dadosSerial.contains("{") && dadosSerial.contains("}")){ // se começar com '{' e terminar com '}'
        qDebug()<< "entrou";
        dadosSerial.replace("{", "");
        dadosSerial.replace("}", "");
        dadosSerial.replace("/", "");
        dadosSerial.replace("r", "");
        dadosSerial.replace("n", "");
        setTemp(dadosSerial);
        qDebug()<<getTemp();

        temp = almoxarifado.compareItem(getTemp()); // compara a tag lida com todas as tags cadastradas
        if(temp==-1){ // se não existir nenhuma tag igual a lida
            ui->lbl_leitura->setText("Leitura concluida!"); // seta a label da leitura como concluida
            ui->lbl_msgEmprestimo->setText("Item não encontrado!"); // seta a do empréstimo como não encontrado
            ui->gb_Emprestimo->setEnabled(false); // seta o group box do empréstimo como falso
        }else{ // se não, se tiver um item cadastrado com essa tag
            Item *a = almoxarifado.retornoItem(temp); // retorna o item
            if(!a->getDispo()){ // se o item não estiver disponível
                ui->lbl_leitura->setText("Esse item já foi cadastrado, tente novamente."); // seta label de leitura informando que o item já foi cadastrado
                ui->lbl_msgEmprestimo->setText("Leitura concluída! Clique em 'ok' para devolver o item: "); //seta label de empréstimo para devolução
                ui->gb_Emprestimo->setEnabled(false); // seta group box do empréstimo como falso
            }else{// se o item existir no almoxarido e estiver disponível
                ui->lbl_leitura->setText("Esse item já foi cadastrado, tente novamente."); // seta label de leitura informando que o item já foi cadastrado
                ui->lbl_msgEmprestimo->setText("Leitura concluída! Informe o nome e clique em 'ok' para o emprestimo do item: "); // pede ao usuário o nome da pessoa que vai pegar o item emprestado
                ui->gb_Emprestimo->setEnabled(true);// seta group box do empréstimo como verdadeiro
            }
        }
        dadosSerial = "";
    }
}

void MainWindow::on_Inserir_clicked()
{

    QString nome = ui->le_nomeObjeto->text(); // pega o que tem escrito no line edit

    if(almoxarifado.compareItem(getTemp()) ==-1 && ui->le_nomeObjeto->text() != 0){ //se o item não existir no almoxarifado e tiver algo escrito no line edit

        Item a (nome, getTemp()); // cadastra o item
        a.setDispo(true); // seta como disponível

        ui->le_nomeObjeto->clear(); // limpa o line edit
        ui->le_nomeObjeto->setFocus(); // seta o foco para o line edit

        almoxarifado.pushBackItem(a); // introduz o item no almoxarifado

        int linhas = ui->tabela->rowCount(); // conta as linhas da tabela de controle

        ui->tabela->insertRow(linhas); // coloca mais uma linha na tabela
        inserirItemTabela(a, linhas); // escreve o novo item na tabela
        setTemp(""); // limpa temp
       QString pacote;
       pacote = "{"+a.getNome()+","+ a.getTagDoItem()+",";
       if(a.getDispo()){
          pacote= pacote + "1}\n";
        }else{
          pacote= pacote + ",0}\n";
        }
        serial.write(pacote.toUtf8());
        // informa ao usuário que foi salvo com sucesso e seta as labels para uma nova leitura
        QMessageBox::information(this, "", "Item salvo com sucesso!");
        ui->lbl_leitura->setText("Aproxime o item do leitor...");
        ui->lbl_msgEmprestimo->setText("Aproxime o item do leitor...");

    }else{// se o item existir, abre uma caixa de mensagem advertindo o usuário que o item já foi cadastrado no almoxarifado
        QMessageBox::critical(this, " ", "Item já existente, tente fazer a leitura novamente.");
    }

}



void MainWindow::on_btn_emprestimo_clicked()
{
    int aux = almoxarifado.compareItem(getTemp()); // compara a leitura do arduino com os itens do almoxarifado
    Item *a;


    if(ui->le_nomeEmprestimo->text()!=0 && aux != -1){ // se tiver algo escrito no line edit e o item existir no almoxarifado
        a = almoxarifado.retornoItem(aux); // 'a' recebe o item
        if(a->getDispo()){ // se o item estiver disponível

            QString nome = ui->le_nomeEmprestimo->text(); // pega o nome no line edit
            ui->le_nomeEmprestimo->clear(); // limpa o line edit

            QDate inicio = QDate::currentDate(); // data atual

            Emprestimos emp(nome, inicio); // inicia um empréstimo com o nome e data
            almoxarifado.inserirHisto(emp, aux); // introduz o empréstimo no histórico do item
            almoxarifado.disponibilidade(aux); // seta a disponibilidade como falso
            QMessageBox::information(this, "Empréstimo", "Item emprestado ao almoxarifado com sucesso!"); // abre uma caixa de mensagem informando que o empréstimo foi concluído com sucesso
            ui->gb_Emprestimo->setEnabled(false); // seta o group box do empréstimo como falso

        }
    }else if(aux!=-1){ // se não se apenas o item existir no almoxarifado
        a= almoxarifado.retornoItem(aux); // 'a' recebe o item
        if(!a->getDispo()){ // se o item não estiver disponível

            almoxarifado.devolucao(aux); // seta a devolução
            almoxarifado.disponibilidade(aux); // seta como disponível
             // abre uma caixa de mensagem informando que a devolução foi concluída com sucesso
            QMessageBox::information(this, "Empréstimo", "Item devolvido ao almoxarifado com sucesso!");

        }else{ // se não, // abre uma caixa de mensagem pedindo que usuário informe o nome
            ui->lbl_msgEmprestimo->setText("É necessário que informe o seu nome para o empréstimo. Tente novamente...");
        }

    }else{ // abre uma caixa de mensagem informando que o item não foi encontrado no almoxarifado
        QMessageBox::critical(this, "Empréstimo", "Esse item não foi encontrado no almoxarifado, tente novamente.");
    }

    atualizarTabelas(); // atualiza todas as tabelas e seta as labels para a próxima leitura

    ui->lbl_leitura->setText("Aproxime o item do leitor... ");
    ui->lbl_msgEmprestimo->setText("Aproxime o item do leitor... ");

    setTemp("");// limpa temp

}

void MainWindow::salvar(){
    QString nome = QFileDialog::getSaveFileName(this,"Almoxarifado","","Texto Puro(*.txt);;Arquivos Separado por Vírgulas(*.csv)"); // pega o nome do arquivo
    if( salvarAlmoxarifado(nome, almoxarifado) ){ // testa se foi possível salvar
        QMessageBox::information(this, "Salvar almoxarifado","Salvo com sucesso!"); // abre uma caixa de mensagem informando que o arquivo foi salvo com sucesso
    }else{// se não, abre uma caixa de mensagem informando que não foi possível salvar o arquivo
        QMessageBox::information(this, "Salvar almoxarifado","Infelizmente, não foi possível salvar os dados!");
    }
}

void MainWindow::carregar(){

    almoxarifado.clear(); // limpa o almoxarifado
    QString nome = QFileDialog::getOpenFileName(this,"Almoxarifado", "", "Texto Puro(*.txt);;Arquivos Separados por Vírgulas(*.csv)"); // pega o nome do arquivo
    if(carregarAlmoxarifado(nome)){ // testa se foi possível carregar o arquivo

        atualizarTabelas(); // atualiza tabelas
    }else{ // se não, abre uma caixa de mensagem informando que não foi possível abrir o arquivo
        QMessageBox::information(this, "Almoxarifado","Infelizmente, não foi possível carregar os dados!");
    }
}

void MainWindow::on_tabela_cellDoubleClicked(int row, int column)
{
    if(column == 0){ // se a coluna for igual a zero

        QString nome;
        nome = QInputDialog::getText(this, "Editar", "Qual é o nome do Item?", QLineEdit::Normal); // abre uma caixa de mensagem com um line edit para que o usuário informe o novo nome
        almoxarifado.alterarNomeItem(row, nome); // altera o nome do item
        atualizarTabelas(); // atualiza  as tabelas

    }else{ // se não, abre uma caixa de mensagem informando que o usuário não tem permissão para alterar o dado
        QMessageBox::information(this, "Editar", "Você não tem permissão para alterar esse dado!");
    }
}



void MainWindow::on_tabelaEmprestimo_cellDoubleClicked(int row, int column) // quando clicar duas vezes na tabela de empréstimo
{
    QString nome; // criando uma variável local
    if(column == 0){ // se a coluna for igual a 0
        nome = QInputDialog::getText(this, "Editar", "Informe o nome: ", QLineEdit::Normal); // abre uma caixa de mensagem com um line edit para que o usuário informe o novo nome
        almoxarifado.alterarDadoEmprestimo(row, nome); // altera o nome
        atualizarTabelas();
    }else{ // se não, abre uma caixa de mensagem informando que o usuário não pode alterar o dado
        QMessageBox::information(this, "Editar", "Você não tem permissão para alterar esse dado!");
    }
}

void MainWindow::on_btn_closeSerial_clicked() // quando o botão para fechar a serial for clicado
{
    serial.close(); // fecha a comunicação serial
    ui->gb_Emprestimo->setEnabled(false); // seta como falso todos os group box
    ui->gb_abaEmprestimo->setEnabled(false);
    ui->gb_cadastro->setEnabled(false);
    ui->lbl_serial->setText("Desconectado!"); // seta uma mensagem na de desconectado
}

void MainWindow::on_btn_buscar_clicked() // quando o botão de busca for clicado
{
    if(ui->le_buscarNome->text()!=0){ // verifica se tem algo escrito no line edit
        if(almoxarifado.compareNomeItem(ui->le_buscarNome->text())!=-1){ //compara o que tem escrito com o vetor
            inserirItemTabelaBusca(ui->le_buscarNome->text()); // introduz na tabela de busca todos os nomes contém a sring pesquisada pelo usuário
        }
    }else{// se não tiver nada escrito, seta um aviso pedindo para que o usuário informe o nome do item
        ui->lbl_msgBusca->setText("É necessário que informe o nome do item.");
    }
}

void MainWindow::on_pushButton_clicked() // quando o botão de limpar a tabela de busca for clicado
{
    for(int i = ui->tabelaBusca->rowCount(); i>=0; i--){ // um for para percorrer todas as linhas da tabela
        ui->tabelaBusca->removeRow(i); // remove a linha na posição i
    }
}
