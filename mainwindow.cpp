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

#include "mainwindow.h"
#include "trayicon.h"
#include "chatwindow.h"
#include "connection/conmanager.h"
#include "connection/connectionobject.h"
#include "model/conmodel.h"
#include "model/confilter.h"
#include "model/condelegate.h"
#include "model/conitem.h"
#include <QListView>
#include <QLabel>
#include <KLineEdit>
#include <KPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QApplication>
#include <QInputDialog>
#include <KDebug>

MainWindow::MainWindow(QWidget* parent)
  : KXmlGuiWindow(parent)
{
  setup();
  m_conManager = new ConManager(this);
  m_chatMap = new QMap<QString, ChatWindow*>();
  connect( m_conManager, SIGNAL(sigNewConnection(ConnectionObject*)), this, SLOT(gotNewConnection(ConnectionObject*)) );
  connect( m_conManager, SIGNAL(sigConnectionUpdated(ConnectionObject*)), this, SLOT(gotConnectionUpdated(ConnectionObject*)) );
  connect( m_conManager, SIGNAL(sigChatMessage(QString,ConnectionObject*)), this, SLOT(gotChatMessage(QString,ConnectionObject*)) );
  connect( m_connectBtn, SIGNAL(clicked()), this, SLOT(tryConnect()) );
  connect( m_startServer, SIGNAL(clicked()), this, SLOT(startServer()) );
  connect( m_chNameBtn, SIGNAL(clicked()), this, SLOT(changeName()) );
  connect( m_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)) );
  connect( m_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openChat(QModelIndex)) );
  show();
}

MainWindow::~MainWindow()
{
  qDeleteAll(*m_chatMap);
  m_chatMap->clear();
  delete m_chatMap;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  qApp->quit();
  event->ignore();
}

void MainWindow::gotNewConnection(ConnectionObject* object)
{
  kDebug();
  m_conManager->sendMyName(m_nameLb->text());
  m_model->addConnection(object);
  ChatWindow* window = m_chatMap->value(QString(object->getIp()+":"+object->getClientPort()), 0);
  if( window != 0 ){
    window->updateConnection(object);
  }
}

void MainWindow::gotConnectionUpdated(ConnectionObject *object)
{
  m_conManager->sendMyName(m_nameLb->text());
  ChatWindow* window = m_chatMap->value(QString(object->getIp()+":"+object->getClientPort()), 0);
  if( window != 0 ){
    window->updateConnection(object);
  }
}

void MainWindow::tryConnect()
{
  m_conManager->tryConnect( m_ipEdit->text(), quint16(m_portEdit->text().toUInt()) );
}

void MainWindow::startServer()
{
  if( m_myPortEdit->isEnabled() ){
    if( m_conManager->startServer( m_myPortEdit->text().toUInt() ) ){
      m_myPortEdit->setEnabled(false);
      m_startServer->setText("Stop");
      m_connectBtn->setEnabled(true);
    }
  } else {
    m_conManager->stopServer();
    m_startServer->setText("Start");
    m_myPortEdit->setEnabled(true);
    m_connectBtn->setEnabled(false);
  }
}

void MainWindow::openChat( QModelIndex index )
{
  ConnectionObject* connection = m_model->getConnection(index);
  if( connection == 0 ){
    kDebug() << "We got a null-pointer for the connection object";
    return;
  }
  openChat( connection );
}

ChatWindow* MainWindow::openChat(ConnectionObject *object)
{
  QString key( object->getIp() + ":" + object->getClientPort() );
  ChatWindow* chatWindow = m_chatMap->value(key, 0);
  if( chatWindow == 0 ){
    QString myName = ( m_nameLb->text() != "" ? m_nameLb->text() : "Me" );
    chatWindow = new ChatWindow(myName, object);
    connect( chatWindow, SIGNAL(sigDestroy(ChatWindow*)), this, SLOT(deleteChat(ChatWindow*)) );
    connect( chatWindow, SIGNAL(sigMessage(QString,ConnectionObject*)), m_conManager, SLOT(sendChatMessage(QString,ConnectionObject*)) );
    m_chatMap->insert(key, chatWindow);
  }
  chatWindow->updateConnection(object);
  chatWindow->show();
  kDebug() << "Done";
  return chatWindow;
}

