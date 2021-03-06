/**********************************************
**    This file is part of Lorris
**    http://tasssadar.github.com/Lorris/
**
**    See README and COPYING
***********************************************/

#ifndef SHUPITOTUNNEL_H
#define SHUPITOTUNNEL_H

#include "connection.h"

class Shupito;

class ShupitoTunnel : public PortConnection
{
    Q_OBJECT

public:
    static QString getCompanionName();

    ShupitoTunnel();

    void SendData(const QByteArray &data);

    void setShupito(Shupito* s);

    bool canSaveToSession() const { return m_shupito != NULL; }

protected:
    ~ShupitoTunnel();
    void doOpen();
    void doClose();

private:
    Shupito *m_shupito;
    bool dataSigConnected;
};

#endif // SHUPITOTUNNEL_H
