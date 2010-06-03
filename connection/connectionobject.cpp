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

#include "connectionobject.h"

ConnectionObject::ConnectionObject(const QString &ip, quint16 server_port, quint16 client_port, bool connected)
{
  m_ip = ip;
  m_server_port = server_port;
  m_client_port = client_port;
  m_isConnected = connected;
}

bool ConnectionObject::isConnected()
{
  return m_isConnected;
}

bool ConnectionObject::hasClientInfo()
{
  return ( m_client_port != 0 );
}

bool ConnectionObject::hasServerInfo()
{
  return ( m_server_port != 0 );
}

const QString& ConnectionObject::getIp()
{
  return m_ip;
}

quint16 ConnectionObject::getServerPort()
{
  return m_server_port;
}

quint16 ConnectionObject::getClientPort()
{
  return m_client_port;
}

void ConnectionObject::changeServerPort(quint16 server_port)
{
  if( m_server_port == server_port )
    return;
  m_server_port = server_port;
  emit sigChange(static_cast<ConnectionObject*>(this));
}

void ConnectionObject::changeClientPort(quint16 client_port)
{
  if( m_client_port == client_port )
    return;
  m_client_port = client_port;
  emit sigChange(static_cast<ConnectionObject*>(this)); 
}

void ConnectionObject::changeConnection(bool connected)
{
  if( m_isConnected == connected )
    return;
  m_isConnected = connected;
  emit sigChange(const_cast<ConnectionObject*>(this));
}
