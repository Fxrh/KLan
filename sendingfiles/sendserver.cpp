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
#include <QApplication>
#include <QDebug>
#include "sendserver.h"

SendServer::SendServer(QObject* parent)
  : QObject(parent)
{
  m_server = new QTcpServer(this);
  m_socket = 0;
  m_file = 0;
}

quint16 SendServer::start(const QString& fileName, int& fileSize)
{
  if( m_file != 0 ){
    return 0;
  }
  m_file = new QFile(fileName);
  if( !m_file->open(QIODevice::ReadOnly) ){
    return 0;
  }
  fileSize = m_file->size();
  m_fileSize = fileSize;
  m_remainingSize = m_fileSize;
  if( !m_server->listen() ){
    return 0;
  }
  connect( m_server, SIGNAL(newConnection()), this, SLOT(gotConnection()) );
  return m_server->serverPort();
}

int SendServer::totalSize()
{
  if( m_file ){
    return m_fileSize;
  }
  return 0;
}

int SendServer::remainingSize()
{
  if( m_file ){
    return m_remainingSize;
  }
  return 0;
}

void SendServer::gotConnection()
{
  m_socket = m_server->nextPendingConnection();
  m_server->close();
  while( !m_file->atEnd() ){
    if( !m_socket->flush() ){
      m_remainingSize -= m_socket->write(m_file->read(5000));
    }
    qApp->processEvents();
  }
  while( !m_socket->flush() ){
    qApp->processEvents();
  }
  m_remainingSize = 0;
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
