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
    
    void startBroadcast( quint16 port );
    void connectTo( const QString& hostIp, quint16 port );
    void disconnectAll();
    
  public slots:
    void startBroadcast();
    void stopBroadcast();
    
  signals:
    void sigChatMessage( QString message );
    void sigShortMessage( QString message );
    
  protected:
    void timerEvent(QTimerEvent* event);
    
  private slots:
    void gotNewMessage();
    void gotBroadcastData();
    
  private:
    ClientConnection* findConnection( const QString& hostIp, quint16 port );
    
    QList<ClientConnection*>* m_connectList;
    QUdpSocket* m_broadcastSocket;
    bool m_broadcastStarted;
    quint16 m_broadcastPort;
    int m_broadcastTimer;
    int m_removeInactiveTimer;
};

#endif //CLIENT_H
