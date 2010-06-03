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

#ifndef CONNECTIONOBJECT_H
#define CONNECTIONOBJECT_H

#include <QObject>

class ConnectionObject : public QObject
{
    Q_OBJECT
  public:
    ConnectionObject( const QString& ip, quint16 server_port, quint16 client_port, bool connected );
    
    bool isConnected();
    bool hasServerInfo();
    bool hasClientInfo();
    const QString& getIp();
    quint16 getServerPort();
    quint16 getClientPort();
    void changeServerPort( quint16 server_port );
    void changeClientPort( quint16 client_port );
    void changeConnection( bool connected );
    
  signals:
    void sigChange(ConnectionObject* object);
    
  private:
    // the Ip the client and the server connect to
    QString m_ip;
    // the port the server connects to
    quint16 m_server_port;
    // the port the client connects to
    quint16 m_client_port;
    bool m_isConnected;
};

#endif
