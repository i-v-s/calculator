#include "calcsockethandler.h"
#include "eval.h"
#include <QHostAddress>
#include <QDataStream>

CalcSocketHandler::CalcSocketHandler(int socketDescriptor, int threadId) :
    socketDescriptor(socketDescriptor),
    threadId(threadId),
    receivingSize(0),
    textOut(stdout)
{
    connect(&tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(&tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));

    if(!tcpSocket.setSocketDescriptor(socketDescriptor))
    {
        emit error(tcpSocket.error());
        return;
    }

    textOut << threadId << ": Incoming connection from " << tcpSocket.peerAddress().toString() << ":" << tcpSocket.peerPort() << endl;
    sendResult("CONNECTED");
}

CalcSocketHandler::~CalcSocketHandler()
{
    tcpSocket.disconnectFromHost();
}

void CalcSocketHandler::sendResult(QString status, QString result)
{
    QByteArray bytes;
    QDataStream outStream(&bytes, QIODevice::WriteOnly);
    outStream.setVersion(QDataStream::Qt_5_7);
    outStream << status;
    outStream << result;
    quint16 size = bytes.size();
    tcpSocket.write(reinterpret_cast<char *>(&size), sizeof(size));
    tcpSocket.write(bytes);
}

void CalcSocketHandler::processExpression(const QString &expression)
{
    QString result, status;
    try
    {
        result = Eval(expression).toString();
        status = "OK";
        textOut << threadId << ": Calculate " << expression << " = " << result << endl;
    }
    catch(const std::logic_error &e)
    {
        textOut << threadId << ": Exception '" << e.what() << "' when evaluating " << expression << endl;
        result = e.what();
        status = "ERROR";
    }
    sendResult(status, result);
}

void CalcSocketHandler::error(QAbstractSocket::SocketError socketError)
{
    switch(socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        textOut << threadId << ": Connection closed by remote host" << endl;
        break;
    default:
        textOut << threadId << ": Socket error: " << tcpSocket.errorString() << endl;
    }
    tcpSocket.abort();
}

void CalcSocketHandler::readData()
{
    while(size_t available = tcpSocket.bytesAvailable())
        if(receivingSize)
        {
            if(available < sizeof(receivingSize)) break;
            QByteArray bytes = tcpSocket.read(receivingSize);
            if((size_t) bytes.size() != receivingSize)
            {
                textOut << threadId << ": Socket read error" << endl;
                tcpSocket.abort();
                break;
            }
            QDataStream inputStream(bytes);
            inputStream.setVersion(QDataStream::Qt_5_7);
            QString expression;
            inputStream >> expression;
            processExpression(expression);
            receivingSize = 0;
        }
        else
        {
            quint16 size;
            if(available < sizeof(size)) break;
            if(tcpSocket.read(reinterpret_cast<char *>(&size), sizeof(size)) != sizeof(size))
            {
                textOut << threadId << ": Socket read error" << endl;
                tcpSocket.abort();
                break;
            }
            receivingSize = size;
        }
}
