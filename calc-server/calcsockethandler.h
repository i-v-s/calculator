#ifndef CALCSOCKETHANDLER_H
#define CALCSOCKETHANDLER_H
#include <QObject>
#include <QTcpSocket>

//! Класс, обслуживающий одно соединение и выполняющий расчёты
class CalcSocketHandler : public QObject
{
    Q_OBJECT
    QTcpSocket tcpSocket;
    int socketDescriptor;
    int threadId; //!<< Идентификатор потока
    size_t receivingSize; //!< Ожидаемый размер данных
    QTextStream textOut; //!< Текстовый исходящий поток
    void sendResult(QString status, QString result = ""); //!< Метод отправки результата выражения
    void processExpression(const QString &expression); //!< Метод обработки выражения
public:
    CalcSocketHandler(int socketDescriptor, int threadId);
    ~CalcSocketHandler();
private slots:
    void error(QAbstractSocket::SocketError socketError);  //!< Слот обработки ошибок
    void readData(); //!< Слот приёма данных
};

#endif // CALCSOCKETHANDLER_H
