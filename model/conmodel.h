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

#ifndef CONMODEL_H
#define CONMODEL_H

#include <QAbstractListModel>

class ConnectionObject;

class ConModel : public QAbstractListModel
{
    Q_OBJECT
  public:
    ConModel( QObject* parent = 0 );
    ~ConModel();
    
    void addConnection( ConnectionObject* object );
    
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent) const;
    
  private slots:
    void infoChanged( ConnectionObject* object );
    void gotRemoved( QObject* object );
    
  private:
    QList<ConnectionObject*>* m_list;
};

#endif //CONMODEL_H
