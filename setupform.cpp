#include "setupform.h"
#include "ui_setupform.h"
#include <QFileDialog>
#include <QtDebug>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

SetupForm::SetupForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupForm)
{
    ui->setupUi(this);


    // считать из настроек имя базы

    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    ui->lineEditBaseName->setText(settings.value(SETTINGS_BASE_NAME, "").toString());

}

SetupForm::~SetupForm()
{
    delete ui;
}

void SetupForm::on_pushButtonBrowse_clicked()
{
    // выбор файла базы данных

    QString filename = QFileDialog::getOpenFileName(this,tr("Open base"),"./",tr("Data base Fules (*.db)"));
    if (!filename.isEmpty()) {
        ui->lineEditBaseName->setText(filename);

    }

}

void SetupForm::on_pushButtonClose_clicked()
{
    // сохранить в настройказ имя базы
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    //пишем настройки
    settings.setValue(SETTINGS_BASE_NAME, ui->lineEditBaseName->text());
    settings.sync();

    this->close();

}

void SetupForm::on_pushButtonCreateTable_clicked()
{
    // подтверждение сознаия
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Внимание!"),
                                                 tr("Уверены в создании таблиц?")))  return;

    QString baseName = ui->lineEditBaseName->text();

    //проверяем на наличие файл базы
    if(!QFile(baseName).exists()){
        qDebug() << "Файла базы нет! Будет создан новый.";
    }

// открываем базу
    QSqlDatabase dbm = QSqlDatabase::addDatabase("QSQLITE","set");
    dbm.setDatabaseName(baseName);
    if(!dbm.open()){
      qDebug() << "Ошибка открытия базы!";
      QMessageBox::critical(this,"Error",dbm.lastError().text());
      return;
    }

    QSqlQuery a_query = QSqlQuery(dbm);
    // запрос на создание таблицы Пол
    // возможно нужно удаление старой - DROP TABLE sex;
    QString str =" CREATE TABLE sex ("
            "ID   INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name VARCHAR,"
            "note VARCHAR"
            ");";
    if (!a_query.exec(str))
        qDebug() << "таблица Пол: " << a_query.lastError().text();

    // запрос на создание таблицы Отделов
    str =" CREATE TABLE department ("
            "ID   INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name VARCHAR"
            ");";
    if (!a_query.exec(str))
        qDebug() << "таблица Отделов: " << a_query.lastError().text();

    // запрос на создание таблицы Работников
    str =" CREATE TABLE workers ("
            "ID         INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name       VARCHAR,"
            "age        INTEGER,"
            "sex        INTEGER REFERENCES sex (ID),"
            "department INTEGER REFERENCES department (ID) "
            ");";
    if (!a_query.exec(str))
        qDebug() << "таблица Работников: " << a_query.lastError().text();

    //
    QMessageBox::information(this,"Info","Создание завершено, перезапустите приложение!");
    dbm.close();
}

void SetupForm::on_pushButtonClear_clicked()
{
    // подтверждение удаление
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Внимание!"),
                                                 tr("Уверены в удалении данных?")))  return;

    QString baseName = ui->lineEditBaseName->text();

    //проверяем на наличие файл базы
    if(!QFile(baseName).exists()){
        qDebug() << "Файла базы нет!";
        QMessageBox::information(this,"Error","Выбранной базы не существует. Удалять нечего!");
        return;
    }

// открываем базу
    QSqlDatabase dbm = QSqlDatabase::addDatabase("QSQLITE","new");
    dbm.setDatabaseName(baseName);
    if(!dbm.open()){
      qDebug() << "Ошибка открытия базы!";
      QMessageBox::critical(this,"Error",dbm.lastError().text());
      return;
    }

    QSqlQuery a_query = QSqlQuery(dbm);
    QString str;

    if (ui->checkBox_Workers->isChecked()){
        // запрос на очистку работников
        str =" DELETE FROM workers;";
        if (!a_query.exec(str))
            qDebug() << "таблица работников: " << a_query.lastError().text();
    }

    if (ui->checkBox_All->isChecked()){
        // запрос на очистку работников
        str =" DELETE FROM workers;";
        if (!a_query.exec(str))
            qDebug() << "таблица работников: " << a_query.lastError().text();
        // запрос на очистку отделов
        str =" DELETE FROM department;";
        if (!a_query.exec(str))
            qDebug() << "таблица отделов: " << a_query.lastError().text();
        // запрос на очистку пола
        str =" DELETE FROM sex;";
        if (!a_query.exec(str))
            qDebug() << "таблица пола: " << a_query.lastError().text();
    }

    //
    QMessageBox::information(this,"Info","Операция завершена.");
    dbm.close();

}

void SetupForm::on_pushButton_Import_clicked()
{
    // подтверждение импорта
//    if(QMessageBox::Yes != QMessageBox::question(this, tr("Импорт работников!"),
//                                                 tr("Уверены в операции импорта?")))  return;


    QString baseName = ui->lineEditBaseName->text();

    //проверяем на наличие файл базы
    if(!QFile(baseName).exists()){
        qDebug() << "Файла базы нет!";
        QMessageBox::information(this,"Error","Выбранной базы не существует. Удалять нечего!");
        return;
    }

// открываем базу
    //dbm.close();
    QSqlDatabase dbm = QSqlDatabase::addDatabase("QSQLITE","import");
    dbm.setDatabaseName(baseName);
    if(!dbm.open()){
      qDebug() << "Ошибка открытия базы!";
      QMessageBox::critical(this,"Error",dbm.lastError().text());
      return;
    }


    //

    QString fileName;
    QString sep = "\t";
       fileName = QFileDialog::getOpenFileName(this,QString::fromUtf8("Открыть файл"), QDir::currentPath(),"Типы файлов (*.csv;*.docx;);;Все файлы (*.*)");
       QFile file(fileName);
       if(file.open (QIODevice::ReadOnly)){

           QTextStream ts (&file);

           QString tabl="workers";
           QString describe = "name, age";

           // Цикл до конца файла
           while(!ts.atEnd()){
               // Cтрока в которую будем формировать запросы
               QString req = "INSERT INTO "+tabl+" ("+describe+") VALUES(";
               // Обрезаем строку до разделителя
               QStringList line = ts.readLine().split(sep);

               qDebug() << "import: " << line;
               // предпологается жетская структура
               // возможно надо сделать выбор колонок для импорта или хотябы проверку названий полей
               req.append("\""+line.at(1)+"\""); // name
               req.append(",");
               req.append(line.at(2)); // age

//               for(int i=0; i<line.length ();i++){
//                       req.append('"'+line.at(i)+'"');
//                   req.append(",");
//               }
//               req.chop(1);
               req.append(");");

               qDebug()<<req;

               QSqlQuery query(dbm);
               if(!query.exec(req))
               {
                   qDebug() << "ERROR Insert " << dbm.lastError().text();
                   return;
               }

            }
           file.close ();
       }

       dbm.close();
}
