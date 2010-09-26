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

#ifndef SIZEDEPLABEL_H
#define SIZEDEPLABEL_H

#include <QLabel>
#include <QSize>
#include <QString>

class QFontMetrics;

class SizeDepLabel : public QLabel
/* This label class shorts the content of it rather than increasing the
   sizeHint and thereby changing the size of the window, if the text is
   too long. If shortened, the text ends with "...". The minimum size is
   set by setStandardSize().
   */
{
    Q_OBJECT
  public:
    SizeDepLabel( int standardWidth, QWidget* parent = 0, Qt::WindowFlags f = 0 );
    SizeDepLabel( const QString& text, int standardWidth, QWidget* parent = 0, Qt::WindowFlags f = 0 );
    ~SizeDepLabel();
    void setStandardSize( int width );
    
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    QSize minimumSize();
    int minimumWidth();
    void setFont( const QFont& font );
    
  public slots:
    void setText( const QString& text );
    
  protected:
    void resizeEvent( QResizeEvent* event );
    
  private:
    int sizeHintWidth;
    QFontMetrics* labelFontMetrics;
    QString* m_text;
};

#endif //SIZEDEPLABEL_H
    
