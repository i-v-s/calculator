#ifndef CALCTHREAD_H
#define CALCTHREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QDataStream>

//! Класс потока, обслуживающего одно соединение и выполняющего расчёты
class CalcThread : public QThread
{
    Q_OBJECT
    QTextStream textOut; //!< Текстовый исходящий поток
    QDataStream dataStream;
public:
    CalcThread(int socketDescriptor, QObject *parent);
    void run() Q_DECL_OVERRIDE;
signals:   
    void error(QTcpSocket::SocketError socketError);
private:
    int socketDescriptor;
    QString calculate(const QString &expression); //!< Метод расчёта выражения
private slots:
    void readData();
};
#endif // CALCTHREAD_H
