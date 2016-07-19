#include "calcthread.h"
#include "eval.h"
#include <QTextStream>
#include <QDataStream>
#include <QHostAddress>

class Response : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString status READ status WRITE setStatus)
    Q_PROPERTY(double result READ result  WRITE setResult)
    QString m_status;
    double m_result;
public:
    Response(const QString &status, double result) : m_status(status), m_result(result) { }
    void setStatus(const QString &value)
    {
        m_status = value;
    }
    QString status() const
    {
        return m_status;
    }
    double result() const
    {
        return m_result;
    }
    void setResult(double Result)
    {
        m_result = Result;
    }
};

CalcThread::CalcThread(int socketDescriptor, QObject *parent) :
    QThread(parent), textOut(stdout), socketDescriptor(socketDescriptor)
{

}

void CalcThread::run()
{
    int id = (int) QThread::currentThreadId();
    QTcpSocket tcpSocket;
    connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    dataStream.setDevice(&tcpSocket);
    dataStream.setVersion(QDataStream::Qt_5_7);

    if (!tcpSocket.setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket.error());
        return;
    }

    textOut << id << ": Incoming connection from " << tcpSocket.peerAddress().toString() << ":" << tcpSocket.peerPort() << endl;
    QDataStream outStream(&tcpSocket);//, QIODevice::WriteOnly);
    outStream.setVersion(QDataStream::Qt_5_7);
    outStream << QString("CONNECTED");

    while(tcpSocket.waitForReadyRead(-1))
    {
        QString result, status;
        dataStream.startTransaction();

        QString expr;
        dataStream >> expr;
        if (!dataStream.commitTransaction())
            return;
        try
        {
            result = calculate(expr);
            status = "OK";
            textOut << id << ": Calculate " << expr << " = " << result << endl;
        }
        catch(const std::logic_error &e)
        {
            textOut << id << ": Exception '" << e.what() << "' when evaluating " << expr << endl;
            result = e.what();
            status = "ERROR";
        }
        dataStream << status;
        dataStream << result;
    }
    textOut << id << ": Connection closed" << endl;
    tcpSocket.disconnectFromHost();
}

QString CalcThread::calculate(const QString &expression)
{
    return Eval(expression).toString();
}

void CalcThread::readData()
{
}

