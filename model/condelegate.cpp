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

#include "condelegate.h"

#include <QPainter>

ConDelegate::ConDelegate(QWidget* parent)
  : QStyledItemDelegate(parent)
{
  boldFont.setBold(true);
}

void ConDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QStyledItemDelegate::paint(painter, option, index);
  QPoint originPoint = option.rect.topLeft();
  originPoint.rx()+=5;
  originPoint.ry()+=5;
  painter->save();
  painter->setFont(boldFont);
  QString address = index.data().toMap()["ip"].toString()+":"+QString::number(index.data().toMap()["port"].toUInt());
  painter->drawText( QRect(originPoint, option.rect.bottomRight() ), address);
  painter->restore();
}

QSize ConDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
  QFontMetrics metrics(boldFont);
  return metrics.boundingRect("888.888.888.888:88888").size() + QSize(10,10);
}
