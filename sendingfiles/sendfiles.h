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
class QScrollArea;

class ConnectionObject;
class SendItem;

class SendFiles : public QWidget
{
    Q_OBJECT
  public:
    SendFiles( QWidget* parent=0 );
    ~SendFiles();
    
  public slots:
    quint16 sendFile(QString& fileName, int& fileSize);
    void getFile( quint16 filePort, const QString& fileName, int fileSize, ConnectionObject* connection );
    
  private:
    QLabel* m_nameLabel;
    QScrollArea* m_itemArea;
    QVBoxLayout* m_itemLayout;
    QWidget* m_itemWidget;
    QVBoxLayout* m_layout;
    QList<SendItem*>* m_list;
};

#endif //SENDFILES_H
