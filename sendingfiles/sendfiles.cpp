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
#include <QScrollArea>
#include <QDebug>
#include <QMessageBox>

#include "../connection/connectionobject.h"
#include "senditem.h"
#include "sendfiles.h"

SendFiles::SendFiles(QWidget* parent)
  : QWidget(parent, Qt::Window)
{
  m_nameLabel = new QLabel("Sending/Receiving Files");
  QFont font;
  font.setBold(true);
  m_nameLabel->setFont(font);
  m_itemWidget = new QWidget();
  m_itemArea = new QScrollArea();
  m_itemArea->setWidgetResizable(true);
  m_itemLayout = new QVBoxLayout();
  m_itemWidget->setLayout(m_itemLayout);
  m_itemArea->setWidget(m_itemWidget);
  m_layout = new QVBoxLayout();
  m_layout->addWidget(m_nameLabel);
  m_layout->addWidget(m_itemArea);
  setLayout(m_layout);
  
  m_list = new QList<SendItem*>();
  show();
}

SendFiles::~SendFiles()
{
  qDebug() << "~SendFiles...";
  delete m_list;
  qDebug() << "~SendFiles.";
}

quint16 SendFiles::sendFile(QString& fileName, int& fileSize)
{
  show();
  QString pathName = QFileDialog::getOpenFileName(this, "Select a file to send", QDir::homePath());
  if( pathName.isNull() ){
    return 0;
  }
  fileName = QFileInfo(pathName).fileName();
  SendItem* newItem = new SendItem();
  int port = newItem->sendFile(pathName, fileSize);
  if( port ){
    m_itemLayout->insertWidget(0, newItem);
    m_itemWidget->resize(m_itemWidget->sizeHint());
  } else {
    qDebug() << "Could not send data: was not able to open port or could not read file";
    QMessageBox::critical(this, "Error: Could not send file", "The application was not able to send your file:\n "
                          "It could not open a port or was unable to open your file.");
    delete newItem;
  }
  return port;
}

void SendFiles::getFile(quint16 filePort, const QString& fileName, int fileSize, ConnectionObject* connection)
{
  show();
  QString name = QFileDialog::getSaveFileName(this, "Select a place to save", QDir::homePath()+QDir::separator()+fileName);
  if( name.isNull() ){
    return;
  }
  SendItem* newItem = new SendItem();
  if( newItem->getFile(name, fileSize, connection->getIp(), filePort) ){
    m_itemLayout->insertWidget(0, newItem);
    m_itemWidget->resize(m_itemWidget->sizeHint());
  } else {
    qDebug() << "Could not receive data: was not able to create file";
    QMessageBox::critical(this, "Error: Could not receive file", "The file could not be created.");
    delete newItem;
  }
}
