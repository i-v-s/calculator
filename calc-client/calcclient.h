#ifndef CALCCLIENT_H
#define CALCCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>

//! Класс, ответственный за связь с сервером
class CalcClient : public QObject
{
    Q_OBJECT
    QTcpSocket tcpSocket;
    size_t receivingSize; //!< Ожидаемый размер данных
public:
    explicit CalcClient(QObject *parent = 0);
    ~CalcClient();
    void connectRemote(const QString &host, int port); //!< Метод подключения к серверу
    void disconnect(); //!< Метод отключения от сервера
    QString peer(); //!< Метод, возвращающий имя и порт сервера
signals:
    void onResult(const QString &result); //!< Сигнал готовности результата
    void onCalcError(const QString &message); //!< Сигнал ошибки расчёта
    void onSocketError(const QString &message); //!< Cигнал ошибки соединения
public slots:
    void sendExpression(const QString &expression); //!< Слот отправки выражения для расчёта
private slots:
    void error(QAbstractSocket::SocketError socketError); //!< Слот обработки ошибок сокета
    void readData(); //!< Слот приёма данных
};

#endif // CALCCLIENT_H
