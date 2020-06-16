#include "mainwidget.h"
#include "setupform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(ORGANIZATION_NAME);
    QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN);
    QCoreApplication::setApplicationName(APPLICATION_NAME);


    QApplication a(argc, argv);
    MainWidget w;
    w.show();
    return a.exec();
}
