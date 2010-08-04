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

#include "conmodel.h"
#include "../connection/connectionobject.h"
#include <KDebug>

ConModel::ConModel(QObject* parent)
  : QAbstractListModel(parent)
{
  m_list = new QList<ConnectionObject*>();
}

ConModel::~ConModel()
{
  kDebug();
  // we don't need to delete the list of connections, ConManager does this
  delete m_list;
  kDebug() << "Deleted.";
}

void ConModel::addConnection(ConnectionObject *object)
{
  connect( object, SIGNAL(sigChange(ConnectionObject*)), this, SLOT(infoChanged(ConnectionObject*)) );
  connect( object, SIGNAL(destroyed(QObject*)), this, SLOT(gotRemoved(QObject*)) );
  beginInsertRows( QModelIndex(), m_list->count(), m_list->count() );
  m_list->append(object);
  endInsertRows();
}

ConnectionObject* ConModel::getConnection( QModelIndex index )
{
  if( !index.isValid() || index.row() < 0 || index.row() >= m_list->count() ){
    return 0;
  }
  return m_list->at(index.row());
}

QVariant ConModel::data(const QModelIndex &index, int role) const
{
  if( !index.isValid() )
    return QVariant();

  if( role != Qt::DisplayRole )
    return QVariant();
  
  int row = index.row();
  if( row < 0 || row >= m_list->count() )
    return QVariant();
  
  ConnectionObject* con = m_list->at(row);  //static_cast<ConnectionObject*>(index.internalPointer());
  QMap<QString,QVariant> map;
  map["name"] = con->getName();
  map["ip"] = con->getIp();
  map["port"] = con->getClientPort();
  map["connected"] = con->isConnected();
  return map;
}

Qt::ItemFlags ConModel::flags(const QModelIndex &index) const
{
  if( !index.isValid() )
    return 0;
  
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

int ConModel::rowCount(const QModelIndex &parent) const
{
  if( !parent.isValid() ){
    return m_list->count();
  }
  return 0;
}

void ConModel::infoChanged(ConnectionObject *object)
{
  int row = m_list->indexOf(object);
  if( row >= 0 && row < m_list->count() ){
    emit dataChanged( index(row,0,QModelIndex()), index(row,0,QModelIndex()) );
  }
}

void ConModel::gotRemoved(QObject *object)
{
  kDebug();
  int row = m_list->indexOf(static_cast<ConnectionObject*>(object));
  if( row >= 0 && row < m_list->count() ){
    beginRemoveRows(QModelIndex(), row, row );
    m_list->removeAt(row);
    endRemoveRows();
  }
}
