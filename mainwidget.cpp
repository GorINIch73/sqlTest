#include "mainwidget.h"
#include "ui_mainwidget.h"

#include <QDebug>
#include <QFile>
#include <QSqlQuery>
#include <QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QPrintDialog>
#include <QTextStream>
#include <QTextDocument>

#include <QPdfWriter>
#include <QPainter>

#include <QDesktopServices>
#include <QSettings>

#include "editsexform.h"
#include "setupform.h"


MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    // повторная инициализация сбивает модели, делаем один раз
    db = QSqlDatabase::addDatabase("QSQLITE");
    // открываем базу
    if (!OpenBase()) return;

    modelDepartment = new QSqlTableModel(this,db);
    modelWorkers = new QSqlRelationalTableModel(this,db);
    Workers_d = new QSqlRelationalDelegate(this);

    //Настраиваем модели
    SetupDepartment();
    SetupWorkers();

    modelDepartment->select();
    modelWorkers->select();

//    QSqlQuery query;
//    query.exec("SELECT * FROM department");
//    while (query.next()) {
//        qDebug() << query.value(0).toString() << query.value(1).toString();
//    }

}

bool MainWidget::OpenBase()
{
    // читаем из настроек имя базы
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);

    //DatabaseName = "../office/office.db";
     DatabaseName = settings.value(SETTINGS_BASE_NAME, "").toString();

    //проверяем на наличие файл базы
    if(!QFile(DatabaseName).exists()){
        qDebug() << "Файла базы нет!";
    //    this->setWindowTitle(tr("Файла базы нет!"));
    }

// открываем базу
    //db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(DatabaseName);
    if(!db.open()){
      qDebug() << "Ошибка открытия базы!";
      this->setWindowTitle("Error!");
      return false;
    }
    // титульный окна имя базы
    this->setWindowTitle(DatabaseName);
    return true;
}

void MainWidget::SetupTable()
{


}

void MainWidget::SetupDepartment()
{
    //Таблица отделов

    modelDepartment->setTable("department");
    // названия колонок
    modelDepartment->setHeaderData(1,Qt::Horizontal,"Название отдела");
    ui->tableView_Department->setModel(modelDepartment);
    //ui->tableView_Department->setColumnHidden(0, true);    // Скрываем колонку с id записей
    //ui->tableView_Department->setEditTriggers(QAbstractItemView::NoEditTriggers);  //запрет редактирования
    ui->tableView_Department->setSelectionBehavior(QAbstractItemView::SelectRows); // Разрешаем выделение строк
    ui->tableView_Department->setSelectionMode(QAbstractItemView::SingleSelection); // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView_Department->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_Department->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // по содержимому

}

void MainWidget::SetupWorkers()
{
    //Таблица работников

       modelWorkers->setTable("workers");
       modelWorkers->setRelation(4, QSqlRelation("department", "ID", "Name"));

       modelWorkers->setRelation(3, QSqlRelation("sex", "ID", "Name"));
       // названия колонок
       modelWorkers->setHeaderData(1,Qt::Horizontal,"ФИО");
       modelWorkers->setHeaderData(2,Qt::Horizontal,"Возраст");
       modelWorkers->setHeaderData(3,Qt::Horizontal,"Пол");
       modelWorkers->setHeaderData(4,Qt::Horizontal,"Отдел");


       ui->tableView_Workers->setModel(modelWorkers);
       ui->tableView_Workers->setItemDelegate(Workers_d);

       ui->tableView_Workers->setSelectionBehavior(QAbstractItemView::SelectRows); // Разрешаем выделение строк
       ui->tableView_Workers->setSelectionMode(QAbstractItemView::SingleSelection); // Устанавливаем режим выделения лишь одно строки в таблице
       ui->tableView_Workers->horizontalHeader()->setStretchLastSection(true);
       ui->tableView_Workers->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // по содержимому

}



MainWidget::~MainWidget()
{
    delete modelWorkers;
    delete modelDepartment;
    delete Workers_d;
    delete ui;
    db.close();
}


void MainWidget::on_pushButton_AddDepartment_clicked()
{
    int row=modelDepartment->rowCount();
    modelDepartment->insertRow(row);
    ui->tableView_Department->selectRow(row);
    ui->tableView_Department->setFocus();
    modelDepartment->submitAll();
    //обновляем работников
    //modelWorkers->select();
    //modelWorkers->submitAll();
    //modelWorkers->fetchMore();
    //ui->tableView_Workers->update();
    //ui->tableView_Workers->
}

