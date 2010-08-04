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
#include "server.h"
#include "serverconnection.h"

#include <QTcpServer>
#include <QHostAddress>
#include <QList>
#include <QStringList>
#include <KDebug>

Server::Server(QObject *parent)
  : QObject(parent)
{
  m_started = false;
  m_port = 0;
  m_server = new QTcpServer(this);
  m_connectList = new QList<ServerConnection*>();
  connect( m_server, SIGNAL(newConnection()), this, SLOT(gotNewConnection()) );
}

Server::~Server()
{
  m_server->close();
  qDeleteAll(*m_connectList);
  delete m_connectList;
}

bool Server::startServer(quint16 port)
{
  if( m_started ){
    stopServer();
  }
  m_port = port;
  m_started = false;
  return startServer();
}

void Server::removeConnection(const QString &ip, quint16 port)
{
  ServerConnection* connection = findConnection(ip, port);
  if( connection ){
    connection->disconnect();
    m_connectList->removeOne(connection);
    delete connection;
    kDebug() << "removed " << ip << port;
  }
}

bool Server::startServer()
{
  if( m_started ){
    return true;
  }
  if( m_server->listen(QHostAddress::Any, m_port) ){
    m_started = true;
    kDebug() << "Started.";
    return true;
  } else {
    kWarning() << "Could not start server!";
    return false;
  }
}

void Server::stopServer()
{
  m_server->close();
  m_started = false;
  qDeleteAll(*m_connectList);
  m_connectList->clear();
  kDebug() << "Stopped.";
}

void Server::gotNewMessage()
{
  ServerConnection* connection = static_cast<ServerConnection*>(sender());
  if( connection ){
    foreach( QString message, *(connection->messages()) ){
      QString cmd = message.left( message.indexOf(' ') );
      if( cmd == connection::Mess_Ping ){
        kDebug() << "PING";
        connection->setActive();
        continue;
      }
      if( cmd == connection::Mess_Chat ){
        QString msg = message.right( message.length()-message.indexOf(' ')-1 );
        emit sigChatMessage(msg, connection->getIp(), connection->getPort() );
        kDebug() << "Chat:" << msg;
        continue;
      }
      if( cmd == connection::Mess_Short ){
        QString msg = message.right( message.length()-message.indexOf(' ')-1 );
        emit sigShortMessage(msg);
        kDebug() << "Short:" << msg;
        continue;
      }
      if( cmd == connection::Mess_Server ){
        QString msg = message.right( message.length()-message.indexOf(' ')-1 );
        quint16 port = (quint16) msg.toUInt();
        if( port <= 0 ){
          continue;
        }
        emit sigServer(port, connection->getIp(), connection->getPort() );
        continue;
      }
      if( cmd == connection::Mess_Name ){
        QString msg = message.right( message.length()-message.indexOf(' ')-1 );
        kDebug() << "Name:" << msg;
        emit sigName(msg, connection->getIp(), connection->getPort() );
      }
      kDebug() << "unknown command: " << cmd;
    }
    connection->clearMessages();
  }
}

void Server::gotNewConnection()
{
  kDebug();
  ServerConnection* newConnection = new ServerConnection( m_server->nextPendingConnection() );
  m_connectList->push_back(newConnection);
  emit sigNewConnection( newConnection->getIp(), newConnection->getPort() );
  connect( newConnection, SIGNAL(sigNewData()), this, SLOT(gotNewMessage()) );
}

void Server::lostConnection()
{
  ServerConnection* connection = static_cast<ServerConnection*>(sender());
  if( connection ){
    connection->disconnect();
    int index = m_connectList->indexOf(connection);
    delete m_connectList->at(index);
    m_connectList->removeAt(index);
  }
}

ServerConnection* Server::findConnection( const QString& hostIp, quint16 port )
{
  ServerConnection* connection = 0;
  foreach( ServerConnection* con, *m_connectList ){
    //kDebug() << con->getIp() << QString(con->getPort());
    if( (con->getIp() == hostIp) && (con->getPort() == port) ){
      connection = con;
      break;
    }
  }
  return connection;
}
