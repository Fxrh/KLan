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

#include "serverconnection.h"

#include <QTcpSocket>
#include <QHostAddress>
#include <KDebug>

ServerConnection::ServerConnection(QTcpSocket *connection, QObject *parent)
  : QObject(parent),
    m_connection(connection)
{
  m_connected = true;
  m_hostIp = m_connection->peerAddress().toString();
  m_port = m_connection->peerPort();
}

ServerConnection::~ServerConnection()
{
  if( m_connected ){
    m_connection->deleteLater();
  }
}

bool ServerConnection::sendMessage(const QString &message)
{
  if( !m_connected || message.isEmpty() ){
    return false;
  }
  kDebug() << "Send data to " << m_connection->peerAddress().toString() << ":"
      << m_connection->peerPort() << " : " << message;
  QDataStream out( m_connection );
  out.setVersion(QDataStream::Qt_4_6);
  out << message;
  return true;
}

void ServerConnection::disconnect()
{
  m_connection->disconnectFromHost();
}

void ServerConnection::gotDisconnected()
{
  kDebug() << "got disconnected";
  m_connected = false;
  m_connection->deleteLater();
  m_connection = 0;
  emit sigDisconnect();
}
