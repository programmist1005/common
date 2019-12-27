#ifndef SOCKETADAPTER_H
# define SOCKETADAPTER_H

#include <QVector>
#include <QTcpSocket>
#include <QDataStream>

#include "isocketadapter.h"

class QTcpSocket;
class SocketAdapter : public ISocketAdapter
{
    Q_OBJECT
public:
    SocketAdapter(QObject *parent, QTcpSocket *pSock = nullptr);
    virtual ~SocketAdapter();
    virtual void sendString(const QString& str);
protected slots:
    void onReadyRead();
    void onDisconnected();
protected:
    QTcpSocket *tcpSocket;
    qint16 msgSize;
};

#endif // SOCKETADAPTER_H
