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
class QUdpSocket;
//struct Connection;

class ConManager : public QObject
{
    Q_OBJECT
  public:
    enum Connected { connected, notConnected };
    ConManager( QObject* parent = 0 );
    ~ConManager();
    
    bool start();
    void stop();
    void setServerPort( quint16 port );
    void tryConnect( const QString& ip, quint16 port );
    void changeBroadcastPort( quint16 port );
    void sendMyName( const QString& name );
    
  public slots:
    void sendChatMessage( QString message, ConnectionObject* connection );
    void sendShortMessage( QString message, ConnectionObject* connection );
    void sendFileMessage( quint16 filePort, QString fileName, int fileSize, ConnectionObject* connection );
    bool startBroadcast();
    void stopBroadcast();
    
protected:
    void timerEvent(QTimerEvent* event);
    
  signals:
    void sigNewConnection( ConnectionObject* object );
    void sigConnectionUpdated( ConnectionObject* object );
    void sigChatMessage( QString message, ConnectionObject* connection );
    void sigShortMessage( QString message, ConnectionObject* connection );
    void sigFile( quint16 filePort, QString fileName, int fileSize, ConnectionObject* connection );
    
  private slots:
    void serverGotConnected( QString ip, quint16 serverPort );
    void clientGotConnected( QString ip, quint16 clientPort );
    void clientGotDisconnected( QString ip, quint16 clientPort );
    void gotBroadcastData();
    void gotServerInfo( quint16 clientPort, QString ip, quint16 serverPort );
    void gotChatMessage( QString message, QString ip, quint16 serverPort );
    void gotShortMessage( QString message, QString ip, quint16 serverPort );
    void gotName( QString name, QString ip, quint16 serverPort );
    void gotFile( quint16 filePort, QString fileName, int fileSize, QString ip, quint16 serverPort );
    
  private:
    int findConnection(const QString& ip, quint16 port, bool portIsClient=true );
    
    Server* m_server;
    Client* m_client;
    QUdpSocket* m_broadcastSocket;
    quint16 m_broadcastPort;
    int m_broadcastTimer;
    bool m_isBroadcastStarted;
    
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
