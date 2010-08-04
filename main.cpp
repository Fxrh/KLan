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

#include <QApplication>
//#include <KAboutData>
//#include <KCmdLineArgs>
//#include <KLocale>

#include "mainwindow.h"

int main( int argc, char* argv[] )
{
//  KAboutData aboutData( "klan",
//                        0,
//                        ki18n("KLan"),
//                        "0.1",
//                        ki18n("A lan communication tool for KDE"),
//                        KAboutData::License_GPL_V3,
//                        ki18n( "(c) 2010 Felix Rohrbach" ),
//                        ki18n(""), // Futher description
//                        "", // Website
//                        "fxrh@gmx.de" );
  
//  KCmdLineArgs::init( argc, argv, &aboutData );
  QApplication app(argc, argv);
  MainWindow window;
  return app.exec();
}
