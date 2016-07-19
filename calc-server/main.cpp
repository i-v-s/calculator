#include <QCoreApplication>
#include "calcserver.h"
#include "eval.h"

int main(int argc, char *argv[])
{
    /*QString t[] = {"2+3", "2*--5", "2*3+3*2", "-2*-3*--2", "-9*7-2", "---", "0/0", "2+3*3+2"};
    for(int x = 0; x < sizeof(t) / sizeof(*t); x++)
    {
        try
        {
            qDebug("%s = %s", t[x].toStdString().data(), Eval(t[x]).toString().toStdString().data());
        }
        catch(const std::logic_error &e)
        {
            qDebug("%s: %s", t[x].toStdString().data(), e.what());
        }
    }*/
    //QString r = calculate("2*-5*3");
    QCoreApplication a(argc, argv);
    CalcServer server(12345);
    return a.exec();
}
