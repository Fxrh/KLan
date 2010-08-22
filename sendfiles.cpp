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

#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QDebug>

#include "connection/connectionobject.h"
#include "connection/sendclient.h"
#include "connection/sendserver.h"
#include "sendfiles.h"

SendFiles::SendFiles(QWidget* parent)
  : QWidget(parent)
{
  m_nameLabel = new QLabel();
  m_sizeLabel = new QLabel();
  m_layout = new QVBoxLayout();
  m_layout->addWidget(m_nameLabel);
  m_layout->addWidget(m_sizeLabel);
  setLayout(m_layout);
  
  m_client = 0;
  m_server = 0;
}

quint16 SendFiles::sendFile()
{
  QString fileName = QFileDialog::getOpenFileName(this, "Select a file to send", QDir::homePath());
  m_sending = true;
  m_server = new SendServer();
  quint16 port = m_server->start(fileName);
  if( port == 0 ){
    qDebug() << "SendFiles: could not send file";
    m_server->deleteLater();
    this->deleteLater();
    return 0;
  }
  show();
  m_nameLabel->setText("Sending file...");
  return port;
}

void SendFiles::getFile(quint16 filePort, ConnectionObject* connection)
{
  QString fileName = QFileDialog::getSaveFileName(this, "Select a place to save", QDir::homePath()+"");
  m_sending = false;
  m_client = new SendClient();
  if( !m_client->tryConnect(fileName, connection->getIp(), filePort) ){
    qDebug() << "SendFiles: could not open file";
    m_client->deleteLater();
    this->deleteLater();
    return;
  }
  QString name = ( connection->getName() == "" ? connection->getIp() : connection->getName() );
  m_nameLabel->setText("Getting file from "+name);
}
