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
#include "connection/conmanager.h"
#include "connection/connectionobject.h"
#include "model/conmodel.h"
#include "model/confilter.h"
#include "model/condelegate.h"
#include <QListView>
#include <QLabel>
#include <KLineEdit>
#include <KPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
  : KXmlGuiWindow(parent)
{
  setup();
  m_conManager = new ConManager(this);
  connect( m_conManager, SIGNAL(sigNewConnection(ConnectionObject*)), this, SLOT(gotNewConnection(ConnectionObject*)) );
  connect( m_connectBtn, SIGNAL(clicked()), this, SLOT(tryConnect()) );
  connect( m_startServer, SIGNAL(clicked()), this, SLOT(startServer()) );
  show();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  qApp->quit();
  event->ignore();
}

void MainWindow::gotNewConnection(ConnectionObject* object)
{
  m_model->addConnection(object);
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
  
  //m_connectLb = new QLabel("Connect: ");
  m_ipEdit = new KLineEdit("127.0.0.1");
  m_colonLb = new QLabel(":");
  m_portEdit = new KLineEdit("47639");
  m_connectBtn = new KPushButton("Connect");
  m_connectBtn->setEnabled(false);
  
  m_myPortLb = new QLabel("My port:");
  m_myPortEdit = new KLineEdit("47639");
  m_startServer = new KPushButton("Start");
  
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
  m_mainLayout->addWidget(m_view);
  m_mainLayout->addLayout(m_connectLayout);
  m_mainLayout->addLayout(m_serverLayout);
  m_centralWid = new QWidget();
  m_centralWid->setLayout(m_mainLayout);
  setCentralWidget(m_centralWid);
}
