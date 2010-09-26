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

#include <QFile>
#include <QTcpSocket>
#include <QApplication>
#include <KDebug>
#include "sendclient.h"

SendClient::SendClient(QObject* parent)
  : QObject(parent)
{
  m_file = 0;
  m_socket = new QTcpSocket(this);
  m_totalSize = 0;
  m_remainingSize = 0;
}

bool SendClient::tryConnect(const QString& fileName, int fileSize, const QString& ip, quint16 port)
{
  if( m_file != 0 ){
    return false;
  }
  m_file = new QFile(fileName);
  if( !m_file->open(QIODevice::WriteOnly) ){
    return false;
  }
  m_totalSize = fileSize;
  m_remainingSize = fileSize;
  m_socket->connectToHost(ip, port);
  connect( m_socket, SIGNAL(readyRead()), this, SLOT(gotData()) );
  connect( m_socket, SIGNAL(disconnected()), this, SLOT(finished()) );
  return true;
}

int SendClient::totalSize()
{
  if( m_file ){
    return m_totalSize;
  }
  return 0;
}

int SendClient::remainingSize()
{
  if( m_file ){
    return m_remainingSize;
  }
  return 0;
}

void SendClient::gotData()
{
  m_remainingSize -= m_file->write(m_socket->readAll());
}

void SendClient::finished()
{
  while( !m_socket->atEnd() ){
    qApp->processEvents();
  }
  m_remainingSize = 0;
  m_socket->close();
  m_file->close();
  kDebug();
}
