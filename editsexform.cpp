#include "editsexform.h"
#include "ui_editsexform.h"
#include <QDebug>
#include <QMessageBox>


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
    // Enter по умолчанию на кнопку редактирования
    ui->pushButtonEdit->setDefault(true);
    //устанавливаем таблицу на первую запись
    ui->tableView->selectRow(0);


}


EditSexForm::~EditSexForm()
{
    delete mapper;
    delete model;
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
    // при изменение строки в таблвьюве устанавливаем маппер на соответствующую запись
    mapper->setCurrentIndex(ui->tableView->currentIndex().row());
}


void EditSexForm::on_pushButton_Add_clicked()
{
    // добавление запись
    // определяем количество записей
    int row=model->rowCount();
    // вставляем следующую
    model->insertRow(row);
    model->submitAll();
    // устанавливаем курсор на строку редактирования
    ui->tableView->selectRow(row);
    // устанавливаем маппер на ту же запись
    mapper->toLast();
    // устанавливаем курсор на редактирование имени
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
    // тест
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

void EditSexForm::on_pushButton_2_clicked()
{
    qDebug() << "Удаляем " << ui->tableView->currentIndex().row();
    // удаляем запись
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Внимание!"),
                                                 tr("Уверены в удалении записи?")))  return;

    model->removeRow(ui->tableView->currentIndex().row());
}
