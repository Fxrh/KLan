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

#ifndef CONITEM_H
#define CONITEM_H

#include "../connection/connectionobject.h"

class ConItem
{
  public:
    ConItem();
    ConItem( ConnectionObject* connectionObj, ConItem* parent );
    ~ConItem();
    
    bool isRoot();
    ConnectionObject* connection();
    
    void appendChild( ConItem* child );
    void removeChild( int row );
    
    ConItem* child( int row);
    int childCount() const;
    QVariant data( int collumn );
    int row() const;
    ConItem* parent();
    
  private:
    QList<ConItem*>* m_childListPtr;
    bool m_isRoot;
    ConnectionObject* m_connection;
    ConItem* m_parent;
};

#endif //CONITEM_H
