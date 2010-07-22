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

#include "conmanager.h"

#include <QTimer>
#include <KDebug>
#include "server.h"
#include "client.h"

ConManager::ConManager( QObject* parent )
  : QObject(parent)
{
  m_server = new Server(this);
  m_client = new Client(this);
  m_conList = new QList<ConnectionObject*>();
  
  m_server->startServer(22786);
  m_client->connectTo("127.0.0.1", 67352);
  
  connect( m_server, SIGNAL(sigNewConnection(QString,quint16)), this, SLOT(serverGotConnected(QString,quint16)) );
  connect( m_client, SIGNAL(newConnection(QString,quint16)), this, SLOT(clientGotConnected(QString,quint16)) );
  connect( m_client, SIGNAL(lostConnection(QString,quint16)), this, SLOT(clientGotDisconnected(QString,quint16)) );
  connect( m_server, SIGNAL(sigServer(quint16,QString,quint16)), this, SLOT(gotServerInfo(quint16,QString,quint16)) );
}

ConManager::~ConManager()
{
  kDebug();
  qDeleteAll(*m_conList);
  delete m_conList;
  kDebug() << "Deleted.";
}

void ConManager::startServer(quint16 port)
{
  m_server->startServer(port);
}

void ConManager::tryConnect(const QString &ip, quint16 port)
{
  m_client->connectTo(ip, port);
}

void ConManager::serverGotConnected(QString ip, quint16 port)
{
  kDebug() << ip << port;
  if( findConnection(ip, port, false) != -1 ){
    return;
  }
  ConnectionObject* conn = new ConnectionObject(ip, port, 0, false); //{ ip, 0, port, notConnected };
  m_conList->push_back(conn);
  emit sigNewConnection(conn);
}

void ConManager::clientGotConnected(QString ip, quint16 port)
{
  int id = findConnection(ip, port);
  if( id != -1 ){
//    if( m_conList->at(id).conState == notConnected ){
//      (*m_conList)[id].conState = connected;
//      kDebug() << "Connection online:" << ip << port;
//      m_client->sendServerInfo( m_server->serverPort(), ip, port );
//    }
    if( !m_conList->at(id)->isConnected() ){
      m_conList->at(id)->changeConnection(true);
      kDebug() << "Connection online:" << ip << port;
      m_client->sendServerInfo( m_server->serverPort(), ip, port );
    }
    return;
  }
  ConnectionObject* conn = new ConnectionObject(ip, 0, port, true ); //{ ip, port, 0, connected };
  m_conList->push_back(conn);
  emit sigNewConnection(conn);
  m_client->sendServerInfo( m_server->serverPort(), ip, port );
}

void ConManager::clientGotDisconnected(QString ip, quint16 port)
{
  int id = findConnection( ip, port );
  if( id == -1 ){
    return;
  }
  if( m_conList->at(id)->getServerPort() != 0 ){
    m_server->removeConnection( m_conList->at(id)->getIp(), m_conList->at(id)->getServerPort() );
  }
  delete m_conList->at(id);
  m_conList->removeAt(id);
}

void ConManager::gotServerInfo(quint16 serverPort, QString ip, quint16 port)
{
  kDebug() << "serverPort:" << serverPort << "Ip:" << ip << "Port:" << port;
  int newId = findConnection( ip, port, false );
  if( newId == -1 ){
    kDebug() << "You don't exist, go away!" << ip << port;
    return;
  }
  int oldId = findConnection( ip, serverPort );
  if( oldId != -1 ){
    // we already have a connection to that server
    delete m_conList->at(newId);
    m_conList->removeAt(newId);
    if( m_conList->at(oldId)->getServerPort() == 0 ){
      (*m_conList)[oldId]->changeServerPort(port);
      kDebug() << "Added Server to client connection";
    }
  } else {
    (*m_conList)[newId]->changeClientPort(serverPort); // client_port is the port the client connects to!!
    m_client->connectTo(ip, serverPort );
    kDebug() << "Added Client to server connection";
  }
}

int ConManager::findConnection(const QString &ip, quint16 port, bool portIsClient)
{
  if( portIsClient ){
    for( int i=0; i<m_conList->count(); ++i ){
      if( m_conList->at(i)->getIp() == ip && m_conList->at(i)->getClientPort() == port ){
        return i;
      }
    }
  } else {
    for( int i=0; i<m_conList->count(); ++i ){
      //kDebug() << "Ip:" << m_conList->at(i)->getIp() << "server_port:" << m_conList->at(i)->getServerPort();
      if( m_conList->at(i)->getIp() == ip && m_conList->at(i)->getServerPort() == port ){
        return i;
      }
    }
  }
  //kDebug() << "Could not find the Connection";
  return -1;
}
