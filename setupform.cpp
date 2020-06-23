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
    QSqlDatabase dbm = QSqlDatabase::addDatabase("QSQLITE");
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
    QSqlDatabase dbm = QSqlDatabase::addDatabase("QSQLITE");
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

}

void SetupForm::on_pushButton_Import_clicked()
{
    // подтверждение импорта
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Импорт работников!"),
                                                 tr("Уверены в операции импорта?")))  return;


}
