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

#ifndef SENDFILES_H
#define SENDFILES_H

#include <QWidget>

class QLabel;
class QVBoxLayout;

class ConnectionObject;
class SendClient;
class SendServer;

class SendFiles : public QWidget
{
    Q_OBJECT
  public:
    SendFiles( QWidget* parent=0 );
    
  public slots:
    quint16 sendFile();
    void getFile( quint16 filePort, ConnectionObject* connection );
    
  private:
    bool m_sending;
    
    QLabel* m_nameLabel;
    QLabel* m_sizeLabel;
    QVBoxLayout* m_layout;
    SendClient* m_client;
    SendServer* m_server;
};

#endif //SENDFILES_H
