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

#include "klansettings.h"

KLanSettings* KLanSettings::instance = 0;

KLanSettings::KLanSettings(  )
{
  settings = new QSettings("QLan", "qlan");
  settings->beginGroup("Connection");
  mMyPort = settings->value("MyPort", 47639).toUInt();
  mBroadcastPort = settings->value("BroadcastPort", 32695).toUInt();
  mUseBroadcast = settings->value("UseBroadcast", true).toBool();
  mAutoStart = settings->value("AutoStart", false).toBool();
  mName = settings->value("Name", "").toString();
  settings->endGroup();
}

KLanSettings::~KLanSettings()
{
  delete settings;
  instance = 0;
}

KLanSettings* KLanSettings::self()
{
  if( instance == 0 ){
    instance = new KLanSettings();
  }
  return instance;
}

void KLanSettings::writeConfig()
{
  settings->beginGroup("Connection");
  settings->setValue("MyPort", mMyPort);
  settings->setValue("BroadcastPort", mBroadcastPort);
  settings->setValue("UseBroadcast", mUseBroadcast);
  settings->setValue("AutoStart", mAutoStart);
  settings->setValue("Name", mName);
  settings->endGroup();
  settings->sync();
}

