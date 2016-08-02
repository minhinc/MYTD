/*
 * fpbttn.cpp
 * Minh YouTube Downloader
 * Copyright (c) 2015-2016 Minh Inc www.minhinc.com
 *
 * This file is part of MYTD.
 *
 * MYTD is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * MYTD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Mgmp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */
#include <QTimer>
#include <QPainter>
#include "fpbttn.h"
#include "debug.h"
fpbttn::fpbttn(QWidget *p):QPushButton(p),old(true),angle(0){
debug("fpbttn::fpbttn")
timer=new QTimer;
connect((QTimer*)timer,SIGNAL(timeout()),this,SLOT(update()));
debug("~fpbttn::fpbttn")
}
fpbttn::fpbttn(const QString& text,QWidget *p):QPushButton(text,p),old(true),angle(0){
debug("fpbttn::fpbttn(QString,QWidget)")
timer=new QTimer;
connect((QTimer*)timer,SIGNAL(timeout()),this,SLOT(update()));
debug("~fpbttn::fpbttn")
}
void fpbttn::paintEvent(QPaintEvent *e){
QPainter p(this);
int side=qMin(rect().width(),rect().height());
QPen pen;
//debug("fpbttn::paintevent"<<old);
if(old)
return QPushButton::paintEvent(e);
p.setViewport((rect().width()-side)/2,(rect().height()-side)/2,side,side);
p.setWindow(-25,-25,50,50);
p.drawRoundedRect(x(),y(),width(),height(),width()/4,height()/4);
pen.setWidth(5);
pen.setColor(QColor(Qt::red));
p.setPen(pen);
p.save();
p.rotate(angle);
p.drawArc(-20,-20,40,40,90*16,120*16);
p.drawLine(-6,-25,0,-20);
p.drawLine(0,-20,-6,-13);
p.drawLine(-6,-25,-6,-13);
pen.setColor(QColor(Qt::green));
p.setPen(pen);
p.drawArc(-20,-20,40,40,270*16,120*16);
p.drawLine(6,13,0,20);
p.drawLine(0,20,6,25);
p.drawLine(6,13,6,25);
p.restore();
angle+=30;
if(angle==360)angle=0;
//debug("~fpbttn::paintevent");
}
void fpbttn::paintold(bool mode,bool enable){
debug("fpbttn::chngold"<<mode)
old=mode;
if(mode==false)
((QTimer*)timer)->start(100);
else
((QTimer*)timer)->stop();
setDisabled(!enable);
update();
debug("~fpbttn::paintold")
}
QSize listwidget::sizeHint() const{
debug("/listwidget::sizeHint")
return QSize(WINDOW_WIDTH,(DEV_MAX<5)?DEV_MAX*MW_ROW_HEIGHT:5*MW_ROW_HEIGHT);
}
