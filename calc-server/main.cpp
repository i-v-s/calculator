#include <QCoreApplication>
#include "calcserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    CalcServer server(12345);
    return application.exec();
}
