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

#include <QObject>

#include "connectionobject.h"

class Server;
class Client;
//struct Connection;

class ConManager : public QObject
{
    Q_OBJECT
  public:
    enum Connected { connected, notConnected };
    ConManager( QObject* parent = 0 );
    ~ConManager();
    
    bool startServer( quint16 port );
    void stopServer();
    void tryConnect( const QString& ip, quint16 port );
    
  public slots:
    void sendChatMessage( QString message, ConnectionObject* connection );
    
  signals:
    void sigNewConnection( ConnectionObject* object );
    void sigChatMessage( QString message, ConnectionObject* connection );
    
  private slots:
    void serverGotConnected( QString ip, quint16 serverPort );
    void clientGotConnected( QString ip, quint16 clientPort );
    void clientGotDisconnected( QString ip, quint16 clientPort );
    void gotServerInfo( quint16 clientPort, QString ip, quint16 serverPort );
    void gotChatMessage( QString message, QString ip, quint16 serverPort );
    
  private:
    int findConnection(const QString& ip, quint16 port, bool portIsClient=true );
    
    Server* m_server;
    Client* m_client;
    
    QList<ConnectionObject*>* m_conList;
};

//struct Connection {
//  // the Ip the client and the server connect to
//  QString ip;
//  // the port the client connects to
//  quint16 client_port;
//  // the port the server connects to
//  quint16 server_port;
//  // state is true if the client port is set
//  ConManager::Connected conState;
//};
