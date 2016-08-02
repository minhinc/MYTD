/*
 * widget.h
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
#ifndef WIDGET_H
#define WIDGET_H
#include <QGroupBox>
#include <QDialog>
#include <QHideEvent>
struct widget{
widget(int,widget* n=0);
virtual ~widget();
int id;
static void* dt;
virtual int handle()=0;
widget* next;
};

struct labelgb:QGroupBox,widget{
labelgb(int,widget* n=0);
void* lbl;
int handle();
private:
Q_OBJECT
};
struct griddb:QDialog,widget{
griddb(int,widget* n=0);
~griddb();
void *rbcache;
void *lbl;
void* dpb;
void* cpb;
int handle();
private slots:
void slotdpbclicked();
void slotcpbclicked();
private:
Q_OBJECT
};
//struct videowd:QWidget,widget{
//videowd(int,widget* n=0);
//~videowd();
//void *mp;
//void *vw;
//void* ppb;
//void* vpb;
//void *sl;
//int *pos;
//int index;
//QSize sizeHint() const;
//int handle();
//private slots:
//void slotppbclicked();
//void slotvpbclicked();
//void slotsldurationchanged(qint64);
//void slotslvaluechanged(qint64);
//void slotslmoved(int);
//protected:
//void hideEvent(QHideEvent* e);
//void enterEvent(QEvent* e);
//void leaveEvent(QEvent* e);
//private:
//Q_OBJECT
//};
struct QPaintEvent;
struct QPushButton;
struct QSlider;
struct QLabel;
struct QTimer;
struct QMouseEvent;
struct QHideEvent;
struct videowd:QWidget,widget{
videowd(int,widget* n=0);
~videowd();
qint64 bt;
int framecount;
//qint64 audiosamplecount;
bool tweak;
bool video;
bool audio;
bool seekfile;
bool pause;
QSlider *sldr;
QLabel *lbl;
//QPushButton *fpb;
QTimer *tmr;
QSize sizeHint() const;
int handle();
private slots:
void pushframe();
//void slotclicked();
void slotsldrvaluechanged(int);
protected:
void paintEvent(QPaintEvent*);
void mousePressEvent(QMouseEvent*);
void enterEvent(QEvent*);
void leaveEvent(QEvent*);
void mouseDoubleClickEvent(QMouseEvent*);
void hideEvent(QHideEvent* e);
private:
Q_OBJECT
};
#endif
