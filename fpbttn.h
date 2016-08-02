/*
 * fpbttn.h
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
#ifndef FPBTTN_H
#define FPBTTN_H
#include <QPushButton>
#include <QListWidget>
struct fpbttn:QPushButton{
fpbttn(QWidget *p=0);
fpbttn(const QString&,QWidget *p=NULL);
void paintEvent(QPaintEvent *e);
bool old;
void *timer;
int angle;
void paintold(bool mode=true,bool enable=true);
private:
Q_OBJECT
};
struct listwidget:QListWidget{
QSize sizeHint()const;
private:
Q_OBJECT
};
#endif
