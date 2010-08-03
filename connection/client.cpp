/**************************************************************************
 *                                                                        *
 * Copyright (C) 2010 Felix Rohrbach <fxrh@gmx.de>                        *
 *                                                                        *
 * This program is free software; you can redistribute it and/or          *
 * modify it under the terms of the GNU General Public License            *
 * as published by the Free Software Foundation; either version 3         *
 * of the License, or (at your option) any later version.                 *
 *                                                                        *
 * This program is distributed in the hope that it will be useful,        *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU General Public License for more details.                           *
 *                                                                        *
 * You should have received a copy of the GNU General Public License      *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                        *
 **************************************************************************/ 

#include "def.h"
#include "client.h"
#include "clientconnection.h"

#include <QUdpSocket>
#include <QHostAddress>
#include <QTimerEvent>
#include <KDebug>

Client::Client(QObject *parent)
  : QObject(parent)
{
  m_broadcastStarted = false;
  m_broadcastSocket = new QUdpSocket(this);
  m_connectList = new QList<ClientConnection*>();
  m_broadcastTimer = 0;
  connect( m_broadcastSocket, SIGNAL(readyRead()), this, SLOT(gotBroadcastData()) );
  m_removeInactiveTimer = startTimer(60*1000);
}

Client::~Client()
{
  stop();
  qDeleteAll(*m_connectList);
  m_connectList->clear();
  delete m_connectList;
}

void Client::startBroadcast(quint16 port)
{
  if( m_broadcastStarted ){
    stopBroadcast();
  }
  m_broadcastPort = port;
  startBroadcast();
}

void Client::connectTo(const QString &hostIp, quint16 port)
{
  ClientConnection* connection = findConnection( hostIp, port );
  if( connection == 0 ){
    kDebug() << "Creating new client";
    connection = new ClientConnection(hostIp, port);
    connection->startClient();
    m_connectList->push_back(connection);
    connect( connection, SIGNAL(sigConnect()), this, SLOT(gotConnected()) );
    connect( connection, SIGNAL(sigDisconnect()), this, SLOT(gotDisconnected()) );
  } else {
    connection->reconnect();
  }
}

void Client::start()
{
  foreach( ClientConnection* connection, *m_connectList ){
    connection->startClient();
  }
}

void Client::stop()
{
  foreach( ClientConnection* connection, *m_connectList ){
    connection->stopClient();
  }
}

void Client::sendChatMessage(const QString &message, const QString &ip, quint16 port)
{
  ClientConnection* connection = findConnection( ip, port );
  if( !connection ){
    kDebug() << "Host/Ip not found: " << ip << port;
    return;
  }
  connection->sendMessage( connection::Mess_Chat + " " + message);
}

void Client::sendShortMessage(const QString &message, const QString &ip, quint16 port)
{
  ClientConnection* connection = findConnection( ip, port );
  if( !connection ){
    kDebug() << "Host/Ip not found: " << ip << port;
    return;
  }
  connection->sendMessage( connection::Mess_Short + " " + message );
}

void Client::sendServerInfo(quint16 serverPort, const QString &ip, quint16 port)
{
  ClientConnection* connection = findConnection( ip, port );
  if( !connection ){
    kDebug() << "Host/Ip not found: " << ip << port;
    return;
  }
  connection->sendMessage( connection::Mess_Server + " " + QString::number(serverPort));
}

void Client::startBroadcast()
{
  if( m_broadcastStarted ){
    return;
  }
  m_broadcastSocket->bind(m_broadcastPort);
  m_broadcastSocket->writeDatagram(QByteArray("KLan"), QHostAddress::Broadcast, m_broadcastPort);
  m_broadcastTimer = startTimer(5*60*1000);
}

void Client::stopBroadcast()
{
  if( !m_broadcastStarted ){
    return;
  }
  m_broadcastSocket->close();
  killTimer(m_broadcastTimer);
  m_broadcastTimer = 0;
}

void Client::timerEvent(QTimerEvent *event)
{
  if( event->timerId() == m_broadcastTimer && m_broadcastStarted ){
    m_broadcastSocket->writeDatagram(QByteArray("KLan"), QHostAddress::Broadcast, m_broadcastPort);
  }
  if( event->timerId() == m_removeInactiveTimer ){
    kDebug() << "Connections: " << m_connectList->count();
    foreach( ClientConnection* con, *m_connectList ){
      if( con->isInactive() ){
        kDebug() << "Removing inactive connection.";
        m_connectList->removeOne(con);
        delete con;
      }
    }
  }
}

void Client::gotBroadcastData()
{
  while( m_broadcastSocket->hasPendingDatagrams() ){
    QByteArray datagram;
    datagram.resize(m_broadcastSocket->pendingDatagramSize());
    QHostAddress sender;
    quint16 senderPort;

    m_broadcastSocket->readDatagram(datagram.data(), datagram.size(),
                                    &sender, &senderPort);
    if( QString(datagram) == "KLan" ){
      //connectTo( sender.toString(), senderPort );
    }
  }
}

void Client::gotConnected()
{
  ClientConnection* connection = static_cast<ClientConnection*>(sender());
  if( connection ){
    emit newConnection( connection->getIp(), connection->getPort() );
  }
}

void Client::gotDisconnected()
{
  ClientConnection* connection = static_cast<ClientConnection*>(sender());
  if( connection ){
    emit lostConnection(connection->getIp(), connection->getPort());
  }
}

ClientConnection* Client::findConnection(const QString &hostIp, quint16 port)
{
  ClientConnection* connection = 0;
  foreach( ClientConnection* con, *m_connectList ){
    if( (con->getIp() == hostIp) && (con->getPort() == port) ){
      connection = con;
      break;
    }
  }
  return connection;
}
