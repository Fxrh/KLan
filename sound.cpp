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

#include "sound.h"
#include <QFile>

Sound::Sound( QObject* parent )
    : QObject(parent)
{
  soundOutput = new Phonon::AudioOutput( Phonon::NotificationCategory, this );
  soundMedia = new Phonon::MediaObject(this);
  
  Phonon::createPath( soundMedia, soundOutput );
  
  connect( soundMedia, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(mediaStateChanged(Phonon::State, Phonon::State )) );
}

void Sound::playShortMessage()
{
  QFile* file = new QFile(":/data/sounds/klan_shortmsg.ogg");
  soundMedia->setCurrentSource(Phonon::MediaSource(file));
  soundMedia->play();
}

void Sound::playChatMessage()
{
  QFile* file = new QFile(":/data/sounds/klan_chat.ogg");
  soundMedia->setCurrentSource(Phonon::MediaSource(file));
  soundMedia->play();
}

void Sound::playConnected()
{
  QFile* file = new QFile(":/data/sounds/klan_connect.ogg");
  soundMedia->setCurrentSource(Phonon::MediaSource(file));
  soundMedia->play();
}

void Sound::playDisconnected()
{
  QFile* file= new QFile(":/data/sounds/klan_disconnect.ogg");
  soundMedia->setCurrentSource(Phonon::MediaSource(file));
  soundMedia->play();
}

void Sound::mediaStateChanged(Phonon::State newState, Phonon::State)
{
  switch( newState ){
    case Phonon::ErrorState:
      qDebug() << "Sound: error: " << soundMedia->errorString();
      break;
    default:
      break;
  }
}
