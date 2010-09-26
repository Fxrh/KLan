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

#ifndef SENDCLIENT_H
#define SENDCLIENT_H

#include <QObject>

class QTcpSocket;
class QFile;

class SendClient : public QObject
{
    Q_OBJECT
  public:
    SendClient( QObject* parent=0);
    bool tryConnect(const QString& fileName, int fileSize, const QString& ip, quint16 port);
    int totalSize();
    int remainingSize();
    
  private slots:
    void gotData();
    void finished();
    
  private:
    QFile* m_file;
    QTcpSocket* m_socket;
    int m_totalSize;
    int m_remainingSize;
};

#endif //SENDCLIENT_H
