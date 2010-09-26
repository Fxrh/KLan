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

#include <QVBoxLayout>
#include <QProgressBar>
#include <QTimerEvent>
#include <QDir>
#include "../widgets/sizedeplabel.h"
#include "sendserver.h"
#include "sendclient.h"

#include "senditem.h"

SendItem::SendItem(QWidget* parent)
  : QWidget(parent)
{
  setMinimumHeight(50);
  m_fileNameLb = new SizeDepLabel(20);
  m_progressBar = new QProgressBar();
  m_progressBar->setMinimum(0);
  m_mainLayout = new QVBoxLayout();
  m_mainLayout->addStretch();
  m_mainLayout->addWidget(m_fileNameLb);
  m_mainLayout->addWidget(m_progressBar);
  m_mainLayout->addStretch();
  setLayout(m_mainLayout);
  
  m_server = 0;
  m_client = 0;
  m_timer = 0;
  m_progressSize = 0;
}

quint16 SendItem::sendFile(const QString& fileName, int& fileSize)
{
  m_sending = true;
  m_fileNameLb->setText(fileName.section(QDir::separator(), -1, -1));
  m_server = new SendServer(this);
  quint16 port = m_server->start(fileName, fileSize);
  if( port ){
    m_progressBar->setMaximum(m_server->totalSize());
    m_progressSize = m_server->remainingSize();
    m_timer = startTimer(300);
    m_progressBar->setValue(m_server->totalSize()-m_progressSize);
    show();
  }
  return port;
}

bool SendItem::getFile(const QString &fileName, int fileSize, const QString &ip, quint16 port)
{
  m_sending = false;
  m_fileNameLb->setText(fileName.section(QDir::separator(), -1, -1));
  m_client = new SendClient(this);
  bool ok = m_client->tryConnect(fileName, fileSize, ip, port);
  if( ok ){
    m_progressBar->setMaximum(fileSize);
    m_progressSize = m_client->remainingSize();
    m_progressBar->setValue(fileSize-m_progressSize);
    m_timer = startTimer(300);
    show();
  }
  return ok;
}

void SendItem::timerEvent(QTimerEvent* event)
{
  if( event->timerId() == m_timer ){
    if( m_sending ){
      int progSize = m_server->remainingSize();
      if( progSize != m_progressSize ){
        m_progressBar->setValue(m_server->totalSize()-progSize);
      }
    } else {
      int progSize = m_client->remainingSize();
      if( progSize != m_progressSize ){
        m_progressBar->setValue(m_client->totalSize()-progSize);
      }
    }
  }
}
