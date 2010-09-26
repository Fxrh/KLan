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

#include "sizedeplabel.h"

#include <QSizePolicy>
#include <QFontMetrics>
#include <QtDebug>

SizeDepLabel::SizeDepLabel(int standardWidth, QWidget* parent, Qt::WindowFlags f )
    : QLabel( parent, f ),
    sizeHintWidth( standardWidth ),
    labelFontMetrics( new QFontMetrics(fontMetrics()) )
{
  m_text = new QString("");
  setSizePolicy( QSizePolicy::MinimumExpanding, QLabel::sizePolicy().verticalPolicy() );
  setMinimumSize( minimumSize() );
  setMinimumWidth( sizeHintWidth );
  qDebug() << "SDL: Constructor: done";
}

SizeDepLabel::SizeDepLabel( const QString& text, int standardWidth, QWidget* parent, Qt::WindowFlags f )
    : QLabel( parent, f ),
    sizeHintWidth( standardWidth ),
    labelFontMetrics( new QFontMetrics(fontMetrics()) )
{
  m_text = new QString("");
  setSizePolicy( QSizePolicy::MinimumExpanding, QLabel::sizePolicy().verticalPolicy() );
  setText( text );
  setMinimumSize( minimumSize() );
  setMinimumWidth( sizeHintWidth );
  qDebug() << "SDL: Constructor: done";
}

SizeDepLabel::~SizeDepLabel()
{
  delete labelFontMetrics;
  delete m_text;
  qDebug() << "SDL: Destructor: done";
}

void SizeDepLabel::setStandardSize( int width )
{
  sizeHintWidth = width;
  if( size().width() < width ){
    resize( sizeHint() );
  }
  setMinimumSize( minimumSize() );
  setMinimumWidth( sizeHintWidth );
}

QSize SizeDepLabel::sizeHint() const
{
  QSize size = QLabel::sizeHint();
  size.setWidth( sizeHintWidth );
  return size;
}

QSize SizeDepLabel::minimumSizeHint() const
{
  QSize size = QLabel::minimumSizeHint();
  size.setWidth( sizeHintWidth );
  return size;
}

QSize SizeDepLabel::minimumSize()
{
  QSize size = QLabel::minimumSize();
  size.setWidth( sizeHintWidth );
  return size;
}

int SizeDepLabel::minimumWidth()
{
  return sizeHintWidth;
}

void SizeDepLabel::setFont( const QFont& font )
{
  delete labelFontMetrics;
  labelFontMetrics = new QFontMetrics( font );
  setText( *m_text );
  QLabel::setFont( font );
  //setMinimumSize( minimumSize() );
  //setMinimumWidth( sizeHintWidth );
}

void SizeDepLabel::setText( const QString& text )
{
  *m_text = text;
  QString newText = text;
  int width = labelFontMetrics->boundingRect( newText ).width();
  bool wasShortened = false;
  while( width > size().width()+3 ){
    newText.remove( newText.count()-1, 1 );
    width = labelFontMetrics->boundingRect( newText ).width();
    wasShortened = true;
  }
  if( wasShortened ){
    newText.remove(newText.count()-3, 3);
    newText.append( "..." );
  }
  
  QLabel::setText( newText );
  setMinimumSize( minimumSize() );
  setMinimumWidth( sizeHintWidth );
}

void SizeDepLabel::resizeEvent( QResizeEvent* event )
{
  setText( *m_text );
  QLabel::resizeEvent( event );
  //setMinimumSize( minimumSize() );
  //setMinimumWidth( sizeHintWidth );
}
