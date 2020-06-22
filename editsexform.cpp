#include "editsexform.h"
#include "ui_editsexform.h"
#include <QDebug>


EditSexForm::EditSexForm(QSqlDatabase db, QWidget *parent):
    QDialog(parent),
    ui(new Ui::EditSexForm)
{
    ui->setupUi(this);


    if(!db.open()){
        qDebug() << "база не открыта!";
        return;
    }

    model= new QSqlTableModel(this);
    model->setTable("sex");
    model->select();

    //ui->comboBoxName->setModel(model);
    //ui->comboBoxName->setModelColumn(1);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(model);
    //mapper->set
    mapper->addMapping(ui->lineEdit_ID, 0);
    mapper->addMapping(ui->lineEdit_Name, 1);
    mapper->addMapping(ui->lineEdit_Note, 2);
    //mp->toFirst();
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);



    ui->tableView->setModel(model);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // Разрешаем выделение строк
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // по содержимому
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //запрет редактирования


    // сигнал изменения строки выделения в tableVew
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                 SLOT(slotSelectionChange(const QItemSelection &, const QItemSelection &)));
    //ui->lineEdit_Name->setFocus();
    ui->pushButtonEdit->setDefault(true);
    //ui->lineEdit_Name->set

}


EditSexForm::~EditSexForm()
{
    delete ui;
}


void EditSexForm::on_pushButton_Close_clicked()
{
    this->close();
}

void EditSexForm::on_tableView_clicked(const QModelIndex &index)
{

//     mapper->setCurrentIndex(index.row());

     //model->data(modelDepartment->index(index.row(), 0)).toString()
}

void EditSexForm::slotSelectionChange(const QItemSelection &current, const QItemSelection &previous)
{
    // изменение строки
    mapper->setCurrentIndex(ui->tableView->currentIndex().row());
}


void EditSexForm::on_pushButton_Add_clicked()
{
    int row=model->rowCount();
    model->insertRow(row);
    model->submitAll();
    //model->submitAll();
    ui->tableView->selectRow(row);
    mapper->toLast();
    ui->lineEdit_Name->setFocus();
}



void EditSexForm::on_pushButton_First_clicked()
{
    ui->tableView->selectRow(0);
}

void EditSexForm::on_pushButton_Previous_clicked()
{
    ui->tableView->selectRow(ui->tableView->currentIndex().row()-1);
}

void EditSexForm::on_pushButton_Next_clicked()
{
    model->submitAll();
    ui->tableView->selectRow(ui->tableView->currentIndex().row()+1);
}

void EditSexForm::on_pushButton_Last_clicked()
{
    ui->tableView->selectRow(model->rowCount()-1);
}


void EditSexForm::on_lineEdit_Note_editingFinished()
{
    qDebug() << "конец редактирования" << ui->lineEdit_Note->text();;
    //model->submitAll();
}

void EditSexForm::on_tableView_doubleClicked(const QModelIndex &index)
{
    qDebug() << "режим редактирования";
    ui->lineEdit_Name->setFocus();
    model->submitAll();

}

void EditSexForm::on_pushButtonEdit_clicked()
{
    qDebug() << "режим редактирования";
    ui->lineEdit_Name->setFocus();
    model->submitAll();
}

void EditSexForm::on_lineEdit_FLT_textChanged(const QString &arg1)
{
    // фильтр
    QString ff = QString(" %1 Like '\%%2%\' ").arg("name", arg1);
    model->setFilter(ff);
    model->select();

}
