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

#ifndef SENDITEM_H
#define SENDITEM_H

#include <QWidget>

class QVBoxLayout;
class SizeDepLabel;
class QProgressBar;
class SendClient;
class SendServer;

class SendItem : public QWidget
{
  public:
    SendItem( QWidget* parent=0 );
    
    quint16 sendFile(const QString& fileName, int& fileSize);
    bool getFile(const QString& fileName, int fileSize, const QString& ip, quint16 port);
    
  protected:
    void timerEvent(QTimerEvent* event);
    
  private:
    bool m_sending;
    SendClient* m_client;
    SendServer* m_server;
    int m_progressSize;
    int m_timer;
    
    SizeDepLabel* m_fileNameLb;
    QProgressBar* m_progressBar;
    QVBoxLayout* m_mainLayout;
};

#endif //SENDITEM_H
