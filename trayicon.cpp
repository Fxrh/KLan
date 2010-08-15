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

#include <KNotification>
#include <KDebug>
#include "trayicon.h"
#include "connection/connectionobject.h"

TrayIcon::TrayIcon(QObject* parent)
  : KStatusNotifierItem(parent)
{
  setCategory(ApplicationStatus);
  setIconByName("klan");
  setToolTipIconByName("klan");
  setTitle("KLan");
  setStatus(Active);
  setToolTipTitle("KLan");
  m_conMap = new QMap<ConnectionObject*, bool>();
}

TrayIcon::~TrayIcon()
{
  delete m_conMap;
}

void TrayIcon::newConnection(ConnectionObject* object)
{
  m_conMap->insert(object, object->isConnected() );
  connect( object, SIGNAL(sigChange(ConnectionObject*)), this, SLOT(connectionStatusChanged(ConnectionObject*)) );
  connect( object, SIGNAL(destroyed(QObject*)), this, SLOT(connectionDestroyed(QObject*)) );
  QString name = (object->getName() == "" ? object->getIp() : object->getName() );
  if( object->isConnected() ){
    sysnotify("connected", "Connected", QString("Connected to %1").arg(name) );
  }
}

void TrayIcon::connectionStatusChanged(ConnectionObject* object){
  if( object->isConnected() != m_conMap->value(object) ){
    (*m_conMap)[object] = object->isConnected();
    QString name = (object->getName() == "" ? object->getIp() : object->getName() );
    if( object->isConnected() ){
      sysnotify("connected", "Connected", QString("Connected to %1").arg(name) );
    } else {
      sysnotify("disconnected", "Disconnected", QString("Disconnected from %1").arg(name) );
    }
  }
}

void TrayIcon::connectionDestroyed(QObject* object)
{
  kDebug();
  ConnectionObject* cobject = static_cast<ConnectionObject*>(object);
  if( m_conMap->value(cobject) ){
//    QString name = (cobject->getName() == ""?QString("%1:%2").arg(cobject->getIp(), cobject->getClientPort()) : cobject->getName() );
//    sysnotify("disconnected", "Disconnected", QString("Disconnected") );
    sysnotify("disconnected", "", "");
  }
  m_conMap->remove( cobject );
}

void TrayIcon::sysnotify(const QString& name, const QString& title, const QString& msg)
{
  kDebug();
  KNotification::event(name, title, msg );
//  KNotification* notify = new KNotification(name);
//  notify->setTitle(title);
//  notify->setText(msg);
//  notify->sendEvent();
}
