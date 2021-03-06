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

#ifndef SENDSERVER_H
#define SENDSERVER_H

#include <QObject>

class QTcpServer;
class QTcpSocket;
class QFile;

class SendServer : public QObject
{
    Q_OBJECT
  public:
    SendServer(QObject* parent=0);
    quint16 start(const QString& fileName, int& fileSize);
    int totalSize();
    int remainingSize();
    
  private slots:
    void gotConnection();
    void finished();
    
  private:
    QTcpServer* m_server;
    QTcpSocket* m_socket;
    QFile* m_file;
    int m_fileSize;
    int m_remainingSize;
};

#endif //SENDSERVER_H
