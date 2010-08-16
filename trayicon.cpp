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

#include "trayicon.h"
#include "connection/connectionobject.h"
#include "sound.h"
#include <QMap>

TrayIcon::TrayIcon(QObject* parent)
  : QSystemTrayIcon(parent)
{
  //setIcon(QIcon(":/data/images/klan.png"));
  m_conMap = new QMap<ConnectionObject*, bool>();
  m_sound = new Sound(this);
  show();
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
    m_sound->playConnected();
  }
}

void TrayIcon::shortMessage(QString message, ConnectionObject *object)
{
  QString name = (object->getName() == "" ? object->getIp() : object->getName() );
  sysnotify( "shortmessage", "Short Message", QString("%1 says: %2").arg(name).arg(message) );
  m_sound->playShortMessage();
}

void TrayIcon::chatMessage(QString message, ConnectionObject *object)
{
  QString name = (object->getName() == "" ? object->getIp() : object->getName() );
  sysnotify("chatmessage", "Chat Message", QString("%1 says: %2").arg(name).arg(message) );
  m_sound->playChatMessage();
}

void TrayIcon::chatSound()
{
  m_sound->playChatMessage();
}

void TrayIcon::connectionStatusChanged(ConnectionObject* object){
  if( object->isConnected() != m_conMap->value(object) ){
    (*m_conMap)[object] = object->isConnected();
    QString name = (object->getName() == "" ? object->getIp() : object->getName() );
    if( object->isConnected() ){
      sysnotify("connected", "Connected", QString("Connected to %1").arg(name) );
      m_sound->playConnected();
    } else {
      sysnotify("disconnected", "Disconnected", QString("Disconnected from %1").arg(name) );
      m_sound->playDisconnected();
    }
  }
}

void TrayIcon::connectionDestroyed(QObject* object)
{
  ConnectionObject* cobject = static_cast<ConnectionObject*>(object);
  if( m_conMap->value(cobject) ){
//    QString name = (cobject->getName() == ""?QString("%1:%2").arg(cobject->getIp(), cobject->getClientPort()) : cobject->getName() );
//    sysnotify("disconnected", "Disconnected", QString("Disconnected") );
    m_sound->playDisconnected();
  }
  m_conMap->remove( cobject );
}

void TrayIcon::sysnotify(const QString& name, const QString& title, const QString& msg)
{
  Q_UNUSED(name);
  this->showMessage(title, msg);
}
