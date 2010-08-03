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

#include "conitem.h"
#include <QVariant>
#include <KDebug>

ConItem::ConItem(ConnectionObject* connectionObj, ConItem* parent )
  : m_connection(connectionObj),
    m_parent(parent)
{
  m_isRoot = false;
  m_childListPtr = 0;
}

ConItem::ConItem()
{
  m_isRoot = true;
  m_childListPtr = new QList<ConItem*>();
  m_parent = 0;
  m_connection = 0;
}

ConItem::~ConItem()
{
  if( m_childListPtr ){
    qDeleteAll( *m_childListPtr );
    delete m_childListPtr;
  }
}

bool ConItem::isRoot()
{
  return m_isRoot;
}

ConnectionObject* ConItem::connection()
{
  if( m_isRoot ){
    kDebug() << "Root has no connection object...";
    return 0;
  }
  return m_connection;
}

void ConItem::appendChild(ConItem* child)
{
  if( !m_isRoot ){
    kDebug() << "Only root can have child items";
    return;
  }
  m_childListPtr->append(child);
}

void ConItem::removeChild(int row)
{
  if( !m_isRoot ){
    kDebug() << "Only root has child items";
    return;
  }
  if( row >= m_childListPtr->count() ){
    kDebug() << "This row does not exist...";
    return;
  }
  m_childListPtr->removeAt(row);
}

ConItem* ConItem::child(int row)
{
  if( !m_isRoot ){
    kDebug() << "Only root has child items";
    return 0;
  }
  if( row >= m_childListPtr->count() ){
    kDebug() << "This row does not exist...";
    return 0;
  }
  return m_childListPtr->at(row);
}

int ConItem::childCount() const
{
  if( !m_isRoot ){
    return 0;
  }
  return m_childListPtr->count();
}

QVariant ConItem::data(int /*collumn*/)
{
  if( m_isRoot ){
    return QVariant();
  }
  QMap<QString,QVariant> map;
  map["ip"] = m_connection->getIp();
  map["port"] = m_connection->getClientPort();
  return map;
}

int ConItem::row() const
{
  if( m_isRoot ){
    return 0;
  }
  return m_parent->m_childListPtr->indexOf(const_cast<ConItem*>(this));
}

ConItem* ConItem::parent()
{
  if( m_isRoot ){
    return 0;
  }
  return m_parent;
}
