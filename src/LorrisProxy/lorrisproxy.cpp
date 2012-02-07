/****************************************************************************
**
**    This file is part of Lorris.
**    Copyright (C) 2012 Vojtěch Boček
**
**    Contact: <vbocek@gmail.com>
**             https://github.com/Tasssadar
**
**    Lorris is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    Lorris is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with Lorris.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <QMessageBox>
#include <QTcpSocket>
#include <QHostAddress>

#include "lorrisproxy.h"
#include "tcpserver.h"

#include "ui_lorrisproxy.h"

LorrisProxy::LorrisProxy() : WorkTab(), ui(new Ui::LorrisProxy)
{
    m_server = new TcpServer();

    ui->setupUi(this);

    connect(ui->connectButton, SIGNAL(clicked()),            SLOT(connectButton()));
    connect(ui->addressEdit,   SIGNAL(textChanged(QString)), SLOT(updateAddressText()));
    connect(ui->portBox,       SIGNAL(valueChanged(int)),    SLOT(updateAddressText()));
    connect(ui->listenButon,   SIGNAL(clicked()),            SLOT(listenChanged()));
    connect(m_server,          SIGNAL(newConnection(QTcpSocket*,quint32)), SLOT(addConnection(QTcpSocket*,quint32)));
    connect(m_server,          SIGNAL(removeConnection(quint32)), SLOT(removeConnection(quint32)));

    ui->addressEdit->setText(sConfig.get(CFG_STRING_PROXY_ADDR));
    ui->portBox->setValue(sConfig.get(CFG_QUINT32_PROXY_PORT));
}

LorrisProxy::~LorrisProxy()
{
    delete m_server;
    delete ui;
}

void LorrisProxy::connectButton()
{
    if(!m_con->isOpen())
    {
        ui->connectButton->setText(tr("Connecting..."));
        ui->connectButton->setEnabled(false);
        connect(m_con, SIGNAL(connectResult(Connection*,bool)), this, SLOT(connectionResult(Connection*,bool)));
        m_con->OpenConcurrent();
    }
    else
    {
        m_con->Close();
        ui->connectButton->setText(tr("Connect to serial port"));
    }
}

void LorrisProxy::connectionResult(Connection */*con*/,bool result)
{
    disconnect(m_con, SIGNAL(connectResult(Connection*,bool)), this, 0);

    ui->connectButton->setEnabled(true);
    if(!result)
    {
        ui->connectButton->setText(tr("Connect to serial port"));
        showErrorBox(tr("Can't open connection!"));
    }
}

void LorrisProxy::connectedStatus(bool connected)
{
    if(connected)
        ui->connectButton->setText(tr("Disconnect from serial port"));
    else
        ui->connectButton->setText(tr("Connect to serial port"));
}

void LorrisProxy::setConnection(Connection *con)
{
    m_con = con;

    connect(m_con,    SIGNAL(dataRead(QByteArray)), m_server, SLOT(SendData(QByteArray)));
    connect(m_server, SIGNAL(newData(QByteArray)),  m_con,    SLOT(SendData(QByteArray)));
    connect(m_con,    SIGNAL(connected(bool)),      this,     SLOT(connectedStatus(bool)));

    m_con->AddUsingTab(m_id);
}

void LorrisProxy::updateAddressText()
{
    QString color = "color :";
    QString address;
    if(m_server->isListening())
    {
        color += "green";
        address = m_server->getAddress();
    }
    else
    {
        color += "red";
        address = ui->addressEdit->text();
    }

    QString text = address + ":" + ui->portBox->text();
    ui->addressLabel->setText(text);
    ui->addressLabel->setStyleSheet(color);
}

void LorrisProxy::listenChanged()
{
    if(m_server->isListening())
    {
        m_server->stopListening();
        ui->listenButon->setText(tr("Start listening"));
        ui->addressEdit->setEnabled(true);
        ui->portBox->setEnabled(true);
    }
    else
    {
        sConfig.set(CFG_STRING_PROXY_ADDR, ui->addressEdit->text());
        sConfig.set(CFG_QUINT32_PROXY_PORT, ui->portBox->value());

        if(m_server->listen(ui->addressEdit->text(), ui->portBox->value()))
        {
            ui->listenButon->setText(tr("Stop listening"));
            ui->addressEdit->setEnabled(false);
            ui->portBox->setEnabled(false);
        }
        else
        {
            QMessageBox box(this);
            box.setIcon(QMessageBox::Critical);
            box.setWindowTitle(tr("Error!"));
            box.setText(tr("Failed to start listening (%1)!").arg(m_server->getLastErr()));
            box.exec();
        }
    }
    updateAddressText();
}

void LorrisProxy::addConnection(QTcpSocket *connection, quint32 id)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(ui->connections);
    item->setText(0, QString::number(id));
    item->setText(1, connection->peerAddress().toString());
}

void LorrisProxy::removeConnection(quint32 id)
{
    QList<QTreeWidgetItem *> items = ui->connections->findItems(QString::number(id), Qt::MatchExactly);
    if(items.empty())
        return;
    delete items[0];
}