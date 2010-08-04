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

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QWidget>
#include <QString>

class QVBoxLayout;
class QHBoxLayout;
class QTextEdit;
class QLineEdit;
class QTextTableFormat;
class QCloseEvent;
class QPushButton;
class ConnectionObject;

class ChatWindow : public QWidget
{
    Q_OBJECT
  public:
    enum MessType { MyMess, ConMess, StatusMess };
    ChatWindow( QString myName, ConnectionObject* connection, QWidget* parent=0);
    ~ChatWindow();
    
  public slots:
    void newMessage(QString mess);
    void myNameChanged(QString name);
    void conNameChanged(QString name);
    void updateConnection( ConnectionObject* connection );
//    void connected();
//    void disconnected();
    
  signals:
    void sigMessage(QString mess, ConnectionObject* connection);
    void sigDestroy( ChatWindow* window );
    
protected:
    void closeEvent(QCloseEvent* event);
    
  private slots:
    void sendMessage();
    void connectionChanged();
    void connectionDestroyed();
    
  private:
    void setupGui();
    void writeMessage(MessType type, QString mess);
//    void closeEvent(QCloseEvent* event);
    void messNotify();
    
    QString m_myName;
    QString m_conName;
    QVBoxLayout* m_layout;
    QHBoxLayout* m_sendLayout;
    QTextEdit* m_chatEdit;
    QLineEdit* m_messageEdit;
    QPushButton* m_sendBtn;
    QTextTableFormat* m_tableFormat;
    
    ConnectionObject* m_chatConnection;
    
};

#endif //CHATWINDOW_H
