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

#ifndef SOUNDH
#define SOUND_H

#include <QObject>
#include <QUrl>
#include <phonon/Global>

#include <phonon/AudioOutput>
#include <phonon/BackendCapabilities>
#include <phonon/MediaObject>

class Sound : public QObject 
{
    Q_OBJECT
  public:
    Sound( QObject* parent = 0 );
    
    void playShortMessage();
    void playChatMessage();
    void playConnected();
    void playDisconnected();
    
  private slots:
    void mediaStateChanged(Phonon::State, Phonon::State);
    
  private:
    Phonon::AudioOutput* soundOutput;
    Phonon::MediaObject* soundMedia;
};

#endif
