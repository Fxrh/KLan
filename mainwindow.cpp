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
#include "klansettings.h"
#include "settingsdialog.h"
#include "sendingfiles/sendfiles.h"
#include <QListView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QApplication>
#include <QInputDialog>
#include <QDebug>
#include <QStatusBar>
#include <QSignalMapper>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
{
  setup();
  m_conManager = new ConManager(this);
  m_chatMap = new QMap<QString, ChatWindow*>();
  m_contextMenu = 0; // will be created if needed
  m_signalMapper = new QSignalMapper(this);
  m_sendFiles = 0;
  connect( m_conManager, SIGNAL(sigNewConnection(ConnectionObject*)), this, SLOT(gotNewConnection(ConnectionObject*)) );
  connect( m_conManager, SIGNAL(sigConnectionUpdated(ConnectionObject*)), this, SLOT(gotConnectionUpdated(ConnectionObject*)) );
  connect( m_conManager, SIGNAL(sigChatMessage(QString,ConnectionObject*)), this, SLOT(gotChatMessage(QString,ConnectionObject*)) );
  connect( m_conManager, SIGNAL(sigShortMessage(QString,ConnectionObject*)), m_trayIcon, SLOT(shortMessage(QString,ConnectionObject*)) );
  connect( m_conManager, SIGNAL(sigFile(quint16,QString,int,ConnectionObject*)), this, SLOT(gotFileMessage(quint16,QString,int,ConnectionObject*)) );
  connect( m_connectBtn, SIGNAL(clicked()), this, SLOT(tryConnect()) );
  connect( m_startServer, SIGNAL(clicked()), this, SLOT(startServer()) );
  connect( m_chNameBtn, SIGNAL(clicked()), this, SLOT(changeName()) );
  connect( m_view, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)) );
  connect( m_view, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(openChat(QModelIndex)) );
  connect( m_signalMapper, SIGNAL(mapped(int)), this, SLOT(sendShortMessage(int)) );
  show();
  if( KLanSettings::autoStart() ){
    startServer();
  }
}

MainWindow::~MainWindow()
{
  qDeleteAll(*m_chatMap);
  m_chatMap->clear();
  delete m_chatMap;
  KLanSettings::setMyPort(m_myPortEdit->text().toUInt());
  KLanSettings::setName(m_nameLb->text());
  KLanSettings::self()->writeConfig();
}

//void MainWindow::closeEvent(QCloseEvent* event)
//{
////  qApp->quit();
//  event->accept();
//}

void MainWindow::gotNewConnection(ConnectionObject* object)
{
//  kDebug();
  m_conManager->sendMyName(m_nameLb->text());
  m_model->addConnection(object);
  m_trayIcon->newConnection(object);
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
    m_conManager->setServerPort( m_myPortEdit->text().toUInt() );
    if( m_conManager->start() ){
      m_myPortEdit->setEnabled(false);
      m_startServer->setText("Stop");
      m_connectBtn->setEnabled(true);
      m_statusLabel->setText("server started - broadcasting stopped");
      if( KLanSettings::useBroadcast() ){
        if( m_conManager->startBroadcast() ){
          m_statusLabel->setText("server started - broadcasting started");
        }
      }
    }
  } else {
    m_conManager->stop();
    m_startServer->setText("Start");
    m_myPortEdit->setEnabled(true);
    m_connectBtn->setEnabled(false);
    m_statusLabel->setText("server stopped - broadcsting stopped");
  }
}