void MainWidget::on_pushButton_DelDepartment_clicked()
{
    qDebug() << "Удаляем " << ui->tableView_Department->currentIndex().row();
    // удаляем запись
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Внимание!"),
                                                 tr("Уверены в удалении записи?")))  return;

    modelDepartment->removeRow(ui->tableView_Department->currentIndex().row());
}

void MainWidget::on_pushButton_AddWorkers_clicked()
{
    //обновляем отделы
    int ii=ui->tableView_Department->currentIndex().row();
    modelDepartment->submitAll();
    modelDepartment->select();
    ui->tableView_Department->selectRow(ii);


    //Workers_d->model
    modelWorkers->submitAll();
    //перенастроить работников - инче не видны новые записи
    SetupWorkers();
    // восстанвить фильтр по отделу
    QString ff = QString(" department = \%1\ ").arg(modelDepartment->data(modelDepartment->index(ui->tableView_Department->currentIndex().row(), 0)).toString());
    modelWorkers->setFilter(ff);
    modelWorkers->select();

    // добавить новую запись
    int row=modelWorkers->rowCount();
    modelWorkers->insertRow(row);
    ui->tableView_Workers->selectRow(row);
    ui->tableView_Workers->setFocus();

    // вставдяем значение отдела
    int tt= modelDepartment->data(modelDepartment->index(ui->tableView_Department->currentIndex().row(), 0)).toInt();
//    qDebug() << "ID: "<< tt;
    modelWorkers->setData(modelWorkers->index(ui->tableView_Workers->currentIndex().row(), 4) ,tt);
    //modelWorkers->setData(modelWorkers->index(ui->tableView_Workers->currentIndex().row(), 1) ,"test");

    //modelWorkers->submitAll();
//    ui->tableView_Workers->update();
}

void MainWidget::on_pushButton_DelWorkers_clicked()
{
    qDebug() << "Удаляем " << ui->tableView_Workers->currentIndex().row();
    // удаляем запись
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Внимание!"),
                                                 tr("Уверены в удалении записи?")))  return;

    modelWorkers->removeRow(ui->tableView_Workers->currentIndex().row());
}

void MainWidget::on_pushButton_Re_clicked()
{
    //  ====================================================================================================
    modelDepartment->submitAll();
    int ii=ui->tableView_Department->currentIndex().row();
    SetupDepartment();
    modelDepartment->select();
    ui->tableView_Department->selectRow(ii);



    modelWorkers->submitAll();
    ii=ui->tableView_Workers->currentIndex().row();
    SetupWorkers();
    modelWorkers->select();
    ui->tableView_Workers->selectRow(ii);

}

void MainWidget::on_pushButton_Exit_clicked()
{
    this->close();
}

void MainWidget::on_tableView_Department_clicked(const QModelIndex &index)
{
    // ----
    //int row = ui->tableView_Department->currentIndex().row();
    //QString ff = QString(" department = \%1\ ").arg(modelDepartment->data(modelDepartment->index(ui->tableView_Department->currentIndex().row(), 0)).toString());

    QString ff = QString(" department = \%1\ ").arg(modelDepartment->data(modelDepartment->index(index.row(), 0)).toString());


    modelWorkers->setFilter(ff);
    modelWorkers->select();

}

void MainWidget::on_lineEditFlt_textChanged(const QString &arg1)
{
//    QString ff = QString(" `%1` Like '\%%2\%' ").arg("name", ui->lineEditFlt->text());
    QString ff = QString(" %1 Like '\%%2%\' ").arg("name", arg1);

    modelDepartment->setFilter(ff);
    modelDepartment->select();
}

void MainWidget::on_lineEditFltW_textChanged(const QString &arg1)
{
    QString ff = QString(" %1 Like '\%%2\%' ").arg("workers.name", arg1);

    modelWorkers->setFilter(ff);
    modelWorkers->select();
}

