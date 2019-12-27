#include "socketadapter.h"
//----------------------------------------------------------------------------------------------------------------------------------
SocketAdapter::SocketAdapter(QObject *parent, QTcpSocket *pSock) : ISocketAdapter(parent), msgSize(-1)
{
    if (nullptr == pSock)
        tcpSocket = new QTcpSocket(this);
    else
        tcpSocket = pSock;
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
}
//----------------------------------------------------------------------------------------------------------------------------------
SocketAdapter::~SocketAdapter()
{

}
//----------------------------------------------------------------------------------------------------------------------------------
void SocketAdapter::sendString(const QString& str)
{
    QByteArray block;
    QDataStream sendStream(&block, QIODevice::ReadWrite);
    sendStream << quint16(0) << str;
    sendStream.device()->seek(0);
    sendStream << static_cast<quint16>(static_cast<quint16>(block.size()) - sizeof(quint16));
    tcpSocket->write(block);
}
//----------------------------------------------------------------------------------------------------------------------------------
void SocketAdapter::onReadyRead()
{
    QString buff;
    QDataStream stream(tcpSocket);

    while(true){
        if (msgSize < 0){
            if (tcpSocket->bytesAvailable() < static_cast<unsigned>(sizeof(int)))
                return;
            stream >> msgSize;
        }else{
            if (tcpSocket->bytesAvailable() < msgSize)
                return;
            stream >> buff;
            emit message(buff);
            msgSize = -1;
        }
    }
}
//----------------------------------------------------------------------------------------------------------------------------------
void SocketAdapter::onDisconnected()
{
    tcpSocket->deleteLater();
    emit disconnected();
}
//----------------------------------------------------------------------------------------------------------------------------------
