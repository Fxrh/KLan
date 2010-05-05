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

#include "server.h"
#include "serverconnection.h"

#include <QTcpServer>
#include <QHostAddress>
#include <QList>
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

void Server::startServer(quint16 port)
{
  if( m_started ){
    stopServer();
  }
  m_port = port;
  m_started = false;
  startServer();
}

void Server::startServer()
{
  if( m_started ){
    return;
  }
  if( m_server->listen(QHostAddress::Any, m_port) ){
    m_started = true;
    kDebug() << "Started.";
  } else {
    kWarning() << "Could not start server!";
  }
}

void Server::stopServer()
{
  m_server->close();
  m_started = false;
  qDeleteAll(*m_connectList);
  kDebug() << "Stopped.";
}

void Server::gotNewConnection()
{
  kDebug();
  ServerConnection* newConnection = new ServerConnection( m_server->nextPendingConnection() );
  m_connectList->push_back(newConnection);
  emit sigNewConnection( newConnection->getIp(), newConnection->getPort() );
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
