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
#include <QTimerEvent>
#include <QStringList>
#include <KDebug>

ServerConnection::ServerConnection(QTcpSocket *connection, QObject *parent)
  : QObject(parent),
    m_connection(connection)
{
  m_messageList = new QStringList();
  m_connected = true;
  m_hostIp = m_connection->peerAddress().toString();
  m_port = m_connection->peerPort();
  m_active = true;
  m_activeTimer = startTimer(2*60000);
  
  kDebug() << m_port;
  connect( m_connection, SIGNAL(readyRead()), this, SLOT(gotNewData()) );
  // let's see if we already got some data
  gotNewData();
}

ServerConnection::~ServerConnection()
{
  if( m_connected ){
    m_connection->deleteLater();
  }
}

void ServerConnection::clearMessages()
{
  m_messageList->clear();
}

void ServerConnection::disconnect()
{
  m_connection->disconnectFromHost();
}

void ServerConnection::timerEvent(QTimerEvent *event)
{
  if( event->timerId() == m_activeTimer ){
    if( !m_active ){
      kDebug() << "Removing inactive server";
      killTimer(m_activeTimer);
      disconnect();
    } else {
      m_active = false;
    }
  }
}

void ServerConnection::gotDisconnected()
{
  kDebug() << "got disconnected";
  m_connected = false;
  m_connection->deleteLater();
  m_connection = 0;
  emit sigDisconnect();
}

void ServerConnection::gotNewData()
{
  QString message;
  QDataStream in(m_connection);
  in.setVersion(QDataStream::Qt_4_6);
  while( true ){
    in >> message;
    //kDebug() << "Got data: " << message;
    if( !message.isEmpty() ){
      m_messageList->append(message);
      emit sigNewData();
      continue;
    }
    break;
  }
}
