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
#include "connection/conmanager.h"
#include "connection/connectionobject.h"
#include "model/conmodel.h"
#include "model/confilter.h"
#include "model/condelegate.h"
#include <QListView>
#include <QLabel>
#include <KLineEdit>
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
  
  m_connectLb = new QLabel("Connect: ");
  m_ipEdit = new KLineEdit("127.0.0.1");
  m_colonLb = new QLabel(":");
  m_portEdit = new KLineEdit("47639");
  
  m_connectLayout = new QHBoxLayout();
  m_connectLayout->addWidget(m_connectLb);
  m_connectLayout->addWidget(m_ipEdit);
  m_connectLayout->addWidget(m_colonLb);
  m_connectLayout->addWidget(m_portEdit);
  m_mainLayout = new QVBoxLayout();
  m_mainLayout->addWidget(m_view);
  m_mainLayout->addLayout(m_connectLayout);
  m_centralWid = new QWidget();
  m_centralWid->setLayout(m_mainLayout);
  setCentralWidget(m_centralWid);
}
