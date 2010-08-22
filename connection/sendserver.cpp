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

#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QDebug>
#include "sendserver.h"

SendServer::SendServer(QObject* parent)
  : QObject(parent)
{
  m_server = new QTcpServer(this);
  m_socket = 0;
  m_file = 0;
}

quint16 SendServer::start(const QString& fileName)
{
  if( m_file != 0 ){
    return 0;
  }
  m_file = new QFile(fileName);
  if( !m_file->open(QIODevice::ReadOnly) ){
    return 0;
  }
  if( !m_server->listen() ){
    return 0;
  }
  connect( m_server, SIGNAL(newConnection()), this, SLOT(gotConnection()) );
  return m_server->serverPort();
}

void SendServer::gotConnection()
{
  m_socket = m_server->nextPendingConnection();
  m_server->close();
  QDataStream inStream(m_file);
  inStream.setVersion(QDataStream::Qt_4_6);
  QDataStream outStream(m_socket);
  outStream.setVersion(QDataStream::Qt_4_6);
  outStream << inStream;
  connect( m_socket, SIGNAL(disconnected()), this, SLOT(finished()));
  m_socket->disconnectFromHost();
  qDebug() << "File sent";
}

void SendServer::finished()
{
  m_socket->deleteLater();
  m_file->close();
  delete m_file;
  this->deleteLater();
}
