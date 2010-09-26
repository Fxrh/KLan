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

#include <QMainWindow>
#include <QModelIndex>

class QLabel;
class QLineEdit;
class QPushButton;
class QListView;
class QHBoxLayout;
class QVBoxLayout;
class QSignalMapper;
class ConManager;
class ConModel;
class ConFilter;
class ConDelegate;
class ConnectionObject;
class TrayIcon;
class ChatWindow;
class SendFiles;

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    MainWindow( QWidget* parent = 0 );
    ~MainWindow();
    
  protected:
//    void closeEvent(QCloseEvent* event);
    
  private slots:
    void gotNewConnection(ConnectionObject*);
    void gotConnectionUpdated(ConnectionObject* object);
    void tryConnect();
    void startServer();
    void openChat( QModelIndex index );
    ChatWindow* openChat( ConnectionObject* object );
    void deleteChat( ChatWindow* window );
    void gotChatMessage( QString message, ConnectionObject* connection );
    void gotFileMessage( quint16 port, QString name, int fileSize, ConnectionObject* connection);
    void showContextMenu( QPoint point );
    void changeName();
    void showConfigDialog();
    void showAbout();
    void sendShortMessage(int id);
    void sendFile();
    
  private:
    void setup();
    
    QListView* m_view;
    ConManager* m_conManager;
    ConModel* m_model;
    ConFilter* m_filter;
    ConDelegate* m_delegate;
    TrayIcon* m_trayIcon;
    SendFiles* m_sendFiles;
    //QLabel * m_connectLb;
    QLabel* m_nameLb;
    QPushButton* m_chNameBtn;
    QLineEdit* m_ipEdit;
    QLabel* m_colonLb;
    QLineEdit* m_portEdit;
    QPushButton* m_connectBtn;
    QLabel* m_myPortLb;
    QLineEdit* m_myPortEdit;
    QPushButton* m_startServer;
    QHBoxLayout* m_nameLayout;
    QHBoxLayout* m_connectLayout;
    QHBoxLayout* m_serverLayout;
    QVBoxLayout* m_mainLayout;
    QWidget* m_centralWid;
    QLabel* m_statusLabel;
    QMenu* m_fileMenu;
    QMenu* m_settingsMenu;
    QMenu* m_helpMenu;
    QAction* m_quitAct;
    QAction* m_settingsAct;
    QAction* m_aboutAct;
    QAction* m_aboutQtAct;
    
    bool isStarted;
    QMap<QString, ChatWindow*>* m_chatMap;
    QMenu* m_contextMenu;
    QSignalMapper* m_signalMapper;
    
};

#endif // MAINWINDOW_H
