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
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QTextCursor>
#include <QTextTable>
#include <QTextTableFormat>
#include <QCloseEvent>
#include <QScrollBar>
#include <QPushButton>
#include <QDebug>
//#include <KNotification>

#include "chatwindow.h"
#include "connection/connectionobject.h"

ChatWindow::ChatWindow( QString myName, ConnectionObject* connection, QWidget* parent)
    : QWidget(parent),
      m_myName(myName),
      m_chatConnection(connection)      
{
  if( m_chatConnection->getName() == "" ){
    m_conName = QString(m_chatConnection->getIp() + ":" + QString::number(m_chatConnection->getClientPort()) );
  } else {
    m_conName = m_chatConnection->getName();
  }
  setWindowTitle(QString("Chat: %1 - KLan").arg(m_conName));
  setupGui();
  
  m_tableFormat = new QTextTableFormat();
  m_tableFormat->setBorder(0);
  
  connect( m_sendBtn, SIGNAL(clicked()), this, SLOT(sendMessage()) );
  connect( m_messageEdit, SIGNAL(returnPressed()), m_sendBtn, SLOT(click()) );
  connect( m_chatConnection, SIGNAL(sigChange(ConnectionObject*)), this, SLOT(connectionChanged()) );
  connect( m_chatConnection, SIGNAL(destroyed()), this, SLOT(connectionDestroyed()) );
}

ChatWindow::~ChatWindow()
{
}

void ChatWindow::newMessage(QString mess)
{
  writeMessage(ConMess, mess);
  activateWindow();
}

void ChatWindow::myNameChanged(QString name)
{
  if( m_myName == name ){
    return;
  }
  writeMessage(StatusMess, QString("%1 changed name to %2").arg(m_myName, name) );
  m_myName = name;
}

void ChatWindow::conNameChanged(QString name)
{
  if( m_conName == name ||( name == "" 
                            && m_conName == QString(m_chatConnection->getIp()+":"+QString::number(m_chatConnection->getClientPort())) ) ){
    return;
  }
  if( name == ""  ){
    name = QString(m_chatConnection->getIp()+":"+QString::number(m_chatConnection->getClientPort()));
  }
  writeMessage(StatusMess, QString("%1 changed name to %2").arg(m_conName, name) );
  m_conName = name;
  setWindowTitle(QString("Chat: %1 - KLan").arg(m_conName));
}

void ChatWindow::updateConnection(ConnectionObject *connection)
{
  if( connection == m_chatConnection ){
    return;
  }
  disconnect( this, SLOT(connectionChanged()) );
  disconnect( this, SLOT(connectionDestroyed()) );
  m_chatConnection = connection;
  connect( m_chatConnection, SIGNAL(sigChange(ConnectionObject*)), this, SLOT(connectionChanged()) );
  connect( m_chatConnection, SIGNAL(destroyed()), this, SLOT(connectionDestroyed()) );
  connectionChanged();
}

//void ChatWindow::connected()
//{
//  if( ! m_sendBtn->isEnabled() ){
//    m_sendBtn->setEnabled(true);
//    writeMessage(StatusMess, QString("Verbunden mit %1").arg(m_conName) );
//  }
//}
//
//void ChatWindow::disconnected()
//{
//  if( m_sendBtn->isEnabled() ){
//    m_sendBtn->setEnabled(false);
//    writeMessage( StatusMess, QString("Verbindung unterbrochen") );
//  }
//}

void ChatWindow::closeEvent(QCloseEvent* event)
{
//  if( m_chatConnection == 0 ){
//    event->ignore();
//    hide();
//    emit sigDestroy(this);
//  }
  event->ignore();
  hide();
}

void ChatWindow::sendMessage()
{
  QString message = m_messageEdit->text();
  writeMessage(MyMess, message);
  m_messageEdit->setText("");
  emit sigMessage(message, m_chatConnection );
}

void ChatWindow::connectionChanged()
{
  if( m_chatConnection->isConnected() && ! m_sendBtn->isEnabled() ){
    m_sendBtn->setEnabled(true);
    writeMessage( StatusMess, QString("Connected to %1").arg(m_conName) );
  }
  if( !m_chatConnection->isConnected() && m_sendBtn->isEnabled() ){
    m_sendBtn->setEnabled(false);
    writeMessage( StatusMess, QString("Disconnected from %1").arg(m_conName) );
  }
  conNameChanged(m_chatConnection->getName());
}

void ChatWindow::connectionDestroyed()
{
  m_chatConnection = 0;
  m_sendBtn->setEnabled(false);
  writeMessage( StatusMess, QString("Disconnected from %1").arg(m_conName) );
}

void ChatWindow::setupGui()
{
  m_chatEdit = new QTextEdit();
  m_chatEdit->setReadOnly(true);
  
  m_messageEdit = new QLineEdit();
  m_messageEdit->setMaxLength(500);
  m_chatEdit->setFocusProxy(m_messageEdit);
  
  m_sendBtn = new QPushButton("Send");
  m_sendBtn->setFocusProxy(m_messageEdit);
  //m_sendBtn->setEnabled(false);
  
  m_sendLayout = new QHBoxLayout();
  m_sendLayout->addWidget(m_messageEdit);
  m_sendLayout->addWidget(m_sendBtn);
  
  m_layout = new QVBoxLayout();
  m_layout->addWidget(m_chatEdit);
  m_layout->addLayout(m_sendLayout);
  
  setLayout(m_layout);
  resize(600, 300);
  m_messageEdit->setFocus();
  
  show();
}

void ChatWindow::writeMessage(MessType type, QString mess )
{
  if( mess.isEmpty() ){
    return;
  }
  
  QTextCursor cursor( m_chatEdit->textCursor() );
  cursor.movePosition(QTextCursor::End);
  QTextTable* table = cursor.insertTable(1, 2, *m_tableFormat );
  QTextCharFormat charFormat;
  
  switch( type ){
    case MyMess :
      charFormat.setForeground(Qt::blue);
      table->cellAt(0,0).firstCursorPosition().insertText("<" + m_myName + "> ", charFormat);
      table->cellAt(0,1).firstCursorPosition().insertText(mess);
      break;
    case ConMess :
      charFormat.setForeground(Qt::darkGreen);
      table->cellAt(0,0).firstCursorPosition().insertText("<" + m_conName + "> ", charFormat);
      table->cellAt(0,1).firstCursorPosition().insertText(mess);
      break;
    case StatusMess :     
      charFormat.setForeground(Qt::darkRed);
      cursor.insertText(" *** " + mess + " *** ", charFormat);
      break;
    default :
      qDebug() << "Wrong MessType";
      break;
  }
  QScrollBar *bar = m_chatEdit->verticalScrollBar();
  bar->setValue(bar->maximum());
}

void ChatWindow::messNotify()
{
//  KNotification* notification = new KNotification("chat");
//  notification->sendEvent();
//  notification->deleteLater();
}

//void ChatWindow::closeEvent(QCloseEvent* event )
//{
//  this->hide();
//  event->ignore();
//}
