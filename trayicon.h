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

#ifndef TRAYICON_H
#define TRAYICON_H

#include <KStatusNotifierItem>

class ConnectionObject;

class TrayIcon : public KStatusNotifierItem
{
    Q_OBJECT
  public:
    ~TrayIcon();
    TrayIcon( QObject* parent=0 );
    
  public slots:
    void newConnection( ConnectionObject* );
    
  private slots:
    void connectionStatusChanged(ConnectionObject*);
    void connectionDestroyed(QObject*);
    
  private:
    void sysnotify( const QString& name, const QString& title, const QString& msg );
    QMap<ConnectionObject*, bool>* m_conMap;
};

#endif
