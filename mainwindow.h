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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>
#include <QModelIndex>

class QLabel;
class KLineEdit;
class KPushButton;
class QListView;
class QHBoxLayout;
class QVBoxLayout;
class ConManager;
class ConModel;
class ConFilter;
class ConDelegate;
class ConnectionObject;
class TrayIcon;
class ChatWindow;

class MainWindow : public KXmlGuiWindow
{
    Q_OBJECT
  public:
    MainWindow( QWidget* parent = 0 );
    ~MainWindow();
    
  protected:
    void closeEvent(QCloseEvent* event);
    
  private slots:
    void gotNewConnection(ConnectionObject*);
    void gotConnectionUpdated(ConnectionObject* object);
    void tryConnect();
    void startServer();
    void openChat( QModelIndex index );
    ChatWindow* openChat( ConnectionObject* object );
    void deleteChat( ChatWindow* window );
    void gotChatMessage( QString message, ConnectionObject* connection );
    void showContextMenu( QPoint point );
    void changeName();
    void showConfigDialog();
    
  private:
    void setup();
    
    QListView* m_view;
    ConManager* m_conManager;
    ConModel* m_model;
    ConFilter* m_filter;
    ConDelegate* m_delegate;
    TrayIcon* m_trayIcon;
    //QLabel * m_connectLb;
    QLabel* m_nameLb;
    KPushButton* m_chNameBtn;
    KLineEdit* m_ipEdit;
    QLabel* m_colonLb;
    KLineEdit* m_portEdit;
    KPushButton* m_connectBtn;
    QLabel* m_myPortLb;
    KLineEdit* m_myPortEdit;
    KPushButton* m_startServer;
    QHBoxLayout* m_nameLayout;
    QHBoxLayout* m_connectLayout;
    QHBoxLayout* m_serverLayout;
    QVBoxLayout* m_mainLayout;
    QWidget* m_centralWid;
    QLabel* m_statusLabel;
    
    bool isStarted;
    QMap<QString, ChatWindow*>* m_chatMap;
    
};

#endif // MAINWINDOW_H
