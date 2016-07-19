#ifndef CALCSERVER_H
#define CALCSERVER_H
#include <QTcpServer>
#include <QTextStream>

//! Класс сервера, слушающего сокет
class CalcServer : public QTcpServer
{
    Q_OBJECT
    QTextStream textOut; //!< Текстовый исходящий поток
public:
    CalcServer(quint16 port, QObject *parent = 0);
protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
};

#endif // CALCSERVER_H