void MainWindow::openChat( QModelIndex index )
{
  index = m_filter->mapToSource(index);
  ConnectionObject* connection = m_model->getConnection(index);
  if( connection == 0 ){
    qDebug() << "We got a null-pointer for the connection object";
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
    connect( chatWindow, SIGNAL(sigChatNotification(QString,ConnectionObject*)), m_trayIcon, SLOT(chatMessage(QString,ConnectionObject*)) );
    connect( chatWindow, SIGNAL(sigChatSound()), m_trayIcon, SLOT(chatSound()) );
    m_chatMap->insert(key, chatWindow);
  }
  chatWindow->updateConnection(object);
  chatWindow->show();
  qDebug() << "openChat: Done";
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

void MainWindow::gotFileMessage(quint16 port, QString name, int fileSize, ConnectionObject* connection)
{
  if( !m_sendFiles ){
    m_sendFiles = new SendFiles(this);
  }
  m_sendFiles->getFile(port, name, fileSize, connection);
}

void MainWindow::showContextMenu(QPoint point)
{
  if( !m_view->indexAt(point).isValid() ){
    return;
  }
  if( m_contextMenu == 0 ){
    m_contextMenu = new QMenu();
    QStringList list = KLanSettings::shortMsgList();
    int len = list.count();
    for( int i=0; i<len; ++i ){
      QAction* action = new QAction(list[i], m_contextMenu);
      m_contextMenu->addAction(action);
      connect( action, SIGNAL(triggered()), m_signalMapper, SLOT(map()) );
      m_signalMapper->setMapping(action, i);
    }
    m_contextMenu->addSeparator();
    QAction* action = new QAction("Send File...", m_contextMenu);
    m_contextMenu->addAction(action);
    connect( action, SIGNAL(triggered()), this, SLOT(sendFile()) );
  }
  m_contextMenu->exec(m_view->mapToGlobal(point));
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

void MainWindow::showConfigDialog()
{
  SettingsDialog dialog;
  if( dialog.exec() == QDialog::Accepted ){
    if( dialog.listChanged() ){
      delete m_contextMenu;
      m_contextMenu = 0;
    }
    m_conManager->changeBroadcastPort( KLanSettings::broadcastPort() );
    if( KLanSettings::useBroadcast() && isStarted ){
      if( m_conManager->startBroadcast() ){
        m_statusLabel->setText("server started - broadcasting started");
        return;
      }
    }
    if( isStarted ){
      m_statusLabel->setText("server started - broadcasting stopped");
    } else {
      m_statusLabel->setText("server stopped - broadcasting stopped");
    }
  }  
}

void MainWindow::showAbout()
{
  QMessageBox::about(this, "About QLan",
                     "<h2>QLan version 0.1</h2>"
                     "<p>Copyright &copy; 2009-2010 Felix Rohrbach <fxrh@gmx.de></p>"
                     "<p>A LAN communication tool</p>");
}

void MainWindow::sendShortMessage(int id)
{
  ConnectionObject* object = m_model->getConnection( m_filter->mapToSource(m_view->currentIndex()) );
  if( object == 0 ){
    qDebug() << "sendShrtMessage: Error: we got a null pointer object";
  }
  m_conManager->sendShortMessage( KLanSettings::shortMsgList()[id], object );
}

void MainWindow::sendFile()
{
  ConnectionObject* object = m_model->getConnection( m_filter->mapToSource(m_view->currentIndex()) );
  if( object == 0 ){
    qDebug() << "sendFile Error: we got a null pointer object";
  }
  if( !m_sendFiles ){
    m_sendFiles = new SendFiles(this);
  }
  int fileSize;
  QString fileName;
  quint16 port = m_sendFiles->sendFile(fileName, fileSize);
  if( port != 0 ){
    m_conManager->sendFileMessage(port, fileName, fileSize, object );
  }
}

void MainWindow::setup()
{
  m_quitAct = new QAction("Quit", this);
  m_fileMenu = new QMenu("File");
  m_fileMenu->addAction(m_quitAct);
  m_settingsAct = new QAction("Configure QLan", this);
  m_settingsMenu = new QMenu("Settings");
  m_settingsMenu->addAction(m_settingsAct);
  m_aboutAct = new QAction("About...", this);
  m_aboutQtAct = new QAction("About Qt", this);
  m_helpMenu = new QMenu("Help");
  m_helpMenu->addAction(m_aboutAct);
  m_helpMenu->addAction(m_aboutQtAct);
  setMenuBar(new QMenuBar());
  menuBar()->addMenu(m_fileMenu);
  menuBar()->addMenu(m_settingsMenu);
  menuBar()->addMenu(m_helpMenu);
  setStatusBar(new QStatusBar());
  m_statusLabel = new QLabel("server stopped - broadcasting stopped");
  statusBar()->addWidget(m_statusLabel);
  
  
  m_view = new QListView(this);
  m_view->setContextMenuPolicy( Qt::CustomContextMenu );
  m_model = new ConModel(this);
  m_filter = new ConFilter(this);
  m_delegate = new ConDelegate(this);
  m_filter->setSourceModel(m_model);
  m_view->setModel(m_filter);
  m_view->setItemDelegate(m_delegate);
  m_trayIcon = new TrayIcon(this);
  
  m_nameLb = new QLabel(KLanSettings::name());
  m_chNameBtn = new QPushButton("Change name");
  
  //m_connectLb = new QLabel("Connect: ");
  m_ipEdit = new QLineEdit("127.0.0.1");
  m_colonLb = new QLabel(":");
  m_portEdit = new QLineEdit("47639");
  m_connectBtn = new QPushButton("Connect");
  m_connectBtn->setEnabled(false);
  
  m_myPortLb = new QLabel("My port:");
  m_myPortEdit = new QLineEdit(QString::number(KLanSettings::myPort()));
  m_startServer = new QPushButton("Start");
  
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
  
  connect( m_quitAct, SIGNAL(triggered()), qApp, SLOT(quit()) );
  connect( m_settingsAct, SIGNAL(triggered()), this, SLOT(showConfigDialog()) );
  connect( m_aboutAct, SIGNAL(triggered()), this, SLOT(showAbout()) );
  connect( m_aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()) );
}
