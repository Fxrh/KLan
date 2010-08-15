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

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

class QCheckBox;
class QLabel;
class QSpinBox;
class QListView;
class QStringListModel;
class KPushButton;
class QHBoxLayout;
class QVBoxLayout;

class SettingsDialog : public QDialog
{
    Q_OBJECT
  public:
    SettingsDialog( QWidget* parent=0 );
    bool listChanged() const { return m_listChanged; }
    
  public slots:
    void accept();
    void reject();
    
  private slots:
    void listAdd();
    void listRm();
    
  private:
    QCheckBox* m_autoStart;
    QCheckBox* m_useBroadcast;
    QLabel* m_broadcastPort;
    QSpinBox* m_broadcastPortEdit;
    QLabel* m_listLb;
    QListView* m_listView;
    QStringListModel* m_listModel;
    KPushButton* m_addBtn;
    KPushButton* m_rmBtn;
    
    KPushButton* m_acceptBtn;
    KPushButton* m_rejectBtn;
    
    QHBoxLayout* m_broadcastLayout;
    QHBoxLayout* m_listBtnLayout;
    QHBoxLayout* m_buttonLayout;
    QVBoxLayout* m_vSettingsLayout;
    QHBoxLayout* m_hSettingsLayout;
    QVBoxLayout* m_mainLayout;
    
    bool m_listChanged;
    QStringList* m_list;
};

#endif //SETTINGSDIALOG_H
