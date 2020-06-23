#ifndef EDITXETFORM_H
#define EDITXETFORM_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QItemSelection>

namespace Ui {
class EditSexForm;
}

class EditSexForm : public QDialog
{
    Q_OBJECT

public:
    explicit EditSexForm(QSqlDatabase db, QWidget *parent = nullptr);
    ~EditSexForm();

private slots:
    void on_pushButton_Close_clicked();

    void on_tableView_clicked(const QModelIndex &index);
    void slotSelectionChange(const QItemSelection &current, const QItemSelection &previous);


    void on_pushButton_Add_clicked();


    void on_pushButton_First_clicked();

    void on_pushButton_Previous_clicked();

    void on_pushButton_Next_clicked();

    void on_pushButton_Last_clicked();



    void on_lineEdit_Note_editingFinished();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButtonEdit_clicked();

    void on_lineEdit_FLT_textChanged(const QString &arg1);

    void on_pushButton_2_clicked();

private:
    Ui::EditSexForm *ui;
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // EDITSEXFORM_H
