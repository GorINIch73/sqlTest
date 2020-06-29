#ifndef SETUPFORM_H
#define SETUPFORM_H

#include <QDialog>
#include <QSqlDatabase>


/* Defining */
#define ORGANIZATION_NAME "GORINICH"
#define ORGANIZATION_DOMAIN "local"
#define APPLICATION_NAME "Offoce"
#define SETTINGS_BASE_NAME "basename"

namespace Ui {
class SetupForm;
}

class SetupForm : public QDialog
{
    Q_OBJECT

public:
    explicit SetupForm(QWidget *parent = nullptr);
    ~SetupForm();

private slots:
    void on_pushButtonBrowse_clicked();

    void on_pushButtonClose_clicked();

    void on_pushButtonCreateTable_clicked();

    void on_pushButtonClear_clicked();

    void on_pushButton_Import_clicked();

private:
    Ui::SetupForm *ui;
    //QSqlDatabase dbm;

    // QString BaseName;

};

#endif // SETUPFORM_H
