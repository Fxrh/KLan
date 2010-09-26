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
#include <QNetworkInterface>
#include <QTimerEvent>
#include <KDebug>

Client::Client(QObject *parent)
  : QObject(parent)
{
  m_connectList = new QList<ClientConnection*>();
  m_removeInactiveTimer = startTimer(60*1000);
  m_started = false;
}

Client::~Client()
{
  stop();
  qDeleteAll(*m_connectList);
  m_connectList->clear();
  delete m_connectList;
}

void Client::connectTo(const QString &hostIp, quint16 port)
{
  if( !m_started ){
    return;
  }
  //Check if hostIp is your Ip
//  if( QNetworkInterface::allAddresses().contains(QHostAddress(hostIp)) ){
//    kDebug() << "No connecting to yourself allowed!";
//    return;
//  }
  
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
  m_started = true;
  foreach( ClientConnection* connection, *m_connectList ){
    connection->startClient();
  }
}

void Client::stop()
{
  m_started = false;
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

void Client::sendMyName(const QString &name)
{
  foreach( ClientConnection* connection, *m_connectList ){
    connection->sendMessage(QString(connection::Mess_Name + " " + name));
  }
}

void Client::sendFileMessage(quint16 filePort, const QString& fileName, int fileSize, const QString& ip, quint16 port)
{
  ClientConnection* connection = findConnection( ip, port );
  if( !connection ){
    kDebug() << "Host/Ip not found: " << ip << port;
    return;
  }
  connection->sendMessage(QString(connection::Mess_File+" "+QString::number(filePort)+" "+QString::number(fileSize)+" "+fileName));
}

void Client::timerEvent(QTimerEvent *event)
{
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
