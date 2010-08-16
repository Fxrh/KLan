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

#ifndef KLANSETTINGS_H
#define KLANSETTINGS_H

#include <QSettings>
#include <QStringList>

class KLanSettings
{
  public:

    static KLanSettings *self();
    ~KLanSettings();
    void writeConfig();

    /**
      Set The port the server uses.
    */
    static
    void setMyPort( quint16 v )
    {
      self()->mMyPort = v;
    }

    /**
      Get The port the server uses.
    */
    static
    uint myPort()
    {
      return self()->mMyPort;
    }

    /**
      Set The port KLan uses to find other users in your network. All users have to use the same port!
    */
    static
    void setBroadcastPort( quint16 v )
    {
      self()->mBroadcastPort = v;
    }

    /**
      Get The port KLan uses to find other users in your network. All users have to use the same port!
    */
    static
    uint broadcastPort()
    {
      return self()->mBroadcastPort;
    }

    /**
      Set Find other users via broadcast calls.
    */
    static
    void setUseBroadcast( bool v )
    {
      self()->mUseBroadcast = v;
    }

    /**
      Get Find other users via broadcast calls.
    */
    static
    bool useBroadcast()
    {
      return self()->mUseBroadcast;
    }

    /**
      Set Start the server automatically after startup.
    */
    static
    void setAutoStart( bool v )
    {
      self()->mAutoStart = v;
    }

    /**
      Get Start the server automatically after startup.
    */
    static
    bool autoStart()
    {
      return self()->mAutoStart;
    }

    /**
      Set Your name.
    */
    static
    void setName( const QString & v )
    {
      self()->mName = v;
    }

    /**
      Get Your name.
    */
    static
    QString name()
    {
      return self()->mName;
    }
    
    static
    void setShortMsgList( const QStringList& v )
    {
      self()->mShortMsgList = v;
    }
    
    static
    QStringList shortMsgList()
    {
      return self()->mShortMsgList;
    }

  protected:
    KLanSettings();


  private:
    static KLanSettings* instance;
    
    // Connection
    quint16 mMyPort;
    quint16 mBroadcastPort;
    bool mUseBroadcast;
    bool mAutoStart;
    QString mName;
    QStringList mShortMsgList;
    
    QSettings* settings;
};

#endif