void MainWidget::on_pushButtonPrint_clicked()
{
    // формирование и печать

    // нужно создать отдельный запрос для печати
    // мы используем отображение Работников

    QString stringStream;
    QTextStream out(&stringStream);

    int rowCount = ui->tableView_Workers->model()->rowCount(); //количество строк
    int columnCount = ui->tableView_Workers->model()->columnCount(); //количество столбцов

    out << "<html>\n" << "<head>\n" << "meta Content=\"Text/html;charsrt=Windows-1251\">\n" <<
           QString("<title>%1</title>\n").arg("Report") <<
           "</head>\n"
           "<body bgcolor = #ffffff link=#5000A0>\n"
           "<table boarder = 1 cellspacing=0 celladding=2>\n";
    out << "<thead><tr bgcolor=#f0f0f0>";
    for(int column = 0; column < columnCount; column++)
        out << QString("<th>%1</th>").arg(ui->tableView_Workers->model()->headerData(column,Qt::Horizontal).toString());
    out << "</tr></head>\n";

    for(int row = 0; row < rowCount; row++){
        out << "<tr>";
        for(int column = 0; column < columnCount; column++){
            QString data = ui->tableView_Workers->model()->data(ui->tableView_Workers->model()->index(row,column)).toString().simplified();
            out <<  QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty())? data:QString("&nbsp;"));
        }
        out << "</tr>\n";
    }
    out << "</table>\n" << "</body>\n" << "</html>\n";

    QTextDocument *document = new QTextDocument();
    document->setHtml(stringStream);

    QPrinter printer;
    QPrintDialog *dialog = new QPrintDialog(&printer);
    if (dialog->exec() == QDialog::Accepted) {
        document->print(&printer);
    }
    delete document;



}

void MainWidget::on_pushButtonPdf_clicked()
{
    // отчет в PDF
    int rowCount = ui->tableView_Workers->model()->rowCount(); //количество строк
    int columnCount = ui->tableView_Workers->model()->columnCount(); //количество столбцов

    QPdfWriter pdf("d:/Qt/Project/test.pdf");
    QPainter painter(&pdf);

    painter.setPen(Qt::blue);
    painter.drawText(100,100,"Test");

    for(int row = 0; row < rowCount; row++){
        for(int column = 0; column < columnCount; column++){
            QString data = ui->tableView_Workers->model()->data(ui->tableView_Workers->model()->index(row,column)).toString().simplified();
            painter.drawText(100+column*1200,300+row*150,QString(" %1 ").arg((!data.isEmpty())? data:QString("&nbsp;")));
        }

    }
    painter.end();


}

void MainWidget::on_pushButtonPrntPdf_clicked()
{
    // формирование и печать

    // нужно создать отдельный запрос для печати
    // мы используем отображение Работников

    QString stringStream;
    QTextStream out(&stringStream);

    int rowCount = ui->tableView_Workers->model()->rowCount(); //количество строк
    int columnCount = ui->tableView_Workers->model()->columnCount(); //количество столбцов

    out << "<html>\n" << "<head>\n" << "meta Content=\"Text/html;charsrt=Windows-1251\">\n" <<
           QString("<title>%1</title>\n").arg("Report") <<
           "</head>\n"
           "<body bgcolor = #ffffff link=#5000A0>\n"
           "<table boarder = 1 cellspacing=0 celladding=2>\n";
    out << "<thead><tr bgcolor=#f0f0f0>";
    for(int column = 0; column < columnCount; column++)
        out << QString("<th>%1</th>").arg(ui->tableView_Workers->model()->headerData(column,Qt::Horizontal).toString());
    out << "</tr></head>\n";

    for(int row = 0; row < rowCount; row++){
        out << "<tr>";
        for(int column = 0; column < columnCount; column++){
            QString data = ui->tableView_Workers->model()->data(ui->tableView_Workers->model()->index(row,column)).toString().simplified();
            out <<  QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty())? data:QString("&nbsp;"));
        }
        out << "</tr>\n";
    }
    out << "</table>\n" << "</body>\n" << "</html>\n";



    QTextDocument document;
    document.setHtml(stringStream);

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFileName("../office/test2.pdf");
    printer.setPageMargins(QMarginsF(15, 15, 15, 15));

    document.print(&printer);

    // откровем созданный отчет
    QDesktopServices::openUrl(QUrl(QUrl::fromLocalFile("../office/test2.pdf")));


}

void MainWidget::on_pushButtonSex_clicked()
{
    EditSexForm dial(db,this);

    dial.exec();


}

void MainWidget::on_pushButtonSetup_clicked()
{
    SetupForm dial(this);
    dial.exec();

    db.close();
    OpenBase();

    SetupDepartment();
    SetupWorkers();
    modelDepartment->select();
    modelWorkers->select();

}

