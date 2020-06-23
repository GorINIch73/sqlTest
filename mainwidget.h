#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QSqlRelation>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWidget; }
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();


private slots:
    void on_pushButton_AddDepartment_clicked();

    void on_pushButton_DelDepartment_clicked();

    void on_pushButton_AddWorkers_clicked();

    void on_pushButton_DelWorkers_clicked();

    void on_pushButton_Re_clicked();

    void on_pushButton_Exit_clicked();

    void on_tableView_Department_clicked(const QModelIndex &index);

    void on_lineEditFlt_textChanged(const QString &arg1);

    void on_lineEditFltW_textChanged(const QString &arg1);

    void on_pushButtonPrint_clicked();

    void on_pushButtonPdf_clicked();

    void on_pushButtonPrntPdf_clicked();

    void on_pushButtonSex_clicked();

    void on_pushButtonSetup_clicked();

private:

    Ui::MainWidget *ui;

    QString DatabaseName;
    QSqlDatabase db;
    QSqlTableModel *modelDepartment;
    QSqlRelationalTableModel *modelWorkers;
    QSqlRelationalDelegate *Workers_d;

    bool OpenBase();
    void SetupTable();
    void SetupDepartment();
    void SetupWorkers();


};
#endif // MAINWIDGET_H
