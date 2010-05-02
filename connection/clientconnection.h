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

#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H

#include <QTcpSocket>

class ClientConnection : protected QTcpSocket
{
    Q_OBJECT
  public:
    ClientConnection( QObject* parent=0 );
    
    void startClient( const QString& hostIp, quint16 port );
    const QString& getIp();
    bool isConnected() { return m_connected; }
    bool isStarted() { return m_started; }
    const QStringList* messages() { return m_messageList; }
    void clearMessages();
    
  public slots:
    void startClient();
    void stopClient();
    void reconnect();
    
  protected:
    void timerEvent( QTimerEvent* event );
    
  signals:
    void sigConnect();
    void sigDisconnect();
    void sigNewData();
    
  private slots:
    void gotConnected();
    void gotDisconnected();
    void gotNewData();
    
  private:
    QStringList* m_messageList;
    bool m_connected;
    bool m_started;
    int m_reconnectTimer;
    bool m_directReconnect;
    QString m_hostIp;
    quint16 m_port;
};

#endif //CLIENTCONNECTION_H
