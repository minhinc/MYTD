/*
 * videowidget.h
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
#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H
#include <QWidget>
struct QAbstractButton;
struct QSlider;
struct QMediaPlayer;

struct videowidget:QWidget{
videowidget(QWidget *p=0);
~videowidget();
QMediaPlayer *mp;
QAbstractButton *pb;
QSlider *sl;
private slots:
void slotpbclicked();
void slotsldurationchanged(qint64);
void slotslvaluechanged(qint64);
void slotslslidermoved(int);
private:
Q_OBJECT
};
#endif
