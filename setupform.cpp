#include "setupform.h"
#include "ui_setupform.h"
#include <QFileDialog>
#include <QtDebug>
#include <QSettings>

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
