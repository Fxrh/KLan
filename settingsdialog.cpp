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

#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QListWidget>
#include <QStringListModel>
#include <KPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QInputDialog>
#include <KIcon>
#include "klansettings.h"
#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget* parent)
  : QDialog(parent)
{
  m_listChanged = false;
  m_list = new QStringList(KLanSettings::shortMsgList());
  
  m_autoStart = new QCheckBox("Start server on startup");
  m_autoStart->setChecked(KLanSettings::autoStart());
  m_useBroadcast = new QCheckBox("Use broadcasting to find other users");
  m_useBroadcast->setChecked(KLanSettings::useBroadcast());
  m_broadcastPort = new QLabel("Broadcast Port:");
  m_broadcastPortEdit = new QSpinBox();
  m_broadcastPortEdit->setMinimum(0);
  m_broadcastPortEdit->setMaximum(65535);
  m_broadcastPortEdit->setValue(KLanSettings::broadcastPort());
  m_broadcastPortEdit->setEnabled( m_useBroadcast->isEnabled() );
  m_listLb = new QLabel("Your Short Messages:");
  m_listModel = new QStringListModel(this);
  m_listView = new QListView();
  m_listView->setModel(m_listModel);
  m_listModel->setStringList(KLanSettings::shortMsgList());
  m_addBtn = new KPushButton(KIcon("list-add"), "Add");
  m_rmBtn = new KPushButton(KIcon("list-remove"), "Remove");
  
  m_acceptBtn = new KPushButton(KIcon("dialog-ok"), "Ok");
  m_acceptBtn->setDefault(true);
  m_rejectBtn = new KPushButton(KIcon("dialog-cancel"), "Cancel");
  
  m_broadcastLayout = new QHBoxLayout();
  m_broadcastLayout->addWidget(m_broadcastPort);
  m_broadcastLayout->addWidget(m_broadcastPortEdit);
  m_listBtnLayout = new QHBoxLayout();
  m_listBtnLayout->addWidget(m_addBtn);
  m_listBtnLayout->addWidget(m_rmBtn);
  m_buttonLayout = new QHBoxLayout();
  m_buttonLayout->addStretch();
  m_buttonLayout->addWidget(m_acceptBtn);
  m_buttonLayout->addWidget(m_rejectBtn);
  m_vSettingsLayout = new QVBoxLayout();
  m_vSettingsLayout->addWidget(m_autoStart);
  m_vSettingsLayout->addWidget(m_useBroadcast);
  m_vSettingsLayout->addLayout(m_broadcastLayout);
  m_vSettingsLayout->addWidget(m_listLb);
  m_vSettingsLayout->addWidget(m_listView);
  m_vSettingsLayout->addLayout(m_listBtnLayout);
  m_hSettingsLayout = new QHBoxLayout();
  m_hSettingsLayout->addLayout(m_vSettingsLayout);
  m_hSettingsLayout->addStretch();
  m_mainLayout = new QVBoxLayout();
  m_mainLayout->addLayout(m_hSettingsLayout);
  m_mainLayout->addStretch();
  m_mainLayout->addLayout(m_buttonLayout);
  setLayout(m_mainLayout);
  
  connect( m_useBroadcast, SIGNAL(toggled(bool)), m_broadcastPortEdit, SLOT(setEnabled(bool)) );
  connect( m_addBtn, SIGNAL(clicked()), this, SLOT(listAdd()) );
  connect( m_rmBtn, SIGNAL(clicked()), this, SLOT(listRm()) );
  connect( m_acceptBtn, SIGNAL(clicked()), this, SLOT(accept()) );
  connect( m_rejectBtn, SIGNAL(clicked()), this, SLOT(reject()) );
}

void SettingsDialog::accept()
{
  KLanSettings::setAutoStart( m_autoStart->isChecked() );
  KLanSettings::setUseBroadcast( m_useBroadcast->isChecked() );
  KLanSettings::setBroadcastPort(m_broadcastPortEdit->value());
  if( m_listChanged ){
    KLanSettings::setShortMsgList(*m_list);
  }
  QDialog::accept();
}

void SettingsDialog::reject()
{
  QDialog::reject();
}

void SettingsDialog::listAdd()
{
  bool ok;
  QString newStr = QInputDialog::getText(this, "New Short Message", "Enter a new short message", QLineEdit::Normal, "", &ok);
  if( newStr != "" && ok ){
    m_listChanged = true;
    m_list->append(newStr);
    m_listModel->setStringList(*m_list);
  }
}

void SettingsDialog::listRm()
{
  int row = m_listView->currentIndex().row();
  if( row != -1 ){
    m_listChanged = true;
    m_list->removeAt(row);
  }
}
