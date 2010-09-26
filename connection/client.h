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

//TODO: Error handling for broadcastSocket

#ifndef CLIENT_H
#define CLIENT_H

class ClientConnection;
class QUdpSocket;

#include <QObject>

class Client : public QObject
{
    Q_OBJECT
  public:
    Client( QObject* parent=0 );
    ~Client();
    
    void connectTo( const QString& hostIp, quint16 port );
    void start();
    void stop();
    
    void sendChatMessage(const QString& message, const QString& ip, quint16 port );
    void sendShortMessage( const QString& message, const QString& ip, quint16 port );
    void sendServerInfo( quint16 serverPort, const QString& ip, quint16 port );
    void sendMyName( const QString& name );
    void sendFileMessage( quint16 filePort, const QString& fileName, int fileSize, const QString& ip, quint16 port );
    
  signals:
    void newConnection( QString ip, quint16 port );
    void lostConnection( QString ip, quint16 port );
    
  protected:
    void timerEvent(QTimerEvent* event);
    
  private slots:
    void gotConnected();
    void gotDisconnected();
    
  private:
    ClientConnection* findConnection( const QString& hostIp, quint16 port );
    
    QList<ClientConnection*>* m_connectList;
    //bool m_broadcastStarted;
    int m_removeInactiveTimer;
    bool m_started;
};

#endif //CLIENT_H