void MainWindow::deleteChat(ChatWindow *window)
{
  QString key = m_chatMap->key(window, "");
  if( key != "" ){
    m_chatMap->remove(key);
  }
  delete window;
}

void MainWindow::gotChatMessage(QString message, ConnectionObject *connection)
{
//  ChatWindow* window = m_chatMap->value(connection, 0);
//  kDebug() << connection;
//  if( window == 0 ){
//    window = new ChatWindow("Me", connection);
//    connect( window, SIGNAL(sigDestroy(ChatWindow*)), this, SLOT(deleteChat(ChatWindow*)) );
//    connect( window, SIGNAL(sigMessage(QString,ConnectionObject*)), m_conManager, SLOT(sendChatMessage(QString,ConnectionObject*)) );
//    m_chatMap->insert(connection, window)
//  }
//  window->newMessage(message);
//  window->show();
  ChatWindow* window = openChat(connection);
  window->newMessage(message);
}

void MainWindow::showContextMenu(QPoint point)
{
  
}

void MainWindow::changeName()
{
  bool ok;
  QString newName = QInputDialog::getText( this, "Change Name", "Your name:", QLineEdit::Normal, m_nameLb->text(), &ok );
  if( ok && newName != m_nameLb->text() ){
    m_conManager->sendMyName(newName);
    m_nameLb->setText(newName);
    if( newName == "" ){
      newName = "Me";
    }
    foreach( ChatWindow* window, *m_chatMap ){
      window->myNameChanged(newName);
    }
  }
}

void MainWindow::setup()
{
  setupGUI();
  m_view = new QListView(this);
  m_model = new ConModel(this);
  m_filter = new ConFilter(this);
  m_delegate = new ConDelegate(this);
  m_filter->setSourceModel(m_model);
  m_view->setModel(m_filter);
  m_view->setItemDelegate(m_delegate);
  m_trayIcon = new TrayIcon(this);
  
  m_nameLb = new QLabel();
  m_chNameBtn = new KPushButton("Change name");
  
  //m_connectLb = new QLabel("Connect: ");
  m_ipEdit = new KLineEdit("127.0.0.1");
  m_colonLb = new QLabel(":");
  m_portEdit = new KLineEdit("47639");
  m_connectBtn = new KPushButton("Connect");
  m_connectBtn->setEnabled(false);
  
  m_myPortLb = new QLabel("My port:");
  m_myPortEdit = new KLineEdit("47639");
  m_startServer = new KPushButton("Start");
  
  m_nameLayout = new QHBoxLayout();
  m_nameLayout->addWidget(m_nameLb);
  m_nameLayout->addWidget(m_chNameBtn);
  m_connectLayout = new QHBoxLayout();
  //m_connectLayout->addWidget(m_connectLb);
  m_connectLayout->addWidget(m_ipEdit);
  m_connectLayout->addWidget(m_colonLb);
  m_connectLayout->addWidget(m_portEdit);
  m_connectLayout->addWidget(m_connectBtn);
  m_serverLayout = new QHBoxLayout();
  m_serverLayout->addWidget(m_myPortLb);
  m_serverLayout->addWidget(m_myPortEdit);
  m_serverLayout->addWidget(m_startServer);
  m_mainLayout = new QVBoxLayout();
  m_mainLayout->addLayout(m_nameLayout);
  m_mainLayout->addWidget(m_view);
  m_mainLayout->addLayout(m_connectLayout);
  m_mainLayout->addLayout(m_serverLayout);
  m_centralWid = new QWidget();
  m_centralWid->setLayout(m_mainLayout);
  setCentralWidget(m_centralWid);
}
