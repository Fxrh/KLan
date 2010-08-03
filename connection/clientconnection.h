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

//TODO: error handling

#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QObject>

class QTcpSocket;

class ClientConnection : public QObject
{
    Q_OBJECT
  public:
    ClientConnection( const QString& hostIp, quint16 port, QObject* parent=0 );
    
    const QString& getIp();
    quint16 getPort() { return m_port; }
    bool isConnected() { return m_connected; }
    bool isStarted() { return m_started; }
    bool isInactive() { return m_isInactive; }
    bool sendMessage( const QString& message );
    
  public slots:
    void startClient();
    void stopClient();
    void reconnect();
    
  protected:
    void timerEvent( QTimerEvent* event );
    
  signals:
    void sigConnect();
    void sigDisconnect();
    
  private slots:
    void gotConnected();
    void gotDisconnected();
    
  private:
    QTcpSocket* m_socket;
    bool m_connected;
    bool m_started;
    bool m_isInactive;
    int m_reconnectTimer;
    int m_inactiveTimer;
    bool m_directReconnect;
    QString m_hostIp;
    quint16 m_port;
    int m_pingTimer;
};

#endif //CLIENTCONNECTION_H
