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

//TODO: error handling for server

#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class QTcpServer;
class ServerConnection;

class Server : public QObject
{
    Q_OBJECT
  public:
    Server( QObject* parent=0 );
    ~Server();
    
    void setPort( quint16 port );
    void removeConnection( const QString& ip, quint16 port );
    quint16 serverPort() const { return m_port; } 
    bool isStarted() const { return m_started; }
    
  public slots:
    bool startServer();
    void stopServer();
    
  signals:
    void sigNewConnection( QString hostIp, quint16 port );
    void sigChatMessage( QString message, QString hostIp, quint16 serverPort );
    void sigShortMessage( QString message );
    void sigServer( quint16 clientPort, QString ip, quint16 serverPort );
    void sigName( QString name, QString hostIp, quint16 serverPort );
    
  private slots:
    void gotNewMessage();
    void gotNewConnection();
    void lostConnection();
    
  private:
    ServerConnection* findConnection( const QString& hostIp, quint16 port );
    
    QTcpServer* m_server;
    QList<ServerConnection*>* m_connectList;
    quint16 m_port;
    bool m_started;
};

#endif //SERVER_H
