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
#include "sendclient.h"

SendClient::SendClient(QObject* parent)
  : QObject(parent)
{
  m_file = 0;
  m_socket = new QTcpSocket(this);
}

bool SendClient::tryConnect(const QString& fileName, const QString& ip, quint16 port)
{
  if( m_file != 0 ){
    return false;
  }
  m_file = new QFile(fileName);
  if( !m_file->open(QIODevice::WriteOnly) ){
    return false;
  }
  m_socket->connectToHost(ip, port);
  connect( m_socket, SIGNAL(readyRead()), this, SLOT(gotData()) );
  connect( m_socket, SIGNAL(disconnected()), this, SLOT(finished()) );
  return true;
}

void SendClient::gotData()
{
  QDataStream inStream(m_socket);
  inStream.setVersion(QDataStream::Qt_4_6);
  QDataStream outStream(m_socket);
  outStream.setVersion(QDataStream::Qt_4_6);
  outStream << inStream;
}

void SendClient::finished()
{
  m_socket->deleteLater();
  m_file->close();
  m_file->deleteLater();
  this->deleteLater();
}
