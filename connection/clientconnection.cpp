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

#include "clientconnection.h"

#include <QStringList>
#include <QHostAddress>
#include <QTimerEvent>
#include <KDebug>

ClientConnection::ClientConnection(QObject *parent)
  : QTcpSocket(parent)
{
  m_connected = false;
  m_started = false;
  m_directReconnect = false;
  m_isInactive = false;
  m_reconnectTimer = 0;
  m_inactiveTimer = 0;
  m_port = 0;
  m_messageList = new QStringList();
  
  connect( this, SIGNAL(connected()), this, SLOT(gotConnected()) );
  connect( this, SIGNAL(disconnected()), this, SLOT(gotDisconnected()) );
  
  m_inactiveTimer = startTimer(2*60*1000);
}

void ClientConnection::startClient(const QString &hostIp, quint16 port)
{
  m_hostIp = hostIp;
  m_port = port;
  if( m_started ){
    stopClient();
    m_directReconnect = true;
  } else {
    startClient();
  }
}

const QString& ClientConnection::getIp()
{
  return m_hostIp;
}

void ClientConnection::clearMessages()
{
  m_messageList->clear();
}

void ClientConnection::startClient()
{
  if( m_started ){
    return;
  }
  if( m_port == 0 ){
    kDebug() << "No port set.";
    return;
  }
  this->connectToHost(QHostAddress(m_hostIp), m_port);
  m_reconnectTimer = startTimer(15000);
  m_started = true;
  kDebug() << "Started: " << m_hostIp << ":" << m_port;
}

void ClientConnection::stopClient()
{
  if( !m_started ){
    return;
  }
  this->disconnectFromHost();
  if( m_reconnectTimer != 0 ){
    killTimer(m_reconnectTimer);
    m_reconnectTimer = 0;
  }
  m_started = false;
  kDebug() << "Stopped: " << m_hostIp << ":" << m_port;
}

void ClientConnection::reconnect()
{
  if( m_started && !m_connected ){
    this->connectToHost(QHostAddress(m_hostIp), m_port);
  }
}

void ClientConnection::timerEvent(QTimerEvent *event)
{
  if( event->timerId() == m_reconnectTimer ){
    reconnect();
  }
  if( event->timerId() == m_inactiveTimer ){
    m_isInactive = true;
  }
}

void ClientConnection::gotConnected()
{
  kDebug() << "Connected to " << this->peerAddress().toString() << ":" << m_port;
  m_connected = true;
  emit sigConnect();
  killTimer(m_inactiveTimer);
  m_inactiveTimer = 0;
  m_isInactive = false;
}

void ClientConnection::gotDisconnected()
{
  kDebug() << "Disconnected from " << m_hostIp << ":" << m_port;
  m_connected = false;
  emit sigDisconnect();
  if( m_directReconnect ){
    startClient();
    m_directReconnect = false;
  }
  m_inactiveTimer = startTimer(2*60*1000);
}

void ClientConnection::gotNewData()
{
  QString message;
  QDataStream in(this);
  in.setVersion(QDataStream::Qt_4_6);
  while( !message.isEmpty() ){
    in >> message;
    kDebug() << "Got data: " << message;
    if( !message.isEmpty() ){
      m_messageList->append(message);
      emit sigNewData();
    }
  }
}
