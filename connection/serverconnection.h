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

#ifndef SERVERCONNECTION_H
#define SERVERCONNECTION_H

#include <QObject>
#include <QString>

class QTcpSocket;

class ServerConnection : public QObject
{
    Q_OBJECT
  public:
    explicit ServerConnection( QTcpSocket* connection, QObject* parent=0 );
    ~ServerConnection();
    
    bool isConnected() { return m_connected; }
    const QString& getIp() { return m_hostIp; }
    int getPort() { return m_port; }
    bool sendMessage( const QString& message );
    
  public slots:
    void disconnect();
    
  signals:
    void sigDisconnect();
    
  private slots:
    void gotDisconnected();
    
  private:
    QTcpSocket* m_connection;
    bool m_connected;
    QString m_hostIp;
    quint16 m_port;
};

#endif //SERVERCONNECTION_H
